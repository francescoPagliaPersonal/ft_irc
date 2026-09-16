#!/usr/bin/env bash

count=${1:-99000}
host=${2:-localhost}
port=${3:-6669}

seq "$count" |
	xargs -n1 -P"$count" sh -c 'exec nc -d -C "$1" "$2"' _ "$host" "$port"
failed=0
