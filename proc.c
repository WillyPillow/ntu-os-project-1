#include "proc.h"
#include "timeutils.h"

struct timespec64 {
	int64_t tv_sec;
	long tv_nsec;
};

noreturn void RunChildProc(int id) {
	fprintf(stderr, "Start: %s (%d) %" PRId64 "\n", (*name)[id], getpid(),
	        (GetTimeNs() - init_time) / ns_per_unit);
	printf("%s %d\n", (*name)[id], getpid());
	fflush(stdout);
	struct timespec64 begin;
	syscall(439, &begin);
	for (int j = 0; j < execution_time[id]; j++) {
		WaitTimeUnit();
	}
	fprintf(stderr, "Stop: %s (%d) %" PRId64 "\n", (*name)[id], getpid(),
	        (GetTimeNs() - init_time) / ns_per_unit);
	syscall(440, (*name)[id], &begin);
	exit(0);
}

void IncPriority(int pid, int cpu) {
	//fprintf(stderr, "Inc: %d\n", pid ? pid : getpid());
	struct sched_param param;
	assert(sched_getparam(pid, &param) == 0);
	param.sched_priority = sched_get_priority_max(SCHED_FIFO);
	assert(sched_setscheduler(pid, SCHED_FIFO, &param) == 0);
	if (cpu != -1) {
		cpu_set_t cs;
		CPU_ZERO(&cs);
		CPU_SET(cpu, &cs);
		assert(sched_setaffinity(pid, sizeof(cs), &cs) == 0);
	}
}

void DecPriority(int pid, int cpu) {
	//fprintf(stderr, "Dec: %d\n", pid ? pid : getpid());
	struct sched_param param;
	assert(sched_getparam(pid, &param) == 0);
	param.sched_priority = sched_get_priority_min(SCHED_IDLE);
	assert(sched_setscheduler(pid, SCHED_IDLE, &param) == 0);
	if (cpu != -1) {
		cpu_set_t cs;
		CPU_ZERO(&cs);
		CPU_SET(cpu, &cs);
		assert(sched_setaffinity(pid, sizeof(cs), &cs) == 0);
	}
}
