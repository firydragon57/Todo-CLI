#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command.h"

AddTaskFlags parseFlagsAdd(char *flag_str) {
    AddTaskFlags flags = {0};
    flags.success = 1;

    char *name_marker = strstr(flag_str, "-n ");
    char *date_marker = strstr(flag_str, "-d ");

    if (name_marker == NULL || date_marker == NULL) {
        flags.success = 0;
        return flags;
    }

    char *first = (name_marker < date_marker) ? name_marker : date_marker;
    char *second = (name_marker < date_marker) ? date_marker : name_marker;

    size_t first_len = second - (first + 3);
    char first_val[256];
    strncpy(first_val, first + 3, first_len);
    first_val[first_len] = '\0';

    // Trim trailing space before next marker
    while (first_len > 0 && first_val[first_len - 1] == ' ') {
        first_val[--first_len] = '\0';
    }

    char *second_val = second + 3;

    if (first == name_marker) {
        strncpy(flags.name, first_val, sizeof(flags.name) - 1);
        strncpy(flags.duedate, second_val, sizeof(flags.duedate) - 1);
    }
    else {
        strncpy(flags.duedate, first_val, sizeof(flags.duedate) - 1);
        strncpy(flags.name, second_val, sizeof(flags.name) - 1);
    }

    return flags;
}

MarkTaskFlags parseFlagsMark(char *flag_str) {
    MarkTaskFlags flags = {0};
    flags.success = 1;

    char *id_marker = strstr(flag_str, "-i ");
    if (id_marker == NULL) {
        flags.success = 0;
        return flags;
    }

    char *id_val = id_marker + 3;
    flags.task_id = atoi(id_val);

    return flags;
}

void printTaskList(TaskList *list, char *err) {
    system("clear");

    if (strlen(err) != 0) {
        printf("%s\n", err);
        strcpy(err, "");
    }

    int spacing = 0;
    for (int i = 0; i < list->size; i++) {
        int len = (int) strlen(list->tasks[i]->name);
        if (len > spacing) {
            spacing = len;
        }
    }

    printTasks(list, spacing);
    printf("$: ");
}

void handleInput(TaskList *list, char *input, size_t input_size, char *err, size_t err_size) {
    if (fgets(input, input_size, stdin) == NULL) {
        snprintf(err, err_size, "Failed to read input\n");
        exit(1);
    }
    input[strcspn(input, "\n")] = '\0';

    if (strcmp(input, "exit") == 0) {
        exit(0);
    }
    else if (strncmp(input, "add", 3) == 0) {
        char *flags = strchr(input, ' ');
        if (flags == NULL) {
            snprintf(err, err_size, "add: invalid arguments -- task needs a name (-n) and a due date (-f): (%s)\n", input);
            return;
        }
        AddTaskFlags addTaskFlags = parseFlagsAdd(flags);
        if (addTaskFlags.success == 0) {
            snprintf(err, err_size, "add: invalid arguments -- malformed flags: (%s)\n", input);
            return;
        }
        time_t parsedDueDate = parseDate(addTaskFlags.duedate);
        if (parsedDueDate == (time_t) - 1) {
            snprintf(err, err_size, "add: invalid date format -- date needs to be in mm-dd-yyy format: %s\n", addTaskFlags.duedate);
            return;
        }
        int add_res = addTask(list, addTaskFlags.name, parsedDueDate);
        if (add_res == -1) {
            snprintf(err, err_size, "add: invalid arguments -- failed to add task\n");
            return;
        }
    }
    else if (strncmp(input, "mark", 4) == 0) {
        char *flags = strchr(input, ' ');
        if (flags == NULL) {
            snprintf(err, err_size, "mark: invalid arguments -- need to specify task id (-i): (%s)\n", input);
            return;
        }
        MarkTaskFlags markTaskFlags = parseFlagsMark(flags);
        if (markTaskFlags.success == 0) {
            snprintf(err, err_size, "mark: invalid arguments -- malformed flags: (%s)\n", input);
            return;
        }
        int remove_res = removeTask(list, markTaskFlags.task_id);
        if (remove_res == -1) {
            snprintf(err, err_size, "mark: invalid arguments -- task id (%d) not found\n", markTaskFlags.task_id);
            return;
        }
    }
    else if (strncmp(input, "-help", 5) == 0) {
        snprintf(
            err, err_size, 
            "List of commands:\n\n"
            "add -n (task name) -d (mm-dd-yyyy)\n"
            "mark -i (task id)\n"
            "exit\n"
        );
    }
    else {
        snprintf(err, err_size, "invalid cmd: %s is not a valid command. (type -help to list of valid commands)\n", input);
        return;
    }
}