#include "s21_smart_calc.h"
#include "s21_notation.h"
#include "s21_stack_double.h"

#define OK 0
#define CALC_ERROR 1

int calculation(char *src, double *result);

int number_convert(char *temp) {
    int i = 0;
    while (is_digit(temp[i])) i++;
    if (temp[i] == '.' && is_digit(temp[i+1])) {
        i++;
        while (is_digit(temp[i])) i++;
    }
    return i;  // возвращает сколько символов занимает число
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
            i = number_convert(temp);
            number = strtod(temp, pt);
            if (negative) {
                negative = 0;
                number *= -1;
            }
            //memmove(temp, temp+i, strlen(temp));
            push_d(&stack, number);

        // если в начале строки оператор
        } else if (is_oper(temp, templates)) {
            // ТУДЫМ

        // если в начале строки функция
        } else if ((i = is_func(temp, templates)) != 0) {
            // СЮДЫМ

        // иначе пропускаем символ
        } else {
            memmove(temp, temp+1, strlen(temp));
        }
    }

    free_reg(&templates);
    return code;
}