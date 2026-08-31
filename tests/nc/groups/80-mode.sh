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
	if ! irc_expect ma "JOIN #mod1"; then
		irc_close ma
		irc_close mb
		return 1
	fi
	irc_send ma "MODE #mod1 +i"
	if ! irc_expect ma "MODE #mod1 +i"; then
		irc_close ma
		irc_close mb
		return 1
	fi
	irc_send mb "JOIN #mod1"
	irc_expect mb " 473 "
	local rc=$?
	irc_close ma
	irc_close mb
	return "$rc"
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
	if ! irc_expect mc "JOIN #mod2"; then
		irc_close mc
		irc_close md
		return 1
	fi
	irc_send mc "MODE #mod2 +k secret"
	if ! irc_expect mc "MODE #mod2 +k"; then
		irc_close mc
		irc_close md
		return 1
	fi
	irc_send md "JOIN #mod2 wrong"
	irc_expect md " 475 "
	local rc=$?
	irc_close mc
	irc_close md
	return "$rc"
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
	if ! irc_expect me "JOIN #mod3"; then
		irc_close me
		irc_close mf
		irc_close mg
		return 1
	fi
	irc_send me "MODE #mod3 +l 2"
	if ! irc_expect me "MODE #mod3 +l"; then
		irc_close me
		irc_close mf
		irc_close mg
		return 1
	fi
	irc_send mf "JOIN #mod3"
	if ! irc_expect mf "JOIN #mod3"; then
		irc_close me
		irc_close mf
		irc_close mg
		return 1
	fi
	irc_send mg "JOIN #mod3"
	irc_expect mg " 471 "
	local rc=$?
	irc_close me
	irc_close mf
	irc_close mg
	return "$rc"
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
	if ! irc_expect mi "JOIN #mod4"; then
		irc_close mh
		irc_close mi
		return 1
	fi
	irc_send mh "MODE #mod4 +o bobm4"
	irc_expect mi "MODE #mod4 +o bobm4"
	local rc=$?
	irc_close mh
	irc_close mi
	return "$rc"
}

test "mode_i_join_denied" test_mode_i_join_denied
test "mode_k_bad_key" test_mode_k_bad_key
test "mode_l_full" test_mode_l_full
test "mode_o_give_take" test_mode_o_give_take

group_end
