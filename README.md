# ft_irc
_This project has been created as part of the 42 curriculum by fpaglia, mweghofe._

## Description
ft_irc is a lightweight, non-blocking IRC (Internet Relay Chat) server implementation written in C++. The project was developed as part of the 42 school curriculum to demonstrate deep understanding of network programming, the IRC protocol, and concurrent event handling.

The server supports multiple simultaneous clients through epoll-based I/O multiplexing, implementing the core features of the IRC protocol as defined in RFC 1459 and RFC 2812. It handles client registration, channel management, user messaging, and operator privileges with a clean, extensible architecture.

### Key Features
* Non-blocking I/O using epoll for efficient client handling
* Full registration flow with PASS, NICK, USER commands and CAP negotiation
* Channel management with join, part, invite, kick, and topic operations
* Channel modes: invite-only (+i), topic restrictions (+t), password (+k), user limit (+l), operator privileges (+o)
* Private messaging and channel broadcasting
* IP-based connection limiting to prevent abuse
* Command policies for validation and permission checking
* RFC-compliant numeric replies and error messages
* Graceful signal handling (SIGINT, SIGTERM, SIGPIPE)

## Instructions
### Prerequisites
* C++ compiler with C++98 standard support (or later)
* Linux operating system (epoll is Linux-specific)
* make build system

### Compilation
```bash
make
```

### Installation
No installation is required. The executable ircserv is generated in the project root directory.

### Execution
```bash
./ircserv <port> <password>
```
Parameters:
* port: Port number between 1024 and 65535
* password: Connection password (minimum 4 characters, must contain letters, numbers, and special characters)

### Connecting to the Server
Using irssi:

```bash
# Local connection
irssi -c localhost -p <port> -w <password>

# Remote connection
irssi -c <server_ip> -p <port> -w <password>
```
From within irssi:

```bash
/connect localhost <port> <password>
/connect <server_ip> <port> <password>
```

Using netcat:

```bash
nc -C localhost <port>
# Then send IRC commands manually (see Resources)
Example Session
text
PASS mypassword
NICK john
USER john 0 * :John Doe
JOIN #welcome
PRIVMSG #welcome :Hello everyone!
TOPIC #welcome :Welcome to the channel
MODE #welcome +i                            // Make channel invite-only
INVITE jane #welcome                        // Invite jane to the channel
KICK #welcome spammer                       // Kick a user from the channel
QUIT :Goodbye!
```

## Resources
### IRC Protocol Documentation
* [IRCv3 consortium](https://ircv3.net/) – Modern IRC extensions and specifications
* [Keywords usage](https://datatracker.ietf.org/doc/html/rfc2119)
* [Modern IRC client protocol](https://modern.ircdocs.horse/) – Comprehensive, up-to-date documentation of the IRC protocol
* [Security concerns](https://www.irchelp.org/security/) – Security guidelines and best practices
* [RFC Internet relay protocol](https://datatracker.ietf.org/doc/html/rfc1459) – Internet Relay Chat Protocol (original specification)
* [RFC client specification](https://datatracker.ietf.org/doc/html/rfc2812) – Internet Relay Chat: Client Protocol (updated specification)


### Development Tools & References
epoll(7) – Linux man page for epoll I/O event notification


### AI Usage Disclosure
AI assistance was used in the following areas of this project:
* Documentation: This README was initiated with AI assistance.
* Learning Resource: AI served as an interactive reference for IRC protocol details and C++ best practices.

Project-specific implementation decisions were made independently, including:
* Architecture design (IServerCtrl interface, Command patterns, policies)
* Channel mode handling and operator privilege management
* Buffer management and message parsing logic
* Epoll event loop structure and client lifecycle management

### Key Architectural Choices
Our ft_irc server is built upon a modular, event-driven architecture that prioritizes performance, maintainability, and protocol compliance. The design revolves around several key architectural decisions that shape the system's behavior and structure.

#### Event-Driven Core with Non-Blocking I/O
At the heart of the server lies an epoll-based event loop that efficiently manages hundreds of concurrent client connections without resorting to multithreading. This single-threaded, event-driven approach eliminates synchronization overhead while maintaining responsive behavior through non-blocking socket operations. The Epoll class serves as a lightweight wrapper around the Linux epoll API, providing edge-triggered event notification that minimizes unnecessary system calls.

#### Interface-Driven Design
The IServerCtrl interface acts as the central contract between components, decoupling command execution from server implementation details. This abstraction allows the Command system and policies to interact with the server without tight coupling, enabling easier testing and future extensibility. The interface encapsulates all core server operations—client management, channel manipulation, message broadcasting, and registration flow—into a clean API.

#### Command Pattern with Policy Composition
Commands are implemented using the Command pattern, where each IRC command (NICK, JOIN, PRIVMSG, etc.) is encapsulated as a Command object with an associated handler function. What makes this architecture particularly powerful is the policy composition system: commands can be decorated with multiple IPolicy objects (argument limits, registration requirements, permission checks) that are evaluated before command execution. This separation of concerns allows for declarative validation rules without cluttering command logic.

#### Message Processing Pipeline
The server implements a clean message pipeline:
1. Raw bytes are received into per-client input buffers
2. Complete IRC messages are parsed into structured Message objects
3. Messages are enqueued for processing
4. The command registry looks up and executes the appropriate handler
5. Responses are generated through the Response class and buffered for output

This pipeline decouples network I/O from protocol logic, allowing the event loop to handle data reception while commands are processed in batches.

#### Channel and Client State Management
Clients and channels are managed through dedicated classes with clear responsibilities:
* Client handles connection state, registration flags, buffering, and channel membership
* Channel manages modes, members, operator privileges, and channel-specific data

The server maintains maps of both entities, with channels keyed by case-insensitive normalized titles to ensure RFC compliance. The IP-based connection limiting system tracks client IPs to prevent abuse, with a maximum of five connections per IP address.

#### Response Generation and Protocol Compliance
The Response class centralizes all RFC-compliant reply formatting, supporting both numeric and regular message formats. This centralization ensures consistency across commands and simplifies error handling through semi-automatic numeric replies. The system includes support for message chunking to respect the 512-byte IRC message limit.

#### Memory and Resource Management
The architecture employs RAII principles with careful attention to ownership semantics:

* The server owns clients and channels, managing their lifecycle
* Commands and policies are owned by the registry
* Epoll watchlists are automatically cleaned up through destructors
* Signals are handled gracefully to ensure proper shutdown

#### Limitations
* The server has no persistent storage (channels and users are not saved between restarts)
* No SSL/TLS encryption
* Limited to IPv4
* Maximum 50,000 clients (configurable)
* No DCC (Direct Client-to-Client) support
* Minimal CAP negotiation (only LS/END support)