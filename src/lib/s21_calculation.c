#include "s21_smart_calc.h"
#include "s21_notation.h"
#include "s21_stack_double.h"

#define OK 0
#define CALC_ERROR 1
#define BAD_DATA 2

int calculation(char *src, double *result);
int number_convert(stack_d *stack, char *temp,int *negative);
int oper_convert(char *temp, stack_d *stack, reg_templates template);
int func_convert(char *temp, stack_d *stack, reg_templates template, int *negative);

int number_convert(stack_d *stack, char *temp, int *negative) {
    int i = 0, dot = 0;
    while (is_digit(temp[i])) i++;
    if (temp[i] == '.' && is_digit(temp[i+1])) {
        dot = i;
        i++;
        while (is_digit(temp[i])) i++;
    }
    if (dot == 0) dot = i+1;

    double number = 0;
    char buff[MAX_LEN];
    strncpy(buff, temp, i);
    sscanf(buff, "%lf", &number);
    
    if (*negative) {
        number *= -1;
        *negative = 0;
    }

    push_d(stack, number);
    return i;  // возвращает сколько символов занимает число
}

int oper_convert(char *temp, stack_d *stack, reg_templates template) {
    int i = is_oper(temp, template);
    int unar = temp[i] == 'u';
    char buff[MAX_LEN] = {0};
    strncpy(buff, temp, i+unar);

    double a, b, c;
    pop_d(stack, &b);

    if (!strcmp(buff, "+u")) {
        c = b + 1;
    } else if (!strcmp(buff, "-u")) {
        c = b - 1;
    } else {
        pop_d(stack, &a);
        if (!strcmp(buff, "*")) {
            c = a * b;
        } else if (!strcmp(buff, "/")) {
            c = a / b;
        } else if (!strcmp(buff, "^")) {
            c = pow(a, b);
        } else if (!strcmp(buff, "mod")) {
            c = fmod(a, b);
        } else if (!strcmp(buff, "+")) {
            c = a + b;
        } else if (!strcmp(buff, "-")) {
            c = a - b;
        } else {
            return BAD_DATA;
        }
    }
    if (!isnormal(c)) {
        return CALC_ERROR;
    }

    push_d(stack, c);
    return OK;
}

int func_convert(char *temp, stack_d *stack, reg_templates template, int *negative) {
    int i = is_func(temp, template);
    char buff[MAX_LEN] = {0};
    strncpy(buff, temp, i);

    double a, c;
    pop_d(stack, &a);

    if (!strcmp(buff, "cos")) {
        c = cos(a);
    } else if (!strcmp(buff, "sin")) {
        c = sin(a);
    } else if (!strcmp(buff, "tan")) {
        c = tan(a);
    } else if (!strcmp(buff, "acos")) {
        c = acos(a);
    } else if (!strcmp(buff, "asin")) {
        c = asin(a);
    } else if (!strcmp(buff, "atan")) {
        c = atan(a);
    } else if (!strcmp(buff, "sqrt")) {
        c = sqrt(a);
    } else if (!strcmp(buff, "ln")) {
        c = log(a);
    } else if (!strcmp(buff, "log")) {
        c = log10(a);
    } else {
        return BAD_DATA;
    }

    if (*negative) {
        c *= -1;
        *negative = 0;
    }

    push_d(stack, c);
    return OK;
}

// подсчет результата строки в обратной польской нотации БЕЗ неизвестных переменных, результат однозначен
int calculation(char *src, double *result) {

    reg_templates templates;
    compile_reg(&templates);

    int code = 0;
    char temp[MAX_LEN];
    stack_d stack;
    init_stack_d(&stack);
    int i = 0, negative = 0;
    strncpy(temp, src, MAX_LEN);  // обработанные символы будут вырезаться из temp

    while (temp[0]) {

        // если в начале знак минус перед числом или функцией, убираем его и ставим флаг
        if (temp[0] == '-' && (is_digit(temp[1]) || is_func(temp+1, templates))) {
            negative = 1;
            memmove(temp, temp+1, strlen(temp));
        }

        // если в начале строки число
        if (is_digit(temp[0])) {
            i = number_convert(&stack, temp, &negative);
            memmove(temp, temp+i, strlen(temp));

        // если в начале строки функция
        } else if ((i = is_func(temp, templates)) != 0) {
            if (func_convert(temp, &stack, templates, &negative)) {
                return CALC_ERROR;
            }
            memmove(temp, temp+i, strlen(temp));

        // если в начале строки оператор
        } else if ((i = is_oper(temp, templates)) != 0) {
            if (oper_convert(temp, &stack, templates)) {
                return CALC_ERROR;
            }
            memmove(temp, temp+i+(temp[i] == 'u'), strlen(temp));

        // иначе пропускаем символ
        } else {
            memmove(temp, temp+1, strlen(temp));
        }
    }
    peek_d(&stack, result);

    free_reg(&templates);
    return code;
}
