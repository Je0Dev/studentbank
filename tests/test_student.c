#include "test_util.h"

#include "account.h"
#include "student.h"
#include "validation.h"

#include <string.h>

static void reset(void) {
    student_free_all();
}

static Student *make_student(const char *am, const char *name) {
    return student_create(am, name, "Doe", DEPT_MECH_ENG, 0, NULL);
}

static void test_create_and_add(void) {
    reset();
    Student *s = make_student("11001", "John");
    CHECK(s != NULL);
    CHECK(student_list_add(s) == 1);
    CHECK(student_count() == 1);
    CHECK(student_has_am("11001"));
    CHECK(!student_has_am("99999"));
    reset();
}

static void test_duplicate_am(void) {
    reset();
    CHECK(student_list_add(make_student("12001", "Alice")) == 1);
    Student *dup = make_student("12001", "Bob");
    CHECK(student_list_add(dup) == 1);
    CHECK(student_count() == 2);
    Student *found = student_find_by_am("12001");
    CHECK(found != NULL);
    CHECK(strcmp(found->name, "Bob") == 0);
    reset();
}

static void test_find_and_remove(void) {
    reset();
    Student *a = make_student("11001", "John");
    Student *b = make_student("13002", "Mary");
    Student *c = make_student("12003", "Kostas");
    CHECK(student_list_add(a) == 1);
    CHECK(student_list_add(b) == 1);
    CHECK(student_list_add(c) == 1);
    CHECK(student_count() == 3);

    CHECK(student_remove_by_am("13002") == 1);
    CHECK(student_find_by_am("13002") == NULL);
    CHECK(student_count() == 2);
    CHECK(student_remove_by_am("99999") == 0);
    CHECK(student_remove_by_am("11001") == 1);
    CHECK(student_remove_by_am("12003") == 1);
    CHECK(student_count() == 0);
    reset();
}

static void test_erasmus_flag(void) {
    reset();
    Student *s = student_create("12004", "Anna", "Smith", DEPT_ELEC_ENG, 1, "Erasmus in Lyon");
    CHECK(student_list_add(s) == 1);
    Student *found = student_find_by_am("12004");
    CHECK(found != NULL);
    CHECK(found->is_erasmus == 1);
    CHECK(strcmp(found->notes, "Erasmus in Lyon") == 0);
    reset();
}

static void test_department_name(void) {
    CHECK(strcmp(department_name(DEPT_MECH_ENG), "Mechanical") == 0);
    CHECK(strcmp(department_name(DEPT_ELEC_ENG), "Electrical") == 0);
    CHECK(strcmp(department_name(DEPT_CIVIL_ENG), "Civil") == 0);
    CHECK(strcmp(department_name(99), "Unknown") == 0);
}

static void test_accounts_linked(void) {
    reset();
    Student *s = make_student("11005", "John");
    CHECK(student_list_add(s) == 1);
    Account *acc = account_create(4001, "1234", 100.0, ACCT_DEPARTMENT, s);
    CHECK(account_add_to_student(s, acc) == 1);
    CHECK(student_find_by_account_number(4001, NULL) == s);
    Account *found = NULL;
    CHECK(student_find_by_account_number(4001, &found) == s);
    CHECK(found == acc);
    reset();
}

static void test_update(void) {
    reset();
    Student *s = make_student("11001", "John");
    CHECK(student_list_add(s) == 1);
    CHECK(student_update(s, "Jane", "Roe", "notes", 1) == 1);
    CHECK(strcmp(s->name, "Jane") == 0);
    CHECK(strcmp(s->surname, "Roe") == 0);
    CHECK(strcmp(s->notes, "notes") == 0);
    CHECK(s->is_erasmus == 1);
    CHECK(student_update(NULL, "x", "y", NULL, 0) == 0);
    reset();
}

void test_student(void) {
    test_create_and_add();
    test_duplicate_am();
    test_find_and_remove();
    test_erasmus_flag();
    test_department_name();
    test_accounts_linked();
    test_update();
}