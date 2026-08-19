#ifndef AUTH_H
#define AUTH_H

#include "constants.h"

typedef struct Session {
    int authenticated;
    int access_mask;
    char username[32];
} Session;

extern Session g_session;

int auth_authenticate(const char *username, const char *password);
int auth_check_department(int department);
int auth_is_authenticated(void);
void auth_logout(void);

#endif