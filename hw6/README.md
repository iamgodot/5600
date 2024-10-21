# Chapter 18

1.

Since `page table size = address space / page size`, page table size increases with larger address space and decreases with bigger page size.

Too big pages will result in memory waste since a process may very likely not be able to consume all of the allocated memory.

2.

When we increase the percentage of pages in the address space, more valid pages can be used.

3.

The first one uses too little memory for both address space and page size, and the last one uses too much.

4.

```bash
# Physical memory size has to be greater than address space
$ ./paging-linear-translate.py -P 1k -a 16k -p 16k

ARG seed 0
ARG address space size 16k
ARG phys mem size 16k
ARG page size 1k
ARG verbose False
ARG addresses -1

Error: physical memory size must be GREATER than address space size (for this simulation)

# Also physical memory size and address space have to be non-zero
$ ./paging-linear-translate.py -P 1k -a 0 -p 16k
$ ./paging-linear-translate.py -P 1k -a 16k -p 0

Error: must specify a non-zero address-space size.
```

# Chapter 19

1.

`clock_gettime()` has the resolution of nanosecond.

2.

See `tlb.c`

3.

100,000 trials

4.

See `plot.jpg`

5.

We can use `gcc -O0` to disable the optimization

6.

We can use `sched_setaffinity` or `hwloc-bind` to bind a specific CPU core. Otherwise the calculated time might get longer since multiple TLB would harm the cache performance.

7.

We can use `calloc()` to initialize the array before measuring.

# System Calls

1. `signalfd`: Instead of traditional signal handlers, `signalfd` allows us to read signals as if they're regular files by returning a file descriptor. The file descriptor becomes readable when signals are pending, and we can read detailed information of the signal from it.

2. `pidfd`: Unlike traditional PIDs which can be reused, `pidfd` guarantees that it always refers to the same process throughout its lifetime.

3. `userfaultfd`: It allows user-space programs to handle page faults by intercepting page faults and handling them in user space.

Ideas about testing `userfaultfd`:

- Basic test: First we set up the monitoring, then by accessing the memory page we can verify if the fault handler is triggered.
- Concurrency test: We can set multiple threads to access the monitored memory region, then verify if the handler works properly under concurrency.
- Performance test: We can measure the time spent with `userfaultfd` to regular page faults.
