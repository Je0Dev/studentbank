#include "student.h"

#include "account.h"
#include "validation.h"

#include <stdlib.h>
#include <string.h>

Student *g_students = NULL;

Student *student_create(const char *am, const char *name, const char *surname,
                        int department, int is_erasmus, const char *notes) {
    Student *s = (Student *)malloc(sizeof(Student));
    if (!s) return NULL;
    memset(s, 0, sizeof(Student));
    strncpy(s->am, am, AM_LEN);
    strncpy(s->name, name, MAX_NAME - 1);
    strncpy(s->surname, surname, MAX_NAME - 1);
    s->department = department;
    s->is_erasmus = is_erasmus;
    if (notes) strncpy(s->notes, notes, NOTES_LEN - 1);
    s->accounts_head = NULL;
    s->num_accounts = 0;
    return s;
}

void student_free(Student *s) {
    if (!s) return;
    account_free_all(s);
    free(s);
}

int student_list_add(Student *s) {
    if (!s) return 0;
    s->next_head = g_students;
    g_students = s;
    return 1;
}

int student_has_am(const char *am) {
    return student_find_by_am(am) != NULL;
}

Student *student_find_by_am(const char *am) {
    for (Student *s = g_students; s; s = s->next_head) {
        if (strcmp(s->am, am) == 0) return s;
    }
    return NULL;
}

Student *student_find_by_account_number(int number, Account **out_account) {
    for (Student *s = g_students; s; s = s->next_head) {
        Account *acc = account_find_in_student(s, number);
        if (acc) {
            if (out_account) *out_account = acc;
            return s;
        }
    }
    if (out_account) *out_account = NULL;
    return NULL;
}

int student_update(Student *s, const char *name, const char *surname,
                   const char *notes, int is_erasmus) {
    if (!s) return 0;
    if (name) strncpy(s->name, name, MAX_NAME - 1);
    if (surname) strncpy(s->surname, surname, MAX_NAME - 1);
    if (notes) strncpy(s->notes, notes, NOTES_LEN - 1);
    s->is_erasmus = is_erasmus;
    return 1;
}

int student_remove_by_am(const char *am) {
    Student **link = &g_students;
    while (*link) {
        if (strcmp((*link)->am, am) == 0) {
            Student *victim = *link;
            *link = victim->next_head;
            student_free(victim);
            return 1;
        }
        link = &(*link)->next_head;
    }
    return 0;
}

int student_count(void) {
    int n = 0;
    for (Student *s = g_students; s; s = s->next_head) n++;
    return n;
}

void student_free_all(void) {
    Student *s = g_students;
    while (s) {
        Student *next = s->next_head;
        student_free(s);
        s = next;
    }
    g_students = NULL;
}

const char *department_name(int dept) {
    switch (dept) {
        case DEPT_MECH_ENG: return "Mechanical";
        case DEPT_ELEC_ENG: return "Electrical";
        case DEPT_CIVIL_ENG: return "Civil";
        default: return "Unknown";
    }
}