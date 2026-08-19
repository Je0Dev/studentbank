#include "validation.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

int validate_am(const char *am) {
    if (!am || strlen(am) != AM_LEN) return 0;
    for (int i = 0; i < AM_LEN; i++) {
        if (!isdigit((unsigned char)am[i])) return 0;
    }
    if (am[0] != '1') return 0;
    int dept = am[1] - '0';
    if (dept < DEPT_MECH_ENG || dept > DEPT_CIVIL_ENG) return 0;
    int number = atoi(am + 2);
    if (number < 1 || number > 999) return 0;
    return 1;
}

int validate_name(const char *name) {
    if (!name || !name[0]) return 0;
    if (strlen(name) >= MAX_NAME) return 0;
    for (int i = 0; name[i]; i++) {
        char c = name[i];
        if (!(isalpha((unsigned char)c) || c == ' ' || c == '-' || c == '\'')) {
            return 0;
        }
    }
    return 1;
}

int validate_pin(const char *pin) {
    if (!pin || strlen(pin) != PIN_DIGITS) return 0;
    for (int i = 0; i < PIN_DIGITS; i++) {
        if (!isdigit((unsigned char)pin[i])) return 0;
    }
    return 1;
}

void format_name(char *name) {
    if (!name || !name[0]) return;
    for (int i = 0; name[i]; i++) {
        name[i] = (char)tolower((unsigned char)name[i]);
    }
    int capitalize = 1;
    for (int i = 0; name[i]; i++) {
        if (capitalize && isalpha((unsigned char)name[i])) {
            name[i] = (char)toupper((unsigned char)name[i]);
            capitalize = 0;
        } else if (name[i] == ' ' || name[i] == '-' || name[i] == '\'') {
            capitalize = 1;
        }
    }
}

void sanitize_input(char *input) {
    if (!input) return;
    char *out = input;
    for (char *in = input; *in; in++) {
        char c = *in;
        if (isalnum((unsigned char)c) || c == ' ' || c == '_' || c == '-' ||
            c == '.' || c == '\'') {
            *out++ = c;
        } else if (c == '/' || c == '\\') {
            *out++ = '_';
        } else if (c == ';' || c == '&' || c == '|') {
            *out++ = ' ';
        }
    }
    *out = '\0';
}