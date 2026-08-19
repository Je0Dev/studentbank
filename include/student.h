#ifndef STUDENT_H
#define STUDENT_H

#include "constants.h"

typedef struct Account Account;

typedef struct Student {
    char am[AM_BUF];
    char name[MAX_NAME];
    char surname[MAX_NAME];
    int department;
    int is_erasmus;
    char notes[NOTES_LEN];
    Account *accounts_head;
    int num_accounts;
    struct Student *next_head;
} Student;

extern Student *g_students;

Student *student_create(const char *am, const char *name, const char *surname,
                        int department, int is_erasmus, const char *notes);
void student_free(Student *s);
int student_list_add(Student *s);
int student_has_am(const char *am);
Student *student_find_by_am(const char *am);
Student *student_find_by_account_number(int number, Account **out_account);
int student_update(Student *s, const char *name, const char *surname,
                   const char *notes, int is_erasmus);
int student_remove_by_am(const char *am);
int student_count(void);
void student_free_all(void);
const char *department_name(int dept);

#endif