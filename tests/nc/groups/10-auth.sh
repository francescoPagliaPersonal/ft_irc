group_begin "AUTH"

test_pass_ok() {
	local out
	out=$(oneshot "PASS $PASSWORD" "NICK aliceok" "USER aliceok 0 * :Alice")
	assert_contains "$out" " 001 "
}

test_pass_mismatch() {
	local out
	out=$(oneshot "PASS wrongpw" "NICK alicebad" "USER alicebad 0 * :Alice")
	assert_contains "$out" " 464 "
}

test_nick_in_use() {
	local out
	if ! register_client a1 aliceuse; then
		FAIL_HINT="first client did not get 001"
		LAST_GOT=$(irc_recv a1)
		return 1
	fi
	out=$(oneshot "PASS $PASSWORD" "NICK aliceuse" "USER otheru 0 * :Other")
	irc_close a1
	assert_contains "$out" " 433 "
}

test_second_user() {
	local out
	out=$(oneshot "PASS $PASSWORD" "NICK alicereg" "USER alicereg 0 * :A" \
		"USER alicereg 0 * :again")
	assert_contains "$out" " 462 "
}

test_join_before_register() {
	local out
	out=$(oneshot "JOIN #lobby")
	assert_contains "$out" " 451 "
}

test_nick_erroneous() {
	local out
	out=$(oneshot "PASS $PASSWORD" "NICK #badnick")
	assert_contains "$out" " 432 "
}

test "pass_ok" test_pass_ok
test "pass_mismatch" test_pass_mismatch
test "nick_in_use" test_nick_in_use
test "second_user" test_second_user
test "join_before_register" test_join_before_register
test "nick_erroneous" test_nick_erroneous

group_end
