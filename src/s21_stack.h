#ifndef SRC_S21_STACK_
#define SRC_S21_STACK_

#include "s21_smart_calc.h"

typedef struct stack {
    int top;
    char data[MAX_LEN][MAX_LEN];
} stack;

int init_stack(stack *pt);
int size(stack *pt);
int is_empty(stack *pt);
int is_full(stack *pt);
int push(stack *pt, char *str);
int peek(stack *pt, char *str);
int pop(stack *pt, char *str);
//void show(stack *pt);

#endif  //  SRC_S21_STACK_