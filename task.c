#include "task.h"

// Creats a task
Task *createTask(int task_id, char *name, time_t due_date, time_t create_at) {
    Task *task = (Task *) malloc(sizeof(Task));
    if (task == NULL) {
        return NULL;
    }

    task->task_id = task_id;
    task->name = malloc(strlen(name) + 1);
    if (task->name == NULL) {
        free(task);
        return NULL;
    }

    strcpy(task->name, name);
    task->due_date = due_date;

    if (create_at == (time_t) - 1) {
        task->created_at = time(NULL);
    }
    else {
        task->created_at = create_at;
    }

    return task;
}

Task *parseTask(char *task_str) {
    char *task_copy = malloc(strlen(task_str) + 1);
    if (task_copy == NULL) {
        fprintf(stderr, "Failed to malloc\n");
        return NULL;
    }
    strcpy(task_copy, task_str);

    char *saveptr2;
    char *task_id = strtok_r(task_copy, "|", &saveptr2);
    char *task_name = strtok_r(NULL, "|", &saveptr2);
    char *task_due = strtok_r(NULL, "|", &saveptr2);
    char *task_created = strtok_r(NULL, "|", &saveptr2);

    if (task_id == NULL || task_name == NULL || task_due == NULL || task_created == NULL) {
        fprintf(stderr, "Failed to read task parameters: task_id: %s | name: %s | due date: %s | created at: %s\n", task_id, task_name, task_due, task_created);
        free(task_copy);
        return NULL;
    }

    int id = atoi(task_id);
    time_t due_date = parseDate(task_due);
    time_t created_at = parseDate(task_created);

    if (due_date == (time_t) - 1 || created_at == (time_t) - 1) {
        fprintf(stderr, "Failed to parse the dates: due date: %s | created at: %s\n", task_due, task_created);
        free(task_copy);
        return NULL;
    }

    Task *task = createTask(id, task_name, due_date, created_at);
    if (task == NULL) {
        fprintf(stderr, "Failed to create task with given parameters task_id: %d | name: %s | due date: %s | created at: %s\n", id, task_name, task_due, task_created);
        free(task_copy);
        return NULL;
    }

    free(task_copy);
    return task;
}

// Gets the date from the user and parses it
time_t parseDate(char *date) {
    struct tm datetime = {0};

    char *month = strtok(date, "-");
    if (month == NULL || atoi(month) == 0) {
        fprintf(stderr, "Failed to read month\n");
        return (time_t) - 1;
    }

    char *day = strtok(NULL, "-");
    if (day == NULL || atoi(day) == 0) {
        fprintf(stderr, "Failed to read day\n");
        return (time_t) - 1;
    }

    char *year = strtok(NULL, "-");
    if (year == NULL || atoi(year) == 0) {
        fprintf(stderr, "Failed to read year\n");
        return (time_t) - 1;
    }

    datetime.tm_year = atoi(year) - 1900;
    datetime.tm_mon = atoi(month) - 1;
    datetime.tm_mday = atoi(day);
    datetime.tm_hour = 23;
    datetime.tm_min = 59;
    datetime.tm_sec = 59;
    datetime.tm_isdst = -1;

    return mktime(&datetime);
}

int computeDueAt(Task *task, char *output, size_t output_size) {
    double timeleft = difftime(task->due_date, time(NULL));
    int days = (int) (timeleft / 86400);
    int hours = (int) ((timeleft - days * 86400) / 3600);
    int minutes = (int) ((timeleft - days * 86400 - hours * 3600) / 60);

    int bytes_outputted = 0;

    if (days > 0) {
        bytes_outputted = snprintf(output, output_size, "%d Day(s)", days);
    }
    else if (hours > 0) {
        bytes_outputted = snprintf(output, output_size, "%d Hr(s)", hours);
    }
    else if (minutes > 0) {
        bytes_outputted = snprintf(output, output_size, "%d Min(s)", minutes);
    }
    else {
        bytes_outputted = snprintf(output, output_size, "Overdue");
    }

    return bytes_outputted;
}

// Prints out task details
int printTask(Task *task, char *output, size_t output_size, int spacing, int right_table_spacing) {
    char dueDate[64];
    char dueAt[64];

    strftime(dueDate, sizeof(dueDate), "%m-%d-%Y", localtime(&task->due_date));
    computeDueAt(task, dueAt, sizeof(dueAt));

    return snprintf(output, output_size, "| %d: %-*s | Due: %s | Due in: %-*s |", task->task_id, spacing, task->name, dueDate, right_table_spacing, dueAt);
}

// Deletes a task
void deleteTask(Task *task) {
    if (task == NULL) {
        return;
    }

    free(task->name);
    task->name = NULL;

    free(task);
}