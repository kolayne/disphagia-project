#!/bin/sh

if [ -z "$ADDR" ]; then
  echo Set '$ADDR' to the address where to send data, e.g., \'192.168.12.34:567/measures\' >&2
  exit 1
fi

hostname="$(echo "$ADDR" | cut -d / -f 1 | cut -d : -f 1)"
if ! ping -c 1 "$hostname" > /dev/null ; then
  echo Failed to ping host "$hostname" >&2
  exit 1
fi

exec ncat -k -l 2500 -c "( curl -X POST http://"$ADDR" -F bin=@- && echo curled || echo not curled ) >&2"
