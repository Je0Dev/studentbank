#ifndef UI_MODELS_H
#define UI_MODELS_H

#include <glib-object.h>
#include <gtk/gtk.h>

#include "account.h"
#include "student.h"

#define TYPE_STUDENT_ROW (student_row_get_type())
G_DECLARE_FINAL_TYPE(StudentRow, student_row, STUDENT, ROW, GObject)

#define TYPE_ACCOUNT_ROW (account_row_get_type())
G_DECLARE_FINAL_TYPE(AccountRow, account_row, ACCOUNT, ROW, GObject)

StudentRow *student_row_new(const Student *s);
AccountRow *account_row_new(const Account *a);

const char *student_row_field(StudentRow *r, int idx);
const char *account_row_field(AccountRow *r, int idx);
int account_row_number(AccountRow *r);

typedef void (*UiListItemBind)(GtkListItemFactory *factory, GtkListItem *item,
                               gpointer user_data);

GtkListItemFactory *ui_label_factory(UiListItemBind bind, gpointer user_data);

#endif