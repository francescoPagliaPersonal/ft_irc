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
	sleep "$IRC_WAIT"
	irc_send ka "KICK #kck1 bobk1 :out"
	sleep "$IRC_WAIT"
	local aout bout
	aout=$(irc_recv ka)
	bout=$(irc_recv kb)
	irc_close ka
	irc_close kb
	assert_contains "$aout" "KICK #kck1 bobk1" \
		&& assert_contains "$bout" "KICK #kck1 bobk1"
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
	sleep "$IRC_WAIT"
	irc_send kd "KICK #kck2 alicek2 :nope"
	sleep "$IRC_WAIT"
	local out
	out=$(irc_recv kd)
	irc_close kc
	irc_close kd
	assert_contains "$out" " 482 "
}

test_kick_not_on_channel() {
	local out
	out=$(oneshot "PASS $PASSWORD" "NICK alicek3" "USER alicek3 0 * :x" \
		"KICK #kck3 nobody :x")
	assert_contains "$out" " 442 " || assert_contains "$out" " 403 "
}

test "kick_by_op" test_kick_by_op
test "kick_non_op" test_kick_non_op
test "kick_not_on_channel" test_kick_not_on_channel

group_end
