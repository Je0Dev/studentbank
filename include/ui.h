#ifndef UI_H
#define UI_H

#include <gtk/gtk.h>

#include "account.h"
#include "student.h"

GtkWidget *ui_window_new(GtkApplication *app);
void ui_refresh_all(void);
void ui_update_session_bar(void);
void ui_show_login_dialog(GtkWindow *parent);

GtkWidget *ui_dashboard_new(void);
GtkWidget *ui_students_new(void);
GtkWidget *ui_accounts_new(void);
void ui_dashboard_refresh(void);
void ui_students_refresh(void);
void ui_accounts_refresh(void);

GtkWidget *ui_students_page(void);
GtkSingleSelection *ui_students_selection(void);
GtkWidget *ui_accounts_page(void);
GtkSingleSelection *ui_accounts_selection(void);

GtkWindow *ui_student_root_window(void);
Student *ui_student_selected(void);
GtkWindow *ui_account_root_window(void);
Account *ui_account_selected(void);

void ui_student_add_clicked(GtkButton *button, gpointer user_data);
void ui_student_edit_clicked(GtkButton *button, gpointer user_data);
void ui_student_delete_clicked(GtkButton *button, gpointer user_data);
void ui_account_deposit_clicked(GtkButton *button, gpointer user_data);
void ui_account_withdraw_clicked(GtkButton *button, gpointer user_data);
void ui_account_add_clicked(GtkButton *button, gpointer user_data);
void ui_account_delete_clicked(GtkButton *button, gpointer user_data);

#endif