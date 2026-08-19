#include "ui_dialogs.h"

typedef struct {
    int result;
    GMainLoop *loop;
} AlertState;

static void alert_finish(GtkWidget *win, AlertState *state, int result) {
    state->result = result;
    g_main_loop_quit(state->loop);
    gtk_window_destroy(GTK_WINDOW(win));
}

static void on_alert_result(GtkButton *button, gpointer data) {
    GtkWidget *win = g_object_get_data(G_OBJECT(button), "win");
    AlertState *state = g_object_get_data(G_OBJECT(win), "state");
    alert_finish(win, state, GPOINTER_TO_INT(data));
}

static gboolean on_close_request(GtkWidget *win, gpointer data) {
    alert_finish(win, data, 0);
    return TRUE;
}

static GtkWidget *alert_shell(GtkWindow *parent, const char *title,
                              AlertState *state) {
    GtkWidget *win = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(win), title);
    gtk_window_set_modal(GTK_WINDOW(win), TRUE);
    gtk_window_set_transient_for(GTK_WINDOW(win), parent);
    gtk_window_set_resizable(GTK_WINDOW(win), FALSE);
    g_object_set_data(G_OBJECT(win), "state", state);
    g_signal_connect(win, "close-request", G_CALLBACK(on_close_request), state);
    GtkWidget *content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_set_margin_top(content, 12);
    gtk_widget_set_margin_bottom(content, 12);
    gtk_widget_set_margin_start(content, 12);
    gtk_widget_set_margin_end(content, 12);
    gtk_window_set_child(GTK_WINDOW(win), content);
    return win;
}

static void alert_add_buttons(GtkWidget *win, const char *cancel_label,
                              const char *ok_label, int ok_result) {
    GtkWidget *content = gtk_window_get_child(GTK_WINDOW(win));
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_widget_set_halign(box, GTK_ALIGN_END);
    GtkWidget *cancel = gtk_button_new_with_label(cancel_label);
    GtkWidget *ok = gtk_button_new_with_label(ok_label);
    g_object_set_data(G_OBJECT(cancel), "win", win);
    g_object_set_data(G_OBJECT(ok), "win", win);
    g_signal_connect(cancel, "clicked", G_CALLBACK(on_alert_result), GINT_TO_POINTER(0));
    g_signal_connect(ok, "clicked", G_CALLBACK(on_alert_result), GINT_TO_POINTER(ok_result));
    gtk_box_append(GTK_BOX(box), cancel);
    gtk_box_append(GTK_BOX(box), ok);
    gtk_box_append(GTK_BOX(content), box);
}

static int alert_run(GtkWindow *win, AlertState *state) {
    gtk_window_present(win);
    g_main_loop_run(state->loop);
    int result = state->result;
    g_main_loop_unref(state->loop);
    g_free(state);
    return result;
}

int ui_confirm(GtkWindow *parent, const char *title, const char *message) {
    AlertState *state = g_new0(AlertState, 1);
    state->loop = g_main_loop_new(NULL, FALSE);
    GtkWidget *win = alert_shell(parent, title, state);
    GtkWidget *content = gtk_window_get_child(GTK_WINDOW(win));
    GtkWidget *lbl = gtk_label_new(message);
    gtk_label_set_xalign(GTK_LABEL(lbl), 0.0);
    gtk_label_set_wrap(GTK_LABEL(lbl), TRUE);
    gtk_box_append(GTK_BOX(content), lbl);
    alert_add_buttons(win, "No", "Yes", 1);
    return alert_run(GTK_WINDOW(win), state);
}

void ui_error(GtkWindow *parent, const char *message) {
    GtkAlertDialog *alert = gtk_alert_dialog_new("%s", message);
    gtk_alert_dialog_show(alert, parent);
}