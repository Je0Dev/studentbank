#ifndef TEST_UTIL_H
#define TEST_UTIL_H

#include <stdio.h>

extern int g_checks;
extern int g_failures;

#define CHECK(cond)                                    \
    do {                                               \
        g_checks++;                                    \
        if (!(cond)) {                                 \
            g_failures++;                              \
            printf("  FAIL %s:%d  %s\n",               \
                   __FILE__, __LINE__, #cond);         \
        }                                              \
    } while (0)

#define RUN_SUITE(name)                       \
    do {                                      \
        int before = g_failures;              \
        printf("== %s ==\n", #name);          \
        name();                               \
        printf("   %s: %s\n", #name,          \
               before == g_failures ? "PASS" : "FAIL"); \
    } while (0)

#define TEST_REPORT() \
    printf("\n%d checks, %d failures\n", g_checks, g_failures)

#endif