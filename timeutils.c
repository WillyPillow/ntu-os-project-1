#include "timeutils.h"

int64_t ns_per_quantum, ns_per_unit, init_time;

int64_t TimespecToI(struct timespec *ts) {
	return ts->tv_sec * 1000000000 + ts->tv_nsec;
}

int64_t GetTimeNs() {
	struct timespec ts;
	assert(clock_gettime(CLOCK_MONOTONIC, &ts) == 0);
	return TimespecToI(&ts);
}

int64_t MeasureTimeQuantum() {
	struct timespec t_before, t_after;
	assert(clock_gettime(CLOCK_MONOTONIC, &t_before) == 0);
	for (int i = 0; i < 500; i++) {
		WaitTimeUnit();
	}
	assert(clock_gettime(CLOCK_MONOTONIC, &t_after) == 0);
	return TimespecToI(&t_after) - TimespecToI(&t_before);
}

void CalibTime() {
	for (int i = 0; i < kCalibTries; i++) {
		int64_t t = MeasureTimeQuantum();
		ns_per_quantum += t;
		fprintf(stderr, "Measure: %" PRId64 "\n", t);
	}
	ns_per_unit = ns_per_quantum / (kTimeQuantumUnits * kCalibTries);
	ns_per_quantum /= kCalibTries;
}

extern inline void WaitTimeUnit(void);

extern inline void WaitUntil(int t);

extern inline int GetTime(void);
