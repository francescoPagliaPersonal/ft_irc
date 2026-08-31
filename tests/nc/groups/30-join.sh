group_begin "JOIN"

test_join_creates_op() {
	if ! register_client jo alicej1; then
		FAIL_HINT="register failed"
		LAST_GOT=$(irc_recv jo 2>/dev/null || true)
		return 1
	fi
	irc_send jo "JOIN #jop1"
	local out
	out=$(irc_recv jo)
	irc_close jo
	assert_contains "$out" " 353 " && assert_contains "$out" "@alicej1" \
		&& assert_contains "$out" " 366 "
}

test_second_is_regular() {
	if ! register_client ja alicej2; then
		return 1
	fi
	if ! register_client jb bobjoin; then
		irc_close ja
		return 1
	fi
	irc_send ja "JOIN #jop2"
	sleep "$IRC_WAIT"
	irc_send jb "JOIN #jop2"
	local out
	out=$(irc_recv jb)
	irc_close ja
	irc_close jb
	assert_contains "$out" " 353 " && assert_contains "$out" "bobjoin" \
		&& assert_not_contains "$out" "@bobjoin"
}

test_bad_mask() {
	local out
	out=$(oneshot "PASS $PASSWORD" "NICK badmask" "USER badmask 0 * :x" "JOIN nohash")
	assert_contains "$out" " 476 "
}

test_join_broadcast() {
	if ! register_client jc carolj; then
		return 1
	fi
	if ! register_client jd davej; then
		irc_close jc
		return 1
	fi
	irc_send jc "JOIN #jbc1"
	sleep "$IRC_WAIT"
	irc_send jd "JOIN #jbc1"
	sleep "$IRC_WAIT"
	local out
	out=$(irc_recv jc)
	irc_close jc
	irc_close jd
	assert_contains "$out" "davej" && assert_contains "$out" "JOIN #jbc1"
}

test "join_creates_op" test_join_creates_op
test "second_is_regular" test_second_is_regular
test "bad_mask" test_bad_mask
test "join_broadcast" test_join_broadcast

group_end
