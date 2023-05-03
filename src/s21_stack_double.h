#ifndef SRC_S21_STACK_DOUBLE_
#define SRC_S21_STACK_DOUBLE_

#include "s21_smart_calc.h"

typedef struct stack_d {
    int top;
    double data;
} stack_d;

int init_stack_d(stack_d *pt);
int size_d(stack_d *pt);
int is_empty_d(stack_d *pt);
int is_full_d(stack_d *pt);
int push_d(stack_d *pt, double num);
int peek_d(stack_d *pt, double num);
int pop_d(stack_d *pt, double num);
//void show_d(stack_d *pt);

#endif  //  SRC_S21_STACK_DOUBLE_
