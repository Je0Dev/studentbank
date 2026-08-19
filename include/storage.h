#ifndef STORAGE_H
#define STORAGE_H

void storage_save(void);
void storage_load(void);
void backup_create(void);
void backup_rotate(void);

void log_message(const char *level, const char *message);
void log_error(const char *function, const char *message);
void log_audit(const char *action, const char *details);

#endif