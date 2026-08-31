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
	if ! irc_expect ia "JOIN #inv1"; then
		irc_close ia
		irc_close ib
		return 1
	fi
	irc_send ia "INVITE bobi1 #inv1"
	if ! irc_expect ia " 341 "; then
		irc_close ia
		irc_close ib
		return 1
	fi
	irc_expect ib "INVITE"
	local rc=$?
	irc_close ia
	irc_close ib
	return "$rc"
}

test_invite_nosuchnick() {
	oneshot_expect " 401 " "PASS $PASSWORD" "NICK alicei2" "USER alicei2 0 * :x" \
		"JOIN #inv2" "INVITE ghosty #inv2"
}

test_invite_nosuchchannel() {
	oneshot_expect " 403 " "PASS $PASSWORD" "NICK alicei3" "USER alicei3 0 * :x" \
		"INVITE alicei3 #zzzz"
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
	if ! irc_expect ic "JOIN #inv3"; then
		irc_close ic
		irc_close id
		irc_close ie
		return 1
	fi
	irc_send id "INVITE caroli4 #inv3"
	irc_expect id " 442 "
	local rc=$?
	irc_close ic
	irc_close id
	irc_close ie
	return "$rc"
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
	if ! irc_expect ig "JOIN #inv4"; then
		irc_close if
		irc_close ig
		return 1
	fi
	irc_send if "INVITE bobi5 #inv4"
	irc_expect if " 443 "
	local rc=$?
	irc_close if
	irc_close ig
	return "$rc"
}

test "invite_ok" test_invite_ok
test "invite_nosuchnick" test_invite_nosuchnick
test "invite_nosuchchannel" test_invite_nosuchchannel
test "invite_notonchannel" test_invite_notonchannel
test "invite_useronchannel" test_invite_useronchannel

group_end
