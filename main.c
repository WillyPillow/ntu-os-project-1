#include "common.h"
#include "policy.h"
#include "proc.h"
#include "timeutils.h"

char kind[kKindLen], (*name)[][kNameLen];
int *ready_time, *execution_time, *id_by_ready, *id_by_exec, *id_by_end, n;

/** Comparators for qsort **/

static int ProcCompareReady(const void *px, const void *py) {
	const int x = *(const int *)px, y = *(const int *)py;
	int tx = ready_time[x], ty = ready_time[y];
	return tx < ty ? -1 : tx > ty ? 1 : x < y ? -1 : x > y ? 1 : 0;
}

static int ProcCompareExec(const void *px, const void *py) {
	const int x = *(const int *)px, y = *(const int *)py;
	int tx = execution_time[x], ty = execution_time[y];
	return tx < ty ? -1 : tx > ty ? 1 : x < y ? -1 : x > y ? 1 : 0;
}

static int ProcCompareEnd(const void *px, const void *py) {
	const int x = *(const int *)px, y = *(const int *)py;
	int tx = ready_time[x] + execution_time[x],
	    ty = ready_time[y] + execution_time[y];
	return tx < ty ? -1 : tx > ty ? 1 : x < y ? -1 : x > y ? 1 : 0;
}

/** Entrypoint **/

int main() {
	assert(mlockall(MCL_CURRENT | MCL_FUTURE) == 0);
	IncPriority(0, kCpuParent);
	assert(sched_getscheduler(0) == SCHED_FIFO &&
	       "You may need to run this program as root.");

	CalibTime();

	scanf("%15s", kind);
	scanf("%d", &n);
	name = (char(*)[][kNameLen])malloc(n * kNameLen);
	ready_time = (int *)malloc(n * sizeof(int));
	execution_time = (int *)malloc(n * sizeof(int));
	id_by_ready = (int *)malloc(n * sizeof(int));
	id_by_exec = (int *)malloc(n * sizeof(int));
	id_by_end = (int *)malloc(n * sizeof(int));
	for (int i = 0; i < n; i++) {
		scanf("%63s%d%d", (*name)[i], &ready_time[i], &execution_time[i]);
		id_by_ready[i] = id_by_exec[i] = id_by_end[i] = i;
	}
	qsort(id_by_ready, n, sizeof(int), ProcCompareReady);
	qsort(id_by_exec, n, sizeof(int), ProcCompareExec);
	qsort(id_by_end, n, sizeof(int), ProcCompareEnd);

	if (strcmp(kind, "FIFO") == 0) {
		PolicyFifo();
	} else if (strcmp(kind, "RR") == 0) {
		PolicyRr();
	} else if (strcmp(kind, "SJF") == 0) {
		PolicySjf();
	} else if (strcmp(kind, "PSJF") == 0) {
		PolicyPsjf();
	} else {
		assert(0 && "Policy not implemented.");
	}
}
