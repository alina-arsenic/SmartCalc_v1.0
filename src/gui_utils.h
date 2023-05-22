#ifndef SRC_GUI_UTILS_H_
#define SRC_GUI_UTILS_H_

#include "lib/s21_smart_calc.h"

#define BORDER_SIZE 20

#define DISPLAY_WIDTH   500
#define DISPLAY_HEIGHT  380

#define APP_WIDTH 540
#define APP_HEIGHT 600

typedef struct {
  GtkWidget *window;
  GtkWidget *grid;
  GtkWidget *button[33];
} s21_numpad;

typedef struct {
    GtkWidget *x1_box;
    GtkWidget *x2_box;
    GtkWidget *y1_box;
    GtkWidget *y2_box;
    char x1_buffer[MAX_LEN];
    char x2_buffer[MAX_LEN];
    char y1_buffer[MAX_LEN];
    char y2_buffer[MAX_LEN];
    double x1, x2, y1, y2;
} s21_limits;

void initialize_buttons(s21_numpad *numpad);
void initialize_upper_grid(GtkWidget *upper_grid, s21_limits *limits);
void draw_axis(cairo_t *cr, s21_limits lims);
int draw_plot(cairo_t *cr, s21_limits lims, char *expr, double *min, double *max);

void fill_area(cairo_t *cr);
void draw_border(cairo_t *cr);

#endif  //  SRC_GUI_UTILS_H_
