#include <stdio.h>


#define JUMP_MENU(name, type, ...) \
    typedef struct {type func; const char *desc;} name##_menu_t; \
    name##_menu_t name[] = {__VA_ARGS__}

// Example function pointer type
typedef int (*my_func_ptr)(int);

// Example functions
int add_one(int x)    { return x + 1; }
int square(int x)     { return x * x; }
int negate(int x)     { return -x; }

int main() {
    // Use the macro to create an array of function-description pairs
    JUMP_MENU(pairs, my_func_ptr,
        {add_one, "Adds one to the input"},
        {square, "Squares the input"},
        {negate, "Negates the input"}
    );

    // Use the pairs
    size_t count = sizeof(pairs)/sizeof(pairs[0]);
    for (size_t i = 0; i < count; i++) {
        printf("Func %zu: %s → %d\n", i, pairs[i].desc, pairs[i].func(10));
    }

    return 0;
}
