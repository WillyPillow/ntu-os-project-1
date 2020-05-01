#!/bin/sh

systemctl restart tlp
wrmsr --all 0x1a0 0x4000850089
cpupower idle-set -D10
sysctl kernel.sched_child_runs_first=0 # This should be the default, but just to be sure...
