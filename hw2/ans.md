# Homework 2

**All the code files can be compiled by `make compile`, and are executable on Linux distributions.**

## Chapter 5

### Simulation

1.

Action: a forks b

- a
  - b

Action: a forks c

- a
  - b
  - c

Action: c EXITS

- a
  - b

Action: a forks d

- a
  - b
  - d

Action: a forks e

- a
  - b
  - d
  - e

2.

With higher `fork_percentage`, the final process tree gets bigger, otherwise it gets smaller and will leave as just the root node if using 0.1.

3.

Yes. The rule is a node disappears representing an exited process, and a new node representing a forked process.

4.

After c exits, its forked process d and e will be attached to a. With `-R` flag, d and e will be attached to b.

In Linux, it used to always reparent orphaned process to the `init` process, while now a subreaper can be assigned to take them.

5.

Action: a forks b
Action: b EXITS
Action: a forks c
Action: c EXITS
Action: a forks d

- a
  - d

6.

For some cases, the actions are non-deterministic, such as:

- a
  - b
    - d
  - c

For its 5 actions, I'm not sure about the other 2 besides the 3 actions to fork b, d and c. For example, there could be another process e forked and exited, but no way to know its position in the tree.

For other cases, actions are certain, e.g.

- a
  - b
    - d
  - c
  - e
  - f

In this case, all 5 actions must be taked to fork the processes, even though different orders are possible.

### Code

1.

The value is initialized as 100 in the child process. Child and parent processes have their own memory thus changing the value of variable won't affect each other.

2.

Both child and parent processes can access the file descriptor. When they're writing to the file together, the output texts might interleave and become garbled.

3.

Yes, by leveraging on a pipe, we can make sure the parent process blocks when reading from it until child process writes something to it as a signal so that parent prints after the child.

4.

Because they provide convenience of usage for different cases. For example, with `execl()` we can specify the abosulte path of the program to execute and `execle()` allows us to specify environment variables for the running program.

5.

The `wait()` system call returns the process id of the child process that has finished, or -1 if an error occurs. For the child process, `wait()` will return -1 since there're no child processes to wait for.

6.

Since `waitpid()` can set a specific process id of a child process, it's useful when there're multiple child processes and we want to wait for a certain one to terminate.

7.

If the output file descriptor is closed, when a child process tries to use `printf()` to print something, the output will not be displayed on the standard output and simply get lost.

## Chapter 6

From my measurement, the average results showed:

- System call: 1 read plus 1 write take 0.29 microseconds in total, so about 0.145 ms for each
- Context switch: about 1.86 ms for 1 switch

See `measure.c` for details.

## Chapter 7

### Simulation

1.

For SJF scheduler:

- Response time: 0, 200, 400
  - Average: 200
- Turnaround time: 200, 400, 600
  - Average: 400

For FIFO scheduler:

- Response time: 0, 200, 400
  - Average: 200
- Turnaround time: 200, 400, 600
  - Average: 400

2.

For SJF scheduler:

- Response time: 0, 100, 300
  - Average: 133.33
- Turnaround time: 100, 300, 600
  - Average: 333.33

For FIFO scheduler:

- Response time: 0, 100, 300
  - Average: 133.33
- Turnaround time: 100, 300, 600
  - Average: 333.33

3.

- Response time: 0, 1, 2
  - Average: 1
- Turnround time: 598, 599, 600
  - Average: 599

4.

One case is all the jobs take the same CPU time, another case can be jobs are scheduled in an ascending order of CPU time.

5.

When all jobs have the same burst time and are the same as quantum length.

6.

Response time will increase accordingly:

- Average response time for 3 jobs of length 100: 100
- Average response time for 3 jobs of length 200: 200
- Average response time for 3 jobs of length 300: 300

7.

When quantum length is smaller than job length, response time with RR increases as quantum length increases, worst-time response time would be `(N-1)*q`.
When quantum length exceeds job length, response time won't change by quantum length and worst-time response time would be the length of the longest job.
