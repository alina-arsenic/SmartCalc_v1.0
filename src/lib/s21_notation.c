#include "s21_smart_calc.h"
#include "s21_notation.h"
#include "s21_stack_double.h"

int is_digit(char c) {
    return (c >= '0' && c <= '9');
}

int is_oper(char *temp, reg_templates templates) {
    if (temp[0] == '+' || temp[0] == '-' || temp[0] == '*' || temp[0] == '/' || temp[0] == '^') {
        return 1;
    } else if (!regexec(&templates.mod, temp, 0, NULL, 0)) {
        return 3;
    }
    return 0;
}

int is_func(char *temp, reg_templates templates) {
    if (!regexec(&templates.ln, temp, 0, NULL, 0)) {
        return 2;
    } else if (!regexec(&templates.cos, temp, 0, NULL, 0) ||
        !regexec(&templates.sin, temp, 0, NULL, 0) ||
        !regexec(&templates.tan, temp, 0, NULL, 0) ||
        !regexec(&templates.log, temp, 0, NULL, 0)) {
        return 3;
    } else if (!regexec(&templates.acos, temp, 0, NULL, 0) ||
        !regexec(&templates.asin, temp, 0, NULL, 0) ||
        !regexec(&templates.atan, temp, 0, NULL, 0) ||
        !regexec(&templates.sqrt, temp, 0, NULL, 0)) {
        return 4;
    }
    return 0;
}

int compile_reg(reg_templates *templates) {
    if (regcomp(&templates->cos, "^cos", 0) ||
        regcomp(&templates->sin, "^sin", 0) ||
        regcomp(&templates->tan, "^tan", 0) ||
        regcomp(&templates->log, "^log", 0) ||
        regcomp(&templates->acos, "^acos", 0) ||
        regcomp(&templates->asin, "^asin", 0) ||
        regcomp(&templates->atan, "^atan", 0) ||
        regcomp(&templates->sqrt, "^sqrt", 0) ||
        regcomp(&templates->ln, "^ln", 0) ||
        regcomp(&templates->mod, "^mod", 0)
        ) return 1;
    else return 0;
}

void free_reg(reg_templates *templates) {
    regfree(&templates->cos);
    regfree(&templates->sin);
    regfree(&templates->tan);
    regfree(&templates->acos);
    regfree(&templates->asin);
    regfree(&templates->atan);
    regfree(&templates->sqrt);
    regfree(&templates->ln);
    regfree(&templates->log);
    regfree(&templates->mod);
}
