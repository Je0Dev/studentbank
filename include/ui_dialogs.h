#ifndef UI_DIALOGS_H
#define UI_DIALOGS_H

#include <gtk/gtk.h>

typedef struct {
    const char *label;
    char value[256];
    GtkWidget *entry;
} FieldSpec;

int ui_prompt_fields(GtkWindow *parent, const char *title, FieldSpec *fields, int n);
char *ui_prompt_text(GtkWindow *parent, const char *title, const char *label,
                     const char *initial);
int ui_confirm(GtkWindow *parent, const char *title, const char *message);
void ui_error(GtkWindow *parent, const char *message);

#endif