#include "s21_smart_calc.h"
#include "s21_stack.h"

#define OK 0
#define BAD_INPUT 1
#define CALC_ERROR 2

typedef struct reg_templates {
    regex_t cos, sin, tan, acos, asin,
            atan, sqrt, ln, log, mod;
} reg_templates;

int is_digit(char c) {
    return (c >= '0' && c <= '9');
}

// если в начале строки оператор, возвращает количество символов в нем, если нет, возваращает 0
int is_oper(char *temp, reg_templates template) {
    if (temp[0] == '+' || temp[0] == '-' || temp[0] == '*' || temp[0] == '/' || temp[0] == '^') {
        return 1;
    } else if (!regexec(&template.mod, temp, 0, NULL, 0)) {
        return 3;
    }
    return 0;
}

// если в начале строки функция, возвращает количество символов в ней, если нет, возваращает 0
int is_func(char *temp, reg_templates template) {
    if (!regexec(&template.ln, temp, 0, NULL, 0)) {
        return 2;
    } else if (!regexec(&template.cos, temp, 0, NULL, 0) ||
        !regexec(&template.sin, temp, 0, NULL, 0) ||
        !regexec(&template.tan, temp, 0, NULL, 0) ||
        !regexec(&template.log, temp, 0, NULL, 0)) {
        return 3;
    } else if (!regexec(&template.acos, temp, 0, NULL, 0) ||
        !regexec(&template.asin, temp, 0, NULL, 0) ||
        !regexec(&template.atan, temp, 0, NULL, 0) ||
        !regexec(&template.sqrt, temp, 0, NULL, 0)) {
        return 4;
    }
    return 0;
}

int number_process(char *temp, char *dst) {
    int i = 0; // сколько символов занимает число
    while (is_digit(temp[i])) i++;
    if (temp[i] == '.' && is_digit(temp[i+1])) {
        i++;
        while (is_digit(temp[i])) i++;
    }
    strncat(dst, temp, i);
    strcat(dst, " ");
    memmove(temp, temp+i, strlen(temp));
    return OK;
}

// начало жести

/*
1 - степень ^
2 - унарные + -
3 - умножение * деление / остаток mod
4 - сложение + вычитание -
0 - ОШИБКА
*/
int oper_priority(char *str, int digit, reg_templates template) {
    if (digit && (str[0] == '^')) return 1;
    if (!digit && (str[0] == '+' || str[0] == '-')) return 2;
    if (digit && str[1] == 'u') return 2;  // если смотрим стек и находим флаг унарной операции
    if (digit && (str[0] == '*' || str[0] == '/' || !regexec(&template.mod, str, 0, NULL, 0))) return 3;
    return 0;
}

int oper_process(char *temp, char *dst, stack *stack, int unar_oper) {
    priority = oper_priority(temp, digit_before, template);
            if (!priority) {
                return BAD_INPUT;  // бинарный оператор с одним числом
            }
            if (!peek(&stack, buf)) {  // стак не пустой
                stack_priority = oper_priority(buf, 1, template);

                // вершина стака является операцией && ее приоритет выше или равен
                while (stack_priority && (stack_priority <= priority)) {

                    pop(&stack, buf);
                    if (!regexec(&template.mod, buf, 0, NULL, 0)) strncat(dst, buf, 3);
                    else strncat(dst, buf, 1);
                    strcat(dst, " ");

                    if (peek(&stack, buf)) break;  // стак стал пустым
                    stack_priority = oper_priority(buf, 1, template);
                }
            }

            if (!regexec(&template.mod, temp, 0, NULL, 0)) i = 3;
            else i = 1;

            strncpy(buf, temp, i);

            if (priority == 2) {
                strcat(buf, "u");  // флаг унарной операции
            } else if (digit_before != 1) {
                return BAD_INPUT;  // нет числа перед бинарной операцией
            }

            push(&stack, buf);
            memmove(temp, temp+i, strlen(temp));
    return OK;
}

int bracket_process(char *temp, char *dst, stack *stack) {
    bracket = 0;
            while (!peek(&stack, buf)) {  // пока стек не пустой
                pop(&stack, buf);
                if (buf[0] == '(') {
                    bracket = 1;
                    if (!peek(&stack, buf) && 
                        (!regexec(&template.cos, buf, 0, NULL, 0) ||
                        !regexec(&template.sin, buf, 0, NULL, 0) ||
                        !regexec(&template.tan, buf, 0, NULL, 0) ||
                        !regexec(&template.log, buf, 0, NULL, 0) ||
                        !regexec(&template.acos, buf, 0, NULL, 0) ||
                        !regexec(&template.asin, buf, 0, NULL, 0) ||
                        !regexec(&template.atan, buf, 0, NULL, 0) ||
                        !regexec(&template.sqrt, buf, 0, NULL, 0) ||
                        !regexec(&template.ln, buf, 0, NULL, 0))) {
                            pop(&stack, buf);
                            strncat(dst, buf, 3);
                            strcat(dst, " ");
                        }
                    break;
                } else if (buf[0] == '+' || buf[0] == '-' ||
                            buf[0] == '*' || buf[0] == '/' ||
                            buf[0] == '^') {
                    strncat(dst, buf, 1);
                    strcat(dst, " ");
                } else if (!regexec(&template.mod, buf, 0, NULL, 0)) {
                    strncat(dst, buf, 3);
                    strcat(dst, " ");
                } else {
                    break;
                }
            }
            if (!bracket) {
                return BAD_INPUT;  // стак закончился, открывающей скобки не найдено
            }
            memmove(temp, temp+1, strlen(temp));
    return OK;
}

// конец жести

int sort_station(char *src, char *dst, reg_templates template) {

    char buf[MAX_LEN];
    char temp[MAX_LEN];
    strncpy(temp, src, MAX_LEN);  // обработанные символы будут вырезаться из temp
    
    stack stack;
    init_stack(&stack);

    int code, need_number = 1, unar_oper = 0;

    while (temp[0]) {
        // если в начале строки пробел, пропускаем его
        if (temp[0] == ' ') {
            memmove(temp, temp+1, strlen(temp));

        // если в начале строки число
        } else if (is_digit(temp[0])) {
            if (!need_number) // ожидался оператор, а не число
                return BAD_INPUT;
            number_process(temp, dst);
            need_number = 0;

        // если в начале строки оператор
        } else if (is_oper(temp, template)) {
            if (need_number) {
                if (temp[0] == '+' || temp[0] == '-') {
                    unar_oper = 1;  // если ожидалось число, а встретился + или -, то оператор унарный
                } else {
                    return BAD_INPUT;  // если ожидалось число, а оператор не + или -, ошибка
                }
            }
            oper_process(temp, dst, &stack, unar_oper);
            need_number = 1;

        // если в начале строки закрывающая скобка
        } else if (temp[0] == ')') {
            if (need_number) {  // еще не было встречнено ожидаемое число
                return BAD_INPUT;
            }
            if (code = bracket_process(temp, dst, &stack)) {
                return code;  // в расположении скобок ошибка
            }
            need_number = 0;

        // если в начале строки функция
        } else if (code = is_func(temp, template)) {
            if (!need_number) // ожидался оператор, а не функция
                return BAD_INPUT;
            strncpy(buf, temp, code);
            push(&stack, buf);
            need_number = 1;

        // если открывающая скобка
        } else if (temp[0] == '(') {
            if (!need_number) // ожидался оператор, а не выражение
                return BAD_INPUT;
            strncpy(buf, temp, 1);
            push(&stack, buf);
            need_number = 1;

        } else {
            return BAD_INPUT;  // подсунули неправильный символ
        }
    }
    if (need_number) {
        return BAD_INPUT;
    }
    return OK;
}

int notation_convert(char *src, char *dst) {

    reg_templates template;
    if (regcomp(&template.cos, "^cos", 0) ||
        regcomp(&template.sin, "^sin", 0) ||
        regcomp(&template.tan, "^tan", 0) ||
        regcomp(&template.log, "^log", 0) ||
        regcomp(&template.acos, "^acos", 0) ||
        regcomp(&template.asin, "^asin", 0) ||
        regcomp(&template.atan, "^atan", 0) ||
        regcomp(&template.sqrt, "^sqrt", 0) ||
        regcomp(&template.ln, "^ln", 0) ||
        regcomp(&template.mod, "^mod", 0)
        ) return CALC_ERROR;

    int code = sort_station(src, dst, template);

    regfree(&template.cos);
    regfree(&template.sin);
    regfree(&template.tan);
    regfree(&template.acos);
    regfree(&template.asin);
    regfree(&template.atan);
    regfree(&template.sqrt);
    regfree(&template.ln);
    regfree(&template.log);
    regfree(&template.mod);

    return code;
}
