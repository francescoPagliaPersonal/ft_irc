group_begin "KICK"

test_kick_by_op() {
	if ! register_client ka alicek1; then
		return 1
	fi
	if ! register_client kb bobk1; then
		irc_close ka
		return 1
	fi
	irc_send ka "JOIN #kck1"
	if ! irc_expect ka " 353 " "@alicek1" " 366 "; then
		irc_close ka
		irc_close kb
		return 1
	fi
	irc_send kb "JOIN #kck1"
	if ! irc_expect kb "JOIN #kck1"; then
		irc_close ka
		irc_close kb
		return 1
	fi
	irc_send ka "KICK #kck1 bobk1 :out"
	if ! irc_expect ka "KICK #kck1 bobk1"; then
		irc_close ka
		irc_close kb
		return 1
	fi
	irc_expect kb "KICK #kck1 bobk1"
	local rc=$?
	irc_close ka
	irc_close kb
	return "$rc"
}

test_kick_non_op() {
	if ! register_client kc alicek2; then
		return 1
	fi
	if ! register_client kd bobk2; then
		irc_close kc
		return 1
	fi
	irc_send kc "JOIN #kck2"
	if ! irc_expect kc " 353 " "@alicek2" " 366 "; then
		irc_close kc
		irc_close kd
		return 1
	fi
	irc_send kd "JOIN #kck2"
	if ! irc_expect kd "JOIN #kck2"; then
		irc_close kc
		irc_close kd
		return 1
	fi
	irc_send kd "KICK #kck2 alicek2 :nope"
	irc_expect kd " 482 "
	local rc=$?
	irc_close kc
	irc_close kd
	return "$rc"
}

test_kick_not_on_channel() {
	oneshot_expect_any " 442 " " 403 " "PASS $PASSWORD" "NICK alicek3" \
		"USER alicek3 0 * :x" "KICK #kck3 nobody :x"
}

test_kick_multi_and_rejoin_op() {
	if ! register_client kk alicek4; then
		return 1
	fi
	if ! register_client kl bobk4; then
		irc_close kk
		return 1
	fi
	if ! register_client km carolk4; then
		irc_close kk
		irc_close kl
		return 1
	fi
	irc_send kk "JOIN #kck4"
	if ! irc_expect kk " 353 " "@alicek4" " 366 "; then
		irc_close kk
		irc_close kl
		irc_close km
		return 1
	fi
	irc_send kl "JOIN #kck4"
	if ! irc_expect kl "JOIN #kck4"; then
		irc_close kk
		irc_close kl
		irc_close km
		return 1
	fi
	irc_send km "JOIN #kck4"
	if ! irc_expect km "JOIN #kck4"; then
		irc_close kk
		irc_close kl
		irc_close km
		return 1
	fi
	irc_send kk "KICK #kck4 bobk4,carolk4,alicek4"
	irc_send kl "JOIN #kck4"
	if ! irc_expect kl " 353 " "@bobk4" " 366 "; then
		irc_close kk
		irc_close kl
		irc_close km
		return 1
	fi
	probe_alive kl kickrejoin
	local rc=$?
	irc_close kk
	irc_close kl
	irc_close km
	return "$rc"
}

test "kick_by_op" test_kick_by_op
test "kick_non_op" test_kick_non_op
test "kick_not_on_channel" test_kick_not_on_channel
test "kick_multi_and_rejoin_op" test_kick_multi_and_rejoin_op

group_end
