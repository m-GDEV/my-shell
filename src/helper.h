// Needed for wait() and maybe other stuff
#include <stddef.h>
#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

// Helper structs
struct {
    char* alias;
    char* actual_command;
} typedef alias_struct;

struct {
    char** strings;
    size_t args_size;
} typedef args_struct;

// Helper functions
void remove_char(char c, char* buf);
void exit_with_error(char* str);
void print_error(char* str);
bool user_wants_exit(char* arg);
void free_memory(args_struct* args, char* buf, char* path);
alias_struct* process_config(int* alias_count);
args_struct* str_to_args(char* str, char* delim);
void print_args(args_struct* args);
