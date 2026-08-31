group_begin "TOPIC"

test_topic_set_view() {
	if ! register_client ta alicet1; then
		return 1
	fi
	irc_send ta "JOIN #top1"
	sleep "$IRC_WAIT"
	irc_send ta "TOPIC #top1 :hello topic"
	sleep "$IRC_WAIT"
	local out
	out=$(irc_recv ta)
	irc_close ta
	assert_contains "$out" " 332 " && assert_contains "$out" "hello topic"
}

test_topic_empty() {
	if ! register_client tb alicet2; then
		return 1
	fi
	irc_send tb "JOIN #top2"
	sleep "$IRC_WAIT"
	irc_send tb "TOPIC #top2"
	sleep "$IRC_WAIT"
	local out
	out=$(irc_recv tb)
	irc_close tb
	assert_contains "$out" " 331 "
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
	sleep "$IRC_WAIT"
	irc_send tc "MODE #top3 +t"
	irc_send td "JOIN #top3"
	sleep "$IRC_WAIT"
	irc_send td "TOPIC #top3 :hijack"
	sleep "$IRC_WAIT"
	local out
	out=$(irc_recv td)
	irc_close tc
	irc_close td
	assert_contains "$out" " 482 "
}

test "topic_set_view" test_topic_set_view
test "topic_empty" test_topic_empty
test "topic_non_op_plus_t" test_topic_non_op_plus_t

group_end
