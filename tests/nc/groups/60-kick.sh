group_begin "KICK"

test_kick_by_op() {
	if ! register_client ka alicek1; then
		return 1
	fi
	if ! register_client kb bobk1; then
		irc_close ka
		return 1
	fi
	irc_send ka "JOIN #kck1"
	irc_send kb "JOIN #kck1"
	if ! irc_expect kb "JOIN #kck1"; then
		irc_close ka
		irc_close kb
		return 1
	fi
	irc_send ka "KICK #kck1 bobk1 :out"
	if ! irc_expect ka "KICK #kck1 bobk1"; then
		irc_close ka
		irc_close kb
		return 1
	fi
	irc_expect kb "KICK #kck1 bobk1"
	local rc=$?
	irc_close ka
	irc_close kb
	return "$rc"
}

test_kick_non_op() {
	if ! register_client kc alicek2; then
		return 1
	fi
	if ! register_client kd bobk2; then
		irc_close kc
		return 1
	fi
	irc_send kc "JOIN #kck2"
	irc_send kd "JOIN #kck2"
	if ! irc_expect kd "JOIN #kck2"; then
		irc_close kc
		irc_close kd
		return 1
	fi
	irc_send kd "KICK #kck2 alicek2 :nope"
	irc_expect kd " 482 "
	local rc=$?
	irc_close kc
	irc_close kd
	return "$rc"
}

test_kick_not_on_channel() {
	oneshot_expect_any " 442 " " 403 " "PASS $PASSWORD" "NICK alicek3" \
		"USER alicek3 0 * :x" "KICK #kck3 nobody :x"
}

test "kick_by_op" test_kick_by_op
test "kick_non_op" test_kick_non_op
test "kick_not_on_channel" test_kick_not_on_channel

group_end
