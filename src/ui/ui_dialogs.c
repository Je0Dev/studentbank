#include "ui_dialogs.h"

#include <stdio.h>

typedef struct {
    int result;
    int nfields;
    FieldSpec *fields;
    GMainLoop *loop;
} DialogState;

static void dialog_finish(GtkWidget *win, DialogState *state, int result) {
    state->result = result;
    g_main_loop_quit(state->loop);
    gtk_window_destroy(GTK_WINDOW(win));
}

static void on_set_result(GtkButton *button, gpointer data) {
    GtkWidget *win = g_object_get_data(G_OBJECT(button), "win");
    DialogState *state = g_object_get_data(G_OBJECT(win), "state");
    dialog_finish(win, state, GPOINTER_TO_INT(data));
}

static gboolean on_close_request(GtkWidget *win, gpointer data) {
    dialog_finish(win, data, 0);
    return TRUE;
}

static GtkWidget *dialog_shell(GtkWindow *parent, const char *title,
                               DialogState *state) {
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

static GtkWidget *dialog_content(GtkWidget *win) {
    return gtk_window_get_child(GTK_WINDOW(win));
}

static void dialog_add_buttons(GtkWidget *win, const char *cancel_label,
                               const char *ok_label, GCallback ok_cb,
                               gpointer ok_data) {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_widget_set_halign(box, GTK_ALIGN_END);
    GtkWidget *cancel = gtk_button_new_with_label(cancel_label);
    GtkWidget *ok = gtk_button_new_with_label(ok_label);
    g_object_set_data(G_OBJECT(cancel), "win", win);
    g_signal_connect(cancel, "clicked", G_CALLBACK(on_set_result), GINT_TO_POINTER(0));
    g_signal_connect(ok, "clicked", ok_cb, ok_data);
    gtk_box_append(GTK_BOX(box), cancel);
    gtk_box_append(GTK_BOX(box), ok);
    gtk_box_append(GTK_BOX(dialog_content(win)), box);
}

static void on_prompt_ok(GtkButton *button, gpointer data) {
    (void)button;
    GtkWidget *win = data;
    DialogState *state = g_object_get_data(G_OBJECT(win), "state");
    for (int i = 0; i < state->nfields; i++) {
        const char *text = gtk_editable_get_text(GTK_EDITABLE(state->fields[i].entry));
        snprintf(state->fields[i].value, sizeof(state->fields[i].value), "%s", text);
    }
    dialog_finish(win, state, 1);
}

static int dialog_run(GtkWindow *win, DialogState *state) {
    gtk_window_present(win);
    g_main_loop_run(state->loop);
    int result = state->result;
    g_main_loop_unref(state->loop);
    g_free(state);
    return result;
}

int ui_prompt_fields(GtkWindow *parent, const char *title, FieldSpec *fields,
                     int n) {
    DialogState *state = g_new0(DialogState, 1);
    state->nfields = n;
    state->fields = fields;
    state->loop = g_main_loop_new(NULL, FALSE);
    GtkWidget *win = dialog_shell(parent, title, state);
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 6);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 8);
    gtk_box_append(GTK_BOX(dialog_content(win)), grid);
    for (int i = 0; i < n; i++) {
        GtkWidget *lbl = gtk_label_new(fields[i].label);
        gtk_label_set_xalign(GTK_LABEL(lbl), 0.0);
        fields[i].entry = gtk_entry_new();
        if (fields[i].value[0]) {
            gtk_editable_set_text(GTK_EDITABLE(fields[i].entry), fields[i].value);
        }
        gtk_grid_attach(GTK_GRID(grid), lbl, 0, i, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), fields[i].entry, 1, i, 1, 1);
    }
    dialog_add_buttons(win, "Cancel", "OK", G_CALLBACK(on_prompt_ok), win);
    return dialog_run(GTK_WINDOW(win), state);
}

char *ui_prompt_text(GtkWindow *parent, const char *title, const char *label,
                     const char *initial) {
    FieldSpec field = {0};
    field.label = label;
    if (initial) {
        snprintf(field.value, sizeof(field.value), "%s", initial);
    }
    if (!ui_prompt_fields(parent, title, &field, 1)) return NULL;
    return g_strdup(field.value);
}