#include "s21_smart_calc.h"
#include "s21_notation.h"
#include "s21_stack_string.h"

#define OK 0
#define BAD_INPUT 1
#define CALC_ERROR 2

int sort_station(char *src, char *dst, reg_templates template);

int notation_convert(char *src, char *dst) {
    
    if (strlen(src) > MAX_USER_LEN) {
        return BAD_INPUT;
    }

    reg_templates templates;
    if (compile_reg(&templates)) {
        return CALC_ERROR;
    }

    memset(dst, 0, MAX_LEN);
    int code = sort_station(src, dst, templates);
    free_reg(&templates);
    return code;
}

int number_process(char *temp, char *dst) {
    int i = 0;
    if (temp[0] == 'x') {
        i = 1;
    } else {
        while (is_digit(temp[i])) i++;
        if (temp[i] == '.' && is_digit(temp[i+1])) {
            i++;
            while (is_digit(temp[i])) i++;
        }
    }
    strncat(dst, temp, i);
    strcat(dst, " ");
    return i;  // возвращает сколько символов занимает число
}

int is_unar(char *temp) {
    int i = 0, flag = 0;
    while (temp[i] == '-' || temp[i] == '+') {flag = 1; i++;}
    if (!flag) return 0;
    flag = 0;
    while (temp[i] == '(') {flag = 1; i++;}
    if (!flag) return 0;
    flag = 0;
    while (is_digit(temp[i])) {flag = 1; i++;}
    if (!flag) return 0;
    flag = 0;
    if (temp[i] == '.' && is_digit(temp[i+1])) {
        i++;
        while (is_digit(temp[i])) i++;
    }
    while (temp[i] == ')') {flag = 1; i++;}
    return flag;
}

int unar_process(char *temp, char *dst) {
    char buf[MAX_LEN] = {0};
    int i = 0, j = 0, num = 0, count = 0;
    while (temp[i] != '(') i++;
    while (temp[i] == '(') {count++; i++;}
    if (temp[i+num] == 'x') {
        num = 1;
    } else {
        while (is_digit(temp[i+num])) num++;
        if (temp[i+num] == '.') {
            num++;
            while (is_digit(temp[i+num])) num++;
        }
    }
    strncat(buf, temp+i, num);
    strcat(buf, " ");
    i += num;
    while (temp[i+j] == ')' && j < count) j++;
    if (j < count) {
        return 0;
    }
    i += j;
    j = 0;
    while (temp[j] == '-' || temp[j] == '+') {
        strncat(buf, temp+j, 1);
        strcat(buf, "u ");
        j++;
    }
    strcat(dst, buf);
    return i;
}

int oper_priority(char *str, reg_templates template) {
    if (str[0] == '-' && str[1] == 'n') {
        return 1;
    } else if (str[0] == '^') {
        return 2;
    } else if (str[0] == '*' || str[0] == '/' || !regexec(&template.mod, str, 0, NULL, 0)) {
        return 3;
    } else if (str[0] == '+' || str[0] == '-') {
        return 4;
    }
    return 0;
}

int oper_process(char *temp, char *dst, stack_s *stack, reg_templates template) {
    char buf[MAX_LEN];
    int priority = oper_priority(temp, template);

    if (!peek_s(stack, buf)) {  // стак не пустой
        int stack_priority = oper_priority(buf, template);
        // вершина стака является операцией && ее приоритет выше или равен
        while (stack_priority && (stack_priority <= priority)) {
            pop_s(stack, buf);
            strcat(dst, buf);
            strcat(dst, " ");
            if (!peek_s(stack, buf)) {
                stack_priority = oper_priority(buf, template);
            } else {
                break;  // стак стал пустым
            }
            
        }
    }
    int i = is_oper(temp, template);
    strncpy(buf, temp, i);
    buf[i] = '\0';
    push_s(stack, buf);
    return i;
}

int bracket_process(char *dst, stack_s *stack, reg_templates template) {
    char buf[MAX_LEN];
    int bracket = 0, i = 0;
    while (!pop_s(stack, buf)) {  // пока стек не пустой
        if (buf[0] == '(') {
            bracket = 1;
            if (!peek_s(stack, buf) && (i = is_func(buf, template))) {
                strncat(dst, buf, i);
                strcat(dst, " ");
                pop_s(stack, buf);
            }
            break;
        } else if ((i = is_oper(buf, template)) != 0) {
            strcat(dst, buf);
            strcat(dst, " ");
        } else {
            break;
        }
    }
    if (!bracket) {
        return BAD_INPUT;  // стак закончился, открывающей скобки не найдено
    }
    return OK;
}

int sort_station(char *src, char *dst, reg_templates template) {

    char buf[MAX_LEN];
    char temp[MAX_LEN];
    strncpy(temp, src, MAX_LEN);  // обработанные символы будут вырезаться из temp
    
    stack_s stack;
    init_stack_s(&stack);
    int i = 0, need_number = 1, negative = 0;

    while (temp[0]) {
        //printf("%s\n", temp);
        //show_s(&stack);
        if (temp[0] == ' ') {  // если в начале строки пробел, пропускаем его
            memmove(temp, temp+1, strlen(temp));

        } else if (is_unar(temp)) {  // если в начале строки унарное выражение

            if (!need_number) {  // ожидался оператор, берем за таковой первый + или -
                i = oper_process(temp, dst, &stack, template);
                memmove(temp, temp+i, strlen(temp));
                need_number = 1;
            } else {
                i = unar_process(temp, dst);
                if (i == 0) {  // ошибка в расстановке скобок в унарном выражении
                    return BAD_INPUT;
                }
                memmove(temp, temp+i, strlen(temp));
                need_number = 0;
                negative = 0;
            }

        } else if (is_digit(temp[0]) || (temp[0] == 'x')) {  // если в начале строки число
            
            if (!need_number) // ожидался оператор, а не число
                return BAD_INPUT;
            i = number_process(temp, dst);
            memmove(temp, temp+i, strlen(temp));
            need_number = 0;
            negative = 0;

        } else if (is_oper(temp, template)) {  // если в начале строки оператор
            
            if (need_number) {
                if (temp[0] == '-' && !negative) {  // ожидалось число, встречен первый минус - отрицательное выражение
                    negative = 1;
                    buf[0] = '\0';
                    strcat(buf, "-n");
                    push_s(&stack, buf);
                    memmove(temp, temp+1, strlen(temp));

                } else {  // ожидалось число, встречен не первый минус или другой оператор - ошибка
                    return BAD_INPUT;
                }
            } else {  // ожидался оператор - обрабатываем нормально
                i = oper_process(temp, dst, &stack, template);
                memmove(temp, temp+i, strlen(temp));
                need_number = 1;
            }

        } else if (temp[0] == ')') {  // если в начале строки закрывающая скобка
            
            if (need_number) {  // еще не было встречнено ожидаемое число
                return BAD_INPUT;
            }
            if ((i = bracket_process(dst, &stack, template)) != 0) {
                return i;  // в расположении скобок ошибка
            }
            memmove(temp, temp+1, strlen(temp));
            need_number = 0;

        } else if ((i = is_func(temp, template)) != 0) {  // если в начале строки функция
            
            if (!need_number) {  // ожидался оператор, а не функция
                return BAD_INPUT;
            }
            strncpy(buf, temp, i);
            buf[i] = '\0';
            push_s(&stack, buf);
            memmove(temp, temp+i, strlen(temp));
            need_number = 1;
            negative = 0;

        } else if (temp[0] == '(') {  // если открывающая скобка

            if (!need_number) {  // ожидался оператор, а не выражение
                return BAD_INPUT;
            }
            strncpy(buf, temp, 1);
            buf[1] = '\0';
            push_s(&stack, buf);
            memmove(temp, temp+1, strlen(temp));
            need_number = 1;
            negative = 0;

        } else {  // подсунули неправильный символ
            return BAD_INPUT;
        }
    }
    if (need_number) {
        return BAD_INPUT;
    }
    while (!pop_s(&stack, buf)) {
        i = is_oper(buf, template);
        if (i == 0) {  // лексем не осталось, а в стеке не оператор
            return BAD_INPUT;
        } else {
            strcat(dst, buf);
            strcat(dst, " ");
        }
    }
    dst[strlen(dst)-1] = '\0';  // убираем пробел в конце строки
    return OK;
}
