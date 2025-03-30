#include <stdio.h>

#include "jump_table.h"

// Example function pointer type
typedef int (*func_ptr)(int);

// Example functions
int add_one(int x) { return x + 1; }
int square(int x) { return x * x; }
int negate(int x) { return -x; }
int doubler(int x) { return x * 2; }

static void test(void);
static void test_EZ(void);

int main() {
    test();
    test_EZ();

    return 0;
}


void test()
{
    MULTIJUMP_TABLE(funx, func_ptr, 2,
        {add_one, square},
        {negate, doubler}
    );
    
    // Test the table
    int base = 5;
    printf("Using %d as a base...\n", base);
    for (int i = 0; i < JUMP_TABLE_COUNT(funx); i++) {
        for (int j = 0; j < JUMP_TABLE_COUNT(funx[i]); j++) {
            printf("Func [%d,%d] result: %d\n", i, j, funx[i][j](base));
        }
    }
}


void test_EZ()
{
    MULTIJUMP_TABLE_EZ(funx, func_ptr,
        {add_one, square},
        {negate, doubler}
    );
    
    // Test the table
    int base = 5;
    printf("Using %d as a base...\n", base);
    for (int i = 0; i < JUMP_TABLE_COUNT(funx); i++) {
        for (int j = 0; j < JUMP_TABLE_COUNT(funx[i]); j++) {
            printf("Func [%d,%d] result: %d\n", i, j, funx[i][j](base));
        }
    }
}