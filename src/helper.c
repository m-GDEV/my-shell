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

void print_error(char* str)
{
    fprintf(stderr, "%s\n", str);
}

bool user_wants_exit(char* arg)
{
    if (strcmp("exit", arg) == 0) {
        return true;
    } else if (strcmp("Exit", arg) == 0) {
        return true;
    } else if (strcmp("bye", arg) == 0) {
        return true;
    } else if (strcmp("e", arg) == 0) {
        return true;
    } else {
        return false;
    }
}

// Stupid i should make this better
void free_memory(args_struct* args, char* buf, char* path)
{
    for (int i = 0; i < args->args_size; i++) {
        free(args->strings[i]);
    }
    free(args->strings);
    free(args);

    free(buf);
    free(path);
}

// Function does not take input as there are default location(s) for config file
alias_struct* process_config(void)
{
    // Try to load config file
    char* user = getenv("USER");
    // config_path: '/home/'+USER+/.myshell.rc+\0
    char* config_path = malloc(sizeof(char) * (6 + strlen(user) + 13));
    sprintf(config_path, "/home/%s/.myshell.rc", user);

    FILE* config_file = fopen(config_path, "r");

    if (config_file == NULL) {
        fprintf(stderr, "myshell: config not found at: %s\n", config_path);
        return NULL;
    }

    // Read lines of config file
    char** aliases = malloc(sizeof(char*) * 1);
    int aliases_size = 1;

    while (!feof(config_file)) {
        char* buf = NULL;
        size_t buf_size;

        getline(&buf, &buf_size, config_file);

        // For some reason a n-lenth file will run n+1 times (maybe EOF is weird or something)
        if (strcmp("", buf) == 0) {
            break;
        }

        remove_char('\n', buf);
        aliases[aliases_size - 1] = buf;
        aliases_size++;
        aliases = realloc(aliases, sizeof(char*) * aliases_size);
    }

    for (int i = 0; i < aliases_size; i++) {
        /** printf("aliases[%d]: %s\n", i, aliases[i]); */
    }

    // Parse config file - wont be robust but will work
    alias_struct* alias_collection = malloc(sizeof(alias_struct) * (aliases_size - 1));

    // For every line in the config (for loop), do strtok and capture information (while loop)
    for (int i = 0; i < aliases_size; i++) {
        char* next = strtok(aliases[i], ">");
        int parse_level = 0;
        while (next != NULL) {
            /** printf("[%d] strtok '%s'\n", i, next); */

            // If first element in line != 'alias'
            if (parse_level == 0 && strcmp("alias", next)) {
                fprintf(stderr, "myshell: cannot parse configuration file at: %s\n", config_path);
                return NULL;
            }
            // capture user-defined alias
            else if (parse_level == 1) {
                alias_collection[i].alias = next;
            }
            // capture user-defined actual command
            else if (parse_level == 2) {
                alias_collection[i].actual_command = next;
            }

            next = strtok(NULL, ">");
            parse_level++;
        }

        // If there are mor that three fields separated by a '>' it violates our config
        // second conditin handles weird EOF thing metnioned above
        if (parse_level != 3 && i != aliases_size - 1) {
            fprintf(stderr, "myshell: cannot parse configuration file at: %s\n", config_path);
            return NULL;
        }
    }

    for (int i = 0; i < aliases_size - 1; i++) {
        printf("[%d] alias: %s | actual: %s\n", i, alias_collection[i].alias, alias_collection[i].actual_command);
    }

    return alias_collection;
}

// Takes str as input as returns segments of str (based on delim) as a null-terminated array
args_struct* str_to_args(char* str, char* delim)
{
    args_struct* args = malloc(sizeof(args_struct) * 1);
    args->args_size = 1;
    // last args should be NULL
    // here we assume there are no other args, if there are we will realloc
    args->strings = malloc(sizeof(char*) * args->args_size);
    if (args == NULL) {
        exit_with_error("allocating memory for args");
    }
    if (args->strings == NULL) {
        exit_with_error("allocating memory for args->strings");
    }

    // Convert input from one string to n strings delimited by " "
    char* next = NULL;
    next = strtok(str, delim);
    /** printf("buf %d: %s\n", args_size - 2, next); */
    while (next != NULL) {
        // Because strtok inserts a \0 at every delimiter we don't need to make the last char \0
        args->strings[args->args_size - 1] = strdup(next);
        if (args->strings[args->args_size - 1] == NULL) {
            exit_with_error("copying to args->strings[i]");
        }

        /** printf("arg %d: %s - len: %ld\n", args->args_size - 2, args->strings[args->args_size - 2], strlen(args->strings[args->args_size - 2])); */
        next = strtok(NULL, delim);

        args->strings = realloc(args->strings, sizeof(char*) * ++args->args_size);
        if (args->strings == NULL) {
            exit_with_error("re-allocating args->strings");
        }
    }

    // Null terminated for exec
    args->strings[args->args_size - 1] = NULL;

    return args;
}
