#ifndef SRC_GUI_UTILS_H_
#define SRC_GUI_UTILS_H_

#include "lib/s21_smart_calc.h"

#define BORDER_SIZE 20

#define DISPLAY_WIDTH   500
#define DISPLAY_HEIGHT  380

#define APP_WIDTH 540
#define APP_HEIGHT 600

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

typedef struct {
  GtkWidget *window;
  GtkWidget *notebook;
  GtkWidget *grid;
  GtkWidget *label;
  GtkWidget *button[33];
  GtkWidget *box;
  GtkDrawingArea *drawingArea;
  cairo_t *cr;
  char expr[MAX_LEN];
} s21_numpad;

typedef struct {
  GtkWidget *grid;
  GtkWidget *button[4];
  GtkWidget *entry_amount;
  GtkWidget *entry_term;
  GtkWidget *entry_rate;
  double value_amount;
  int value_term;
  double value_rate;
  GtkWidget *label[15];
  GtkWidget *message;
  GtkWidget *text_wall;
  int credit_type;
} s21_credit;

extern s21_limits limits;
extern s21_numpad numpad;
extern s21_credit credit;

void calculate(GtkButton *button, gpointer data);
gboolean on_draw (GtkWidget *widget, GdkEventExpose *event, gpointer data);
int parse_double(char *str, double *result);
int make_plot();

void initialize_buttons(s21_numpad *numpad);
void initialize_upper_grid(GtkWidget *upper_grid, s21_limits *limits);
void draw_axis(cairo_t *cr, s21_limits lims);
int draw_plot(cairo_t *cr, s21_limits lims, char *expr, double *min, double *max);

void fill_area(cairo_t *cr);
void draw_border(cairo_t *cr);

#endif  //  SRC_GUI_UTILS_H_
