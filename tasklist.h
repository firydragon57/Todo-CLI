#ifndef TASK_LIST_H
#define TASK_LIST_H

#include <time.h>

#include "task.h"

#define INITIAL_CAPACITY 10
#define GROW_FACTOR 2

typedef struct TaskList {
    int size;
    int capacity;
    Task **tasks;
} TaskList;

// Generates a task id
int generateTaskId(void);

// Initializes the next task id
void initNextTaskId(TaskList *list);

// Creates a task list
TaskList *createList(void);

// Adds task to the end of the list
int addTask(TaskList *list, char *name, time_t due_date);

// Adds task to the end of list give task object
int addTaskObj(TaskList *list, Task *task);

// Removes task from the list by task id
int removeTask(TaskList *list, int task_id);

// Prints out all the tasks
void printTasks(TaskList *list, int spacing);

// Sort list by duedate
void sortTasks(TaskList *list);

// Deletes a task list
void deleteList(TaskList *list);

#endif