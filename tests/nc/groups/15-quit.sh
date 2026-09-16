group_begin "QUIT"

test_quit_with_reason() {
	if ! register_client qa aliceq1; then
		return 1
	fi
	if ! register_client qb bobq1; then
		irc_close qa
		return 1
	fi
	irc_send qa "JOIN #quit1"
	if ! irc_expect qa "JOIN #quit1"; then
		irc_close qa
		irc_close qb
		return 1
	fi
	irc_send qb "JOIN #quit1"
	if ! irc_expect qb "JOIN #quit1"; then
		irc_close qa
		irc_close qb
		return 1
	fi
	irc_send qa "QUIT :bye"
	if ! irc_expect qa "ERROR"; then
		irc_close qa
		irc_close qb
		return 1
	fi
	irc_expect qb " QUIT "
	local rc=$?
	if [ "$rc" -eq 0 ]; then
		assert_contains "$(irc_recv qb)" "bye" || rc=$?
	fi
	irc_close qa
	irc_close qb
	return "$rc"
}

test "quit_with_reason" test_quit_with_reason

group_end
