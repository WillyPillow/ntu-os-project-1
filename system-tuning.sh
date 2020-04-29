#!/bin/sh

systemctl restart tlp
wrmsr --all 0x1a0 0x4000850089
cpupower idle-set -D10
