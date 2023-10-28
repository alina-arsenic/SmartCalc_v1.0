#ifndef SRC_GUI_UTILS_H_
#define SRC_GUI_UTILS_H_

#include <time.h>

#include "lib/s21_smart_calc.h"

#define BORDER_SIZE 20

#define DISPLAY_WIDTH 500
#define DISPLAY_HEIGHT 380

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
  double value_term;
  double value_rate;
  GtkWidget *label[15];
  GtkWidget *message;
  GtkWidget *text_wall;
  GtkWidget *scrolled_window;
  int credit_type;
} s21_credit;

typedef struct {
  int day;
  int month;
  int year;
} s21_date;

typedef struct {
  GtkWidget *grid;
  struct s21_entry {
    GtkWidget *amount;
    GtkWidget *date;
    GtkWidget *term;
    GtkWidget *tax;
    GtkWidget *rate;
  } entry;
  s21_date date;
  struct s21_value {
    double amount;
    double term;
    double tax;
    double rate;
  } value;
  GtkWidget *pay_type_box;
  GtkWidget *capital_box;
  int pay_type;
  int capital;
  struct s21_text {
    GtkWidget *in_box;
    GtkWidget *out_box;
    GtkWidget *result_box;
    GtkWidget *in_window;
    GtkWidget *out_window;
    GtkWidget *result_window;
  } text;
  GtkWidget *label;
  GtkWidget *message;
  GtkWidget *button[2];
} s21_deposit;

extern s21_limits limits;
extern s21_numpad numpad;
extern s21_credit credit;
extern s21_deposit deposit;

int parse_double(char *str, double *result);

void calculator_initialization();
void credit_initialization();
void deposit_initialization();

void calculate(GtkButton *button, gpointer data);
int make_plot();
void draw_axis(cairo_t *cr, s21_limits lims);
int draw_plot(cairo_t *cr, s21_limits lims, char *expr, double *min,
              double *max);
void fill_area(cairo_t *cr);
void draw_border(cairo_t *cr);

int check_input_credit();
void credit_calculate(GtkButton *button, gpointer data);
void credit_reset(GtkButton *button, gpointer data);

int check_input_deposit();
void deposit_calculate(GtkButton *button, gpointer data);
void deposit_reset(GtkButton *button, gpointer data);

#endif  //  SRC_GUI_UTILS_H_
