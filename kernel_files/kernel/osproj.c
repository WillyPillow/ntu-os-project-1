#include <linux/kernel.h>
#include <linux/ktime.h>
#include <linux/time.h>
#include <linux/syscalls.h>

SYSCALL_DEFINE1(osproj_gettime, struct timespec64 __user*, ts) {
	struct timespec64 kts;
	ktime_get_ts64(&kts);
	copy_to_user(ts, &kts, sizeof(kts));
	return 0;
}

SYSCALL_DEFINE2(osproj_printk, char __user*, name, struct timespec64 __user*, begin) {
	struct timespec64 beg, end;
	char buf[64];
	copy_from_user(&beg, begin, sizeof(beg));
	strncpy_from_user(buf, name, 64);
	ktime_get_ts64(&end);
	printk("[Project1] %s %lld.%ld %lld.%ld\n", buf, beg.tv_sec, beg.tv_nsec, end.tv_sec, end.tv_nsec);
	return 0;
}
