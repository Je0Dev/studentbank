#include "ui.h"

#include "account.h"
#include "auth.h"
#include "storage.h"
#include "student.h"
#include "transaction.h"
#include "ui_dialogs.h"
#include "ui_models.h"

GtkWindow *ui_account_root_window(void) {
    return GTK_WINDOW(gtk_widget_get_root(ui_accounts_page()));
}

Account *ui_account_selected(void) {
    GtkSingleSelection *selection = ui_accounts_selection();
    guint idx = gtk_single_selection_get_selected(selection);
    if (idx == GTK_INVALID_LIST_POSITION) return NULL;
    AccountRow *row = g_list_model_get_item(G_LIST_MODEL(selection), idx);
    if (!row) return NULL;
    Account *acc = NULL;
    student_find_by_account_number(account_row_number(row), &acc);
    g_object_unref(row);
    return acc;
}

static double prompt_amount(GtkWindow *win, const char *title) {
    char *text = ui_prompt_text(win, title, "Amount", NULL);
    if (!text) return -1.0;
    double amount = g_strtod(text, NULL);
    g_free(text);
    return amount;
}

static void transaction_flow(GtkWindow *win, const char *action, int deposit) {
    Account *acc = ui_account_selected();
    if (!acc) {
        ui_error(win, "Select an account first.");
        return;
    }
    char *pin = ui_prompt_text(win, action, "Enter 4-digit PIN", NULL);
    if (!pin) return;
    if (!account_verify_pin(acc, pin)) {
        ui_error(win, "Wrong PIN.");
        g_free(pin);
        return;
    }
    g_free(pin);
    double amount = prompt_amount(win, action);
    if (amount < 0) return;
    int ok = deposit ? tx_deposit(acc, amount) : tx_withdraw(acc, amount);
    if (!ok) {
        ui_error(win, deposit ? "Invalid deposit amount."
                              : "Invalid amount or insufficient funds.");
        return;
    }
    log_audit("TX", acc->owner->am);
    storage_save();
    ui_refresh_all();
}

void ui_account_deposit_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    (void)user_data;
    transaction_flow(ui_account_root_window(), "Deposit", 1);
}

void ui_account_withdraw_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    (void)user_data;
    transaction_flow(ui_account_root_window(), "Withdraw", 0);
}