#include "auth.h"

#include <string.h>

Session g_session = {0};

static int grant_access(const char *username, int mask) {
    g_session.authenticated = 1;
    g_session.access_mask = mask;
    strncpy(g_session.username, username, sizeof(g_session.username) - 1);
    g_session.username[sizeof(g_session.username) - 1] = '\0';
    return 1;
}

int auth_authenticate(const char *username, const char *password) {
    if (!username || !password || !username[0] || !password[0]) return 0;
    if (strcmp(username, "admin") == 0 && strcmp(password, "securepass") == 0) {
        return grant_access(username, AUTH_ALL_ACCESS);
    }
    if (strcmp(username, "mech_user") == 0 && strcmp(password, "mechpass") == 0) {
        return grant_access(username, AUTH_BIT_MECH);
    }
    if (strcmp(username, "elec_user") == 0 && strcmp(password, "elecpass") == 0) {
        return grant_access(username, AUTH_BIT_ELEC);
    }
    if (strcmp(username, "civil_user") == 0 && strcmp(password, "civilpass") == 0) {
        return grant_access(username, AUTH_BIT_CIVIL);
    }
    return 0;
}

static int department_bit(int department) {
    switch (department) {
        case DEPT_MECH_ENG: return AUTH_BIT_MECH;
        case DEPT_ELEC_ENG: return AUTH_BIT_ELEC;
        case DEPT_CIVIL_ENG: return AUTH_BIT_CIVIL;
        default: return 0;
    }
}

int auth_check_department(int department) {
    if (!g_session.authenticated) return 0;
    if (g_session.access_mask == AUTH_ALL_ACCESS) return 1;
    return (g_session.access_mask & department_bit(department)) != 0;
}

int auth_is_authenticated(void) {
    return g_session.authenticated;
}

void auth_logout(void) {
    g_session.authenticated = 0;
    g_session.access_mask = 0;
    g_session.username[0] = '\0';
}