#!/usr/bin/env bash
# Interactive test launcher for `make tests`.

set -euo pipefail

: "${C_RESET:=\033[0m}"
: "${C_BOLD:=\033[1m}"
: "${C_DIM:=\033[2m}"
: "${C_AUTUMN_ORANGE:=\033[38;2;255;158;100m}"
: "${C_FUJI_WHITE:=\033[38;2;235;219;178m}"
: "${C_FUJI_GRAY3:=\033[38;2;147;137;117m}"
: "${C_SAKURA_BLOSSOM:=\033[38;2;255;204;212m}"
: "${C_WINTER_BLUE:=\033[38;2;110;134;161m}"

MENU_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$MENU_DIR/.." && pwd)"

MENU_ITEMS=(
	"Run automated nc tests"
	"Run automated unit tests"
	"Leave"
)

MENU_COUNT=${#MENU_ITEMS[@]}
SEL=0
TTY_SAVED=
TTY_RESTORED=0
ALT_SCREEN=0

cb() { printf '%b' "$1"; }

die_tty() {
	printf '%bmake tests requires an interactive terminal%b\n' \
		"$(cb "$C_AUTUMN_ORANGE")" "$(cb "$C_RESET")" >&2
	exit 2
}

restore_tty() {
	if [ "$TTY_RESTORED" -eq 0 ] && [ -n "${TTY_SAVED:-}" ]; then
		stty "$TTY_SAVED" < /dev/tty 2>/dev/null || true
		TTY_RESTORED=1
	fi
}

tty_cooked() {
	[ -n "${TTY_SAVED:-}" ] && stty "$TTY_SAVED" < /dev/tty 2>/dev/null || true
}

tty_cbreak() {
	stty -echo -icanon min 1 time 0 < /dev/tty 2>/dev/null || true
}

alt_screen_on() {
	if [ "$ALT_SCREEN" -eq 0 ]; then
		printf '\033[?1049h\033[?25l'
		ALT_SCREEN=1
	fi
}

alt_screen_off() {
	if [ "$ALT_SCREEN" -eq 1 ]; then
		printf '\033[?25h\033[?1049l'
		ALT_SCREEN=0
	fi
}

on_exit() {
	alt_screen_off
	restore_tty
}

trap on_exit EXIT INT TERM

read_key() {
	local key

	tty_cbreak
	key="$(dd bs=1 count=1 2>/dev/null < /dev/tty)" || {
		tty_cooked
		return 1
	}
	tty_cooked
	printf '%s' "$key"
}

read_key_timed() {
	local key

	tty_cbreak
	IFS= read -r -N 1 -t 0.05 key < /dev/tty 2>/dev/null || {
		tty_cooked
		return 1
	}
	tty_cooked
	[ -n "$key" ] || return 1
	printf '%s' "$key"
}

read_escape_sequence() {
	local seq
	seq="$(read_key_timed)" || return 1
	[ "$seq" = '[' ] || return 1
	seq="$(read_key_timed)" || return 1
	printf '%s' "$seq"
}

draw_menu() {
	local i label prefix

	tty_cooked
	clear
	printf '\n%bft_irc tests%b\n\n' "$(cb "$C_WINTER_BLUE")" "$(cb "$C_RESET")"
	printf '%b%-10sw  s  ·  j  k  ·  ↑ ↓%b\n' \
		"$(cb "$C_FUJI_GRAY3")" "Navigate:" "$(cb "$C_RESET")"
	printf '%b%-10sd  l  ·  →  ·  space  enter  ·  1  2  3%b\n\n' \
		"$(cb "$C_FUJI_GRAY3")" "Select:" "$(cb "$C_RESET")"

	for ((i = 0; i < MENU_COUNT; ++i)); do
		label="${MENU_ITEMS[$i]}"
		if [ "$i" -eq "$SEL" ]; then
			prefix="$(cb "$C_BOLD")$(cb "$C_AUTUMN_ORANGE")> "
			printf '  %s%d) %s%b\n' "$prefix" "$((i + 1))" "$label" "$(cb "$C_RESET")"
		else
			printf '  %b%d) %s%b\n' "$(cb "$C_FUJI_WHITE")" "$((i + 1))" "$label" "$(cb "$C_RESET")"
		fi
	done
	printf '\n'
}

move_up() {
	SEL=$((SEL - 1))
	if [ "$SEL" -lt 0 ]; then
		SEL=$((MENU_COUNT - 1))
	fi
}

move_down() {
	SEL=$((SEL + 1))
	if [ "$SEL" -ge "$MENU_COUNT" ]; then
		SEL=0
	fi
}

run_nc_tests() {
	alt_screen_off
	restore_tty
	exec bash "$MENU_DIR/nc/run.sh"
}

run_unit_tests() {
	alt_screen_off
	restore_tty
	cd "$REPO_ROOT"
	exec make -C tests/unit val
}

leave_menu() {
	alt_screen_off
	restore_tty
	exit 0
}

dispatch_selection() {
	case "$SEL" in
	0) run_nc_tests ;;
	1) run_unit_tests ;;
	2) leave_menu ;;
	*) leave_menu ;;
	esac
}

handle_key() {
	local key="$1"

	case "$key" in
	$'\x1b')
		local arrow
		arrow="$(read_escape_sequence)" || { leave_menu; return; }
		case "$arrow" in
		A) move_up ;;
		B) move_down ;;
		C) dispatch_selection ;;
		D) : ;;
		esac
		;;
	[WwKk]) move_up ;;
	[SsJj]) move_down ;;
	[AaHh]) : ;;
	[Dd]|[Ll]|$'\n'|$'\r'|$' '|'') dispatch_selection ;;
	1) SEL=0; dispatch_selection ;;
	2) SEL=1; dispatch_selection ;;
	3|[Qq]) leave_menu ;;
	esac
}

main() {
	[ -t 0 ] && [ -t 1 ] || die_tty

	TTY_SAVED="$(stty -g < /dev/tty)"
	alt_screen_on

	draw_menu
	while true; do
		key="$(read_key)" || break
		handle_key "$key"
		draw_menu
	done
}

main "$@"
