group_begin "FRAMING"

test_ping_pong() {
	oneshot_expect "PONG" "PASS $PASSWORD" "NICK pinger1" "USER pinger1 0 * :p" "PING :hello"
}

test_partial_ping() {
	irc_open fr
	irc_write fr 'PI'
	sleep 0.25
	irc_write fr $'NG :x\r\n'
	irc_expect fr "PONG"
	local rc=$?
	irc_close fr
	return "$rc"
}

test_two_commands_one_write() {
	irc_open tw
	irc_write tw "$(printf 'PASS %s\r\nNICK twoatonce\r\nUSER twoatonce 0 * :x\r\n' "$PASSWORD")"
	irc_expect tw " 001 "
	local rc=$?
	irc_close tw
	return "$rc"
}

test_unknown_command() {
	oneshot_expect " 421 " "PASS $PASSWORD" "NICK unkncmd" "USER unkncmd 0 * :x" "FOOBAR"
}

test "ping_pong" test_ping_pong
test "partial_ping" test_partial_ping
test "two_commands_one_write" test_two_commands_one_write
test "unknown_command" test_unknown_command

group_end
