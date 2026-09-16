group_begin "FRAMING"

test_ping_pong() {
	oneshot_expect "$PONG_NEEDLE" "PASS $PASSWORD" "NICK pinger1" "USER pinger1 0 * :p" "PING :${SRVNAME}"
}

test_partial_ping() {
	irc_open fr
	irc_write fr 'PI'
	sleep 0.25
	irc_write fr "NG :${SRVNAME}"$'\r\n'
	irc_expect fr "$PONG_NEEDLE"
	local rc=$?
	irc_close fr
	return "$rc"
}

test_two_commands_one_write() {
	irc_open_raw tw
	# Do not wrap printf in $(); command substitution strips the last newline.
	irc_write tw "PASS ${PASSWORD}"$'\r\nNICK twoatonce\r\nUSER twoatonce 0 * :x\r\n'
	irc_expect tw " 001 "
	local rc=$?
	irc_close tw
	return "$rc"
}

test_unknown_command() {
	oneshot_expect " 421 " "PASS $PASSWORD" "NICK unkncmd" "USER unkncmd 0 * :x" "FOOBAR"
}

# --- wild / wire ---

_fr_silence_probe() {
	local id="$1"
	local token="$2"
	if ! probe_alive "$id" "$token"; then
		return 1
	fi
	assert_not_contains "$(irc_recv "$id")" " 421 " || return 1
	assert_not_contains "$(irc_recv "$id")" " 461 "
}

test_empty_crlf() {
	irc_open_raw fe
	irc_write fe $'\r\n'
	_fr_silence_probe fe empty1
	local rc=$?
	irc_close fe
	return "$rc"
}

test_whitespace_only() {
	irc_open_raw fw
	irc_write fw $'   \r\n'
	_fr_silence_probe fw white1
	local rc=$?
	irc_close fw
	return "$rc"
}

test_prefix_only() {
	irc_open_raw fp
	irc_write fp $':nobody\r\n'
	_fr_silence_probe fp pref1
	local rc=$?
	irc_close fp
	return "$rc"
}

test_many_empty_lines() {
	irc_open_raw fm
	irc_write fm $'\r\n\r\n\r\n\r\n'
	_fr_silence_probe fm many1
	local rc=$?
	irc_close fm
	return "$rc"
}

test_lone_lf() {
	# Server splits only on CRLF, so a lone LF stays in the buffer and glues onto
	# the next complete line: the token then holds the LF and is rejected with
	# 402. Either way the client must get an answer and recover.
	irc_open_raw fl
	irc_write fl "PING :${SRVNAME}"$'\n'
	sleep 0.1
	irc_write fl "PING :${SRVNAME}"$'\r\n'
	if ! irc_expect_any fl "$PONG_NEEDLE" " 402 "; then
		irc_close fl
		return 1
	fi
	probe_alive fl afterlf
	local rc=$?
	irc_close fl
	return "$rc"
}

test_cr_then_lf() {
	irc_open_raw fc
	irc_write fc "PING :${SRVNAME}"$'\r'
	sleep 0.1
	irc_write fc $'\n'
	irc_expect fc "$PONG_NEEDLE"
	local rc=$?
	irc_close fc
	return "$rc"
}

test_oversize_no_crlf() {
	if ! register_client fos frsurv1; then
		return 1
	fi
	irc_open_raw fox
	irc_write fox "$(printf '%*s' 513 '' | tr ' ' 'A')"
	sleep 0.15
	# nc may stay up after the TCP drop; the fat client must not PONG.
	if probe_alive fox shoulddie; then
		FAIL_HINT="oversize client still answered PING"
		irc_close fox
		irc_close fos
		return 1
	fi
	probe_alive fos stillok
	local rc=$?
	irc_close fox
	irc_close fos
	return "$rc"
}

test_oversize_with_crlf() {
	if ! register_client fot frsurv2; then
		return 1
	fi
	irc_open_raw foy
	irc_write foy "$(printf '%*s' 513 '' | tr ' ' 'B')"$'\r\n'
	sleep 0.15
	if probe_alive foy shoulddie2; then
		FAIL_HINT="oversize client still answered PING"
		irc_close foy
		irc_close fot
		return 1
	fi
	probe_alive fot stillok2
	local rc=$?
	irc_close foy
	irc_close fot
	return "$rc"
}

test_maxish_ping() {
	irc_open_raw fz
	irc_write fz "PING :${SRVNAME}"$'\r\n'
	irc_expect fz "$PONG_NEEDLE"
	local rc=$?
	irc_close fz
	return "$rc"
}

test "ping_pong" test_ping_pong
test "partial_ping" test_partial_ping
test "two_commands_one_write" test_two_commands_one_write
test "unknown_command" test_unknown_command
test "empty_crlf" test_empty_crlf
test "whitespace_only" test_whitespace_only
test "prefix_only" test_prefix_only
test "many_empty_lines" test_many_empty_lines
test "lone_lf" test_lone_lf
test "cr_then_lf" test_cr_then_lf
test "oversize_no_crlf" test_oversize_no_crlf
test "oversize_with_crlf" test_oversize_with_crlf
test "maxish_ping" test_maxish_ping

group_end
