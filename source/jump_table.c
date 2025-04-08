#include <stdio.h>

#include <jump_table.h>

// Example function pointer type
typedef int (*func_ptr)(int);

// Example functions
int add_one(int x) { return x + 1; }
int square(int x) { return x * x; }
int negate(int x) { return -x; }

int main() {
    JUMP_TABLE(funcs, func_ptr, add_one, square, negate);
    size_t count = JUMP_TABLE_COUNT(funcs);

     // Test the array
     int base = 5;
     printf("Size: %ld. Using %d as a base...\n", count, base);
     for (int i = 0; i < (int)(count); i++) {
        printf("Func %d result: %d\n", i, funcs[i](base));
    }

    return 0;
}
