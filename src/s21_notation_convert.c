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
    printf("%s\n", dst);
    int i = 0; // сколько символов занимает число
    while (is_digit(temp[i])) i++;
    if (temp[i] == '.' && is_digit(temp[i+1])) {
        i++;
        while (is_digit(temp[i])) i++;
    }
    strncat(dst, temp, i);
    //printf("i is %d\n", i);
    strcat(dst, " ");
    return i;
}

int oper_priority(char *str, int unar, reg_templates template) {
    if (str[0] == '^') {
        return 1;
    } else if (unar && (str[0] == '+' || str[0] == '-')) {
        return 2;
    } else if (str[0] == '*' || str[0] == '/' || !regexec(&template.mod, str, 0, NULL, 0)) {
        return 3;
    } else if (!unar && (str[0] == '+' || str[0] == '-')) {
        return 4;
    }
    return 0;
}

int oper_process(char *temp, char *dst, stack *stack, int unar, reg_templates template) {
    char buf[MAX_LEN];
    int priority = oper_priority(temp, unar, template);

    if (!peek(stack, buf)) {  // стак не пустой
        int stack_priority = oper_priority(buf, (buf[1] == 'u'), template);
        // вершина стака является операцией && ее приоритет выше или равен
        while (stack_priority && (stack_priority <= priority)) {
            pop(stack, buf);
            strncat(dst, buf, is_oper(buf, template));
            strcat(dst, " ");
            if (!peek(stack, buf)) {
                stack_priority = oper_priority(buf, (buf[1] == 'u'), template);
            } else {
                break;  // стак стал пустым
            }
            
        }
    }

    int i = is_oper(temp, template);
    strncpy(buf, temp, i);
    buf[i] = '\0';
    if (unar) buf[1] = 'u';
    push(stack, buf);
    return i;
}

int bracket_process(char *dst, stack *stack, reg_templates template) {
    char buf[MAX_LEN];
    int bracket = 0, i = 0;
    while (!pop(stack, buf)) {  // пока стек не пустой
        if (buf[0] == '(') {
            bracket = 1;
            if (!peek(stack, buf) && (i = is_func(buf, template))) {
                strncat(dst, buf, i);
                strcat(dst, " ");
                pop(stack, buf);
            }
            break;
        } else if ((i = is_oper(buf, template)) != 0) {
            strncat(dst, buf, i);
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
    
    stack stack;
    init_stack(&stack);

    int i, need_number = 1, unar_oper = 0;

    while (temp[0]) {
        //printf("NOW TEMP IS '%s'\n", temp);
        //printf("NOW DST IS '%s'\n", dst);
        //show(&stack);
        // если в начале строки пробел, пропускаем его
        if (temp[0] == ' ') {
            memmove(temp, temp+1, strlen(temp));

        // если в начале строки число
        } else if (is_digit(temp[0])) {
            if (!need_number) // ожидался оператор, а не число
                return BAD_INPUT;
            i = number_process(temp, dst);
            memmove(temp, temp+i, strlen(temp));
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
            i = oper_process(temp, dst, &stack, unar_oper, template);
            memmove(temp, temp+i, strlen(temp));
            need_number = 1;

        // если в начале строки закрывающая скобка
        } else if (temp[0] == ')') {
            if (need_number) {  // еще не было встречнено ожидаемое число
                return BAD_INPUT;
            }
            int code;
            if ((code = bracket_process(dst, &stack, template)) != 0) {
                return code;  // в расположении скобок ошибка
            }
            memmove(temp, temp+1, strlen(temp));
            need_number = 0;

        // если в начале строки функция
        } else if ((i = is_func(temp, template)) != 0) {
            if (!need_number) // ожидался оператор, а не функция
                return BAD_INPUT;
            strncpy(buf, temp, i);
            buf[i] = '\0';
            push(&stack, buf);
            memmove(temp, temp+i, strlen(temp));
            need_number = 1;

        // если открывающая скобка
        } else if (temp[0] == '(') {
            if (!need_number) // ожидался оператор, а не выражение
                return BAD_INPUT;
            strncpy(buf, temp, 1);
            buf[1] = '\0';
            push(&stack, buf);
            memmove(temp, temp+1, strlen(temp));
            need_number = 1;

        } else {
            return BAD_INPUT;  // подсунули неправильный символ
        }
    }
    if (need_number) {
        return BAD_INPUT;
    }
    while (!pop(&stack, buf)) {
        i = is_oper(buf, template);
        //printf("%d %s\n", i, buf);
        if (!i) {  // лексем не осталось, а в стеке не оператор
            return BAD_INPUT;
        } else {
            strncat(dst, buf, i);
            strcat(dst, " ");
            
        }
    }
    dst[strlen(dst)-1] = '\0';  // убираем пробел в конце строки
    //printf("'%s' LENGTH %ld\n", dst, strlen(dst));
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

    memset(dst, 0, MAX_LEN);
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
