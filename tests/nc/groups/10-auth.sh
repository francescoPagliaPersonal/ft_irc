group_begin "AUTH"

test_pass_ok() {
	oneshot_expect " 001 " "PASS $PASSWORD" "NICK aliceok" "USER aliceok 0 * :Alice"
}

test_pass_mismatch() {
	oneshot_expect " 464 " "PASS wrongpw" "NICK alicebad" "USER alicebad 0 * :Alice"
}

test_nick_in_use() {
	if ! register_client a1 aliceuse; then
		FAIL_HINT="first client did not get 001"
		LAST_GOT=$(irc_recv a1)
		return 1
	fi
	oneshot_expect " 433 " "PASS $PASSWORD" "NICK aliceuse" "USER otheru 0 * :Other"
	local rc=$?
	irc_close a1
	return "$rc"
}

test_second_user() {
	oneshot_expect " 462 " "PASS $PASSWORD" "NICK alicereg" "USER alicereg 0 * :A" \
		"USER alicereg 0 * :again"
}

test_join_before_register() {
	oneshot_expect " 451 " "JOIN #lobby"
}

test_nick_erroneous() {
	oneshot_expect " 432 " "PASS $PASSWORD" "NICK #badnick"
}

test "pass_ok" test_pass_ok
test "pass_mismatch" test_pass_mismatch
test "nick_in_use" test_nick_in_use
test "second_user" test_second_user
test "join_before_register" test_join_before_register
test "nick_erroneous" test_nick_erroneous

group_end
