group_begin "PARSER"

test_mixed_case_ping() {
	if ! register_client pc pcase; then
		return 1
	fi
	irc_send pc "pInG :${SRVNAME}"
	irc_expect pc "$PONG_NEEDLE"
	local rc=$?
	irc_close pc
	return "$rc"
}

test_client_prefix_ping() {
	if ! register_client pp ppref; then
		return 1
	fi
	irc_send pp ":ppref PING :${SRVNAME}"
	irc_expect pp "$PONG_NEEDLE"
	local rc=$?
	irc_close pp
	return "$rc"
}

test_ping_no_args() {
	if ! register_client pn pnone; then
		return 1
	fi
	irc_send pn "PING"
	irc_expect pn " 461 "
	local rc=$?
	irc_close pn
	return "$rc"
}

test_ping_too_many_params() {
	if ! register_client pm pmany; then
		return 1
	fi
	irc_send pm "PING a b c"
	irc_expect_any pm " 400 " " 461 "
	local rc=$?
	irc_close pm
	return "$rc"
}

test_tab_separator() {
	if ! register_client pt ptab; then
		return 1
	fi
	irc_send pt $'PING\thello'
	irc_expect pt " 421 "
	local rc=$?
	irc_close pt
	return "$rc"
}

test_ping_no_space_colon() {
	if ! register_client ps pcolon; then
		return 1
	fi
	irc_send ps "PING:nospace"
	irc_expect ps " 421 "
	local rc=$?
	irc_close ps
	return "$rc"
}

test_unknown_numeric_cmd() {
	if ! register_client pu pnum; then
		return 1
	fi
	irc_send pu "123"
	irc_expect pu " 421 "
	local rc=$?
	irc_close pu
	return "$rc"
}

test_unknown_bang() {
	if ! register_client pb pbang; then
		return 1
	fi
	irc_send pb "!"
	irc_expect pb " 421 "
	local rc=$?
	irc_close pb
	return "$rc"
}

test_join_no_params() {
	if ! register_client pj pjoin; then
		return 1
	fi
	irc_send pj "JOIN"
	irc_expect pj " 461 "
	local rc=$?
	irc_close pj
	return "$rc"
}

test_privmsg_no_params() {
	if ! register_client pv pmsg0; then
		return 1
	fi
	irc_send pv "PRIVMSG"
	irc_expect pv " 461 "
	local rc=$?
	irc_close pv
	return "$rc"
}

test_ping_extra_spaces() {
	if ! register_client px pspaces; then
		return 1
	fi
	irc_send px "PING     :${SRVNAME}"
	irc_expect px "$PONG_NEEDLE"
	local rc=$?
	irc_close px
	return "$rc"
}

test_flood_pings() {
	local i msg=""
	irc_open_raw pf
	for i in $(seq 1 30); do
		msg="${msg}PING :${SRVNAME}"$'\r\n'
	done
	irc_write pf "$msg"
	irc_expect_count pf "$PONG_NEEDLE" 30
	local rc=$?
	irc_close pf
	return "$rc"
}

test_utf8_ping() {
	if ! register_client pu8 putf; then
		return 1
	fi
	irc_send pu8 "PING :${SRVNAME}"
	irc_expect pu8 "$PONG_NEEDLE"
	local rc=$?
	irc_close pu8
	return "$rc"
}

test_ctcp_ping() {
	if ! register_client pa pact; then
		return 1
	fi
	irc_send pa "PING :${SRVNAME}"
	irc_expect pa "$PONG_NEEDLE"
	local rc=$?
	irc_close pa
	return "$rc"
}

test_nul_in_line() {
	local fd
	irc_open_raw pz
	fd=$(cat "$RUNDIR/cli_pz/wfd" 2>/dev/null) || { irc_close pz; return 1; }
	printf 'PING\0 :x\r\n' >&"$fd" 2>/dev/null || { irc_close pz; return 1; }
	# leftover bytes before the NUL may sit without a CRLF; flush then probe
	irc_write pz $'\r\n'
	probe_alive pz nulafter
	local rc=$?
	irc_close pz
	return "$rc"
}

test "mixed_case_ping" test_mixed_case_ping
test "client_prefix_ping" test_client_prefix_ping
test "ping_no_args" test_ping_no_args
test "ping_too_many_params" test_ping_too_many_params
test "tab_separator" test_tab_separator
test "ping_no_space_colon" test_ping_no_space_colon
test "unknown_numeric_cmd" test_unknown_numeric_cmd
test "unknown_bang" test_unknown_bang
test "join_no_params" test_join_no_params
test "privmsg_no_params" test_privmsg_no_params
test "ping_extra_spaces" test_ping_extra_spaces
test "flood_pings" test_flood_pings
test "utf8_ping" test_utf8_ping
test "ctcp_ping" test_ctcp_ping
test "nul_in_line" test_nul_in_line

group_end
