#include "s21_stack.h"

#define OK 0
#define ERROR 1

int init_stack(stack *pt) {
    pt->top = -1;
    return OK;
}

int size(stack *pt) {
    return pt->top + 1;
}

int is_empty(stack *pt) {
    return size(pt) == 0;
}

int is_full(stack *pt) {
    return size(pt) == MAX_LEN;
}

int push(stack *pt, char *str) {
    if (is_full(pt)) {
        return ERROR;
    }
    pt->top += 1;
    strcpy(pt->data[pt->top], str);
    return OK;
}

int peek(stack *pt, char *str) {
    if (is_empty(pt)) {
        return ERROR;
    }
    strcpy(str, pt->data[pt->top]);
    return OK;
}

int pop(stack *pt, char *str) {
    if (is_empty(pt)) {
        return ERROR;
    }
    strcpy(str, pt->data[pt->top]);
    pt->top -= 1;
    return OK;
}

/*void show(stack *pt) {
    for (int i = pt->top; i >= 0; i--) {
        printf("STACK №%d %s\n", i, pt->data[i]);
    }
}*/
