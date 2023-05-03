#include "s21_stack_double.h"

#define OK 0
#define ERROR 1

int init_stack_d(stack_d *pt)
{
    pt->top = -1;
    return OK;
}

int size_d(stack_d *pt)
{
    return pt->top + 1;
}

int is_empty_d(stack_d *pt)
{
    return size_d(pt) == 0;
}

int is_full_d(stack_d *pt)
{
    return size_d(pt) == MAX_LEN;
}

int push_d(stack_d *pt, double num)
{
    if (is_full_d(pt))
    {
        return ERROR;
    }
    pt->top += 1;
    pt->data = num;
    return OK;
}

int peek_d(stack_d *pt, double num)
{
    if (is_empty_d(pt))
    {
        return ERROR;
    }
    pt->data = num;
    return OK;
}

int pop_d(stack_d *pt, double num)
{
    if (is_empty_d(pt))
    {
        return ERROR;
    }
    pt->data = num;
    pt->top -= 1;
    return OK;
}

/*void show(stack_d *pt) {
    for (int i = pt->top; i >= 0; i--) {
        printf("STACK №%d %lf\n", i, pt->data);
    }
}*/
