#include "ui.h"

#include "account.h"
#include "student.h"
#include "ui_models.h"

static GtkWidget *page = NULL;
static GListStore *store = NULL;
static GtkSingleSelection *selection = NULL;

static void account_bind(GtkListItemFactory *factory, GtkListItem *item,
                         gpointer user_data) {
    (void)factory;
    AccountRow *row = gtk_list_item_get_item(item);
    GtkWidget *label = gtk_list_item_get_child(item);
    gtk_label_set_text(GTK_LABEL(label),
                       account_row_field(row, GPOINTER_TO_INT(user_data)));
}

static GtkColumnViewColumn *account_col(const char *title, int idx) {
    return gtk_column_view_column_new(
        title, ui_label_factory(account_bind, GINT_TO_POINTER(idx)));
}

static GtkWidget *toolbar_button(const char *label, GCallback cb) {
    GtkWidget *b = gtk_button_new_with_label(label);
    g_signal_connect(b, "clicked", cb, NULL);
    return b;
}

GtkWidget *ui_accounts_page(void) {
    return page;
}

GtkSingleSelection *ui_accounts_selection(void) {
    return selection;
}

GtkWidget *ui_accounts_new(void) {
    page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_set_margin_top(page, 12);
    gtk_widget_set_margin_bottom(page, 12);
    gtk_widget_set_margin_start(page, 12);
    gtk_widget_set_margin_end(page, 12);

    GtkWidget *toolbar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_box_append(GTK_BOX(page), toolbar);
    gtk_box_append(GTK_BOX(toolbar),
                   toolbar_button("Deposit", G_CALLBACK(ui_account_deposit_clicked)));
    gtk_box_append(GTK_BOX(toolbar),
                   toolbar_button("Withdraw", G_CALLBACK(ui_account_withdraw_clicked)));
    gtk_box_append(GTK_BOX(toolbar),
                   toolbar_button("Add Account", G_CALLBACK(ui_account_add_clicked)));
    gtk_box_append(GTK_BOX(toolbar),
                   toolbar_button("Delete", G_CALLBACK(ui_account_delete_clicked)));

    store = g_list_store_new(TYPE_ACCOUNT_ROW);
    selection = gtk_single_selection_new(G_LIST_MODEL(store));

    GtkWidget *view = gtk_column_view_new(GTK_SELECTION_MODEL(selection));
    gtk_column_view_append_column(GTK_COLUMN_VIEW(view), account_col("Number", 0));
    gtk_column_view_append_column(GTK_COLUMN_VIEW(view), account_col("Type", 1));
    gtk_column_view_append_column(GTK_COLUMN_VIEW(view), account_col("Owner", 2));
    gtk_column_view_append_column(GTK_COLUMN_VIEW(view), account_col("Balance", 3));

    GtkWidget *scroll = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), view);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll),
                                   GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_vexpand(scroll, TRUE);
    gtk_box_append(GTK_BOX(page), scroll);

    return page;
}

void ui_accounts_refresh(void) {
    if (!page) return;
    g_list_store_remove_all(store);
    for (Student *s = g_students; s; s = s->next_head) {
        for (Account *a = s->accounts_head; a; a = a->next) {
            AccountRow *row = account_row_new(a);
            g_list_store_append(store, row);
            g_object_unref(row);
        }
    }
}