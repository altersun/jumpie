#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>


#define JUMP_MENU(name, type, ...) \
    typedef struct {type func; const char *desc;} name##_menu_t; \
    name##_menu_t name[] = {__VA_ARGS__}

#define JUMPTABLE_COUNT(table) \
    (sizeof(table)/sizeof(table[0]))

#define JUMP_MENU_PRINT(menu, print_func) \
    for (int index = 0; index < JUMPTABLE_COUNT(menu); index++) { \
        print_func("%d: %s\n", index, menu[index].desc); \
    }

#define JUMP_MENU_STRINGIFY(menu, dest) \
    int dest##_remaining = sizeof(dest);\
    for (int index = 0; index < JUMPTABLE_COUNT(menu) && dest##_remaining > 0; index++) { \
        char* start = (dest + sizeof(dest)) - dest##_remaining; \
        dest##_remaining -= snprintf(start, dest##_remaining, "%d: %s\n", index, menu[index].desc ); \
    }



// Function type and examples
typedef int (*my_func_ptr)(int);
int increment(int x)  { return x + 1; }
int decrement(int x)  { return x - 1; }
int square(int x)     { return x * x; }
int negate(int x)     { return -x; }

// Presentation program helpers
bool is_integer(const char* str, int* output);
void print_usage(const char* progname, const char* options);


int main(int argc, char* argv[]) {
    
    // Use the macro to create an array of function-description pairs
    JUMP_MENU(pairs, my_func_ptr,
        {increment, "Increment input by 1"},
        {decrement, "Decrement input by 1"},
        {square, "Squares the input"},
        {negate, "Negates the input"}
    );

    // Needed for help menus
    char opt_list[128];
    JUMP_MENU_STRINGIFY(pairs, opt_list);

    // Check for help flag
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0], opt_list);
            return 0;
        }
    }

    // Validate argument count
    if (argc > 3) {
        printf("Error: Too many arguments. Provide at most two integers.\n");
        return -1;
    }

    // User args
    int user_selection = -1;
    int user_input = -1;

    // Parse user selection
    if (argc > 1) {
        int ret = strtoul(argv[1], NULL, 0);
        if (ret == 0 && strcmp(argv[1], "0") != 0) {
            printf("Bad option selection '%s'\n", argv[1]);
            return -1;
        }
        user_selection = ret;
    }

    // Parse user input
    if (argc > 2) {
        int ret = strtoul(argv[2], NULL, 0);
        if (ret == 0 && strcmp(argv[2], "0") != 0) {
            printf("Bad input '%s'\n", argv[2]);
            return -1;
        }
        user_input = ret;
    }

    // If user option not selected, request one
    if (user_selection < 0) {
        printf("Please select an option\n%s", opt_list);
        int ret = scanf("%d", &user_selection);
        if (user_selection < 0 || user_selection >= JUMPTABLE_COUNT(pairs)) {
            printf("Selection out of bounds\n");
            return -1;
        } else if (ret != 1) {
            printf("Input not recognized as an integral selection\n");
            return -1;
        }
    }

    // If user input not provided, request one
    if (user_input < 0) {
        printf("Please provide an input for '%s'\n", pairs[user_selection].desc);
        int ret = scanf("%d", &user_input);
        if (ret != 1) {
            printf("Input not recognized as an integral value\n");
            return -1;
        }
    }

    // Where the magic happens!
    printf("Result: %d\n", pairs[user_selection].func(user_input));

    // Magic over, get out >:-(
    return 0;
}

// Function to check if a string is a valid integer
bool is_integer(const char* str, int* output)
{
    if (*str == '-' || *str == '+') {   // Allow leading sign
        str++;
    }

    if (*str == '\0') {  // Empty string after sign is invalid
        return false;
    }

    while (*str) {
        if (!isdigit(*str)) {
            return false;  // Invalid if any character is non-digit
        }
        str++;
    }
    *output = atoi(str);
    return true;
}

// Display usage instructions
void print_usage(const char* progname, const char* options) 
{
    printf("Usage: %s [options] [operation] [input]\n", progname);
    printf("Options:\n");
    printf("  -h, --help     Display this help message\n");
    printf("\nProvide up to two integer arguments. Non-integer arguments will be rejected.\n");
    printf("Missing args will be requested via prompt\n");
    printf("Operations:\n%s", options);
}
