#!/usr/bin/env fish

for f in TIME_MEASUREMENT.txt FIFO_1.txt PSJF_2.txt RR_3.txt SJF_4.txt
	echo "=== $f ==="
	dmesg -C
	./project1 < tests/$f | tee demo/demo-out/(echo "$f" | sed 's/\.txt/_stdout.txt/')
	dmesg | grep Project1 | tee demo/demo-out/(echo "$f" | sed 's/\.txt/_dmesg.txt/')
end
