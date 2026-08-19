#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "constants.h"

typedef struct Student Student;

typedef struct Account {
    int number;
    char pin[PIN_BUF];
    double balance;
    int type;
    Student *owner;
    struct Account *next;
} Account;

Account *account_create(int number, const char *pin, double balance,
                        int type, Student *owner);
void account_free_all(Student *s);
int account_add_to_student(Student *s, Account *a);
int account_remove_from_student(Student *s, int number);
Account *account_find_in_student(Student *s, int number);
int account_verify_pin(Account *a, const char *pin);
Account *account_find_global(int number, Student **owner);
int account_count_for(Student *s);
int account_generate_number(void);
int account_number_exists(int number);
const char *account_type_name(int type);

#endif