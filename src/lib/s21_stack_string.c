#include "s21_stack_string.h"

#define OK 0
#define ERROR 1

int init_stack_s(stack_s *pt) {
    pt->top = -1;
    return OK;
}

int size_s(stack_s *pt) {
    return pt->top + 1;
}

int is_empty_s(stack_s *pt) {
    return size_s(pt) == 0;
}

int is_full_s(stack_s *pt) {
    return size_s(pt) == MAX_LEN;
}

int push_s(stack_s *pt, char *str) {
    if (is_full_s(pt)) {
        return ERROR;
    }
    pt->top += 1;
    strcpy(pt->data[pt->top], str);
    return OK;
}

int peek_s(stack_s *pt, char *str) {
    if (is_empty_s(pt)) {
        return ERROR;
    }
    strcpy(str, pt->data[pt->top]);
    return OK;
}

int pop_s(stack_s *pt, char *str) {
    if (is_empty_s(pt)) {
        return ERROR;
    }
    strcpy(str, pt->data[pt->top]);
    pt->top -= 1;
    return OK;
}

/*void show(stack_s *pt) {
    for (int i = pt->top; i >= 0; i--) {
        printf("STACK №%d %s\n", i, pt->data[i]);
    }
}*/
