NTU Operating Systems Project 1
===

[Assignment Specification](https://hackmd.io/@Ue96nvjESj2XsDXw532-qA/ryYqceUrU)

Building
---

``` sh
make
```

Running
---

``` sh
sudo ./system-tuning.sh
sudo ./project1 < INPUT_FILE
```

Alternatively, one might wish to use the fish scripts described below.

File Structure
---

```
.
├── demo
│   ├── demo-out/ # Output files of the demo
│   ├── demo-out-old/ # Output files of the old demo, which the tables in the report are based on
│   └── demo.mkv # Video of the demo
├── kernel_files/ # Files required to build modified Linux kernel; see report for the base kernel version
│   └── osproj.patch # Alternatively, this patch can be applied instead of copying the files :)
├── tests/ # Test set provided by TAs
├── output/ # Output on the whole test set
├── README.md # You are here :)
├── report.md
├── report.pdf
├── run-all.fish # Fish script to run all tests
├── run-demo.fish # Fish script to run only the demo test set
├── compare.fish # Fish script that calculates the `end_time - start_time` of processes in unit time
├── system-tuning.sh # Tune system for lower jitter; see report for details
├── *.c, *.h # Program source
├── Makefile
```
