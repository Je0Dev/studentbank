#include "ui.h"

#include "auth.h"
#include "storage.h"
#include "ui_dialogs.h"

static void do_login(GtkWindow *parent, GtkWidget *user_entry,
                     GtkWidget *pass_entry, GtkWidget *win) {
    const char *user = gtk_editable_get_text(GTK_EDITABLE(user_entry));
    const char *pass = gtk_editable_get_text(GTK_EDITABLE(pass_entry));
    if (auth_authenticate(user, pass)) {
        log_audit("AUTH_SUCCESS", user);
        ui_update_session_bar();
        ui_refresh_all();
        gtk_window_destroy(GTK_WINDOW(win));
    } else {
        log_audit("AUTH_FAILED", user);
        ui_error(parent, "Invalid username or password.");
    }
}

static void on_login_clicked(GtkButton *button, gpointer data) {
    (void)button;
    GtkWidget *win = data;
    GtkWidget *user_entry = g_object_get_data(G_OBJECT(win), "user");
    GtkWidget *pass_entry = g_object_get_data(G_OBJECT(win), "pass");
    GtkWindow *parent = GTK_WINDOW(gtk_widget_get_root(GTK_WIDGET(win)));
    do_login(parent, user_entry, pass_entry, win);
}

static void on_cancel_clicked(GtkButton *button, gpointer data) {
    (void)button;
    gtk_window_destroy(GTK_WINDOW(data));
}

void ui_show_login_dialog(GtkWindow *parent) {
    GtkWidget *win = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(win), "Login");
    gtk_window_set_modal(GTK_WINDOW(win), TRUE);
    gtk_window_set_transient_for(GTK_WINDOW(win), parent);
    gtk_window_set_resizable(GTK_WINDOW(win), FALSE);
    GtkWidget *content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_set_margin_top(content, 12);
    gtk_widget_set_margin_bottom(content, 12);
    gtk_widget_set_margin_start(content, 12);
    gtk_widget_set_margin_end(content, 12);
    gtk_window_set_child(GTK_WINDOW(win), content);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 6);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 8);
    gtk_box_append(GTK_BOX(content), grid);

    GtkWidget *user_label = gtk_label_new("Username");
    gtk_label_set_xalign(GTK_LABEL(user_label), 0.0);
    GtkWidget *user_entry = gtk_entry_new();
    GtkWidget *pass_label = gtk_label_new("Password");
    gtk_label_set_xalign(GTK_LABEL(pass_label), 0.0);
    GtkWidget *pass_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(pass_entry), FALSE);
    gtk_entry_set_input_purpose(GTK_ENTRY(pass_entry), GTK_INPUT_PURPOSE_PASSWORD);

    gtk_grid_attach(GTK_GRID(grid), user_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), user_entry, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), pass_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), pass_entry, 1, 1, 1, 1);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_widget_set_halign(box, GTK_ALIGN_END);
    GtkWidget *cancel = gtk_button_new_with_label("Cancel");
    GtkWidget *login = gtk_button_new_with_label("Login");
    gtk_box_append(GTK_BOX(box), cancel);
    gtk_box_append(GTK_BOX(box), login);
    gtk_box_append(GTK_BOX(content), box);

    g_object_set_data(G_OBJECT(win), "user", user_entry);
    g_object_set_data(G_OBJECT(win), "pass", pass_entry);
    g_signal_connect(cancel, "clicked", G_CALLBACK(on_cancel_clicked), win);
    g_signal_connect(login, "clicked", G_CALLBACK(on_login_clicked), win);
    g_signal_connect(pass_entry, "activate", G_CALLBACK(on_login_clicked), win);
    gtk_window_set_default_widget(GTK_WINDOW(win), login);
    gtk_window_present(GTK_WINDOW(win));
}