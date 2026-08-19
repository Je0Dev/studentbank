#include "ui.h"

#include <stdlib.h>

#include "auth.h"
#include "storage.h"
#include "student.h"
#include "ui_dialogs.h"
#include "ui_models.h"
#include "validation.h"

GtkWindow *ui_student_root_window(void) {
    return GTK_WINDOW(gtk_widget_get_root(ui_students_page()));
}

Student *ui_student_selected(void) {
    GtkSingleSelection *selection = ui_students_selection();
    guint idx = gtk_single_selection_get_selected(selection);
    if (idx == GTK_INVALID_LIST_POSITION) return NULL;
    StudentRow *row = g_list_model_get_item(G_LIST_MODEL(selection), idx);
    if (!row) return NULL;
    Student *s = student_find_by_am(student_row_field(row, 0));
    g_object_unref(row);
    return s;
}

void ui_student_add_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    (void)user_data;
    GtkWindow *win = ui_student_root_window();
    if (!auth_is_authenticated()) {
        ui_error(win, "Login required to add students.");
        return;
    }
    FieldSpec fields[5] = {
        {"AM (5 digits, e.g. 11001)", {0}, NULL},
        {"First name", {0}, NULL},
        {"Surname", {0}, NULL},
        {"Erasmus (0/1)", {0}, NULL},
        {"Notes", {0}, NULL},
    };
    if (!ui_prompt_fields(win, "Add Student", fields, 5)) return;
    if (!validate_am(fields[0].value)) {
        ui_error(win, "Invalid AM format.");
        return;
    }
    int dept = fields[0].value[1] - '0';
    if (!auth_check_department(dept)) {
        ui_error(win, "No access to this department.");
        return;
    }
    if (student_has_am(fields[0].value)) {
        ui_error(win, "AM already exists.");
        return;
    }
    if (!validate_name(fields[1].value) || !validate_name(fields[2].value)) {
        ui_error(win, "Invalid name format.");
        return;
    }
    format_name(fields[1].value);
    format_name(fields[2].value);
    int erasmus = atoi(fields[3].value) != 0;
    Student *s = student_create(fields[0].value, fields[1].value, fields[2].value,
                                dept, erasmus, fields[4].value);
    student_list_add(s);
    log_audit("STUDENT_ADD", fields[0].value);
    storage_save();
    ui_refresh_all();
}