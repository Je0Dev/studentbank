#include "storage.h"

#include "account.h"
#include "constants.h"
#include "student.h"

#include <stdio.h>

static void write_student(FILE *fp, const Student *s) {
    fwrite(s->am, 1, AM_BUF, fp);
    fwrite(s->name, 1, MAX_NAME, fp);
    fwrite(s->surname, 1, MAX_NAME, fp);
    fwrite(&s->department, sizeof(int), 1, fp);
    fwrite(&s->is_erasmus, sizeof(int), 1, fp);
    fwrite(s->notes, 1, NOTES_LEN, fp);
    int count = s->num_accounts;
    fwrite(&count, sizeof(int), 1, fp);
    for (Account *a = s->accounts_head; a; a = a->next) {
        fwrite(&a->number, sizeof(int), 1, fp);
        fwrite(a->pin, 1, PIN_BUF, fp);
        fwrite(&a->balance, sizeof(double), 1, fp);
        fwrite(&a->type, sizeof(int), 1, fp);
    }
}

void storage_save(void) {
    FILE *fp = fopen(DATA_FILE, "wb");
    if (!fp) {
        log_error("storage_save", "cannot open data file for writing");
        return;
    }
    int count = student_count();
    fwrite(&count, sizeof(int), 1, fp);
    for (Student *s = g_students; s; s = s->next_head) {
        write_student(fp, s);
    }
    fclose(fp);
}

static Student *read_student(FILE *fp) {
    Student *s = student_create("00000", "", "", DEPT_MECH_ENG, 0, "");
    if (fread(s->am, 1, AM_BUF, fp) != AM_BUF) {
        student_free(s);
        return NULL;
    }
    fread(s->name, 1, MAX_NAME, fp);
    fread(s->surname, 1, MAX_NAME, fp);
    fread(&s->department, sizeof(int), 1, fp);
    fread(&s->is_erasmus, sizeof(int), 1, fp);
    fread(s->notes, 1, NOTES_LEN, fp);
    int count = 0;
    fread(&count, sizeof(int), 1, fp);
    for (int i = 0; i < count; i++) {
        Account *a = account_create(0, "0000", 0.0, ACCT_DEPARTMENT, s);
        fread(&a->number, sizeof(int), 1, fp);
        fread(a->pin, 1, PIN_BUF, fp);
        fread(&a->balance, sizeof(double), 1, fp);
        fread(&a->type, sizeof(int), 1, fp);
        account_add_to_student(s, a);
    }
    return s;
}

void storage_load(void) {
    FILE *fp = fopen(DATA_FILE, "rb");
    if (!fp) return;
    student_free_all();
    int count = 0;
    if (fread(&count, sizeof(int), 1, fp) != 1) {
        fclose(fp);
        return;
    }
    for (int i = 0; i < count; i++) {
        Student *s = read_student(fp);
        if (!s) break;
        student_list_add(s);
    }
    fclose(fp);
}