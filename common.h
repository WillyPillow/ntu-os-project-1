#pragma once
#define _GNU_SOURCE
#include <assert.h>
#include <inttypes.h>
#include <sched.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define kKindLen 16
#define kNameLen 64
#define kTimeQuantumUnits 500
#define kCalibTries 10
#define kCpuParent 2
#define kCpuChild 1
#define kEps 4

extern char kind[kKindLen], (*name)[][kNameLen];
extern int *ready_time, *execution_time, *id_by_ready, *id_by_exec, *id_by_end, n;
extern int64_t ns_per_quantum, ns_per_unit, init_time;
