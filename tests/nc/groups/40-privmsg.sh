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
	sleep "$IRC_WAIT"
	local out
	out=$(irc_recv pb)
	irc_close pa
	irc_close pb
	assert_contains "$out" "PRIVMSG bobp1 :hello bob"
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
	irc_send pd "JOIN #pmsg"
	sleep "$IRC_WAIT"
	irc_send pc "PRIVMSG #pmsg :chan hello"
	sleep "$IRC_WAIT"
	local bout aout
	bout=$(irc_recv pd)
	aout=$(irc_recv pc)
	irc_close pc
	irc_close pd
	assert_contains "$bout" "PRIVMSG #pmsg :chan hello" \
		&& assert_not_contains "$aout" "PRIVMSG #pmsg :chan hello"
}

test_nosuchnick() {
	local out
	out=$(oneshot "PASS $PASSWORD" "NICK alicep3" "USER alicep3 0 * :x" \
		"PRIVMSG nosuch :hi")
	assert_contains "$out" " 401 "
}

test_not_on_channel() {
	if ! register_client pe alicep5; then
		return 1
	fi
	irc_send pe "JOIN #nmem"
	sleep "$IRC_WAIT"
	local out
	out=$(oneshot "PASS $PASSWORD" "NICK alicep4" "USER alicep4 0 * :x" \
		"PRIVMSG #nmem :nope")
	irc_close pe
	assert_contains "$out" " 404 "
}

test "privmsg_nick" test_privmsg_nick
test "privmsg_channel_others" test_privmsg_channel_others
test "nosuchnick" test_nosuchnick
test "not_on_channel" test_not_on_channel

group_end
