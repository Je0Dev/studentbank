#include "test_util.h"

#include "account.h"
#include "student.h"
#include "validation.h"

#include <stdlib.h>
#include <string.h>

static void reset(void) {
    student_free_all();
}

static Student *make_student(const char *am) {
    Student *s = student_create(am, "John", "Doe", DEPT_MECH_ENG, 0, NULL);
    student_list_add(s);
    return s;
}

static void test_create(void) {
    reset();
    Student *s = make_student("11001");
    Account *acc = account_create(2001, "1234", 50.0, ACCT_DEPARTMENT, s);
    CHECK(acc != NULL);
    CHECK(acc->number == 2001);
    CHECK(strcmp(acc->pin, "1234") == 0);
    CHECK(acc->balance == 50.0);
    CHECK(acc->type == ACCT_DEPARTMENT);
    CHECK(acc->owner == s);
    free(acc);
    reset();
}

static void test_add_and_limit(void) {
    reset();
    Student *s = make_student("11001");
    for (int i = 0; i < MAX_ACCOUNTS_PER_STUDENT; i++) {
        Account *acc = account_create(3000 + i, "1234", 0.0, ACCT_DEPARTMENT, s);
        CHECK(account_add_to_student(s, acc) == 1);
    }
    CHECK(s->num_accounts == MAX_ACCOUNTS_PER_STUDENT);
    Account *extra = account_create(9999, "1234", 0.0, ACCT_DEPARTMENT, s);
    CHECK(account_add_to_student(s, extra) == 0);
    free(extra);
    reset();
}

static void test_find_and_remove(void) {
    reset();
    Student *s = make_student("11001");
    account_add_to_student(s, account_create(5001, "1234", 10.0, ACCT_DEPARTMENT, s));
    account_add_to_student(s, account_create(5002, "5678", 20.0, ACCT_ERASMUS, s));

    CHECK(account_find_in_student(s, 5001) != NULL);
    CHECK(account_find_in_student(s, 5002) != NULL);
    CHECK(account_find_in_student(s, 7000) == NULL);

    Student *owner = NULL;
    Account *g = account_find_global(5002, &owner);
    CHECK(g != NULL);
    CHECK(owner == s);

    CHECK(account_remove_from_student(s, 5001) == 1);
    CHECK(account_find_in_student(s, 5001) == NULL);
    CHECK(s->num_accounts == 1);
    CHECK(account_remove_from_student(s, 5001) == 0);
    reset();
}

static void test_number_generation(void) {
    reset();
    Student *s = make_student("11001");
    account_add_to_student(s, account_create(4000, "1234", 0.0, ACCT_DEPARTMENT, s));
    int n = account_generate_number();
    CHECK(n >= 1000 && n <= 9999);
    CHECK(!account_number_exists(n));
    reset();
}

static void test_type_name(void) {
    CHECK(strcmp(account_type_name(ACCT_DEPARTMENT), "Department") == 0);
    CHECK(strcmp(account_type_name(ACCT_ERASMUS), "Erasmus") == 0);
    CHECK(strcmp(account_type_name(ACCT_NOTES), "Notes") == 0);
    CHECK(strcmp(account_type_name(42), "Other") == 0);
}

static void test_invalid_pin_rejected(void) {
    reset();
    Student *s = make_student("11001");
    Account *acc = account_create(6000, "12a4", 0.0, ACCT_DEPARTMENT, s);
    CHECK(account_add_to_student(s, acc) == 0);
    free(acc);
    CHECK(s->num_accounts == 0);
    reset();
}

static void test_verify_pin(void) {
    reset();
    Student *s = make_student("11001");
    Account *acc = account_create(6001, "4321", 0.0, ACCT_DEPARTMENT, s);
    account_add_to_student(s, acc);
    CHECK(account_verify_pin(acc, "4321"));
    CHECK(!account_verify_pin(acc, "0000"));
    CHECK(!account_verify_pin(acc, NULL));
    CHECK(!account_verify_pin(NULL, "4321"));
    reset();
}

void test_account(void) {
    test_create();
    test_add_and_limit();
    test_find_and_remove();
    test_number_generation();
    test_type_name();
    test_invalid_pin_rejected();
    test_verify_pin();
}