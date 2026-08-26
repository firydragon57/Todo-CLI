#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tasklist.h"
#include "command.h"

#define TASK_FILE "./tasks.txt"

void saveTasks(TaskList *list) {
    
}

void readTasks(TaskList *list) {
    FILE *task_file = fopen(TASK_FILE, "r");
    if (task_file == NULL) {
        fprintf(stderr, "Failed to open tasks.txt\n");
        return;
    }

    fseek(task_file, 0, SEEK_END);
    long len = ftell(task_file);
    fseek(task_file, 0, SEEK_SET);

    char *tasks_str = malloc(len + 1);
    if (tasks_str == NULL) {
        fprintf(stderr, "Failed to malloc\n");
        fclose(task_file);
        return;
    }

    size_t bytes_read = fread(tasks_str, 1, len, task_file);
    tasks_str[bytes_read] = '\0';
    if (bytes_read == 0 && !feof(task_file)) {
        // a real read error occurred (not just reaching end of file)
        fprintf(stderr, "Failed to read tasks file\n");
        free(tasks_str);
        fclose(task_file);
        return;
    }

    char *tasks_str_copy = malloc(strlen(tasks_str) + 1);
    if (tasks_str_copy == NULL) {
        fprintf(stderr, "Failed to malloc\n");
        free(tasks_str);
        fclose(task_file);
        return;
    }
    strcpy(tasks_str_copy, tasks_str);

    char *saveptr1;
    char *task_str = strtok_r(tasks_str_copy, "\n", &saveptr1);
    if (task_str == NULL) {
        fprintf(stderr, "Failed to read task parameters\n");
        free(tasks_str_copy);
        free(tasks_str);
        fclose(task_file);
        return;
    }

    while (task_str != NULL) {
        Task *task = parseTask(task_str);
        if (task == NULL) {
            fprintf(stderr, "Failed to parse task\n");
            free(tasks_str_copy);
            free(tasks_str);
            fclose(task_file);
            return;
        }
        int add_res = addTaskObj(list, task);
        if (add_res == -1) {
            fprintf(stderr, "Failed to add task to list\n");
            deleteTask(task);
            free(tasks_str_copy);
            free(tasks_str);
            fclose(task_file);
            return;
        }
        task_str = strtok_r(NULL, "\n", &saveptr1);
    }

    free(tasks_str_copy);
    free(tasks_str);
    fclose(task_file);
}

int main() {
    TaskList *list = createList();
    // char input[256] = {0};
    // char err[256] = {0};

    // while (1) {
    //     printTaskList(list, err);
    //     handleInput(list, input, sizeof(input), err, sizeof(err));
    // }

    readTasks(list);
    printTasks(list, 20);

    deleteList(list);

    return 0;
}