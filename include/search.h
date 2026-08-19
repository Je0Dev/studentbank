#ifndef SEARCH_H
#define SEARCH_H

#include "student.h"

int student_matches_name(const Student *s, const char *query);
int student_matches_department(const Student *s, int dept);
int search_count_by_department(int dept);
int search_count_erasmus(void);
int search_total_accounts(void);
double search_total_balance(void);

#endif