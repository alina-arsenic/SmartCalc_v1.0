#ifndef SRC_SMART_CALC_GUI_
#define SRC_SMART_CALC_GUI_

#include "lib/s21_smart_calc.h"

typedef struct {
  GtkWidget *window;
  GtkWidget *grid;
  GtkWidget *button[31];
} s21_numpad;

extern GtkWidget *box;

void calculate(GtkButton *button, gpointer data);
void activate(GtkApplication *app, gpointer user_data);

#endif  //  SRC_SMART_CALC_GUI_
