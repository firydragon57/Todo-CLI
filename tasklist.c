#include "tasklist.h"

int generateTaskId() {
    return next_task_id++;
}

// Creates a task list
TaskList *createList() {
    TaskList *list = (TaskList *) malloc(sizeof(TaskList));
    if (list == NULL) {
        return NULL;
    }

    list->size = 0;
    list->capacity = INITIAL_CAPACITY;

    Task **taskList = malloc(sizeof(Task *) * INITIAL_CAPACITY);
    if (taskList == NULL) {
        free(list);
        return NULL;
    }

    list->tasks = taskList;

    return list;
}

// Adds task to the end of the list
int addTask(TaskList *list, char *name, time_t due_date) {
    if (list == NULL) {
        return -1;
    }

    Task *new_task = createTask(generateTaskId(), name, due_date, (time_t) - 1);
    if (new_task == NULL) {
        return -1;
    }

    if (list->size + 1 > list->capacity) {
        int new_capacity = list->capacity * GROW_FACTOR;
        Task **new_tasks = realloc(list->tasks, sizeof(Task *) * new_capacity);
        if (new_tasks == NULL) {
            return -1;
        }
        
        list->capacity = new_capacity;
        list->tasks = new_tasks;
    }

    list->tasks[list->size] = new_task;
    list->size++;
    return 0;
}

// Adds task to the end of list give task object
int addTaskObj(TaskList *list, Task *task) {
    if (list == NULL) {
        return -1;
    }

    if (list->size + 1 > list->capacity) {
        int new_capacity = list->capacity * GROW_FACTOR;
        Task **new_tasks = realloc(list->tasks, sizeof(Task *) * new_capacity);
        if (new_tasks == NULL) {
            return -1;
        }
        
        list->capacity = new_capacity;
        list->tasks = new_tasks;
    }

    list->tasks[list->size] = task;
    list->size++;
    return 0;
}

static void printDivider(int divLength) {
    if (divLength == 0) {
        return;
    }

    for (int i = 0; i < divLength; i++) {
        putchar('-');
    }
    putchar('\n');
}

// Prints out all the tasks
void printTasks(TaskList *list, int spacing) {
    if (list == NULL) {
        return;
    }

    int divLength = 0;
    int rightTableSpacing = 0;
    char output[256];

    // Find the divider length
    for (int i = 0; i < list->size; i++) {
        int dueAtLen = computeDueAt(list->tasks[i], NULL, 0);
        if (dueAtLen > rightTableSpacing) {
            rightTableSpacing = dueAtLen;
        }

        int bytes_outputted = printTask(list->tasks[i], NULL, 0, spacing, rightTableSpacing);
        if (bytes_outputted > divLength) {
            divLength = bytes_outputted;
        }
    }

    printf("Your To-Do List:\n\n");
    printDivider(divLength);
    for (int i = 0; i < list->size; i++) {
        printTask(list->tasks[i], output, sizeof(output), spacing, rightTableSpacing);
        printf("%s\n", output);
    }
    printDivider(divLength);
    printf("\n");
}

// Removes task from the list at a given position
int removeTask(TaskList *list, int task_id) {
    if (list == NULL) {
        return -1;
    }

    int del_index = 0;
    for (int i = 0; i < list->size; i++) {
        if (list->tasks[i]->task_id == task_id) {
            deleteTask(list->tasks[i]);
            del_index = i;
            break;
        }
        if (i == list->size - 1) {
            return -1; // task id not in list of tasks
        }
    }

    // Shift everything after the index down by one
    for (int i = del_index; i < list->size; i++) {
        list->tasks[i] = list->tasks[i + 1];
    }

    list->tasks[list->size - 1] = NULL;
    list->size--;
    return 0;
}

// Deletes a task list
void deleteList(TaskList *list) {
    if (list == NULL) {
        return;
    }

    for (int i = 0; i < list->size; i++) {
        deleteTask(list->tasks[i]);
        list->tasks[i] = NULL;
    }

    free(list->tasks);
    free(list);
}