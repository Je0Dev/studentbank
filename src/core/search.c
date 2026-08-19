#include "search.h"

#include "account.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

static char lower_char(char c) {
    return (char)tolower((unsigned char)c);
}

int student_matches_name(const Student *s, const char *query) {
    if (!s || !query || !query[0]) return 0;
    size_t qlen = strlen(query);
    if (qlen > MAX_NAME * 2 + 1) return 0;
    char combined[MAX_NAME * 2 + 2];
    snprintf(combined, sizeof(combined), "%s %s", s->name, s->surname);
    for (size_t i = 0; combined[i]; i++) combined[i] = lower_char(combined[i]);
    for (size_t i = 0; i + qlen <= strlen(combined); i++) {
        int match = 1;
        for (size_t j = 0; j < qlen; j++) {
            if (combined[i + j] != lower_char(query[j])) {
                match = 0;
                break;
            }
        }
        if (match) return 1;
    }
    return 0;
}

int student_matches_department(const Student *s, int dept) {
    return s && s->department == dept;
}

int search_count_by_department(int dept) {
    int n = 0;
    for (Student *s = g_students; s; s = s->next_head) {
        if (student_matches_department(s, dept)) n++;
    }
    return n;
}

int search_count_erasmus(void) {
    int n = 0;
    for (Student *s = g_students; s; s = s->next_head) {
        if (s->is_erasmus) n++;
    }
    return n;
}

int search_total_accounts(void) {
    int n = 0;
    for (Student *s = g_students; s; s = s->next_head) {
        n += s->num_accounts;
    }
    return n;
}

double search_total_balance(void) {
    double total = 0.0;
    for (Student *s = g_students; s; s = s->next_head) {
        for (Account *a = s->accounts_head; a; a = a->next) {
            total += a->balance;
        }
    }
    return total;
}