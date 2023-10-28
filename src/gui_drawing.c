#include "gui_utils.h"

void fill_area(cairo_t *cr) {
  cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
  cairo_paint(cr);
}

void draw_border(cairo_t *cr) {
  int step = 1;
  cairo_set_source_rgb(cr, 0.7, 0.7, 0.7);
  cairo_move_to(cr, step, step);
  cairo_line_to(cr, step, DISPLAY_HEIGHT - step);
  cairo_line_to(cr, DISPLAY_WIDTH - step, DISPLAY_HEIGHT - step);
  cairo_line_to(cr, DISPLAY_WIDTH - step, step);
  cairo_line_to(cr, step, step);
  cairo_stroke(cr);
}

void draw_axis(cairo_t *cr, s21_limits lims) {
  cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
  double k;
  if (lims.x1 <= 0 && lims.x2 >= 0) {
    k = fabs(lims.x1) / (fabs(lims.x1) + lims.x2);
    cairo_move_to(cr, DISPLAY_WIDTH * k, 0.0);
    cairo_line_to(cr, DISPLAY_WIDTH * k, DISPLAY_HEIGHT);
  }
  if (lims.y1 <= 0 && lims.y2 >= 0) {
    k = lims.y2 / (fabs(lims.y1) + lims.y2);
    cairo_move_to(cr, 0.0, DISPLAY_HEIGHT * k);
    cairo_line_to(cr, DISPLAY_WIDTH, DISPLAY_HEIGHT * k);
  }
  cairo_stroke(cr);
}

void replace_x(char *src, char *dst, double num) {
  int negative = num < 0;
  if (negative) num *= -1;
  char number[MAX_LEN] = {0};
  sprintf(number, "%.7lf", num);
  char *dot_point = strchr(number, ',');
  if (dot_point) *dot_point = '.';
  int n = strlen(number);
  char buff[MAX_LEN] = {0};
  int i = 0, j = 0;
  char *ptr = src;
  while (src[i]) {
    while (src[i] && src[i] != 'x') {
      i++;
      j++;
    }
    strncat(buff, ptr, j);
    if (src[i] == 'x') {
      strcat(buff, number);
      if (negative) strcat(buff, " -n");
      ptr = src + i + 1;
    }
    i++;
    j = 0;
  }

  memset(dst, 0, MAX_LEN);
  strcpy(dst, buff);
}

// найти x не влезающий в drawing area для плавной отрисовки
int find_closest(char *expr, s21_limits lims, double k_x, double k_y, double x,
                 double delta_x, double *x_result, double *y_result) {
  double y, x1 = x - delta_x, x2 = x, x_mid = x - delta_x / 2;
  char modified_expr[MAX_LEN];
  int count = 0, stop = 0;
  double canvas_x, canvas_y;
  while (count < 100 && !stop) {
    replace_x(expr, modified_expr, x_mid);
    if (calculation(modified_expr, &y) == 0) {
      canvas_x = (x - lims.x1) * k_x;
      canvas_y = (lims.y2 - y) * k_y;
      if (canvas_x < 0 || canvas_x > DISPLAY_WIDTH || canvas_y < 0 ||
          canvas_y > DISPLAY_HEIGHT) {
        *x_result = x_mid;
        *y_result = y;
        stop = 1;
      } else {
        x2 = x_mid;
        x_mid = (x1 + x_mid) / 2;
      }
    } else {
      x1 = x_mid;
      x_mid = (x2 + x_mid) / 2;
    }
    count++;
  }
  if (stop)
    return 1;
  else
    return 0;
}

int draw_plot(cairo_t *cr, s21_limits lims, char *expr, double *min,
              double *max) {
  cairo_set_source_rgb(cr, 0.0, 0.5, 1.0);
  int x_count = DISPLAY_WIDTH * 10;
  int y_count = DISPLAY_HEIGHT;
  double y, x = lims.x1;
  double x_total = fabs(lims.x1 - lims.x2);
  double y_total = fabs(lims.y1 - lims.y2);
  double x_delta = x_total / (double)(x_count);
  double y_delta = y_total / (double)(y_count);
  double k_x = DISPLAY_WIDTH / x_total;
  double k_y = DISPLAY_HEIGHT / y_total;
  double canvas_y = 0, canvas_x = 0;
  char modified_expr[MAX_LEN];
  int flag_max = 0, flag_gap = 0, count_outside = 0;
  while (x < lims.x2) {
    replace_x(expr, modified_expr, x);
    if (calculation(modified_expr, &y) == 0) {
      if (!flag_max) {
        *min = y;
        *max = y;
        flag_max = 1;
      } else {
        if (y > *max) *max = y;
        if (y < *min) *min = y;
      }
      canvas_x = (x - lims.x1) * k_x;
      canvas_y = (lims.y2 - y) * k_y;
      if (canvas_x < 0 || canvas_x > DISPLAY_WIDTH || canvas_y < 0 ||
          canvas_y > DISPLAY_HEIGHT) {
        if (!count_outside)
          cairo_line_to(cr, canvas_x, canvas_y);
        else
          cairo_move_to(cr, canvas_x, canvas_y);
        count_outside += 1;
      } else {
        count_outside = 0;
        if (flag_gap) {
          double x_test, y_test;
          if (find_closest(expr, lims, k_x, k_y, x, x_delta, &x_test,
                           &y_test)) {
            cairo_move_to(cr, (x_test - lims.x1) * k_x,
                          (lims.y2 - y_test) * k_y);
          } else {
            cairo_move_to(cr, canvas_x, canvas_y);
          }
          flag_gap = 0;
        } else {
          cairo_line_to(cr, canvas_x, canvas_y);
        }
      }

    } else {
      flag_gap = 1;
    }
    x += x_delta;
  }
  cairo_stroke(cr);
  return 0;
}
