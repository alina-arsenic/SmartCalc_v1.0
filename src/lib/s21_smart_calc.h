#ifndef SRC_S21_SMART_CALC_
#define SRC_S21_SMART_CALC_

#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <math.h>
#include <stdio.h>

#define MAX_LEN 512
#define MAX_USER_LEN 255

int notation_convert(char *src, char *dst);
int calculation(char *src, double *result);

#endif  //  SRC_S21_SMART_CALC_
