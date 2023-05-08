#ifndef SRC_S21_NOTATION_
#define SRC_S21_NOTATION_

#include "s21_smart_calc.h"

typedef struct reg_templates {
    regex_t cos, sin, tan, acos, asin,
            atan, sqrt, ln, log, mod;
} reg_templates;

int is_digit(char c);

// если в начале строки оператор, возвращает количество символов в нем, если нет, возваращает 0
int is_oper(char *temp, reg_templates templates);

// если в начале строки функция, возвращает количество символов в ней, если нет, возваращает 0
int is_func(char *temp, reg_templates templates);

int compile_reg(reg_templates *templates);
void free_reg(reg_templates *templates);

#endif  //  SRC_S21_NOTATION_
