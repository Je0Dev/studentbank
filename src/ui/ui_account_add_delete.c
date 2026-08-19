#include "ui.h"

#include <stdio.h>
#include <stdlib.h>

#include "account.h"
#include "auth.h"
#include "storage.h"
#include "student.h"
#include "ui_dialogs.h"
#include "validation.h"

void ui_account_add_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    (void)user_data;
    GtkWindow *win = ui_account_root_window();
    if (!auth_is_authenticated()) {
        ui_error(win, "Login required to add accounts.");
        return;
    }
    FieldSpec fields[4] = {
        {"Owner AM", {0}, NULL},
        {"Type (1=Dept, 2=Erasmus, 3=Notes)", {0}, NULL},
        {"PIN (4 digits)", {0}, NULL},
        {"Initial balance", {0}, NULL},
    };
    if (!ui_prompt_fields(win, "Add Account", fields, 4)) return;
    Student *s = student_find_by_am(fields[0].value);
    if (!s) {
        ui_error(win, "Student not found.");
        return;
    }
    if (!auth_check_department(s->department)) {
        ui_error(win, "No access to this department.");
        return;
    }
    int type = atoi(fields[1].value);
    if (type < ACCT_DEPARTMENT || type > ACCT_NOTES ||
        !validate_pin(fields[2].value)) {
        ui_error(win, "Invalid type or PIN.");
        return;
    }
    Account *acc = account_create(account_generate_number(), fields[2].value,
                                  g_strtod(fields[3].value, NULL), type, s);
    if (!account_add_to_student(s, acc)) {
        ui_error(win, "Account limit reached or invalid.");
        return;
    }
    log_audit("ACCOUNT_ADD", s->am);
    storage_save();
    ui_refresh_all();
}

void ui_account_delete_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    (void)user_data;
    GtkWindow *win = ui_account_root_window();
    Account *acc = ui_account_selected();
    if (!acc) {
        ui_error(win, "Select an account first.");
        return;
    }
    if (!auth_check_department(acc->owner->department)) {
        ui_error(win, "No access to this department.");
        return;
    }
    char msg[128];
    snprintf(msg, sizeof(msg), "Delete account %d?", acc->number);
    if (!ui_confirm(win, "Confirm", msg)) return;
    account_remove_from_student(acc->owner, acc->number);
    log_audit("ACCOUNT_DELETE", "");
    storage_save();
    ui_refresh_all();
}