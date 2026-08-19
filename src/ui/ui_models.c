#include "ui_models.h"

#include <stdio.h>

struct _StudentRow {
    GObject parent;
    char am[AM_BUF];
    char name[MAX_NAME];
    char surname[MAX_NAME];
    char dept_label[16];
    char erasmus_label[8];
    char accounts_label[8];
};

struct _AccountRow {
    GObject parent;
    char number_label[8];
    char type_label[16];
    char balance_label[32];
    char owner_label[100];
    int account_number;
};

G_DEFINE_TYPE(StudentRow, student_row, G_TYPE_OBJECT)
G_DEFINE_TYPE(AccountRow, account_row, G_TYPE_OBJECT)

static void student_row_init(StudentRow *self) { (void)self; }
static void student_row_class_init(StudentRowClass *klass) { (void)klass; }
static void account_row_init(AccountRow *self) { (void)self; }
static void account_row_class_init(AccountRowClass *klass) { (void)klass; }

StudentRow *student_row_new(const Student *s) {
    StudentRow *r = g_object_new(TYPE_STUDENT_ROW, NULL);
    snprintf(r->am, sizeof(r->am), "%s", s->am);
    snprintf(r->name, sizeof(r->name), "%s", s->name);
    snprintf(r->surname, sizeof(r->surname), "%s", s->surname);
    snprintf(r->dept_label, sizeof(r->dept_label), "%s",
             department_name(s->department));
    snprintf(r->erasmus_label, sizeof(r->erasmus_label), "%s",
             s->is_erasmus ? "Yes" : "No");
    snprintf(r->accounts_label, sizeof(r->accounts_label), "%d",
             s->num_accounts);
    return r;
}

AccountRow *account_row_new(const Account *a) {
    AccountRow *r = g_object_new(TYPE_ACCOUNT_ROW, NULL);
    r->account_number = a->number;
    snprintf(r->number_label, sizeof(r->number_label), "%d", a->number);
    snprintf(r->type_label, sizeof(r->type_label), "%s",
             account_type_name(a->type));
    snprintf(r->balance_label, sizeof(r->balance_label), "%.2f", a->balance);
    snprintf(r->owner_label, sizeof(r->owner_label), "%s %s",
             a->owner->name, a->owner->surname);
    return r;
}

const char *student_row_field(StudentRow *r, int idx) {
    switch (idx) {
        case 0: return r->am;
        case 1: return r->name;
        case 2: return r->surname;
        case 3: return r->dept_label;
        case 4: return r->erasmus_label;
        case 5: return r->accounts_label;
        default: return "";
    }
}

const char *account_row_field(AccountRow *r, int idx) {
    switch (idx) {
        case 0: return r->number_label;
        case 1: return r->type_label;
        case 2: return r->owner_label;
        case 3: return r->balance_label;
        default: return "";
    }
}

int account_row_number(AccountRow *r) {
    return r->account_number;
}

static void label_setup(GtkListItemFactory *factory, GtkListItem *item,
                        gpointer user_data) {
    (void)factory;
    (void)user_data;
    gtk_list_item_set_child(item, gtk_label_new(NULL));
}

GtkListItemFactory *ui_label_factory(UiListItemBind bind,
                                     gpointer user_data) {
    GtkListItemFactory *factory = gtk_signal_list_item_factory_new();
    g_signal_connect(factory, "setup", G_CALLBACK(label_setup), NULL);
    g_signal_connect(factory, "bind", G_CALLBACK(bind), user_data);
    return factory;
}