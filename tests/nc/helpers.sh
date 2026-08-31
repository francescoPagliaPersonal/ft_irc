# helpers for tests/nc — sourced by run.sh. All I/O to the server goes through nc.

NC_DIR="${NC_DIR:-$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)}"
RUNDIR="${NC_DIR}/.run"
NC_TIMEOUT="${NC_TIMEOUT:-1}"
NC_POLL="${NC_POLL:-0.01}"

: "${C_RESET:=\033[0m}"
: "${C_BOLD:=\033[1m}"
: "${C_DIM:=\033[2m}"
: "${C_AUTUMN_RED:=\033[38;2;223;113;113m}"
: "${C_AUTUMN_GREEN:=\033[38;2;152;195;121m}"
: "${C_AUTUMN_ORANGE:=\033[38;2;255;158;100m}"
: "${C_AUTUMN_YELLOW:=\033[38;2;224;175;104m}"
: "${C_BAMBOO_GREEN:=\033[38;2;133;153;0m}"
: "${C_FUJI_WHITE:=\033[38;2;235;219;178m}"
: "${C_FUJI_GRAY3:=\033[38;2;147;137;117m}"
: "${C_SAKURA_BLOSSOM:=\033[38;2;255;204;212m}"
: "${C_WINTER_BLUE:=\033[38;2;110;134;161m}"
NC="stdbuf -o0 nc -4"

TOTAL_OK=0
TOTAL_FAIL=0
TOTAL_CRASH=0
TOTAL_VGFAIL=0
GROUP_OK=0
GROUP_FAIL=0
GROUP_CRASH=0
VG_N=0
INTERRUPTED=0
SRV_PID=
VG_LOG=
SRV_LOG=
VG_EVALUATED=0

cb() { printf '%b' "$1"; }

print_ok()    { printf '%b%s%b' "$(cb "$C_BAMBOO_GREEN")" "$1" "$(cb "$C_RESET")"; }
print_fail()  { printf '%b%s%b' "$(cb "$C_AUTUMN_RED")" "$1" "$(cb "$C_RESET")"; }
print_crash() { printf '%b%s%b' "$(cb "$C_AUTUMN_ORANGE")" "$1" "$(cb "$C_RESET")"; }
print_warn()  { printf '%b%s%b' "$(cb "$C_AUTUMN_YELLOW")" "$1" "$(cb "$C_RESET")"; }

require_env() {
	local missing=0
	local v
	for v in BIN PORT PASSWORD HOST VALGRIND_FLAGS; do
		if [ -z "${!v:-}" ]; then
			printf '%bmissing %s%b\n' "$(cb "$C_AUTUMN_RED")" "$v" "$(cb "$C_RESET")"
			missing=1
		fi
	done
	[ "$missing" -eq 0 ] || exit 2
	if [ ! -x "$BIN" ] && [ ! -x "./$BIN" ]; then
		printf '%bnot executable: %s%b\n' "$(cb "$C_AUTUMN_RED")" "$BIN" "$(cb "$C_RESET")"
		exit 2
	fi
	[ -x "$BIN" ] || BIN="./$BIN"
}

# --- server lifecycle -------------------------------------------------------

wait_for_port() {
	local i=0
	while [ "$i" -lt 80 ]; do
		if $NC -z -w 1 "$HOST" "$PORT" 2>/dev/null; then
			return 0
		fi
		if [ -n "${SRV_PID:-}" ] && ! kill -0 "$SRV_PID" 2>/dev/null; then
			return 1
		fi
		sleep 0.25
		i=$((i + 1))
	done
	return 1
}

wait_valgrind_summary() {
	local i=0
	while [ "$i" -lt 80 ]; do
		if [ -n "${VG_LOG:-}" ] && grep -q "ERROR SUMMARY" "$VG_LOG" 2>/dev/null; then
			sleep 0.2
			return 0
		fi
		sleep 0.25
		i=$((i + 1))
	done
	return 1
}

start_server() {
	mkdir -p "$RUNDIR"
	close_all_clients
	VG_N=$((VG_N + 1))
	VG_LOG="$RUNDIR/valgrind.${VG_N}.log"
	SRV_LOG="$RUNDIR/server.${VG_N}.log"
	VG_EVALUATED=0
	# shellcheck disable=SC2086
	valgrind $VALGRIND_FLAGS --log-file="$VG_LOG" \
		"$BIN" "$PORT" "$PASSWORD" >"$SRV_LOG" 2>&1 &
	SRV_PID=$!
	if ! wait_for_port; then
		printf '%bserver did not listen on %s:%s%b\n' \
			"$(cb "$C_AUTUMN_RED")" "$HOST" "$PORT" "$(cb "$C_RESET")"
		if [ -f "$SRV_LOG" ]; then
			printf '%b--- server.log ---\n%b' "$(cb "$C_FUJI_GRAY3")" "$(cb "$C_RESET")"
			tail -n 40 "$SRV_LOG"
		fi
		kill -TERM "$SRV_PID" 2>/dev/null || true
		wait_valgrind_summary || true
		wait "$SRV_PID" 2>/dev/null || true
		evaluate_valgrind
		SRV_PID=
		return 1
	fi
	return 0
}

stop_server() {
	close_all_clients
	if [ -z "${SRV_PID:-}" ]; then
		return 0
	fi
	if ! kill -0 "$SRV_PID" 2>/dev/null; then
		wait "$SRV_PID" 2>/dev/null || true
		evaluate_valgrind
		SRV_PID=
		return 0
	fi
	kill -TERM "$SRV_PID" 2>/dev/null || true
	wait_valgrind_summary || true
	wait "$SRV_PID" 2>/dev/null || true
	evaluate_valgrind
	SRV_PID=
}

# Wait status 130 = 128 + SIGINT. Valgrind may also exit 0 after SIGINT.
is_sigint_status() {
	local st="$1"
	[ "$st" -eq 130 ] || [ "$st" -eq 2 ]
}

handle_dead_server() {
	local st=0
	wait "$SRV_PID" 2>/dev/null || st=$?
	SRV_PID=
	if is_sigint_status "$st"; then
		INTERRUPTED=1
		return 2
	fi
	evaluate_valgrind
	printf '      %bhow: wait status %s%b\n' "$(cb "$C_AUTUMN_ORANGE")" "$st" "$(cb "$C_RESET")"
	if [ -f "$SRV_LOG" ]; then
		tail -n 20 "$SRV_LOG" | sed 's/^/      /'
	fi
	if ! start_server; then
		printf '%bcould not restart server%b\n' "$(cb "$C_AUTUMN_RED")" "$(cb "$C_RESET")"
	fi
	return 1
}

server_alive() {
	[ -n "${SRV_PID:-}" ] && kill -0 "$SRV_PID" 2>/dev/null
}

# --- valgrind ---------------------------------------------------------------

evaluate_valgrind() {
	if [ "${VG_EVALUATED:-0}" = 1 ]; then
		return 0
	fi
	VG_EVALUATED=1
	local log="${VG_LOG:-}"
	local fail=0
	local line nerr lost ilost

	if [ -z "$log" ] || [ ! -f "$log" ]; then
		print_warn "      valgrind: no log"
		printf '\n'
		TOTAL_VGFAIL=$((TOTAL_VGFAIL + 1))
		return 1
	fi

	if grep -q "Process terminating with default action of signal" "$log"; then
		if grep -qE "signal 2 \(SIGINT\)" "$log"; then
			:
		else
			fail=1
			printf '      %bvalgrind: fatal signal%b\n' "$(cb "$C_AUTUMN_RED")" "$(cb "$C_RESET")"
			grep "Process terminating with default action of signal" "$log" | sed 's/^/      /'
		fi
	fi

	if grep -qE "Invalid (read|write)|Use of uninitialised" "$log"; then
		fail=1
		printf '      %bvalgrind: invalid access / uninit%b\n' "$(cb "$C_AUTUMN_RED")" "$(cb "$C_RESET")"
		grep -E "Invalid (read|write)|Use of uninitialised" "$log" | head -n 12 | sed 's/^/      /'
	fi

	line=$(grep "ERROR SUMMARY" "$log" | tail -n 1 || true)
	if [ -n "$line" ]; then
		nerr=$(printf '%s\n' "$line" | sed -n 's/.*ERROR SUMMARY: \([0-9][0-9]*\) errors.*/\1/p')
		if [ -n "$nerr" ] && [ "$nerr" -ne 0 ]; then
			fail=1
			printf '      %b%s%b\n' "$(cb "$C_AUTUMN_RED")" "$line" "$(cb "$C_RESET")"
		fi
	fi

	lost=$(grep "definitely lost:" "$log" | tail -n 1 | sed -n 's/.*definitely lost: \([0-9,]*\) bytes.*/\1/p' | tr -d ',')
	ilost=$(grep "indirectly lost:" "$log" | tail -n 1 | sed -n 's/.*indirectly lost: \([0-9,]*\) bytes.*/\1/p' | tr -d ',')
	if [ -n "$lost" ] && [ "$lost" -ne 0 ]; then
		fail=1
		grep "definitely lost:" "$log" | tail -n 1 | sed 's/^/      /'
	fi
	if [ -n "$ilost" ] && [ "$ilost" -ne 0 ]; then
		fail=1
		grep "indirectly lost:" "$log" | tail -n 1 | sed 's/^/      /'
	fi
	if grep -q "still reachable:" "$log"; then
		local reach
		reach=$(grep "still reachable:" "$log" | tail -n 1 | sed -n 's/.*still reachable: \([0-9,]*\) bytes.*/\1/p' | tr -d ',')
		if [ -n "$reach" ] && [ "$reach" -ne 0 ]; then
			print_warn "      valgrind: still reachable ${reach} bytes"
			printf '\n'
		fi
	fi

	if grep -q "FILE DESCRIPTORS:" "$log"; then
		local extra
		extra=$(awk '
			/Open file descriptor/ {
				line=$0
				if (getline nxt) {
					if (nxt ~ /inherited from parent/) next
				}
				if (line ~ /valgrind/ || line ~ /\.log/) next
				print line
			}
		' "$log")
		if [ -n "$extra" ]; then
			fail=1
			printf '      %bvalgrind: leftover fds%b\n' "$(cb "$C_AUTUMN_RED")" "$(cb "$C_RESET")"
			printf '%s\n' "$extra" | head -n 16 | sed 's/^/      /'
		fi
	fi

	if [ "$fail" -ne 0 ]; then
		TOTAL_VGFAIL=$((TOTAL_VGFAIL + 1))
		printf '      %b--- valgrind.log (tail) ---%b\n' "$(cb "$C_FUJI_GRAY3")" "$(cb "$C_RESET")"
		tail -n 24 "$log" | sed 's/^/      /'
		return 1
	fi
	return 0
}

# --- time -------------------------------------------------------------------

_now() {
	if [ -n "${EPOCHREALTIME:-}" ]; then
		printf '%s' "$EPOCHREALTIME"
	else
		date +%s.%N
	fi
}

_timed_out() {
	awk -v s="$1" -v t="$2" -v n="$3" 'BEGIN { exit ((n - s) >= t) ? 0 : 1 }'
}

# --- nc I/O -----------------------------------------------------------------
# nc stays open and writes into a capture file. Tests poll that file and
# kill nc as soon as the expected (or forbidden) text shows up. NC_TIMEOUT
# is only the deadline for silence / no-match.

irc_open() {
	local id="$1"
	local dir="$RUNDIR/cli_$id"
	local fd
	rm -rf "$dir"
	mkdir -p "$dir"
	mkfifo "$dir/in"
	: >"$dir/out"
	# RDWR open does not block; then nc can attach as reader.
	exec {fd}<>"$dir/in"
	echo "$fd" >"$dir/wfd"
	$NC -C -w 60 "$HOST" "$PORT" <"$dir/in" >"$dir/out" &
	echo $! >"$dir/pid"
	echo "$id" >>"$RUNDIR/clients"
}

irc_send() {
	local id="$1"
	shift
	local fd
	fd=$(cat "$RUNDIR/cli_$id/wfd" 2>/dev/null) || return 1
	printf '%s\n' "$*" >&"$fd" 2>/dev/null || return 1
}

# Write bytes as-is (no extra newline). For framing tests.
irc_write() {
	local id="$1"
	shift
	local fd
	fd=$(cat "$RUNDIR/cli_$id/wfd" 2>/dev/null) || return 1
	printf '%s' "$*" >&"$fd" 2>/dev/null || return 1
}

irc_recv() {
	local id="$1"
	cat "$RUNDIR/cli_$id/out" 2>/dev/null || true
}

# Return as soon as every needle is present in the capture; else wait NC_TIMEOUT.
irc_expect() {
	local id="$1"
	shift
	local file="$RUNDIR/cli_$id/out"
	local start n ok
	FAIL_HINT="expected to contain: $*"
	start=$(_now)
	while true; do
		ok=1
		for n in "$@"; do
			if ! grep -q -- "$n" "$file" 2>/dev/null; then
				ok=0
				break
			fi
		done
		if [ "$ok" -eq 1 ]; then
			LAST_GOT=$(cat "$file")
			return 0
		fi
		if _timed_out "$start" "$NC_TIMEOUT" "$(_now)"; then
			LAST_GOT=$(cat "$file" 2>/dev/null || true)
			return 1
		fi
		sleep "$NC_POLL"
	done
}

# Return as soon as any needle is present; else wait NC_TIMEOUT.
irc_expect_any() {
	local id="$1"
	shift
	local file="$RUNDIR/cli_$id/out"
	local start n
	FAIL_HINT="expected one of: $*"
	start=$(_now)
	while true; do
		for n in "$@"; do
			if grep -q -- "$n" "$file" 2>/dev/null; then
				LAST_GOT=$(cat "$file")
				return 0
			fi
		done
		if _timed_out "$start" "$NC_TIMEOUT" "$(_now)"; then
			LAST_GOT=$(cat "$file" 2>/dev/null || true)
			return 1
		fi
		sleep "$NC_POLL"
	done
}

# Fail as soon as needle appears. Pass only after NC_TIMEOUT with no match.
irc_expect_absent() {
	local id="$1"
	local needle="$2"
	local file="$RUNDIR/cli_$id/out"
	local start
	FAIL_HINT="expected NOT to contain: ${needle}"
	start=$(_now)
	while true; do
		if grep -q -- "$needle" "$file" 2>/dev/null; then
			LAST_GOT=$(cat "$file")
			return 1
		fi
		if _timed_out "$start" "$NC_TIMEOUT" "$(_now)"; then
			LAST_GOT=$(cat "$file" 2>/dev/null || true)
			return 0
		fi
		sleep "$NC_POLL"
	done
}

irc_close() {
	local id="$1"
	local dir="$RUNDIR/cli_$id"
	local fd pid
	[ -d "$dir" ] || return 0
	fd=$(cat "$dir/wfd" 2>/dev/null) || true
	if [ -n "${fd:-}" ]; then
		eval "exec ${fd}>&-" 2>/dev/null || true
	fi
	pid=$(cat "$dir/pid" 2>/dev/null) || true
	if [ -n "${pid:-}" ]; then
		kill "$pid" 2>/dev/null || true
		wait "$pid" 2>/dev/null || true
	fi
	rm -f "$dir/wfd"
}

close_all_clients() {
	local id
	if [ -f "$RUNDIR/clients" ]; then
		while IFS= read -r id; do
			[ -n "$id" ] || continue
			irc_close "$id"
		done <"$RUNDIR/clients"
		rm -f "$RUNDIR/clients"
	fi
}

register_client() {
	local id="$1"
	local nick="$2"
	irc_open "$id"
	irc_send "$id" "PASS $PASSWORD"
	irc_send "$id" "NICK $nick"
	irc_send "$id" "USER $nick 0 * :$nick"
	irc_expect "$id" " 001 "
}

# Send commands on a throwaway client; return when needle(s) match.
oneshot_expect() {
	local needle="$1"
	shift
	local id="_os"
	irc_open "$id"
	local c
	for c in "$@"; do
		irc_send "$id" "$c" || { irc_close "$id"; return 1; }
	done
	irc_expect "$id" "$needle"
	local rc=$?
	irc_close "$id"
	return "$rc"
}

oneshot_expect_any() {
	local id="_os"
	local n1="$1" n2="$2"
	shift 2
	irc_open "$id"
	local c
	for c in "$@"; do
		irc_send "$id" "$c" || { irc_close "$id"; return 1; }
	done
	irc_expect_any "$id" "$n1" "$n2"
	local rc=$?
	irc_close "$id"
	return "$rc"
}

# --- assertions / test runner ----------------------------------------------

LAST_GOT=
FAIL_HINT=

assert_contains() {
	local haystack="$1"
	local needle="$2"
	LAST_GOT="$haystack"
	FAIL_HINT="expected to contain: ${needle}"
	printf '%s' "$haystack" | grep -q -- "$needle"
}

assert_not_contains() {
	local haystack="$1"
	local needle="$2"
	LAST_GOT="$haystack"
	FAIL_HINT="expected NOT to contain: ${needle}"
	! printf '%s' "$haystack" | grep -q -- "$needle"
}

print_result() {
	local name="$1"
	local status="$2"
	printf '%b  %-36s ' "$(cb "$C_FUJI_WHITE")" "$name"
	case "$status" in
		OK)    print_ok "OK" ;;
		FAIL)  print_fail "FAIL" ;;
		CRASH) print_crash "CRASH" ;;
		*)     printf '%s' "$status" ;;
	esac
	printf '%b\n' "$(cb "$C_RESET")"
}

dump_fail() {
	if [ -n "${FAIL_HINT:-}" ]; then
		printf '      %b%s%b\n' "$(cb "$C_AUTUMN_YELLOW")" "$FAIL_HINT" "$(cb "$C_RESET")"
	fi
	printf '      %bgot:%b\n' "$(cb "$C_FUJI_GRAY3")" "$(cb "$C_RESET")"
	if [ -z "${LAST_GOT:-}" ]; then
		printf '        (empty)\n'
	else
		printf '%s\n' "$LAST_GOT" | sed 's/^/        /' | head -n 20
	fi
}

check_alive_or_crash() {
	if server_alive; then
		return 0
	fi
	handle_dead_server
	local hs=$?
	if [ "$hs" -eq 2 ]; then
		exit 130
	fi
	return 1
}

test() {
	local name="$1"
	shift
	LAST_GOT=
	FAIL_HINT=

	if ! check_alive_or_crash; then
		print_result "$name" "CRASH"
		GROUP_CRASH=$((GROUP_CRASH + 1))
		GROUP_FAIL=$((GROUP_FAIL + 1))
		TOTAL_CRASH=$((TOTAL_CRASH + 1))
		TOTAL_FAIL=$((TOTAL_FAIL + 1))
		return 0
	fi

	if "$@"; then
		if ! check_alive_or_crash; then
			print_result "$name" "CRASH"
			GROUP_CRASH=$((GROUP_CRASH + 1))
			GROUP_FAIL=$((GROUP_FAIL + 1))
			TOTAL_CRASH=$((TOTAL_CRASH + 1))
			TOTAL_FAIL=$((TOTAL_FAIL + 1))
			return 0
		fi
		print_result "$name" "OK"
		GROUP_OK=$((GROUP_OK + 1))
		TOTAL_OK=$((TOTAL_OK + 1))
	else
		if ! server_alive; then
			handle_dead_server
			local hs=$?
			if [ "$hs" -eq 2 ]; then
				exit 130
			fi
			print_result "$name" "CRASH"
			dump_fail
			GROUP_CRASH=$((GROUP_CRASH + 1))
			GROUP_FAIL=$((GROUP_FAIL + 1))
			TOTAL_CRASH=$((TOTAL_CRASH + 1))
			TOTAL_FAIL=$((TOTAL_FAIL + 1))
			return 0
		fi
		print_result "$name" "FAIL"
		dump_fail
		GROUP_FAIL=$((GROUP_FAIL + 1))
		TOTAL_FAIL=$((TOTAL_FAIL + 1))
	fi
}

group_begin() {
	local title="$1"
	GROUP_OK=0
	GROUP_FAIL=0
	GROUP_CRASH=0
	printf '\n%b--- %s ---%b\n' "$(cb "$C_SAKURA_BLOSSOM")" "$title" "$(cb "$C_RESET")"
}

group_end() {
	printf '%b    group: %s ok, %s fail' "$(cb "$C_FUJI_GRAY3")" "$GROUP_OK" "$GROUP_FAIL"
	if [ "$GROUP_CRASH" -ne 0 ]; then
		printf ', %s crash' "$GROUP_CRASH"
	fi
	printf '%b\n' "$(cb "$C_RESET")"
}
