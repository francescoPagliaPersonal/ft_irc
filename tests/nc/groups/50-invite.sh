group_begin "INVITE"

test_invite_ok() {
	if ! register_client ia alicei1; then
		return 1
	fi
	if ! register_client ib bobi1; then
		irc_close ia
		return 1
	fi
	irc_send ia "JOIN #inv1"
	sleep "$IRC_WAIT"
	irc_send ia "INVITE bobi1 #inv1"
	sleep "$IRC_WAIT"
	local aout bout
	aout=$(irc_recv ia)
	bout=$(irc_recv ib)
	irc_close ia
	irc_close ib
	assert_contains "$aout" " 341 " && assert_contains "$bout" "INVITE"
}

test_invite_nosuchnick() {
	local out
	out=$(oneshot "PASS $PASSWORD" "NICK alicei2" "USER alicei2 0 * :x" \
		"JOIN #inv2" "INVITE ghosty #inv2")
	assert_contains "$out" " 401 "
}

test_invite_nosuchchannel() {
	local out
	out=$(oneshot "PASS $PASSWORD" "NICK alicei3" "USER alicei3 0 * :x" \
		"INVITE alicei3 #zzzz")
	assert_contains "$out" " 403 "
}

test_invite_notonchannel() {
	if ! register_client ic alicei4; then
		return 1
	fi
	if ! register_client id bobi4; then
		irc_close ic
		return 1
	fi
	if ! register_client ie caroli4; then
		irc_close ic
		irc_close id
		return 1
	fi
	irc_send ic "JOIN #inv3"
	sleep "$IRC_WAIT"
	irc_send id "INVITE caroli4 #inv3"
	sleep "$IRC_WAIT"
	local out
	out=$(irc_recv id)
	irc_close ic
	irc_close id
	irc_close ie
	assert_contains "$out" " 442 "
}

test_invite_useronchannel() {
	if ! register_client if alicei5; then
		return 1
	fi
	if ! register_client ig bobi5; then
		irc_close if
		return 1
	fi
	irc_send if "JOIN #inv4"
	irc_send ig "JOIN #inv4"
	sleep "$IRC_WAIT"
	irc_send if "INVITE bobi5 #inv4"
	sleep "$IRC_WAIT"
	local out
	out=$(irc_recv if)
	irc_close if
	irc_close ig
	assert_contains "$out" " 443 "
}

test "invite_ok" test_invite_ok
test "invite_nosuchnick" test_invite_nosuchnick
test "invite_nosuchchannel" test_invite_nosuchchannel
test "invite_notonchannel" test_invite_notonchannel
test "invite_useronchannel" test_invite_useronchannel

group_end
