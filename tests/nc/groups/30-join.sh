group_begin "JOIN"

test_join_creates_op() {
	if ! register_client jo alicej1; then
		FAIL_HINT="register failed"
		LAST_GOT=$(irc_recv jo 2>/dev/null || true)
		return 1
	fi
	irc_send jo "JOIN #jop1"
	irc_expect jo " 353 " "@alicej1" " 366 "
	local rc=$?
	irc_close jo
	return "$rc"
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
	if ! irc_expect ja " 353 "; then
		irc_close ja
		irc_close jb
		return 1
	fi
	irc_send jb "JOIN #jop2"
	if ! irc_expect jb " 353 " "bobjoin" " 366 "; then
		irc_close ja
		irc_close jb
		return 1
	fi
	assert_not_contains "$LAST_GOT" "@bobjoin"
	local rc=$?
	irc_close ja
	irc_close jb
	return "$rc"
}

test_bad_mask() {
	oneshot_expect " 476 " "PASS $PASSWORD" "NICK badmask" "USER badmask 0 * :x" "JOIN nohash"
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
	if ! irc_expect jc "JOIN #jbc1"; then
		irc_close jc
		irc_close jd
		return 1
	fi
	irc_send jd "JOIN #jbc1"
	irc_expect jc "davej" "JOIN #jbc1"
	local rc=$?
	irc_close jc
	irc_close jd
	return "$rc"
}

test "join_creates_op" test_join_creates_op
test "second_is_regular" test_second_is_regular
test "bad_mask" test_bad_mask
test "join_broadcast" test_join_broadcast

group_end
