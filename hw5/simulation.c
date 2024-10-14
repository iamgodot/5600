#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MEMORY_SIZE 1000
#define MAX_ALLOCATIONS 100

typedef struct {
  int start;
  int size;
  bool is_free;
} MemoryBlock;

MemoryBlock memory[MAX_ALLOCATIONS];
int num_blocks = 0;

void initialize_memory() {
  memory[0].start = 0;
  memory[0].size = MEMORY_SIZE;
  memory[0].is_free = true;
  num_blocks = 1;
}

void print_memory_state() {
  printf("Memory state:\n");
  for (int i = 0; i < num_blocks; i++) {
    printf("[%d-%d] %s\n", memory[i].start,
           memory[i].start + memory[i].size - 1,
           memory[i].is_free ? "Free" : "Allocated");
  }
  printf("\n");
}

int allocate_first_fit(int size) {
  for (int i = 0; i < num_blocks; i++) {
    if (memory[i].is_free && memory[i].size >= size) {
      if (memory[i].size > size) {
        MemoryBlock new_block = {memory[i].start + size, memory[i].size - size,
                                 true};
        for (int j = num_blocks; j > i + 1; j--) {
          memory[j] = memory[j - 1];
        }
        memory[i + 1] = new_block;
        num_blocks++;
      }
      memory[i].size = size;
      memory[i].is_free = false;
      return memory[i].start;
    }
  }
  return -1;
}

int allocate_best_fit(int size) {
  int best_fit_index = -1;
  int smallest_suitable_size = MEMORY_SIZE + 1;

  for (int i = 0; i < num_blocks; i++) {
    if (memory[i].is_free && memory[i].size >= size) {
      if (memory[i].size < smallest_suitable_size) {
        smallest_suitable_size = memory[i].size;
        best_fit_index = i;
      }
    }
  }

  if (best_fit_index != -1) {
    if (memory[best_fit_index].size > size) {
      MemoryBlock new_block = {memory[best_fit_index].start + size,
                               memory[best_fit_index].size - size, true};
      for (int j = num_blocks; j > best_fit_index + 1; j--) {
        memory[j] = memory[j - 1];
      }
      memory[best_fit_index + 1] = new_block;
      num_blocks++;
    }
    memory[best_fit_index].size = size;
    memory[best_fit_index].is_free = false;
    return memory[best_fit_index].start;
  }
  return -1;
}

int allocate_worst_fit(int size) {
  int worst_fit_index = -1;
  int largest_free_size = 0;

  for (int i = 0; i < num_blocks; i++) {
    if (memory[i].is_free && memory[i].size >= size) {
      if (memory[i].size > largest_free_size) {
        largest_free_size = memory[i].size;
        worst_fit_index = i;
      }
    }
  }

  if (worst_fit_index != -1) {
    if (memory[worst_fit_index].size > size) {
      MemoryBlock new_block = {memory[worst_fit_index].start + size,
                               memory[worst_fit_index].size - size, true};
      for (int j = num_blocks; j > worst_fit_index + 1; j--) {
        memory[j] = memory[j - 1];
      }
      memory[worst_fit_index + 1] = new_block;
      num_blocks++;
    }
    memory[worst_fit_index].size = size;
    memory[worst_fit_index].is_free = false;
    return memory[worst_fit_index].start;
  }
  return -1;
}

void deallocate(int start) {
  for (int i = 0; i < num_blocks; i++) {
    if (memory[i].start == start && !memory[i].is_free) {
      memory[i].is_free = true;

      // Merge with previous block if it's free
      if (i > 0 && memory[i - 1].is_free) {
        memory[i - 1].size += memory[i].size;
        for (int j = i; j < num_blocks - 1; j++) {
          memory[j] = memory[j + 1];
        }
        num_blocks--;
        i--;
      }

      // Merge with next block if it's free
      if (i < num_blocks - 1 && memory[i + 1].is_free) {
        memory[i].size += memory[i + 1].size;
        for (int j = i + 1; j < num_blocks - 1; j++) {
          memory[j] = memory[j + 1];
        }
        num_blocks--;
      }

      return;
    }
  }
}

void evaluate_policy(int (*allocate_function)(int), const char *policy_name) {
  initialize_memory();
  printf("Evaluating %s policy:\n", policy_name);

  int allocations[MAX_ALLOCATIONS];
  int num_allocations = 0;

  // Perform a series of allocations
  for (int i = 0; i < 10; i++) {
    int size = rand() % 100 + 1;
    int result = allocate_function(size);
    if (result != -1) {
      allocations[num_allocations++] = result;
      printf("Allocated %d bytes at address %d\n", size, result);
    } else {
      printf("Failed to allocate %d bytes\n", size);
    }
  }

  print_memory_state();

  // Perform some deallocations
  for (int i = 0; i < num_allocations; i += 2) {
    printf("Deallocating memory at address %d\n", allocations[i]);
    deallocate(allocations[i]);
  }

  print_memory_state();

  // Try to allocate again
  for (int i = 0; i < 5; i++) {
    int size = rand() % 100 + 1;
    int result = allocate_function(size);
    if (result != -1) {
      printf("Allocated %d bytes at address %d\n", size, result);
    } else {
      printf("Failed to allocate %d bytes\n", size);
    }
  }

  print_memory_state();
}

int main() {
  srand(42); // Set a fixed seed for reproducibility

  evaluate_policy(allocate_first_fit, "First Fit");
  evaluate_policy(allocate_best_fit, "Best Fit");
  evaluate_policy(allocate_worst_fit, "Worst Fit");

  return 0;
}
