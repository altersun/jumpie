#ifndef JUMP_TABLE_H
#define JUMP_TABLE_H

#include <stdio.h> // For snprintf

#define JUMP_TABLE_COUNT(table) \
    (sizeof(table)/sizeof(table[0]))

#define JUMP_TABLE(name, type, ...) \
    type name[] = {__VA_ARGS__}

#define MULTIJUMP_TABLE(name, type, size, ...) \
    type name[][size] = {__VA_ARGS__}

#define JUMP_MENU(name, type, ...) \
    typedef struct {type func; const char *desc;} name##_menu_t; \
    name##_menu_t name[] = {__VA_ARGS__}

#define JUMP_MENU_PRINT(menu, print_func) \
    for (int index = 0; index < JUMP_TABLE_COUNT(menu); index++) { \
        print_func("%d: %s\n", index, menu[index].desc); \
    }

#define JUMP_MENU_STRINGIFY(menu, dest) \
    int dest##_remaining = sizeof(dest);\
    for (int index = 0; index < JUMP_TABLE_COUNT(menu) && dest##_remaining > 0; index++) { \
        char* start = (dest + sizeof(dest)) - dest##_remaining; \
        dest##_remaining -= snprintf(start, dest##_remaining, "%d: %s\n", index, menu[index].desc ); \
    }

#endif // JUMP_TABLE_H