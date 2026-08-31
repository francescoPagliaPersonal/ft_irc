group_begin "MODE"

test_mode_i_join_denied() {
	if ! register_client ma alicem1; then
		return 1
	fi
	if ! register_client mb bobm1; then
		irc_close ma
		return 1
	fi
	irc_send ma "JOIN #mod1"
	sleep "$IRC_WAIT"
	irc_send ma "MODE #mod1 +i"
	sleep "$IRC_WAIT"
	irc_send mb "JOIN #mod1"
	sleep "$IRC_WAIT"
	local out
	out=$(irc_recv mb)
	irc_close ma
	irc_close mb
	assert_contains "$out" " 473 "
}

test_mode_k_bad_key() {
	if ! register_client mc alicem2; then
		return 1
	fi
	if ! register_client md bobm2; then
		irc_close mc
		return 1
	fi
	irc_send mc "JOIN #mod2"
	sleep "$IRC_WAIT"
	irc_send mc "MODE #mod2 +k secret"
	sleep "$IRC_WAIT"
	irc_send md "JOIN #mod2 wrong"
	sleep "$IRC_WAIT"
	local out
	out=$(irc_recv md)
	irc_close mc
	irc_close md
	assert_contains "$out" " 475 "
}

test_mode_l_full() {
	if ! register_client me alicem3; then
		return 1
	fi
	if ! register_client mf bobm3; then
		irc_close me
		return 1
	fi
	if ! register_client mg carolm3; then
		irc_close me
		irc_close mf
		return 1
	fi
	irc_send me "JOIN #mod3"
	sleep "$IRC_WAIT"
	irc_send me "MODE #mod3 +l 2"
	irc_send mf "JOIN #mod3"
	sleep "$IRC_WAIT"
	irc_send mg "JOIN #mod3"
	sleep "$IRC_WAIT"
	local out
	out=$(irc_recv mg)
	irc_close me
	irc_close mf
	irc_close mg
	assert_contains "$out" " 471 "
}

test_mode_o_give_take() {
	if ! register_client mh alicem4; then
		return 1
	fi
	if ! register_client mi bobm4; then
		irc_close mh
		return 1
	fi
	irc_send mh "JOIN #mod4"
	irc_send mi "JOIN #mod4"
	sleep "$IRC_WAIT"
	irc_send mh "MODE #mod4 +o bobm4"
	sleep "$IRC_WAIT"
	local out
	out=$(irc_recv mi)
	irc_close mh
	irc_close mi
	assert_contains "$out" "MODE #mod4 +o bobm4"
}

test "mode_i_join_denied" test_mode_i_join_denied
test "mode_k_bad_key" test_mode_k_bad_key
test "mode_l_full" test_mode_l_full
test "mode_o_give_take" test_mode_o_give_take

group_end
