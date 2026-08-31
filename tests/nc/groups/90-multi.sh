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
	if ! irc_expect xa "PONG"; then
		irc_close xa
		irc_close xb
		return 1
	fi
	irc_expect xb "PONG"
	local rc=$?
	irc_close xa
	irc_close xb
	return "$rc"
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
	if ! irc_expect xe "JOIN #mlt1"; then
		irc_close xc
		irc_close xd
		irc_close xe
		return 1
	fi
	irc_send xc "PRIVMSG #mlt1 :hi all"
	if ! irc_expect xd "PRIVMSG #mlt1 :hi all"; then
		irc_close xc
		irc_close xd
		irc_close xe
		return 1
	fi
	irc_expect xe "PRIVMSG #mlt1 :hi all"
	local rc=$?
	irc_close xc
	irc_close xd
	irc_close xe
	return "$rc"
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
	irc_send xg "PING :stillhere"
	irc_expect xg "PONG"
	local rc=$?
	irc_close xg
	return "$rc"
}

test "two_clients_ping" test_two_clients_ping
test "three_clients" test_three_clients
test "disconnect_one_other_still_works" test_disconnect_one_other_still_works

group_end
