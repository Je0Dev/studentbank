#include "test_util.h"

#include <string.h>

#include "validation.h"

static void test_am_valid(void) {
    CHECK(validate_am("11001"));
    CHECK(validate_am("12045"));
    CHECK(validate_am("13999"));
    CHECK(validate_am("13001"));
}

static void test_am_invalid(void) {
    CHECK(!validate_am("21001"));
    CHECK(!validate_am("10001"));
    CHECK(!validate_am("14001"));
    CHECK(!validate_am("1100"));
    CHECK(!validate_am("11000"));
    CHECK(!validate_am("11a01"));
    CHECK(!validate_am(NULL));
    CHECK(!validate_am(""));
}

static void test_pin(void) {
    CHECK(validate_pin("1234"));
    CHECK(!validate_pin("123"));
    CHECK(!validate_pin("12345"));
    CHECK(!validate_pin("12a4"));
    CHECK(!validate_pin(NULL));
}

static void test_name(void) {
    CHECK(validate_name("John"));
    CHECK(validate_name("Mary-Anne O'Neil"));
    CHECK(!validate_name(""));
    CHECK(!validate_name("John123"));
    CHECK(!validate_name("x;rm -rf /"));
    CHECK(!validate_name(NULL));
}

static void test_format_name(void) {
    char n1[32] = "jOHN dOE";
    format_name(n1);
    CHECK(strcmp(n1, "John Doe") == 0);

    char n2[32] = "mary-anne o'neil";
    format_name(n2);
    CHECK(strcmp(n2, "Mary-Anne O'Neil") == 0);
}

static void test_sanitize(void) {
    char s1[64] = "../../etc/passwd";
    sanitize_input(s1);
    CHECK(strstr(s1, "/") == NULL);

    char s2[64] = "ls; rm -rf & | x";
    sanitize_input(s2);
    CHECK(strstr(s2, ";") == NULL);
    CHECK(strstr(s2, "&") == NULL);
    CHECK(strstr(s2, "|") == NULL);
}

void test_validation(void) {
    test_am_valid();
    test_am_invalid();
    test_pin();
    test_name();
    test_format_name();
    test_sanitize();
}