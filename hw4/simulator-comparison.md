# Comparison of MLFQ implementations

## My implementation

I missed the setup for time allotment of each queue by mixing its concept with time slice. Hence the job in a queue will move down to the next after executing for a single time slice, which should bring down the average turnaround time for switching too many times between the queues.

In addition, I didn't implement for I/O cases in which every process that's going to read or write should be moved out of the queue and inserted back once it's finished. This is essential for a MLFQ since I/O are very common in programs.

## Meilin's implementation

https://github.com/MeilinNiu/scheduler-simulation/blob/main/mlfq.c

I can see that time slice and time allotment are properly defined and used in her implementation. However, the time allotment value for the lowest-priority queue was set to `INT_MAX` which is a bit confusing and I think it's to let processes in the last queue re-entroll to it again since this case is already handled by the code.

Moreover, I think it's better to add structs for the queue and MLFQ instead of using a 2-dim array, which will improve the readability and make it easier to debug.

## Tianyu's implementation

https://github.com/Tianyu-Fang/CS5600/blob/main/MLFQ/MLFQ.h

I didn't seem to find the implementation of boosting all the processes and resetting them for toppest priority periodically, which I think is essential in a MLFQ. And I can see the code tries to apply different scheduling methods for queues, like for Q2 the Shortest Remaining Time First(SRTF) algorithm is used. However, this requires the knowledge of burst time of a process beforehand, while in reality it's hard to access that information.
