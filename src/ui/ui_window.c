#include "ui.h"

#include "auth.h"
#include "storage.h"

static GtkWidget *window = NULL;
static GtkWidget *session_label = NULL;
static GtkWidget *login_button = NULL;

static void on_login_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    (void)user_data;
    if (auth_is_authenticated()) {
        log_audit("LOGOUT", "");
        auth_logout();
        ui_update_session_bar();
        ui_refresh_all();
    } else {
        ui_show_login_dialog(GTK_WINDOW(window));
    }
}

void ui_update_session_bar(void) {
    if (!session_label) return;
    if (auth_is_authenticated()) {
        gtk_label_set_text(GTK_LABEL(session_label), g_session.username);
        gtk_button_set_label(GTK_BUTTON(login_button), "Logout");
    } else {
        gtk_label_set_text(GTK_LABEL(session_label), "Guest");
        gtk_button_set_label(GTK_BUTTON(login_button), "Login");
    }
}

void ui_refresh_all(void) {
    ui_dashboard_refresh();
    ui_students_refresh();
    ui_accounts_refresh();
    ui_update_session_bar();
}

GtkWidget *ui_window_new(GtkApplication *app) {
    storage_load();
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Student Bank");
    gtk_window_set_default_size(GTK_WINDOW(window), 900, 600);

    GtkWidget *header = gtk_header_bar_new();
    gtk_window_set_titlebar(GTK_WINDOW(window), header);

    login_button = gtk_button_new_with_label("Login");
    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_clicked), NULL);
    gtk_header_bar_pack_end(GTK_HEADER_BAR(header), login_button);

    session_label = gtk_label_new("Guest");
    gtk_header_bar_pack_end(GTK_HEADER_BAR(header), session_label);

    GtkWidget *stack = gtk_stack_new();
    gtk_stack_set_transition_type(GTK_STACK(stack),
                                  GTK_STACK_TRANSITION_TYPE_CROSSFADE);
    gtk_widget_set_vexpand(stack, TRUE);

    GtkWidget *switcher = gtk_stack_switcher_new();
    gtk_stack_switcher_set_stack(GTK_STACK_SWITCHER(switcher), GTK_STACK(stack));
    gtk_header_bar_pack_start(GTK_HEADER_BAR(header), switcher);

    gtk_stack_add_titled(GTK_STACK(stack), ui_dashboard_new(), "dashboard",
                         "Dashboard");
    gtk_stack_add_titled(GTK_STACK(stack), ui_students_new(), "students",
                         "Students");
    gtk_stack_add_titled(GTK_STACK(stack), ui_accounts_new(), "accounts",
                         "Accounts");

    gtk_window_set_child(GTK_WINDOW(window), stack);
    ui_refresh_all();
    return window;
}