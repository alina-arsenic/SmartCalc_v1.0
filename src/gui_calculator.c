#include "gui_utils.h"
#include "lib/s21_notation.h"
#include "lib/s21_smart_calc.h"

int check_x_count(char *expr) {
  int count = 0;
  char *ptr = expr;
  while (ptr = strstr(ptr, "x")) {
    count++;
    ptr++;
  }
  if (count > 15) {
    return 1;
  } else {
    return 0;
  }
}

int check_limits(s21_limits *lims) {
  memset(lims->x1_buffer, 0, MAX_LEN);
  memset(lims->x2_buffer, 0, MAX_LEN);
  memset(lims->y1_buffer, 0, MAX_LEN);
  memset(lims->y2_buffer, 0, MAX_LEN);
  strcat(lims->x1_buffer, gtk_entry_get_text(GTK_ENTRY(lims->x1_box)));
  strcat(lims->x2_buffer, gtk_entry_get_text(GTK_ENTRY(lims->x2_box)));
  strcat(lims->y1_buffer, gtk_entry_get_text(GTK_ENTRY(lims->y1_box)));
  strcat(lims->y2_buffer, gtk_entry_get_text(GTK_ENTRY(lims->y2_box)));
  if (!strcmp("", lims->x1_buffer) || !strcmp("", lims->x2_buffer) ||
      !strcmp("", lims->y1_buffer) || !strcmp("", lims->y2_buffer)) {
    return 1;
  }
  if (parse_double(lims->x1_buffer, &lims->x1) ||
      parse_double(lims->x2_buffer, &lims->x2) ||
      parse_double(lims->y1_buffer, &lims->y1) ||
      parse_double(lims->y2_buffer, &lims->y2)) {
    return 3;
  }
  if (fabs(lims->x1) > 1000000.0 || fabs(lims->x2) > 1000000.0 ||
      fabs(lims->y1) > 1000000.0 || fabs(lims->y2) > 1000000.0) {
    return 2;
  }
  if (lims->x1 == lims->x2 || lims->y1 == lims->y2) {
    return 4;
  }
  double temp;
  if (lims->x1 > lims->x2) {
    temp = lims->x1;
    lims->x1 = lims->x2;
    lims->x2 = temp;
  }
  if (lims->y1 > lims->y2) {
    temp = lims->y1;
    lims->y1 = lims->y2;
    lims->y2 = temp;
  }
  return 0;
}

int make_plot() {
  GdkWindow *window = gtk_widget_get_window((GtkWidget *)numpad.drawingArea);
  cairo_region_t *cairoRegion = cairo_region_create();
  GdkDrawingContext *drawingContext =
      gdk_window_begin_draw_frame(window, cairoRegion);
  numpad.cr = gdk_drawing_context_get_cairo_context(drawingContext);

  fill_area(numpad.cr);
  draw_border(numpad.cr);

  if (numpad.expr[0]) {
    double min = 0, max = 0;
    draw_axis(numpad.cr, limits);
    draw_plot(numpad.cr, limits, numpad.expr, &min, &max);

    if (limits.y1 > max || limits.y2 < min) {
      char buf[MAX_LEN] = {0};
      sprintf(buf, "Graph is not visible, y ∈ [ %g ; %g ]", min, max);
      gtk_label_set_text(GTK_LABEL(numpad.label), buf);
    }
  }

  gdk_window_end_draw_frame(window, drawingContext);
  cairo_region_destroy(cairoRegion);

  return 0;
}

void calculate(GtkButton *button, gpointer data) {
  double result = 0;
  const gchar *text = gtk_button_get_label(button);
  gtk_label_set_text(GTK_LABEL(numpad.label), "");
  memset(numpad.expr, 0, MAX_LEN);

  if (strcmp("=", text) == 0) {
    char input_buffer[MAX_LEN] = {0};
    char output_buffer[MAX_LEN] = {0};
    strcat(input_buffer, gtk_entry_get_text(GTK_ENTRY(numpad.box)));
    int code = notation_convert(input_buffer, numpad.expr);
    if (!code) {
      if (strchr(input_buffer, 'x')) {
        code += check_limits(&limits);
        if (code == 0) {
          code += check_x_count(numpad.expr);
          if (code == 0) {
            char buf[MAX_LEN] = {0};
            double x_scale =
                fabs(limits.x1 - limits.x2) / (double)(DISPLAY_WIDTH);
            double y_scale =
                fabs(limits.y1 - limits.y2) / (double)(DISPLAY_HEIGHT);
            sprintf(buf, "X   1 : %g      Y   1 : %g", x_scale, y_scale);
            gtk_label_set_text(GTK_LABEL(numpad.label), buf);
            make_plot();
          } else {
            gtk_label_set_text(GTK_LABEL(numpad.label),
                               "No more than 15 'x' please");
          }

        } else {
          memset(numpad.expr, 0, MAX_LEN);
          make_plot();
          if (code == 1) {
            gtk_label_set_text(GTK_LABEL(numpad.label),
                               "Please specify limits");
          } else if (code == 2) {
            gtk_label_set_text(
                GTK_LABEL(numpad.label),
                "Values in limits must be from -1000000 to 1000000");
          } else if (code == 4) {
            gtk_label_set_text(GTK_LABEL(numpad.label),
                               "Values in limits must be different");
          } else {
            gtk_label_set_text(GTK_LABEL(numpad.label),
                               "Incorrect symbols in limits");
          }
        }
      } else {
        code += calculation(numpad.expr, &result);
        memset(numpad.expr, 0, MAX_LEN);
        if (!code) {
          sprintf(output_buffer, "%.7lf", result);
          char *dot_point = strchr(output_buffer, ',');
          if (dot_point) *dot_point = '.';
          gtk_entry_set_text(GTK_ENTRY(numpad.box), output_buffer);
        } else {
          gtk_label_set_text(GTK_LABEL(numpad.label), "Answer is not defined");
        }
      }
    } else {
      gtk_label_set_text(GTK_LABEL(numpad.label), "Incorrect expression");
    }

  } else if (strcmp("C", text) == 0) {
    gtk_entry_set_text(GTK_ENTRY(numpad.box), "");

  } else if (strcmp("clear X", text) == 0) {
    gtk_entry_set_text(GTK_ENTRY(limits.x1_box), "");
    gtk_entry_set_text(GTK_ENTRY(limits.x2_box), "");

  } else if (strcmp("clear Y", text) == 0) {
    gtk_entry_set_text(GTK_ENTRY(limits.y1_box), "");
    gtk_entry_set_text(GTK_ENTRY(limits.y2_box), "");

  } else {
    GtkWidget *box_ptr = gtk_window_get_focus(GTK_WINDOW(numpad.window));
    guint position = gtk_editable_get_position(GTK_EDITABLE(box_ptr));
    gtk_entry_buffer_insert_text(gtk_entry_get_buffer(GTK_ENTRY(box_ptr)),
                                 position, text, -1);
    gtk_editable_set_position(GTK_EDITABLE(box_ptr), position + strlen(text));
  }
}
