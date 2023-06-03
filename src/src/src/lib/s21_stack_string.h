#ifndef SRC_S21_STACK_STRING_
#define SRC_S21_STACK_STRING_

#include "s21_smart_calc.h"

typedef struct stack_s {
    int top;
    char data[MAX_LEN][MAX_LEN];
} stack_s;

int init_stack_s(stack_s *pt);
int size_s(stack_s *pt);
int is_empty_s(stack_s *pt);
int is_full_s(stack_s *pt);
int push_s(stack_s *pt, char *str);
int peek_s(stack_s *pt, char *str);
int pop_s(stack_s *pt, char *str);
//void show_s(stack_s *pt);

#endif  //  SRC_S21_STACK_STRING_
