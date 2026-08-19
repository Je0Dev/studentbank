#include "account.h"

#include "student.h"
#include "validation.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>

Account *account_create(int number, const char *pin, double balance,
                        int type, Student *owner) {
    Account *a = (Account *)malloc(sizeof(Account));
    if (!a) return NULL;
    a->number = number;
    strncpy(a->pin, pin, PIN_BUF - 1);
    a->pin[PIN_BUF - 1] = '\0';
    a->balance = balance;
    a->type = type;
    a->owner = owner;
    a->next = NULL;
    return a;
}

void account_free_all(Student *s) {
    if (!s) return;
    Account *a = s->accounts_head;
    while (a) {
        Account *next = a->next;
        free(a);
        a = next;
    }
    s->accounts_head = NULL;
    s->num_accounts = 0;
}

int account_add_to_student(Student *s, Account *a) {
    if (!s || !a || !validate_pin(a->pin)) return 0;
    if (s->num_accounts >= MAX_ACCOUNTS_PER_STUDENT) return 0;
    a->next = s->accounts_head;
    s->accounts_head = a;
    s->num_accounts++;
    return 1;
}

int account_remove_from_student(Student *s, int number) {
    if (!s) return 0;
    Account **link = &s->accounts_head;
    while (*link) {
        if ((*link)->number == number) {
            Account *victim = *link;
            *link = victim->next;
            free(victim);
            s->num_accounts--;
            return 1;
        }
        link = &(*link)->next;
    }
    return 0;
}

Account *account_find_in_student(Student *s, int number) {
    for (Account *a = s ? s->accounts_head : NULL; a; a = a->next) {
        if (a->number == number) return a;
    }
    return NULL;
}

int account_verify_pin(Account *a, const char *pin) {
    return a && pin && strcmp(a->pin, pin) == 0;
}

Account *account_find_global(int number, Student **owner) {
    for (Student *s = g_students; s; s = s->next_head) {
        Account *a = account_find_in_student(s, number);
        if (a) {
            if (owner) *owner = s;
            return a;
        }
    }
    if (owner) *owner = NULL;
    return NULL;
}

int account_count_for(Student *s) {
    return s ? s->num_accounts : 0;
}

int account_number_exists(int number) {
    return account_find_global(number, NULL) != NULL;
}

int account_generate_number(void) {
    int n;
    do {
        n = 1000 + rand() % 9000;
    } while (account_number_exists(n));
    return n;
}

const char *account_type_name(int type) {
    switch (type) {
        case ACCT_DEPARTMENT: return "Department";
        case ACCT_ERASMUS: return "Erasmus";
        case ACCT_NOTES: return "Notes";
        default: return "Other";
    }
}