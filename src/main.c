#include "helper.h"
#include <stdlib.h>

int main(void)
{
    int return_code = 0;

    while (1) {
        // prompt
        if (return_code != 0) {
            printf("[%d] ", return_code);
        }
        printf("$ ");

        // last args should be NULL
        // here we assume there are no other args, if there are we will realloc
        char** args = malloc(sizeof(char*) * 1);

        if (args == NULL) {
            exit_with_error("allocating memory for args");
        }

        int args_size = 1;
        size_t buf_size = 0;
        char* buf = NULL;
        char* next = NULL;

        if (getline(&buf, &buf_size, stdin) == -1) {
            exit_with_error("reading input");
        }
        remove_char('\n', buf);

        next = strtok(buf, " ");
        /** printf("buf %d: %s\n", args_size - 2, next); */
        while (next != NULL) {
            args[args_size - 1] = strdup(next);
            if (args[args_size - 1] == NULL) {
                exit_with_error("copying to args[i]");
            }

            /** printf("arg %d: %s - len: %ld\n", args_size - 2, args[args_size - 2], strlen(args[args_size - 2])); */

            next = strtok(NULL, " ");

            args = realloc(args, sizeof(char*) * ++args_size);
            if (args == NULL) {
                exit_with_error("re-allocating args");
            }
        }

        // Null terminated for exec
        args[args_size - 1] = NULL;

        // User wants to exit
        if (user_wants_exit(args[0])) {
            exit(0);
        }

        // path = '/bin/'+args[0]+null terminator
        char* path = malloc(sizeof(char) * (strlen(args[0]) + 1));
        if (path == NULL) {
            free_memory(args, args_size, buf, path);
            exit_with_error("allocating path");
        }

        // Crafting path for exec
        sprintf(path, "%s", args[0]);

        pid_t result = fork();

        if (result == -1) {
            exit_with_error("could not fork");
        } else if (result == 0) {

            if (execvp(path, args) == -1) {
                printf("shell: %s: command not found\n", path);

                // Clean up memory
                free_memory(args, args_size, buf, path);

                exit(1);
                /** exit_with_error("exec failed"); */
            }
        } else {
            int child_status = 0;
            wait(&child_status);

            if (WIFEXITED(child_status)) {
                return_code = WEXITSTATUS(child_status);
            }

            // Clean up memory
            free_memory(args, args_size, buf, path);
        }
    }
}
