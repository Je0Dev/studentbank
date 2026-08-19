#ifndef VALIDATION_H
#define VALIDATION_H

#include "constants.h"

int validate_am(const char *am);
int validate_name(const char *name);
int validate_pin(const char *pin);
void format_name(char *name);
void sanitize_input(char *input);

#endif