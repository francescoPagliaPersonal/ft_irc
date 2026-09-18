[Back to README](../README.md)

# ft_irc Server Reference (`ircserv`)

This document provides a comprehensive operational and technical guide for the `ft_irc` server daemon (`ircserv`).

---

## Contents

- [Overview & Launching](#overview--launching)
- [Client Registration Flow](#client-registration-flow)
- [Command Reference](#command-reference)
  - [Connection & Authentication](#connection--authentication)
  - [Channel Management](#channel-management)
  - [Messaging](#messaging)
  - [Server Queries & Keepalive](#server-queries--keepalive)
- [Channel Modes](#channel-modes)
- [Operational Constraints & Abuse Mitigation](#operational-constraints--abuse-mitigation)
- [Interactive Examples](#interactive-examples)
  - [Irssi Walkthrough](#irssi-walkthrough)
  - [Raw TCP / Netcat Session](#raw-tcp--netcat-session)

---

## Overview & Launching

The server is invoked from the command line:

```bash
./ircserv <port> <password>
```

### Parameter Requirements

- **`port`**: Unsigned 16-bit integer strictly between `1024` and `65535`.
- **`password`**: Connection secret. Validated by `arg2password`:
  - Minimum length: **4 characters**
  - Must include at least **one alphabetical letter** (`[a-zA-Z]`)
  - Must include at least **one numerical digit** (`[0-9]`)
  - Must include at least **one non-alphanumeric special character** (e.g. `.` or `-`)
  - Example: `1o.0`

---

## Client Registration Flow

To participate in the IRC network, a client must successfully complete registration before invoking channel or messaging commands.

### Standard Registration Sequence

1. **`PASS <password>`**: Supplies the connection password. Must match the password given when starting `ircserv`.
2. **`NICK <nickname>`**: Chooses a unique nickname.
3. **`USER <username> 0 * :<realname>`**: Specifies the user identity and display name.

### Modern Client Handshake (`CAP`)

Modern clients like `irssi` initiate capability negotiation on connection:

```text
Client -> Server:  CAP LS 302
Client -> Server:  PASS 1o.0
Client -> Server:  NICK alice
Client -> Server:  USER alice 0 * :Alice L.
Server -> Client:  :irc.CoolServ.42 CAP alice LS :
Client -> Server:  CAP END
Server -> Client:  :irc.CoolServ.42 001 alice :Welcome to the Internet Relay Network alice!~alice@127.0.0.1
Server -> Client:  :irc.CoolServ.42 002 alice :Your host is irc.CoolServ.42, running version 1.0
Server -> Client:  :irc.CoolServ.42 003 alice :This server was created <timestamp>
Server -> Client:  :irc.CoolServ.42 004 alice :irc.CoolServ.42 1.0  oiktl
```

Once `PASS`, `NICK`, and `USER` have been verified and `CAP END` is processed, registration is complete (`REG_DONE`).

---

## Command Reference

### Connection & Authentication

#### `PASS <password>`
- **Description**: Verifies client password against the server password.
- **Policy**: Must be sent before registration is complete (`AlreadyRegisteredPlcy(false)`). Requires exactly 1 argument.
- **Errors**: `462 ERR_ALREADYREGISTERED`, `464 ERR_PASSWDMISMATCH`, `461 ERR_NEEDMOREPARAMS`.

#### `NICK <nickname>`
- **Description**: Sets or updates the client's nickname.
- **Policy**: Requires 1 to 2 arguments.
- **Constraints**: Maximum 32 characters. Cannot contain whitespace or ` .,*?!@`. Cannot start with `$`, `:`, `~`, `&`, `#`, `@`, `%`, `+`.
- **Errors**: `431 ERR_NONICKNAMEGIVEN`, `432 ERR_ERRONEUSNICKNAME`, `433 ERR_NICKNAMEINUSE`.

#### `USER <username> <hostname> <servername> :<realname>`
- **Description**: Specifies client username, hostname, servername, and realname trailing string.
- **Policy**: Must be sent before registration is complete (`AlreadyRegisteredPlcy(false)`). Requires 4 arguments.
- **Constraints**: Length of <username> and <realname> are capped at 32 characters. Cannot contain whitespace or ` .,*?!@`. Cannot start with `$`, `:`, `~`, `&`, `#`, `@`, `%`, `+`.
- **Errors**: `462 ERR_ALREADYREGISTERED`, `461 ERR_NEEDMOREPARAMS`. additionally `400 ERR_UNKNOWNERROR` used for non conform names

#### `CAP <subcommand> [<args>]`
- **Description**: Handles IRCv3 client capability queries.
- **Supported Subcommands**:
  - `LS`: Acknowledges capability query.
  - `END`: Completes the capability negotiation and triggers registration finalization.

#### `QUIT [:<message>]`
- **Description**: Disconnects client from server.
- **Behavior**: Broadcasts `QUIT` notification to all peers sharing channels with this client, removes the client from all channels, and closes the TCP connection.

---

### Channel Management

#### `JOIN <channels> [<keys>]`
- **Description**: Joins one or more channels (comma-separated list).
- **Parameters**: Channel names must start with `#` or `&`, length 4 to 32 characters, valid charset `[A-Za-z0-9_-]`. Keys correspond to channels set with `+k`.
- **Behavior**: If the channel does not exist, it is created with the relative password if presented and the creator is automatically granted channel operator status (`+o`).
- **Replies**: Channel join broadcast, `332 RPL_TOPIC` (or `331 RPL_NOTOPIC`), `353 RPL_NAMREPLY`, `366 RPL_ENDOFNAMES`.
- **Errors**: `403 ERR_NOSUCHCHANNEL`, `405 ERR_TOOMANYCHANNELS`, `471 ERR_CHANNELISFULL`, `473 ERR_INVITEONLYCHAN`, `475 ERR_BADCHANNELKEY`.

#### `PART <channels> [:<message>]`
- **Description**: Leaves one or more channels (comma-separated list).
- **Behavior**: Broadcasts `PART` to all channel members. If the last member leaves, the channel is destroyed and its memory deallocated.
- **Errors**: `403 ERR_NOSUCHCHANNEL`, `442 ERR_NOTONCHANNEL`, `461 ERR_NEEDMOREPARAMS`.

#### `TOPIC <channel> [:<new_topic>]`
- **Description**: Views or updates the channel topic.
- **Behavior**:
  - If `<new_topic>` is omitted: returns `332 RPL_TOPIC` or `331 RPL_NOTOPIC`.
  - If `<new_topic>` is supplied: updates topic and broadcasts change to channel members. If mode `+t` is active, the caller must be a channel operator.
- **Errors**: `403 ERR_NOSUCHCHANNEL`, `442 ERR_NOTONCHANNEL`, `482 ERR_CHANOPRIVSNEEDED`.

#### `INVITE <nickname> <channel>`
- **Description**: Invites a client to a channel.
- **Behavior**:
  - Caller must be on the channel.
  - If mode `+i` is active on the channel, caller must be a channel operator.
  - Target client must exist and must not already be in the channel.
  - Adds target client to channel invite whitelist and sends an invite notification to the target.
- **Replies**: `341 RPL_INVITING`.
- **Errors**: `401 ERR_NOSUCHNICK`, `403 ERR_NOSUCHCHANNEL`, `442 ERR_NOTONCHANNEL`, `443 ERR_USERONCHANNEL`, `482 ERR_CHANOPRIVSNEEDED`.

#### `KICK <channel> <nickname> [:<comment>]`
- **Description**: Forcefully removes a user from a channel.
- **Behavior**: Caller must be a channel operator. Target user must be present in the channel.
- **Replies**: Broadcasts `KICK` message to all channel members.
- **Errors**: `403 ERR_NOSUCHCHANNEL`, `442 ERR_NOTONCHANNEL`, `441 ERR_USERNOTINCHANNEL`, `482 ERR_CHANOPRIVSNEEDED`.

#### `MODE <channel> [<modes>] [<mode_args>...]`
- **Description**: Inspects or alters channel modes.
- **Query**: `MODE <channel>` (without mode changes) returns `324 RPL_CHANNELMODEIS` with current flags.
- **Modifications**: Requires channel operator privileges (`+o`). See [Channel Modes](#channel-modes) below.
- **User MODE**: `MODE <nickname> +i` returns `221 RPL_UMODEIS` compatibility response.

---

### Messaging

#### `PRIVMSG <targets> :<text>`
- **Description**: Sends a message to one or more targets (comma-separated).
- **Channel Target** (`#` or `&`):
  - Sender must be a member of the target channel (`442 ERR_NOTONCHANNEL`).
  - Broadcasts text to all members of the channel except the sender.
- **User Target**:
  - Delivers direct private message to the designated nickname.
- **RFC Compliance**: Large trailing messages are automatically split into valid 512-byte chunks (`chunkifyTrailing`).
- **Errors**: `411 ERR_NORECIPIENT`, `412 ERR_NOTEXTTOSEND`, `401 ERR_NOSUCHNICK`, `403 ERR_NOSUCHCHANNEL`.
- **Bonus (file transfer)**: Trailing text is forwarded as-is, including CTCP delimiters (`0x01`). That is how clients negotiate DCC file transfers; see [File Transfer](file-transfer.md).

---

### Server Queries & Keepalive

#### `PING <token>`
- **Description**: Keepalive verification from client or server.
- **Reply**: `PONG <servername> :<token>`.

#### `PONG <token>`
- **Description**: Client response to a server-initiated keepalive `PING`. Resets client inactivity counter.

---

## Channel Modes

Channel modes control access, permissions, and behavior. Modifying channel modes requires channel operator privileges.

| Mode Flag | Name | Argument | Description |
|---|---|---|---|
| `+i` / `-i` | Invite-only | None | When enabled, only users added via `INVITE` can join |
| `+t` / `-t` | Topic Protection | None | When enabled, only channel operators can change the `TOPIC` |
| `+k` / `-k` | Channel Key | `<key>` (on `+k`) | Password required to join the channel (`JOIN #chan <key>`) |
| `+o` / `-o` | Operator Status | `<nick>` | Promotes a user to channel operator or revokes operator privileges |
| `+l` / `-l` | User Limit | `<limit>` (on `+l`) | Maximum number of concurrent members allowed in channel |

### Mode Usage Examples

```text
MODE #dev +t               # Lock topic changes to operators
MODE #dev +k Secret123     # Require key "Secret123" to join
MODE #dev +l 10            # Restrict membership to 10 users
MODE #dev +o bob           # Promote bob to operator
MODE #dev -k               # Remove channel key
MODE #dev -i               # Make channel public (remove invite-only)
```

---

## Operational Constraints & Abuse Mitigation

To maintain server stability without multithreading or external processes, `ircserv` enforces internal limits defined in `include/ft_irc.hpp`:

| Metric | Limit | Consequence / Action |
|---|---|---|
| Max Concurrent Clients | `50,000` | Refuses connections when capacity is reached |
| Max Connections per IP | `5` | Additional connection attempts from same IP rejected |
| Max Channels per User | `3` | `405 ERR_TOOMANYCHANNELS` on further `JOIN` attempts |
| Max Channel Title Length | `32` characters | Channel title rejected if non-compliant |
| Min Channel Title Length | `4` characters | Channel title rejected if shorter |
| Max Nickname Length | `32` characters | `432 ERR_ERRONEUSNICKNAME` |
| Max Topic Length | `300` characters | Truncated / rejected if exceeded |
| Max Channel Capacity | `500` users | Hard ceiling on individual channel membership |
| Max Message Size | `512` bytes | Aggregated and safely split into chunks |
| Ping Interval | `90` seconds | Server sends periodic `PING` heartbeat to inactive clients |
| Inactivity Disconnect | `1` missed ping | Marked for disconnection and removed during housekeeping |
| Anti-Spam Rate Limit | `42` msgs / `2.0` sec | Client connection terminated on flood detection |

---

## Interactive Examples

### Irssi Walkthrough

1. Connect to the local server:
   ```bash
   irssi -c localhost -p 6669 -w 1o.0 -n developer
   ```
2. Join a channel and set a topic:
   ```text
   /join #project
   /topic #project Development discussion
   ```
3. Set channel modes (you are operator as channel creator):
   ```text
   /mode #project +t
   /mode #project +k Pass123
   ```
4. Invite another user:
   ```text
   /invite alice #project
   ```

---

### Raw TCP / Netcat Session

Use `nc -C` to ensure standard `\r\n` (CRLF) line terminators:

```bash
nc -C 127.0.0.1 6669
```

Paste or type the following lines sequentially:

```text
PASS 1o.0
NICK tester
USER tester 0 * :Test User
JOIN #lobby
PRIVMSG #lobby :Hello, world!
MODE #lobby
PART #lobby :Done testing
QUIT :Signing off
```

Example server transcript received in terminal:

```text
:irc.CoolServ.42 001 tester :Welcome to the Internet Relay Network tester!~tester@127.0.0.1
:irc.CoolServ.42 002 tester :Your host is irc.CoolServ.42, running version 1.0
:irc.CoolServ.42 003 tester :This server was created Wed Sep 16 2026
:irc.CoolServ.42 004 tester :irc.CoolServ.42 1.0  oiktl
:tester!~tester@127.0.0.1 JOIN :#lobby
:irc.CoolServ.42 331 tester #lobby :No topic is set
:irc.CoolServ.42 353 tester = #lobby :@tester
:irc.CoolServ.42 366 tester #lobby :End of /NAMES list.
:tester!~tester@127.0.0.1 PRIVMSG #lobby :Hello, world!
:irc.CoolServ.42 324 tester #lobby +
:tester!~tester@127.0.0.1 PART #lobby :Done testing
ERROR :Closing Link: tester (Quit: Signing off)
```
