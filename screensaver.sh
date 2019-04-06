./test.sh &>/dev/null &
child_pid=$!
sleep 2
watch -n .1 -tg "xdotool getmouselocation --shell" &>/dev/null
kill -- -$(ps -o pgid= $child_pid | grep -o [0-9]*) &>/dev/null
