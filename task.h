#ifndef TASK_H
#define TASK_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct Task {
    int task_id;
    char *name;
    time_t due_date;
    time_t created_at;
} Task;

// Creats a task
Task *createTask(int task_id, char *name, time_t due_date, time_t create_at);

// Creates a task from a string of parameters separated by a |
Task *parseTask(char *task_str);

// parses date in mm-dd-yyyy format to time_t
time_t parseDate(char *date);

// Computes the time left on a task and outputed it to the given buffer of given size
int computeDueAt(Task *task, char *output, size_t output_size);

// Prints out task details
int printTask(Task *task, char *output, size_t output_size, int spacing, int right_table_spacing);

// Deletes a task
void deleteTask(Task *task);

#endif