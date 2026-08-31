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

test_join_no_args() {
	oneshot_expect " 461 " "PASS $PASSWORD" "NICK jnone" "USER jnone 0 * :x" "JOIN"
}

test_join_too_short() {
	oneshot_expect " 476 " "PASS $PASSWORD" "NICK jshort" "USER jshort 0 * :x" "JOIN #ab"
}

test_join_too_long() {
	local ch
	ch="#$(printf '%*s' 32 '' | tr ' ' 'x')"
	oneshot_expect " 476 " "PASS $PASSWORD" "NICK jlongc" "USER jlongc 0 * :x" "JOIN $ch"
}

test_join_too_many_channels() {
	if ! register_client jt jfour; then
		return 1
	fi
	irc_send jt "JOIN #jok1,#jok2,#jok3,#jok4"
	irc_expect jt " 405 "
	local rc=$?
	irc_close jt
	return "$rc"
}

test_join_trailing_comma() {
	if ! register_client jc2 jtrc; then
		return 1
	fi
	irc_send jc2 "JOIN #jtrc,"
	if ! irc_expect_any jc2 " 353 " " 476 "; then
		irc_close jc2
		return 1
	fi
	probe_alive jc2 jtrca
	local rc=$?
	irc_close jc2
	return "$rc"
}

test_join_empty_slot() {
	if ! register_client je jemp; then
		return 1
	fi
	irc_send je "JOIN #jemp,,#jxxx"
	if ! irc_expect je " 476 "; then
		irc_close je
		return 1
	fi
	probe_alive je jempa
	local rc=$?
	irc_close je
	return "$rc"
}

test_join_zero() {
	oneshot_expect " 476 " "PASS $PASSWORD" "NICK jzero" "USER jzero 0 * :x" "JOIN 0"
}

test "join_creates_op" test_join_creates_op
test "second_is_regular" test_second_is_regular
test "bad_mask" test_bad_mask
test "join_broadcast" test_join_broadcast
test "join_no_args" test_join_no_args
test "join_too_short" test_join_too_short
test "join_too_long" test_join_too_long
test "join_too_many_channels" test_join_too_many_channels
test "join_trailing_comma" test_join_trailing_comma
test "join_empty_slot" test_join_empty_slot
test "join_zero" test_join_zero

group_end
