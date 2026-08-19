#include <stdlib.h>
#include "test_util.h"

void test_validation(void);
void test_student(void);
void test_account(void);
void test_transaction(void);
void test_auth(void);

int g_checks = 0;
int g_failures = 0;

int main(void) {
    RUN_SUITE(test_validation);
    RUN_SUITE(test_student);
    RUN_SUITE(test_account);
    RUN_SUITE(test_transaction);
    RUN_SUITE(test_auth);
    TEST_REPORT();
    return g_failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}