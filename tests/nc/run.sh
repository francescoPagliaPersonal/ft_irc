#!/usr/bin/env bash
# Main live nc suite. Invoked from `make tests` (menu) with HOST/PORT/PASSWORD/BIN/VALGRIND_FLAGS.

NC_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
# shellcheck source=helpers.sh
. "$NC_DIR/helpers.sh"

trap '' PIPE
require_env
wipe_rundir

on_int() {
	INTERRUPTED=1
	printf '\n%bInterrupted (Ctrl+C).%b\n' "$(cb "$C_AUTUMN_ORANGE")" "$(cb "$C_RESET")"
	close_all_clients
	if [ -n "${SRV_PID:-}" ] && kill -0 "$SRV_PID" 2>/dev/null; then
		kill -INT "$SRV_PID" 2>/dev/null || true
		wait "$SRV_PID" 2>/dev/null || true
	fi
	SRV_PID=
	trap - EXIT
	exit 130
}

on_exit() {
	if [ "$INTERRUPTED" = 1 ]; then
		return 0
	fi
	if [ -n "${SRV_PID:-}" ]; then
		stop_server
	fi
}

trap on_int INT
trap on_exit EXIT

printf '%bnc live suite%b  %s:%s  bin=%s\n' \
	"$(cb "$C_WINTER_BLUE")" "$(cb "$C_RESET")" "$HOST" "$PORT" "$BIN"

if ! start_server; then
	printf '%bfailed to start server%b\n' "$(cb "$C_AUTUMN_RED")" "$(cb "$C_RESET")"
	exit 1
fi

first=1
for group in "$NC_DIR/groups/"*.sh; do
	[ -f "$group" ] || continue
	if [ "$first" -eq 1 ]; then
		first=0
	else
		stop_server
		if ! start_server; then
			printf '%bcould not restart server for next group%b\n' \
				"$(cb "$C_AUTUMN_RED")" "$(cb "$C_RESET")"
			exit 1
		fi
	fi
	# shellcheck disable=SC1090
	. "$group"
done

trap - INT
stop_server
trap - EXIT

printf '\n%b============ summary ============%b\n' "$(cb "$C_SAKURA_BLOSSOM")" "$(cb "$C_RESET")"
printf '  %b%-8s%b %s\n' "$(cb "$C_BAMBOO_GREEN")" "ok" "$(cb "$C_RESET")" "$TOTAL_OK"
printf '  %b%-8s%b %s\n' "$(cb "$C_AUTUMN_RED")" "fail" "$(cb "$C_RESET")" "$TOTAL_FAIL"
printf '  %b%-8s%b %s\n' "$(cb "$C_AUTUMN_ORANGE")" "crash" "$(cb "$C_RESET")" "$TOTAL_CRASH"
printf '  %b%-8s%b %s\n' "$(cb "$C_AUTUMN_YELLOW")" "valgrind" "$(cb "$C_RESET")" "$TOTAL_VGFAIL"

if [ "$TOTAL_FAIL" -eq 0 ] && [ "$TOTAL_VGFAIL" -eq 0 ]; then
	printf '%ball tests passed%b\n' "$(cb "$C_SPRING_GREEN")" "$(cb "$C_RESET")"
	exit 0
fi
printf '%bsuite failed%b\n' "$(cb "$C_AUTUMN_RED")" "$(cb "$C_RESET")"
exit 1
