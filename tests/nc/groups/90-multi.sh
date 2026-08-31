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
	if ! irc_expect xa "PONG CoolServ :one"; then
		irc_close xa
		irc_close xb
		return 1
	fi
	if ! irc_expect xb "PONG CoolServ :two"; then
		irc_close xa
		irc_close xb
		return 1
	fi
	assert_not_contains "$(irc_recv xa)" ":two"
	local rc=$?
	if [ "$rc" -eq 0 ]; then
		assert_not_contains "$(irc_recv xb)" ":one"
		rc=$?
	fi
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
	if ! irc_expect xc " 366 "; then
		irc_close xc
		irc_close xd
		irc_close xe
		return 1
	fi
	irc_send xd "JOIN #mlt1"
	if ! irc_expect xd " 366 "; then
		irc_close xc
		irc_close xd
		irc_close xe
		return 1
	fi
	irc_send xe "JOIN #mlt1"
	if ! irc_expect xe " 366 "; then
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
	if ! irc_expect xe "PRIVMSG #mlt1 :hi all"; then
		irc_close xc
		irc_close xd
		irc_close xe
		return 1
	fi
	assert_not_contains "$(irc_recv xc)" "PRIVMSG #mlt1 :hi all"
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
	irc_send xg "PRIVMSG alex6 :gone"
	if ! irc_expect xg " 401 "; then
		irc_close xg
		return 1
	fi
	irc_send xg "PING :stillhere"
	irc_expect xg "PONG CoolServ :stillhere"
	local rc=$?
	irc_close xg
	return "$rc"
}

test_junk_isolation() {
	if ! register_client xh alex8; then
		return 1
	fi
	if ! register_client xi alex9; then
		irc_close xh
		return 1
	fi
	irc_send xh $'   \r'
	irc_send xh ":nobody"
	irc_send xh "FOOBAR"
	irc_write xh "$(printf '%*s' 513 '' | tr ' ' 'Z')"
	wait_client_gone xh || true
	probe_alive xi isolated
	local rc=$?
	irc_close xh
	irc_close xi
	return "$rc"
}

test "two_clients_ping" test_two_clients_ping
test "three_clients" test_three_clients
test "disconnect_one_other_still_works" test_disconnect_one_other_still_works
test "junk_isolation" test_junk_isolation

group_end
