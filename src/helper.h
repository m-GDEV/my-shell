// Needed for wait() and maybe other stuff
#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

// Helper functions
void remove_char(char c, char* buf);
void exit_with_error(char* str);
bool user_wants_exit(char* arg);
void free_memory(char** args, int args_size, char* buf, char* path);
