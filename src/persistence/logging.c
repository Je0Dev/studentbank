#include "storage.h"

#include "constants.h"

#include <stdio.h>
#include <time.h>

void log_message(const char *level, const char *message) {
    FILE *log = fopen(LOG_FILE, "a");
    if (!log) return;
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char ts[25];
    strftime(ts, sizeof(ts), "%Y-%m-%d %H:%M:%S", t);
    fprintf(log, "[%s] [%-5s] %s\n", ts, level, message);
    fclose(log);
}

void log_error(const char *function, const char *message) {
    char msg[300];
    snprintf(msg, sizeof(msg), "[%s] %s", function, message);
    log_message("ERROR", msg);
}

void log_audit(const char *action, const char *details) {
    char msg[400];
    snprintf(msg, sizeof(msg), "[Action: %s] %s", action, details);
    log_message("AUDIT", msg);
}