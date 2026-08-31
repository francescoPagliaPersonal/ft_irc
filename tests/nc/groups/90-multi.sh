group_begin "MULTI"

test_two_clients_ping() {
	if ! register_client xa alex1; then
		return 1
	fi
	if ! register_client xb alex2; then
		irc_close xa
		return 1
	fi
	irc_send xa "PING :one"
	irc_send xb "PING :two"
	sleep "$IRC_WAIT"
	local aout bout
	aout=$(irc_recv xa)
	bout=$(irc_recv xb)
	irc_close xa
	irc_close xb
	assert_contains "$aout" "PONG" && assert_contains "$bout" "PONG"
}

test_three_clients() {
	if ! register_client xc alex3; then
		return 1
	fi
	if ! register_client xd alex4; then
		irc_close xc
		return 1
	fi
	if ! register_client xe alex5; then
		irc_close xc
		irc_close xd
		return 1
	fi
	irc_send xc "JOIN #mlt1"
	irc_send xd "JOIN #mlt1"
	irc_send xe "JOIN #mlt1"
	sleep "$IRC_WAIT"
	irc_send xc "PRIVMSG #mlt1 :hi all"
	sleep "$IRC_WAIT"
	local dout eout
	dout=$(irc_recv xd)
	eout=$(irc_recv xe)
	irc_close xc
	irc_close xd
	irc_close xe
	assert_contains "$dout" "PRIVMSG #mlt1 :hi all" \
		&& assert_contains "$eout" "PRIVMSG #mlt1 :hi all"
}

test_disconnect_one_other_still_works() {
	if ! register_client xf alex6; then
		return 1
	fi
	if ! register_client xg alex7; then
		irc_close xf
		return 1
	fi
	irc_close xf
	sleep "$IRC_WAIT"
	irc_send xg "PING :stillhere"
	sleep "$IRC_WAIT"
	local out
	out=$(irc_recv xg)
	irc_close xg
	assert_contains "$out" "PONG"
}

test "two_clients_ping" test_two_clients_ping
test "three_clients" test_three_clients
test "disconnect_one_other_still_works" test_disconnect_one_other_still_works

group_end
