#pragma once
#include "common.h"

int64_t TimespecToI(struct timespec *ts);

int64_t GetTimeNs(void);

int64_t MeasureTimeQuantum(void);

void CalibTime(void);

inline void WaitTimeUnit(void) {
	volatile unsigned long i;
	for (i = 0; i < 1000000UL; i++)
		;
}

inline void WaitUntil(int t) {
	while ((GetTimeNs() - init_time) < t * ns_per_unit)
		;
}

inline int GetTime(void) {
	return (int)((GetTimeNs() - init_time) / ns_per_unit);
}
