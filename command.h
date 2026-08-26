#ifndef COMMAND_H
#define COMMAND_H

#include <time.h>

#include "tasklist.h"
#include "task.h"

#define TASK_FILE "./tasks.txt"

typedef struct AddTaskFlags {
    char name[128];
    char duedate[64];
    int success;
} AddTaskFlags;

typedef struct MarkTaskFlags {
    int task_id;
    int success;
} MarkTaskFlags;

time_t parseDate(char *date);
AddTaskFlags parseFlagsAdd(char *flag_str);
MarkTaskFlags parseFlagsMark(char *flag_str);
void printTaskList(TaskList *list, char *err);
void handleInput(TaskList *list, char *input, size_t input_size, char *err, size_t err_size);

// Saves tasks to a file on disk
void saveTasks(TaskList *list);

// Initializes tasks to empty list of tasks
void readTasks(TaskList *list);

#endif