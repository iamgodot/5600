#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESSES 100
#define NUM_QUEUES 3
#define BOOST_INTERVAL 50 // Boost all processes every 50 time units

typedef struct {
  int id;
  int remaining_time;
  int arrival_time;
  int priority;
  bool has_arrived;
} Process;

typedef struct {
  Process *processes[MAX_PROCESSES];
  int front, rear;
  int size;
} Queue;

typedef struct {
  Queue queues[NUM_QUEUES];
  int time_quantum[NUM_QUEUES];
} MLFQ;

void initializeQueue(Queue *q) {
  q->front = q->rear = -1;
  q->size = 0;
}

bool isQueueEmpty(Queue *q) { return q->size == 0; }

void enqueue(Queue *q, Process *p) {
  if (q->size == MAX_PROCESSES)
    return;

  if (isQueueEmpty(q)) {
    q->front = q->rear = 0;
  } else {
    q->rear = (q->rear + 1) % MAX_PROCESSES;
  }

  q->processes[q->rear] = p;
  q->size++;
}

Process *dequeue(Queue *q) {
  if (isQueueEmpty(q))
    return NULL;

  Process *p = q->processes[q->front];

  if (q->front == q->rear) {
    q->front = q->rear = -1;
  } else {
    q->front = (q->front + 1) % MAX_PROCESSES;
  }

  q->size--;
  return p;
}

void initializeMFLQ(MLFQ *mlfq) {
  for (int i = 0; i < NUM_QUEUES; i++) {
    initializeQueue(&mlfq->queues[i]);
    mlfq->time_quantum[i] = (i + 1) * 2;
  }
}

void boostProcesses(MLFQ *mlfq) {
  printf("Boosting all processes to highest priority queue\n");
  for (int i = NUM_QUEUES - 1; i > 0; i--) {
    while (!isQueueEmpty(&mlfq->queues[i])) {
      Process *p = dequeue(&mlfq->queues[i]);
      enqueue(&mlfq->queues[0], p);
    }
  }
}

void checkNewArrivals(MLFQ *mlfq, Process processes[], int n,
                      int current_time) {
  for (int i = 0; i < n; i++) {
    if (!processes[i].has_arrived &&
        processes[i].arrival_time <= current_time) {
      enqueue(&mlfq->queues[0], &processes[i]);
      processes[i].has_arrived = true;
      printf("Process %d arrived at time %d\n", processes[i].id, current_time);
    }
  }
}

void schedule(MLFQ *mlfq, Process processes[], int n) {
  int current_time = 0;
  int completed = 0;
  int time_since_last_boost = 0;

  while (completed < n) {
    checkNewArrivals(mlfq, processes, n, current_time);
    bool processed_any = false;

    for (int i = 0; i < NUM_QUEUES; i++) {
      while (!isQueueEmpty(&mlfq->queues[i])) {
        Process *p = dequeue(&mlfq->queues[i]);
        // FIX: There should be time slice and time allotment as different vars
        int time_slice = (p->remaining_time < mlfq->time_quantum[i])
                             ? p->remaining_time
                             : mlfq->time_quantum[i];

        p->remaining_time -= time_slice;
        current_time += time_slice;
        time_since_last_boost += time_slice;
        processed_any = true;

        printf("Process %d ran for %d units. Time: %d\n", p->id, time_slice,
               current_time);

        if (p->remaining_time == 0) {
          completed++;
          printf("Process %d completed. Turnaround time: %d\n", p->id,
                 current_time - p->arrival_time);
        } else if (i < NUM_QUEUES - 1) {
          enqueue(&mlfq->queues[i + 1], p);
        } else {
          enqueue(&mlfq->queues[i], p);
        }

        checkNewArrivals(mlfq, processes, n, current_time);

        // Check if it's time to boost processes
        if (time_since_last_boost >= BOOST_INTERVAL) {
          boostProcesses(mlfq);
          time_since_last_boost = 0;
        }
      }
    }

    // If no process was processed in this iteration, increment time
    if (!processed_any) {
      current_time++;
      time_since_last_boost++;
      // Check if it's time to boost processes
      if (time_since_last_boost >= BOOST_INTERVAL) {
        boostProcesses(mlfq);
        time_since_last_boost = 0;
      }
    }
  }
}

int main() {
  MLFQ mlfq;
  initializeMFLQ(&mlfq);
  Process processes[] = {{1, 100, 0, 0, false},
                         {2, 60, 0, 0, false},
                         {3, 30, 0, 0, false},
                         {4, 90, 10, 0, false},
                         {5, 50, 15, 0, false}};
  int n = sizeof(processes) / sizeof(processes[0]);
  schedule(&mlfq, processes, n);
  return 0;
}
