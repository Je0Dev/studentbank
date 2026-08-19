#include "test_util.h"

#include "account.h"
#include "student.h"
#include "transaction.h"

static void reset(void) {
    student_free_all();
}

static Account *make_account(double balance) {
    Student *s = student_create("11001", "John", "Doe", DEPT_MECH_ENG, 0, NULL);
    student_list_add(s);
    Account *acc = account_create(2000, "1234", balance, ACCT_DEPARTMENT, s);
    account_add_to_student(s, acc);
    return acc;
}

static void test_valid_amount(void) {
    CHECK(tx_valid_amount(0.01));
    CHECK(tx_valid_amount(100.0));
    CHECK(!tx_valid_amount(0.0));
    CHECK(!tx_valid_amount(-5.0));
}

static void test_deposit(void) {
    reset();
    Account *acc = make_account(100.0);
    CHECK(tx_deposit(acc, 50.0) == 1);
    CHECK(tx_balance(acc) == 150.0);
    CHECK(tx_deposit(acc, 0.0) == 0);
    CHECK(tx_deposit(acc, -10.0) == 0);
    CHECK(tx_balance(acc) == 150.0);
    reset();
}

static void test_withdraw(void) {
    reset();
    Account *acc = make_account(100.0);
    CHECK(tx_withdraw(acc, 40.0) == 1);
    CHECK(tx_balance(acc) == 60.0);
    CHECK(tx_withdraw(acc, 61.0) == 0);
    CHECK(tx_balance(acc) == 60.0);
    CHECK(tx_withdraw(acc, 60.0) == 1);
    CHECK(tx_balance(acc) == 0.0);
    CHECK(tx_withdraw(acc, 1.0) == 0);
    CHECK(tx_withdraw(acc, -5.0) == 0);
    reset();
}

static void test_null_account(void) {
    CHECK(tx_deposit(NULL, 10.0) == 0);
    CHECK(tx_withdraw(NULL, 10.0) == 0);
    CHECK(tx_balance(NULL) == 0.0);
}

void test_transaction(void) {
    test_valid_amount();
    test_deposit();
    test_withdraw();
    test_null_account();
}