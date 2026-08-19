#include "ui.h"

#include "search.h"
#include "student.h"
#include "ui_models.h"

static GtkWidget *page = NULL;
static GListStore *store = NULL;
static GtkSingleSelection *selection = NULL;
static GtkWidget *search_entry = NULL;

static void student_bind(GtkListItemFactory *factory, GtkListItem *item,
                         gpointer user_data) {
    (void)factory;
    StudentRow *row = gtk_list_item_get_item(item);
    GtkWidget *label = gtk_list_item_get_child(item);
    gtk_label_set_text(GTK_LABEL(label),
                       student_row_field(row, GPOINTER_TO_INT(user_data)));
}

static GtkColumnViewColumn *student_col(const char *title, int idx) {
    return gtk_column_view_column_new(
        title, ui_label_factory(student_bind, GINT_TO_POINTER(idx)));
}

static void on_search_changed(GtkEditable *editable, gpointer user_data) {
    (void)editable;
    (void)user_data;
    ui_students_refresh();
}

static GtkWidget *toolbar_button(const char *label, GCallback cb) {
    GtkWidget *b = gtk_button_new_with_label(label);
    g_signal_connect(b, "clicked", cb, NULL);
    return b;
}

GtkWidget *ui_students_page(void) {
    return page;
}

GtkSingleSelection *ui_students_selection(void) {
    return selection;
}

GtkWidget *ui_students_new(void) {
    page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_set_margin_top(page, 12);
    gtk_widget_set_margin_bottom(page, 12);
    gtk_widget_set_margin_start(page, 12);
    gtk_widget_set_margin_end(page, 12);

    GtkWidget *toolbar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_box_append(GTK_BOX(page), toolbar);

    search_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(search_entry), "Search by name...");
    g_signal_connect(search_entry, "changed", G_CALLBACK(on_search_changed), NULL);
    gtk_widget_set_hexpand(search_entry, TRUE);
    gtk_box_append(GTK_BOX(toolbar), search_entry);
    gtk_box_append(GTK_BOX(toolbar),
                   toolbar_button("Add", G_CALLBACK(ui_student_add_clicked)));
    gtk_box_append(GTK_BOX(toolbar),
                   toolbar_button("Edit", G_CALLBACK(ui_student_edit_clicked)));
    gtk_box_append(GTK_BOX(toolbar),
                   toolbar_button("Delete", G_CALLBACK(ui_student_delete_clicked)));

    store = g_list_store_new(TYPE_STUDENT_ROW);
    selection = gtk_single_selection_new(G_LIST_MODEL(store));

    GtkWidget *view = gtk_column_view_new(GTK_SELECTION_MODEL(selection));
    gtk_column_view_append_column(GTK_COLUMN_VIEW(view), student_col("AM", 0));
    gtk_column_view_append_column(GTK_COLUMN_VIEW(view), student_col("Name", 1));
    gtk_column_view_append_column(GTK_COLUMN_VIEW(view), student_col("Surname", 2));
    gtk_column_view_append_column(GTK_COLUMN_VIEW(view), student_col("Dept", 3));
    gtk_column_view_append_column(GTK_COLUMN_VIEW(view), student_col("Erasmus", 4));
    gtk_column_view_append_column(GTK_COLUMN_VIEW(view), student_col("Accts", 5));

    GtkWidget *scroll = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), view);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll),
                                   GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_vexpand(scroll, TRUE);
    gtk_box_append(GTK_BOX(page), scroll);

    return page;
}

void ui_students_refresh(void) {
    if (!page) return;
    g_list_store_remove_all(store);
    const char *query = gtk_editable_get_text(GTK_EDITABLE(search_entry));
    for (Student *s = g_students; s; s = s->next_head) {
        if (query[0] && !student_matches_name(s, query)) continue;
        StudentRow *row = student_row_new(s);
        g_list_store_append(store, row);
        g_object_unref(row);
    }
}