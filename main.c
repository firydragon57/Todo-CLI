#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tasklist.h"
#include "command.h"

int main() {
    TaskList *list = createList();
    char input[256] = {0};
    char err[256] = {0};

    readTasks(list);
    sortTasks(list);
    initNextTaskId(list);

    while (1) {
        printTaskList(list, err);
        handleInput(list, input, sizeof(input), err, sizeof(err));
    }

    deleteList(list);

    return 0;
}