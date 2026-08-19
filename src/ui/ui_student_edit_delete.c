#include "ui.h"

#include <stdio.h>

#include "auth.h"
#include "storage.h"
#include "student.h"
#include "ui_dialogs.h"
#include "validation.h"

void ui_student_edit_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    (void)user_data;
    GtkWindow *win = ui_student_root_window();
    Student *s = ui_student_selected();
    if (!s) {
        ui_error(win, "Select a student first.");
        return;
    }
    if (!auth_check_department(s->department)) {
        ui_error(win, "No access to this department.");
        return;
    }
    FieldSpec fields[3] = {
        {"First name", {0}, NULL},
        {"Surname", {0}, NULL},
        {"Notes", {0}, NULL},
    };
    snprintf(fields[0].value, sizeof(fields[0].value), "%s", s->name);
    snprintf(fields[1].value, sizeof(fields[1].value), "%s", s->surname);
    snprintf(fields[2].value, sizeof(fields[2].value), "%s", s->notes);
    if (!ui_prompt_fields(win, "Edit Student", fields, 3)) return;
    if (!validate_name(fields[0].value) || !validate_name(fields[1].value)) {
        ui_error(win, "Invalid name format.");
        return;
    }
    format_name(fields[0].value);
    format_name(fields[1].value);
    student_update(s, fields[0].value, fields[1].value, fields[2].value,
                   s->is_erasmus);
    log_audit("STUDENT_EDIT", s->am);
    storage_save();
    ui_refresh_all();
}

void ui_student_delete_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    (void)user_data;
    GtkWindow *win = ui_student_root_window();
    Student *s = ui_student_selected();
    if (!s) {
        ui_error(win, "Select a student first.");
        return;
    }
    if (!auth_check_department(s->department)) {
        ui_error(win, "No access to this department.");
        return;
    }
    char msg[128];
    snprintf(msg, sizeof(msg), "Delete student %s?", s->am);
    if (!ui_confirm(win, "Confirm", msg)) return;
    student_remove_by_am(s->am);
    log_audit("STUDENT_DELETE", s->am);
    storage_save();
    ui_refresh_all();
}