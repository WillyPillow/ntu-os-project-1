#!/usr/bin/env fish

for f in FIFO_1.txt FIFO_2.txt FIFO_3.txt FIFO_4.txt FIFO_5.txt PSJF_1.txt PSJF_2.txt PSJF_3.txt PSJF_4.txt PSJF_5.txt RR_1.txt RR_2.txt RR_3.txt RR_4.txt RR_5.txt SJF_1.txt SJF_2.txt SJF_3.txt SJF_4.txt SJF_5.txt TIME_MEASUREMENT.txt
	echo "=== $f ==="
	dmesg -C
	./project1 < tests/$f | tee output/(echo "$f" | sed 's/\.txt/_stdout.txt/')
	dmesg | grep Project1 | tee output/(echo "$f" | sed 's/\.txt/_dmesg.txt/')
end
