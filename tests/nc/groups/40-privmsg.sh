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

test "privmsg_nick" test_privmsg_nick
test "privmsg_channel_others" test_privmsg_channel_others
test "nosuchnick" test_nosuchnick
test "not_on_channel" test_not_on_channel

group_end
