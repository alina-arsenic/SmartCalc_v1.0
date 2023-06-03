#include "gui_utils.h"
#include "lib/s21_notation.h"
#include "lib/s21_smart_calc.h"

s21_limits limits;
s21_numpad numpad;
s21_credit credit;
s21_deposit deposit;

int parse_double(char *str, double *result) {
  int i = 0, dot = 0;
  if (str[i] == '-') i++;
  while (is_digit(str[i])) i++;
  if (str[i] == '.' && is_digit(str[i + 1])) {
    dot = i;
    i++;
    while (is_digit(str[i])) i++;
  }
  if (i != strlen(str)) {
    return 1;
  }
  double test = 0;
  sscanf(str, "%lf", &test);
  char *dot_point = strchr(str, '.');
  if (dot_point) *dot_point = ',';
  sscanf(str, "%lf", result);
  if (fabs(test) > fabs(*result)) *result = test;
  return 0;
}

void activate(GtkApplication *app, gpointer user_data) {
  // создание окна приложения
  numpad.window = gtk_application_window_new(app);
  gtk_window_set_position(GTK_WINDOW(numpad.window), GTK_WIN_POS_CENTER);
  gtk_window_set_title(GTK_WINDOW(numpad.window), "Smart Calculator");
  gtk_window_set_default_size(GTK_WINDOW(numpad.window), APP_WIDTH, APP_HEIGHT);
  gtk_container_set_border_width(GTK_CONTAINER(numpad.window), BORDER_SIZE);

  // создание виджета с вкладками
  numpad.notebook = gtk_notebook_new();
  gtk_container_add(GTK_CONTAINER(numpad.window), numpad.notebook);
  gtk_notebook_set_show_border(GTK_NOTEBOOK(numpad.notebook), FALSE);

  // создание вкладки с математическим калькулятором
  numpad.grid = gtk_grid_new();
  GtkWidget *notebook_label = gtk_label_new("Math");
  gtk_notebook_append_page(GTK_NOTEBOOK(numpad.notebook), numpad.grid,
                           notebook_label);
  calculator_initialization();

  // создание вкладки с кредитным калькулятором
  credit.grid = gtk_grid_new();
  notebook_label = gtk_label_new("Credit");
  gtk_notebook_append_page(GTK_NOTEBOOK(numpad.notebook), credit.grid,
                           notebook_label);
  credit_initialization();

  // создание вкладки с калькулятором вкладов
  deposit.grid = gtk_grid_new();
  notebook_label = gtk_label_new("Deposit");
  gtk_notebook_append_page(GTK_NOTEBOOK(numpad.notebook), deposit.grid,
                           notebook_label);
  deposit_initialization();

  gtk_widget_show_all(numpad.window);
}

int main(int argc, char **argv) {
  GtkApplication *app;
  int status;

  app = gtk_application_new("org.gtk.calculator", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);

  g_object_unref(app);
  return status;
}
