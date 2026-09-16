group_begin "PRIVMSG"

test_privmsg_nick() {
	if ! register_client pa alicep1; then
		return 1
	fi
	if ! register_client pb bobp1; then
		irc_close pa
		return 1
	fi
	irc_send pa "PRIVMSG bobp1 :hello bob"
	irc_expect pb "PRIVMSG bobp1 :hello bob"
	local rc=$?
	irc_close pa
	irc_close pb
	return "$rc"
}

test_privmsg_channel_others() {
	if ! register_client pc alicep2; then
		return 1
	fi
	if ! register_client pd bobp2; then
		irc_close pc
		return 1
	fi
	irc_send pc "JOIN #pmsg"
	if ! irc_expect pc "JOIN #pmsg"; then
		irc_close pc
		irc_close pd
		return 1
	fi
	irc_send pd "JOIN #pmsg"
	if ! irc_expect pd "JOIN #pmsg"; then
		irc_close pc
		irc_close pd
		return 1
	fi
	irc_send pc "PRIVMSG #pmsg :chan hello"
	if ! irc_expect pd "PRIVMSG #pmsg :chan hello"; then
		irc_close pc
		irc_close pd
		return 1
	fi
	assert_not_contains "$(irc_recv pc)" "PRIVMSG #pmsg :chan hello"
	local rc=$?
	irc_close pc
	irc_close pd
	return "$rc"
}

test_nosuchnick() {
	oneshot_expect " 401 " "PASS $PASSWORD" "NICK alicep3" "USER alicep3 0 * :x" \
		"PRIVMSG nosuch :hi"
}

test_not_on_channel() {
	if ! register_client pe alicep5; then
		return 1
	fi
	irc_send pe "JOIN #nmem"
	if ! irc_expect pe "JOIN #nmem"; then
		irc_close pe
		return 1
	fi
	oneshot_expect " 404 " "PASS $PASSWORD" "NICK alicep4" "USER alicep4 0 * :x" \
		"PRIVMSG #nmem :nope"
	local rc=$?
	irc_close pe
	return "$rc"
}

test_privmsg_no_text() {
	if ! register_client pn1 alicepw1; then
		return 1
	fi
	irc_send pn1 "PRIVMSG alicepw1"
	irc_expect_any pn1 " 461 " " 412 "
	local rc=$?
	irc_close pn1
	return "$rc"
}

test_privmsg_only_trailing() {
	# KNOWN FAILURE: the trailing is taken as the recipient, so this answers 401.
	if ! register_client pn2 alicepw2; then
		return 1
	fi
	irc_send pn2 "PRIVMSG :onlytext"
	irc_expect_any pn2 " 461 " " 411 "
	local rc=$?
	irc_close pn2
	return "$rc"
}

test_privmsg_no_colon() {
	if ! register_client pn3 alicepw3; then
		return 1
	fi
	if ! register_client pn4 bobpw3; then
		irc_close pn3
		return 1
	fi
	irc_send pn3 "PRIVMSG bobpw3 hello"
	irc_expect_any pn3 " 412 " " 461 "
	local rc=$?
	irc_close pn3
	irc_close pn4
	return "$rc"
}

test_privmsg_empty_comma() {
	# Empty recipient in a comma list: recipients[i][0] is a crash candidate.
	if ! register_client pn5 alicepw4; then
		return 1
	fi
	if ! register_client pn6 bobpw4; then
		irc_close pn5
		return 1
	fi
	irc_send pn5 "JOIN #pwld"
	if ! irc_expect pn5 "JOIN #pwld"; then
		irc_close pn5
		irc_close pn6
		return 1
	fi
	irc_send pn6 "JOIN #pwld"
	if ! irc_expect pn6 "JOIN #pwld"; then
		irc_close pn5
		irc_close pn6
		return 1
	fi
	irc_send pn5 "PRIVMSG #pwld,,bobpw4 :hi"
	probe_alive pn6 pempty
	local rc=$?
	irc_close pn5
	irc_close pn6
	return "$rc"
}

test_privmsg_two_nicks() {
	if ! register_client pn7 alicepw5; then
		return 1
	fi
	if ! register_client pn8 bobpw5; then
		irc_close pn7
		return 1
	fi
	if ! register_client pn9 carolpw5; then
		irc_close pn7
		irc_close pn8
		return 1
	fi
	irc_send pn7 "PRIVMSG bobpw5,carolpw5 :hi both"
	if ! irc_expect pn8 "PRIVMSG bobpw5 :hi both"; then
		irc_close pn7
		irc_close pn8
		irc_close pn9
		return 1
	fi
	irc_expect pn9 "PRIVMSG carolpw5 :hi both"
	local rc=$?
	irc_close pn7
	irc_close pn8
	irc_close pn9
	return "$rc"
}

test_privmsg_long_trailing() {
	local body
	if ! register_client pn10 alicepw6; then
		return 1
	fi
	if ! register_client pn11 bobpw6; then
		irc_close pn10
		return 1
	fi
	body=$(printf '%*s' 400 '' | tr ' ' 'L')
	irc_send pn10 "PRIVMSG bobpw6 :$body"
	if ! irc_expect pn11 "PRIVMSG bobpw6 :$body"; then
		# delivered or dropped is acceptable; session must survive
		probe_alive pn10 longok
		local rc=$?
		irc_close pn10
		irc_close pn11
		return "$rc"
	fi
	probe_alive pn10 longok2
	local rc=$?
	irc_close pn10
	irc_close pn11
	return "$rc"
}

test "privmsg_nick" test_privmsg_nick
test "privmsg_channel_others" test_privmsg_channel_others
test "nosuchnick" test_nosuchnick
test "not_on_channel" test_not_on_channel
test "privmsg_no_text" test_privmsg_no_text
test "privmsg_only_trailing" test_privmsg_only_trailing
test "privmsg_no_colon" test_privmsg_no_colon
test "privmsg_empty_comma" test_privmsg_empty_comma
test "privmsg_two_nicks" test_privmsg_two_nicks
test "privmsg_long_trailing" test_privmsg_long_trailing

group_end
