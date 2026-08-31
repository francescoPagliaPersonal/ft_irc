group_begin "FRAMING"

test_ping_pong() {
	local out
	out=$(oneshot "PASS $PASSWORD" "NICK pinger1" "USER pinger1 0 * :p" "PING :hello")
	assert_contains "$out" "PONG"
}

test_partial_ping() {
	local out
	out=$(
		{
			printf 'PI'
			sleep 0.25
			printf 'NG :x\r\n'
			sleep "$IRC_WAIT"
		} | stdbuf -o0 nc -4 -w 8 "$HOST" "$PORT"
	)
	assert_contains "$out" "PONG"
}

test_two_commands_one_write() {
	local out
	out=$( ( printf 'PASS %s\r\nNICK twoatonce\r\nUSER twoatonce 0 * :x\r\n' \
		"$PASSWORD"; sleep "$IRC_WAIT" ) | stdbuf -o0 nc -4 -w 8 "$HOST" "$PORT")
	assert_contains "$out" " 001 "
}

test_unknown_command() {
	local out
	out=$(oneshot "PASS $PASSWORD" "NICK unkncmd" "USER unkncmd 0 * :x" "FOOBAR")
	assert_contains "$out" " 421 "
}

test "ping_pong" test_ping_pong
test "partial_ping" test_partial_ping
test "two_commands_one_write" test_two_commands_one_write
test "unknown_command" test_unknown_command

group_end
