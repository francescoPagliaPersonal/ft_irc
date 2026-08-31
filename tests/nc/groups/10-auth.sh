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

test_nick_too_long() {
	local long
	long=$(printf '%*s' 33 '' | tr ' ' 'n')
	oneshot_expect " 432 " "PASS $PASSWORD" "NICK $long"
}

test_nick_dot() {
	oneshot_expect " 432 " "PASS $PASSWORD" "NICK .dot"
}

test_nick_star() {
	oneshot_expect " 432 " "PASS $PASSWORD" "NICK *star"
}

test_nick_no_param() {
	oneshot_expect_any " 461 " " 431 " "PASS $PASSWORD" "NICK"
}

test_nick_trailing_only() {
	# Policy counts trailing as an arg; handler reads params[0] — crash candidate.
	irc_open nt
	irc_send nt "PASS $PASSWORD"
	irc_send nt "NICK :ntrail"
	irc_expect_any nt " 432 " " 001 " " 461 " " 431 "
	local rc=$?
	irc_close nt
	return "$rc"
}

test_user_too_few() {
	oneshot_expect " 461 " "PASS $PASSWORD" "NICK ufew" "USER ufew 0 *"
}

test_user_no_params() {
	oneshot_expect " 461 " "PASS $PASSWORD" "NICK uzero" "USER"
}

test_user_too_many() {
	irc_open um
	irc_send um "PASS $PASSWORD"
	irc_send um "NICK umany"
	irc_send um "USER a b c d e"
	irc_expect_any um "42001" " 461 "
	local rc=$?
	irc_close um
	return "$rc"
}

test_pass_trailing_only() {
	# PASS :pw has trailing and no params — may 001, 461, 464, or crash.
	irc_open pt
	irc_send pt "PASS :$PASSWORD"
	irc_send pt "NICK ptrail"
	irc_send pt "USER ptrail 0 * :x"
	irc_expect_any pt " 001 " " 461 " " 464 "
	local rc=$?
	irc_close pt
	return "$rc"
}

test_pass_extra_spaces() {
	oneshot_expect " 001 " "PASS    $PASSWORD" "NICK pspace" "USER pspace 0 * :x"
}

test "pass_ok" test_pass_ok
test "pass_mismatch" test_pass_mismatch
test "nick_in_use" test_nick_in_use
test "second_user" test_second_user
test "join_before_register" test_join_before_register
test "nick_erroneous" test_nick_erroneous
test "nick_too_long" test_nick_too_long
test "nick_dot" test_nick_dot
test "nick_star" test_nick_star
test "nick_no_param" test_nick_no_param
test "nick_trailing_only" test_nick_trailing_only
test "user_too_few" test_user_too_few
test "user_no_params" test_user_no_params
test "user_too_many" test_user_too_many
test "pass_trailing_only" test_pass_trailing_only
test "pass_extra_spaces" test_pass_extra_spaces

group_end
