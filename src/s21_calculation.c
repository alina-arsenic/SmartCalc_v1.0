#include "s21_smart_calc.h"
#include "s21_notation.h"
#include "s21_stack_double.h"

#define OK 0
#define CALC_ERROR 1

int calculation(char *src, double *result);
int number_convert(char *temp, double *number);
int oper_convert(char *temp, stack_d *stack, reg_templates template);

int number_convert(char *temp, double *number) {
    int i = 0, dot = 0;
    while (is_digit(temp[i])) i++;
    if (temp[i] == '.' && is_digit(temp[i+1])) {
        dot = i;
        i++;
        while (is_digit(temp[i])) i++;
    }
    if (dot == 0) dot = i+1;

    *number = 0;
    char buff[MAX_LEN];
    strncpy(buff, temp, i);
    sscanf(buff, "%lf", number);

    return i;  // возвращает сколько символов занимает число
}

int oper_convert(char *temp, stack_d *stack, reg_templates template) {
    int i = is_oper(temp, template);
    char buff[MAX_LEN];
    strncpy(buff, temp, i);
    buff[i] = '\0';

    double a, b, c;
    pop_d(stack, &a);
    pop_d(stack, &b);
    printf("HERE ARE OUR HEROES\n%lf %s %lf\n\n", a, buff, b);

    if (strcmp(buff, "*")) {
        c = a * b;
    } else if (strcmp(buff, "/")) {
        c = a / b;
    } else if (strcmp(buff, "^")) {
        c = pow(a, b);
    } else if (strcmp(buff, "mod")) {
        c = fmod(a, b);
    } else if (strcmp(buff, "+")) {
        c = a + b;
    } else if (strcmp(buff, "-")) {
        c = a - b;
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
    char **pt;
    stack_d stack;
    init_stack_d(&stack);
    int i = 0, unar_oper = 0, negative = 0;
    double number = 0;
    strncpy(temp, src, MAX_LEN);  // обработанные символы будут вырезаться из temp

    while (temp[0]) {
        // если в начале строки число
        if (is_digit(temp[0])) {
            i = number_convert(temp, &number);
            //number = strtod(temp, pt);
            if (negative) {
                negative = 0;
                number *= -1;
            }
            memmove(temp, temp+i, strlen(temp));
            push_d(&stack, number);

            printf("%s\n", temp);
            show_d(&stack);
            printf("\n");

        // если в начале строки оператор
        } else if (i = is_oper(temp, templates)) {
            if (oper_convert(temp, &stack, templates)) {
                return CALC_ERROR;
            }
            memmove(temp, temp+i, strlen(temp));

        // если в начале строки функция
        } else if ((i = is_func(temp, templates)) != 0) {
            // СЮДЫМ

        // иначе пропускаем символ
        } else {
            memmove(temp, temp+1, strlen(temp));
        }
    }

    peek_d(&stack, result);

    free_reg(&templates);
    return code;
}
