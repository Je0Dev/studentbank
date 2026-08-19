#include "ui.h"

#include "search.h"
#include "storage.h"
#include "ui_dialogs.h"

#include <stdio.h>

static GtkWidget *page = NULL;
static GtkWidget *lbl_students = NULL;
static GtkWidget *lbl_mech = NULL;
static GtkWidget *lbl_elec = NULL;
static GtkWidget *lbl_civil = NULL;
static GtkWidget *lbl_erasmus = NULL;
static GtkWidget *lbl_accounts = NULL;
static GtkWidget *lbl_balance = NULL;

static void set_number(GtkWidget *label, int value) {
    char buf[32];
    snprintf(buf, sizeof(buf), "%d", value);
    gtk_label_set_text(GTK_LABEL(label), buf);
}

static void on_backup_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    (void)user_data;
    backup_create();
    backup_rotate();
    ui_error(GTK_WINDOW(gtk_widget_get_root(page)), "Backup created.");
}

GtkWidget *ui_dashboard_new(void) {
    page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_set_margin_top(page, 16);
    gtk_widget_set_margin_bottom(page, 16);
    gtk_widget_set_margin_start(page, 16);
    gtk_widget_set_margin_end(page, 16);

    GtkWidget *title = gtk_label_new("System Overview");
    gtk_label_set_xalign(GTK_LABEL(title), 0.0);
    gtk_box_append(GTK_BOX(page), title);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 8);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 16);
    gtk_box_append(GTK_BOX(page), grid);

    const char *names[] = {"Total students", "Mechanical", "Electrical",
                           "Civil", "Erasmus students", "Total accounts",
                           "Total balance"};
    GtkWidget **targets[] = {&lbl_students, &lbl_mech,   &lbl_elec,
                             &lbl_civil,    &lbl_erasmus, &lbl_accounts,
                             &lbl_balance};
    for (int i = 0; i < 7; i++) {
        GtkWidget *name = gtk_label_new(names[i]);
        gtk_label_set_xalign(GTK_LABEL(name), 0.0);
        *targets[i] = gtk_label_new("-");
        gtk_label_set_xalign(GTK_LABEL(*targets[i]), 0.0);
        gtk_grid_attach(GTK_GRID(grid), name, 0, i, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), *targets[i], 1, i, 1, 1);
    }

    GtkWidget *backup_btn = gtk_button_new_with_label("Create backup");
    g_signal_connect(backup_btn, "clicked", G_CALLBACK(on_backup_clicked), NULL);
    gtk_box_append(GTK_BOX(page), backup_btn);

    return page;
}

void ui_dashboard_refresh(void) {
    if (!page) return;
    set_number(lbl_students, student_count());
    set_number(lbl_mech, search_count_by_department(DEPT_MECH_ENG));
    set_number(lbl_elec, search_count_by_department(DEPT_ELEC_ENG));
    set_number(lbl_civil, search_count_by_department(DEPT_CIVIL_ENG));
    set_number(lbl_erasmus, search_count_erasmus());
    set_number(lbl_accounts, search_total_accounts());
    char buf[64];
    snprintf(buf, sizeof(buf), "%.2f", search_total_balance());
    gtk_label_set_text(GTK_LABEL(lbl_balance), buf);
}