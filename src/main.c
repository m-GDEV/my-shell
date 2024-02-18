#include "helper.h"

int main(void)
{
    int return_code = 0;

    /** alias_struct* alias_collection = process_config(); */

    while (1) {
        // prompt
        if (return_code != 0) {
            printf("[%d] ", return_code);
        }
        printf("$ ");

        // Declaring our variables
        size_t buf_size = 0;
        char* buf = NULL;

        // Get input from user
        if (getline(&buf, &buf_size, stdin) == -1) {
            exit_with_error("reading input");
        }

        remove_char('\n', buf);

        // Check if user even entered anything
        if (strcmp("", buf) == 0) {
            free(buf);
            continue;
        }

        args_struct* args = str_to_args(buf, " ");

        // Check if args->strings[0] is an alias, if so replace it with alias
        // I'm pretty sure bash and other shells only uses aliases for the command and not args->strings
        /** if (strcmp(alias_collection[0].alias, args->strings[0])) { */
        /**     free(args->strings[0]); */
        /**     args->strings[0] = alias_collection[0].actual_command; */
        /** } */

        // User wants to exit
        if (user_wants_exit(args->strings[0])) {
            free_memory(args, buf, NULL);
            exit(0);
        }

        // Built in 'cd' command
        if (strcmp("cd", args->strings[0]) == 0) {

            char* cd_path = args->strings[1];
            char* home_path = NULL;

            // User wants to cd home
            if (args->strings[1] == NULL || strcmp("~", args->strings[1]) == 0) {
                char* user = getenv("USER");
                // '/home/'+USER+\0
                home_path = malloc(sizeof(char) * (6 + strlen(user) + 1));
                sprintf(home_path, "/home/%s", user);
                printf("%s\n", home_path);
                cd_path = home_path;
            }

            if (chdir(cd_path) == -1) {
                printf("my-shell: cd: %s: %s\n", args->strings[1], strerror(errno));
            }

            free_memory(args, buf, home_path);
            continue;
        }

        // path = '/bin/'+args->strings[0]+null terminator
        char* path = malloc(sizeof(char) * (strlen(args->strings[0]) + 1));
        if (path == NULL) {
            free_memory(args, buf, path);
            exit_with_error("allocating path");
        }

        // Crafting path for exec
        sprintf(path, "%s", args->strings[0]);

        // Fork and then exec command
        pid_t result = fork();

        if (result == -1) {
            exit_with_error("could not fork");
        } else if (result == 0) {

            if (execvp(path, args->strings) == -1) {
                printf("my-shell: %s: command not found\n", path);

                // Clean up memory
                free_memory(args, buf, path);

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
            free_memory(args, buf, path);
        }
    }
}
