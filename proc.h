#pragma once
#include "common.h"

noreturn void RunChildProc(int id);

void IncPriority(int pid, int cpu);

void DecPriority(int pid, int cpu);
