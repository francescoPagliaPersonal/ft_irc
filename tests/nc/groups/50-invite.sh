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

test_invite_no_args() {
	oneshot_expect " 461 " "PASS $PASSWORD" "NICK invw0" "USER invw0 0 * :x" "INVITE"
}

test_invite_one_arg() {
	oneshot_expect " 461 " "PASS $PASSWORD" "NICK invw1" "USER invw1 0 * :x" "INVITE onlyone"
}

test_invite_swapped_args() {
	if ! register_client iw1 aliceiw1; then
		return 1
	fi
	if ! register_client iw2 bobiw1; then
		irc_close iw1
		return 1
	fi
	irc_send iw1 "JOIN #invw"
	if ! irc_expect iw1 "JOIN #invw"; then
		irc_close iw1
		irc_close iw2
		return 1
	fi
	irc_send iw1 "INVITE #invw bobiw1"
	irc_expect_any iw1 " 401 " " 403 "
	local rc=$?
	irc_close iw1
	irc_close iw2
	return "$rc"
}

test_invite_mixed_case() {
	if ! register_client iw3 aliceiw2; then
		return 1
	fi
	if ! register_client iw4 bobiw2; then
		irc_close iw3
		return 1
	fi
	irc_send iw3 "JOIN #invx"
	if ! irc_expect iw3 "JOIN #invx"; then
		irc_close iw3
		irc_close iw4
		return 1
	fi
	irc_send iw3 "  invite   bobiw2   #invx"
	if ! irc_expect iw3 " 341 "; then
		irc_close iw3
		irc_close iw4
		return 1
	fi
	probe_alive iw3 invmix
	local rc=$?
	irc_close iw3
	irc_close iw4
	return "$rc"
}

test "invite_ok" test_invite_ok
test "invite_nosuchnick" test_invite_nosuchnick
test "invite_nosuchchannel" test_invite_nosuchchannel
test "invite_notonchannel" test_invite_notonchannel
test "invite_useronchannel" test_invite_useronchannel
test "invite_no_args" test_invite_no_args
test "invite_one_arg" test_invite_one_arg
test "invite_swapped_args" test_invite_swapped_args
test "invite_mixed_case" test_invite_mixed_case

group_end
