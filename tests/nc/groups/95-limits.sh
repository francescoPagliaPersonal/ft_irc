group_begin "LIMITS"

test_ip_connection_limit() {
	local id i
	for i in 1 2 3 4 5; do
		id="ip$i"
		if ! register_client "$id" "ipnick$i"; then
			local j
			for j in $(seq 1 "$i"); do
				irc_close "ip$j"
			done
			return 1
		fi
	done
	irc_open ip6
	irc_send ip6 "PASS $PASSWORD"
	irc_send ip6 "NICK ipnick6"
	irc_send ip6 "USER ipnick6 0 * :sixth"
	irc_expect ip6 "ERROR: too many connection from IP"
	local rc=$?
	irc_close ip6
	for i in 1 2 3 4 5; do
		irc_close "ip$i"
	done
	return "$rc"
}

test_spam_disconnect() {
	if ! register_client sp spammer; then
		return 1
	fi
	local i
	for i in $(seq 1 65); do
		irc_send sp "PING :${SRVNAME}" || break
	done
	if ! wait_client_gone sp; then
		irc_close sp
		return 1
	fi
	if ! register_client sp2 alive; then
		return 1
	fi
	probe_alive sp2 spamok
	local rc=$?
	irc_close sp2
	return "$rc"
}

test "ip_connection_limit" test_ip_connection_limit
test "spam_disconnect" test_spam_disconnect

group_end
