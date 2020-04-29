NTU Operating Systems Project 1
===

[Assignment Specification](https://hackmd.io/@Ue96nvjESj2XsDXw532-qA/ryYqceUrU)

NOTE: The output format in the demo video does not conform perfectly to the specifications, and has been manually fixed. The correction, however, is not reflected in the video.

Building
---

``` sh
make
```

File Structure
---

```
.
├── demo
│   ├── demo-out/ # Output files of the demo
│   ├── demo-out-old/ # Output files of the old demo, which the tables in the report are based on
│   └── demo.webm # Video of the demo
├── kernel_files/ # Files required to build modified Linux kernel; see report for the base kernel version
│   └── osproj.patch # Alternatively, one can apply this patch :)
├── tests/ # Test set provided by TAs
├── output/ # Output on the whole test set
├── Makefile
├── README.md # You are here :)
├── report.md
├── report.pdf
├── run-all.fish # Fish script to run all tests
├── run-demo.fish # Fish script to run only the demo test set
├── system-tuning.sh # Tune system for lower jitter; see report for details
```
