#include "storage.h"

#include "constants.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

static int file_exists(const char *path) {
    struct stat st;
    return stat(path, &st) == 0;
}

void backup_create(void) {
    mkdir(BACKUP_DIR, 0755);
    if (!file_exists(DATA_FILE)) return;
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char path[300];
    snprintf(path, sizeof(path), "%sstudents_backup_%04d%02d%02d_%02d%02d%02d.dat",
             BACKUP_DIR, t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
             t->tm_hour, t->tm_min, t->tm_sec);
    FILE *src = fopen(DATA_FILE, "rb");
    FILE *dst = fopen(path, "wb");
    if (!src || !dst) {
        if (src) fclose(src);
        if (dst) fclose(dst);
        return;
    }
    char buf[4096];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), src)) > 0) {
        fwrite(buf, 1, n, dst);
    }
    fclose(src);
    fclose(dst);
}

void backup_rotate(void) {
    DIR *dir = opendir(BACKUP_DIR);
    if (!dir) return;
    time_t times[MAX_BACKUPS + 5];
    char *names[MAX_BACKUPS + 5];
    int count = 0;
    struct dirent *e;
    while ((e = readdir(dir)) && count < MAX_BACKUPS + 5) {
        if (strstr(e->d_name, "students_backup") == NULL) continue;
        struct stat st;
        char full[512];
        snprintf(full, sizeof(full), "%s%s", BACKUP_DIR, e->d_name);
        if (stat(full, &st) != 0) continue;
        times[count] = st.st_mtime;
        names[count] = malloc(strlen(full) + 1);
        if (names[count]) strcpy(names[count], full);
        count++;
    }
    closedir(dir);
    if (count <= MAX_BACKUPS) return;
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (times[i] > times[j]) {
                time_t tt = times[i];
                times[i] = times[j];
                times[j] = tt;
                char *tn = names[i];
                names[i] = names[j];
                names[j] = tn;
            }
        }
    }
    for (int i = 0; i < count - MAX_BACKUPS; i++) {
        remove(names[i]);
        free(names[i]);
    }
    for (int i = 0; i < count; i++) {
        free(names[i]);
    }
}