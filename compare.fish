#!/usr/bin/env fish

set f TIME_MEASUREMENT
set unit (
	paste (sort -k3 "$argv[1]/output/""$f""_dmesg.txt" | psub) (sort -k2 "$argv[1]/output/""$f""_stdout.txt" | psub) |
		sed -e 's/\[.*\]//' |
		awk '{a += ($3 - $2); b += 500} END {print a / b}'
)
echo $unit


cat tests/list | while read -l f
	echo "=== $f ==="
	paste (sort -k3 "$argv[1]/output/""$f""_dmesg.txt" | psub) (sort -k2 "$argv[1]/output/""$f""_stdout.txt" | psub) | sed -e 's/\[.*\]//' | awk '{print $4, ($3 - $2) / '$unit', $3, $2}' | sort -k 3
end
