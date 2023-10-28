#include "gui_utils.h"

#define BUFFER_LENGTH 4096 * 2

void annuity_calc(char *text) {
  double i = credit.value_rate / 12;
  double n = credit.value_term;
  double k = i * pow((1 + i), n) / (pow(1 + i, n) - 1);
  double month_pay = k * credit.value_amount;
  double total_pay = month_pay * n;
  double overpay = total_pay - credit.value_amount;
  char buf[MAX_LEN] = {0};
  sprintf(buf, "Monthly payment:       %lf\n", month_pay);
  strcat(text, buf);
  memset(buf, 0, MAX_LEN);
  sprintf(buf, "Overpay:                           %lf\n", overpay);
  strcat(text, buf);
  memset(buf, 0, MAX_LEN);
  sprintf(buf, "Total payment:              %lf\n", total_pay);
  strcat(text, buf);
}

void differen_calc(char *text) {
  char buf[MAX_LEN] = {0};
  char text_temp[BUFFER_LENGTH] = {0};

  time_t time_raw_format;
  time(&time_raw_format);
  int month, year;
  strftime(buf, MAX_LEN, "%m", localtime(&time_raw_format));
  sscanf(buf, "%d", &month);
  strftime(buf, MAX_LEN, "%Y", localtime(&time_raw_format));
  sscanf(buf, "%d", &year);

  double m = 0;
  double P = credit.value_amount;
  double i = credit.value_rate / 12;
  double n = credit.value_term;
  double D = 0, total_pay = 0;

  while (m < n) {
    memset(buf, 0, MAX_LEN);
    D = P / n + (P - m * (P / n)) * i;
    sprintf(buf, "%02d.%d       %lf\n", month, year, D);
    strcat(text_temp, buf);
    total_pay += D;
    m += 1;
  }

  memset(buf, 0, MAX_LEN);
  sprintf(buf, "Total payment:              %lf\n", total_pay);
  strcat(text, buf);
  memset(buf, 0, MAX_LEN);
  sprintf(buf, "Overpay:                           %lf\n",
          total_pay - credit.value_amount);
  strcat(text, buf);
  memset(buf, 0, MAX_LEN);
  sprintf(buf, "Monthly payments:\n\n");
  strcat(text, buf);
  strcat(text, text_temp);
}

int check_input_credit() {
  char buf[MAX_LEN] = {0};
  strcat(buf, gtk_entry_get_text(GTK_ENTRY(credit.entry_amount)));
  if (buf[0] == '\0') {
    return 1;
  } else if (parse_double(buf, &credit.value_amount)) {
    return 2;
  } else if ((credit.value_amount <= 0.0) ||
             (credit.value_amount > 1000000000000.0)) {
    return 3;
  }
  memset(buf, 0, MAX_LEN);
  strcat(buf, gtk_entry_get_text(GTK_ENTRY(credit.entry_term)));
  if (buf[0] == '\0') {
    return 1;
  } else if (parse_double(buf, &credit.value_term)) {
    return 2;
  } else if (trunc(credit.value_term) != credit.value_term) {
    return 7;
  } else if ((credit.value_term < 1.0) || (credit.value_term > 240.0)) {
    return 4;
  }
  memset(buf, 0, MAX_LEN);
  strcat(buf, gtk_entry_get_text(GTK_ENTRY(credit.entry_rate)));
  if (buf[0] == '\0') {
    return 1;
  } else if (parse_double(buf, &credit.value_rate)) {
    return 2;
  } else if ((credit.value_rate <= 0.0) || (credit.value_rate > 99.0)) {
    return 5;
  }
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(credit.button[2])) ==
          FALSE &&
      gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(credit.button[3])) ==
          FALSE) {
    return 6;
  }
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(credit.button[2]))) {
    credit.credit_type = 1;
  }
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(credit.button[3]))) {
    credit.credit_type = 2;
  }
  credit.value_rate /= 100.0;
  return 0;
}

void credit_calculate(GtkButton *button, gpointer data) {
  GtkTextBuffer *empty_buffer = gtk_text_buffer_new(NULL);
  char empty_text[1] = {0};
  gtk_text_buffer_set_text(empty_buffer, empty_text, strlen(empty_text));
  gtk_text_view_set_buffer(GTK_TEXT_VIEW(credit.text_wall), empty_buffer);
  int code = check_input_credit();
  if (code == 1) {
    gtk_label_set_text(GTK_LABEL(credit.message), "Please fill all entries");
  } else if (code == 2) {
    gtk_label_set_text(GTK_LABEL(credit.message),
                       "Incorrect symbols in values");
  } else if (code == 3) {
    gtk_label_set_text(GTK_LABEL(credit.message),
                       "Credit amount must be >0 and <=1000000000000 RUB");
  } else if (code == 4) {
    gtk_label_set_text(GTK_LABEL(credit.message),
                       "Term must be >=1 and <=240 mounth");
  } else if (code == 5) {
    gtk_label_set_text(GTK_LABEL(credit.message),
                       "Interest rate must be >0 and <=99 \%");
  } else if (code == 6) {
    gtk_label_set_text(GTK_LABEL(credit.message),
                       "Please choose type of monthly payments");
  } else if (code == 7) {
    gtk_label_set_text(GTK_LABEL(credit.message), "Term must be integer");
  } else if (code == 0) {
    gtk_label_set_text(GTK_LABEL(credit.message), "");
    GtkTextBuffer *result_buffer = gtk_text_buffer_new(NULL);
    char result_text[BUFFER_LENGTH] = {0};
    if (credit.credit_type == 1) {
      annuity_calc(result_text);
    } else {
      differen_calc(result_text);
    }
    gtk_text_buffer_set_text(result_buffer, result_text, strlen(result_text));
    gtk_text_view_set_buffer(GTK_TEXT_VIEW(credit.text_wall), result_buffer);
  }
}

void credit_reset(GtkButton *button, gpointer data) {
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(credit.button[2]), FALSE);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(credit.button[3]), FALSE);
  credit.credit_type = 0;
  gtk_label_set_text(GTK_LABEL(credit.message), "");
  gtk_entry_set_text(GTK_ENTRY(credit.entry_amount), "");
  gtk_entry_set_text(GTK_ENTRY(credit.entry_rate), "");
  gtk_entry_set_text(GTK_ENTRY(credit.entry_term), "");
  GtkTextBuffer *empty_buffer = gtk_text_buffer_new(NULL);
  char empty_text[1] = {0};
  gtk_text_buffer_set_text(empty_buffer, empty_text, strlen(empty_text));
  gtk_text_view_set_buffer(GTK_TEXT_VIEW(credit.text_wall), empty_buffer);
}
