#include "test_util.h"

#include "auth.h"

#include <string.h>

static void reset(void) {
    auth_logout();
}

static void test_valid_credentials(void) {
    reset();
    CHECK(auth_authenticate("admin", "securepass") == 1);
    CHECK(auth_is_authenticated());
    CHECK(strcmp(g_session.username, "admin") == 0);
    CHECK(g_session.access_mask == AUTH_ALL_ACCESS);

    reset();
    CHECK(auth_authenticate("mech_user", "mechpass") == 1);
    CHECK(auth_authenticate("elec_user", "elecpass") == 1);
    CHECK(auth_authenticate("civil_user", "civilpass") == 1);
}

static void test_invalid_credentials(void) {
    reset();
    CHECK(auth_authenticate("admin", "wrong") == 0);
    CHECK(auth_authenticate("hacker", "securepass") == 0);
    CHECK(auth_authenticate("", "") == 0);
    CHECK(auth_authenticate(NULL, NULL) == 0);
    CHECK(!auth_is_authenticated());
}

static void test_admin_access(void) {
    reset();
    auth_authenticate("admin", "securepass");
    CHECK(auth_check_department(DEPT_MECH_ENG));
    CHECK(auth_check_department(DEPT_ELEC_ENG));
    CHECK(auth_check_department(DEPT_CIVIL_ENG));
}

static void test_dept_access_matrix(void) {
    reset();
    auth_authenticate("mech_user", "mechpass");
    CHECK(auth_check_department(DEPT_MECH_ENG));
    CHECK(!auth_check_department(DEPT_ELEC_ENG));
    CHECK(!auth_check_department(DEPT_CIVIL_ENG));

    reset();
    auth_authenticate("elec_user", "elecpass");
    CHECK(!auth_check_department(DEPT_MECH_ENG));
    CHECK(auth_check_department(DEPT_ELEC_ENG));

    reset();
    auth_authenticate("civil_user", "civilpass");
    CHECK(auth_check_department(DEPT_CIVIL_ENG));
    CHECK(!auth_check_department(DEPT_MECH_ENG));
}

static void test_unauthenticated_denied(void) {
    reset();
    CHECK(!auth_check_department(DEPT_MECH_ENG));
}

static void test_logout(void) {
    reset();
    auth_authenticate("admin", "securepass");
    auth_logout();
    CHECK(!auth_is_authenticated());
    CHECK(!auth_check_department(DEPT_MECH_ENG));
    CHECK(strlen(g_session.username) == 0);
}

void test_auth(void) {
    test_valid_credentials();
    test_invalid_credentials();
    test_admin_access();
    test_dept_access_matrix();
    test_unauthenticated_denied();
    test_logout();
}