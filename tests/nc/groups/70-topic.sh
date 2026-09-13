group_begin "TOPIC"

test_topic_set_view() {
	if ! register_client ta alicet1; then
		return 1
	fi
	irc_send ta "JOIN #top1"
	if ! irc_expect ta "JOIN #top1"; then
		irc_close ta
		return 1
	fi
	irc_send ta "TOPIC #top1 :hello topic"
	irc_expect ta " 332 " "hello topic"
	local rc=$?
	irc_close ta
	return "$rc"
}

test_topic_empty() {
	if ! register_client tb alicet2; then
		return 1
	fi
	irc_send tb "JOIN #top2"
	if ! irc_expect tb "JOIN #top2"; then
		irc_close tb
		return 1
	fi
	irc_send tb "TOPIC #top2"
	irc_expect tb " 331 "
	local rc=$?
	irc_close tb
	return "$rc"
}

test_topic_non_op_plus_t() {
	if ! register_client tc alicet3; then
		return 1
	fi
	if ! register_client td bobt3; then
		irc_close tc
		return 1
	fi
	irc_send tc "JOIN #top3"
	if ! irc_expect tc "JOIN #top3"; then
		irc_close tc
		irc_close td
		return 1
	fi
	irc_send tc "MODE #top3 +t"
	if ! irc_expect tc "MODE #top3 +t"; then
		irc_close tc
		irc_close td
		return 1
	fi
	irc_send td "JOIN #top3"
	if ! irc_expect td "JOIN #top3"; then
		irc_close tc
		irc_close td
		return 1
	fi
	irc_send td "TOPIC #top3 :hijack"
	irc_expect td " 482 "
	local rc=$?
	irc_close tc
	irc_close td
	return "$rc"
}

test_topic_clear() {
	if ! register_client te alicet4; then
		return 1
	fi
	irc_send te "JOIN #top4"
	if ! irc_expect te "JOIN #top4"; then
		irc_close te
		return 1
	fi
	irc_send te "TOPIC #top4 :hello"
	if ! irc_expect te " 332 " "hello"; then
		irc_close te
		return 1
	fi
	irc_send te "TOPIC #top4 :"
	if ! irc_expect te " 331 "; then
		irc_close te
		return 1
	fi
	probe_alive te topclr
	local rc=$?
	irc_close te
	return "$rc"
}

test_topic_non_op_without_plus_t() {
	if ! register_client tf alicet5; then
		return 1
	fi
	if ! register_client tg bobt5; then
		irc_close tf
		return 1
	fi
	irc_send tf "JOIN #top5"
	if ! irc_expect tf "JOIN #top5"; then
		irc_close tf
		irc_close tg
		return 1
	fi
	irc_send tg "JOIN #top5"
	if ! irc_expect tg "JOIN #top5"; then
		irc_close tf
		irc_close tg
		return 1
	fi
	irc_send tg "TOPIC #top5 :member sets"
	if ! irc_expect tf "member sets"; then
		irc_close tf
		irc_close tg
		return 1
	fi
	probe_alive tg topmem
	local rc=$?
	irc_close tf
	irc_close tg
	return "$rc"
}

test "topic_set_view" test_topic_set_view
test "topic_empty" test_topic_empty
test "topic_non_op_plus_t" test_topic_non_op_plus_t
test "topic_clear" test_topic_clear
test "topic_non_op_without_plus_t" test_topic_non_op_without_plus_t

group_end
