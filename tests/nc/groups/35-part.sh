group_begin "PART"

test_part_leave() {
	if ! register_client pa alicep1; then
		return 1
	fi
	if ! register_client pb bobp1; then
		irc_close pa
		return 1
	fi
	irc_send pa "JOIN #part1"
	if ! irc_expect pa "JOIN #part1"; then
		irc_close pa
		irc_close pb
		return 1
	fi
	irc_send pb "JOIN #part1"
	if ! irc_expect pb "JOIN #part1"; then
		irc_close pa
		irc_close pb
		return 1
	fi
	irc_send pb "PART #part1 :leaving"
	if ! irc_expect pa " PART #part1 :leaving"; then
		irc_close pa
		irc_close pb
		return 1
	fi
	probe_alive pa partok
	local rc=$?
	irc_close pa
	irc_close pb
	return "$rc"
}

test_part_last_member() {
	if ! register_client pc alicep2; then
		return 1
	fi
	irc_send pc "JOIN #part2"
	if ! irc_expect pc "JOIN #part2"; then
		irc_close pc
		return 1
	fi
	irc_send pc "PART #part2"
	probe_alive pc partlast
	local rc=$?
	irc_close pc
	return "$rc"
}

test_part_not_on_channel() {
	oneshot_expect " 442 " "PASS $PASSWORD" "NICK part442" "USER part442 0 * :x" \
		"PART #part442"
}

test_part_op_failover() {
	if ! register_client pf alicepf; then
		return 1
	fi
	if ! register_client pg bobpf; then
		irc_close pf
		return 1
	fi
	irc_send pf "JOIN #partfo"
	if ! irc_expect pf " 353 " "@alicepf" " 366 "; then
		irc_close pf
		irc_close pg
		return 1
	fi
	irc_send pg "JOIN #partfo"
	if ! irc_expect pg "JOIN #partfo"; then
		irc_close pf
		irc_close pg
		return 1
	fi
	irc_send pf "PART #partfo"
	if ! irc_expect pg " PART #partfo"; then
		irc_close pf
		irc_close pg
		return 1
	fi
	irc_send pg "MODE #partfo +i"
	irc_expect pg "MODE #partfo +i"
	local rc=$?
	irc_close pf
	irc_close pg
	return "$rc"
}

test "part_leave" test_part_leave
test "part_last_member" test_part_last_member
test "part_not_on_channel" test_part_not_on_channel
test "part_op_failover" test_part_op_failover

group_end
