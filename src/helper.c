#include "helper.h"

void remove_char(char c, char* buf)
{
    if (buf == NULL) {
        exit_with_error("buf must not be NULL");
    }

    for (int i = 0; i < strlen(buf); i++) {
        if (buf[i] == c) {
            buf[i] = '\0';
        }
    }
}

void exit_with_error(char* str)
{
    printf("Error: %s\n", str);
    exit(1);
}

bool user_wants_exit(char* arg)
{
    if (strcmp("exit", arg) == 0) {
        return true;
    } else if (strcmp("Exit", arg) == 0) {
        return true;
    } else if (strcmp("bye", arg) == 0) {
        return true;
    } else {
        return false;
    }
}

// Stupid i should make this better
void free_memory(char** args, int args_size, char* buf, char* path)
{
    for (int i = 0; i < args_size; i++) {
        free(args[i]);
    }
    free(args);

    free(buf);
    free(path);
}
