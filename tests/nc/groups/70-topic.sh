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

test "topic_set_view" test_topic_set_view
test "topic_empty" test_topic_empty
test "topic_non_op_plus_t" test_topic_non_op_plus_t

group_end
