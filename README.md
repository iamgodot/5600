# Homework 1

1.

CPU utilization should be 100% all the time, since 2 processes are both fully CPU-intensive. The result is as follows:

```
> python process-run.py -l 5:100,5:100 -c -p

Time        PID: 0        PID: 1           CPU           IOs
  1        RUN:cpu         READY             1
  2        RUN:cpu         READY             1
  3        RUN:cpu         READY             1
  4        RUN:cpu         READY             1
  5        RUN:cpu         READY             1
  6           DONE       RUN:cpu             1
  7           DONE       RUN:cpu             1
  8           DONE       RUN:cpu             1
  9           DONE       RUN:cpu             1
 10           DONE       RUN:cpu             1

Stats: Total Time 10
Stats: CPU Busy 10 (100.00%)
Stats: IO Busy  0 (0.00%)
```

2.

Total time is 11 units, in which process 1 takes 4 and process 2 takes 7.

3.

After switching order, total time is 7 units. The whole execution time decreases because now when process 1 was running IO and blocked, CPU gets to execute for process 2.

4.

Now when process 1 was running IO, process 2 had to wait and CPU was idled. Hence the total time is 11 units, like in problem 2.

5.

Now when process 1 was running IO, CPU was utilized for executing process 2. Just like in problem 3, total time is 7 units.

6.

After process 1 finished the first IO instruction, the next processes were executed before process 1 got to its last 2 IO instructions, during which time CPU was idled and wasted.

The system resources were not effectively utilized with CPU usage of 67.74% and IO usage of 48.39%.

7.

Now whenever process 1 finishes an IO instruction, it gets executed immediately which leads to CPU getting fully utilized.

Running a process that just completed an IO helps eliminate its waiting time and since it runs IO, CPU can still be used for other processes.

8.

With `-I IO_RUN_IMMEDIATE`, both CPU and IO resources are more utilized than with `-I IO_RUN_LATER`.

With `-S SWITCH_ON_IO`, IO usage may decrease a bit but CPU utilization boosts than with `-S SWITCH_ON_END`.
