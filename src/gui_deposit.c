#include "gui_utils.h"
#include "lib/s21_notation.h"

#define BUFFER_LENGTH 4096 * 2

int year_days(int n) {
  if ((n % 4 == 0 && n % 100 != 0) || n % 400 == 0) {
    return 366;
  } else {
    return 356;
  }
}

int month_days(int n, int leap) {
  if (n == 2) {
    if (leap)
      return 29;
    else
      return 28;
  } else if (n == 4 || n == 6 || n == 9 || n == 11) {
    return 30;
  } else {
    return 31;
  }
}

int check_input_deposit() {
  char buf[MAX_LEN] = {0};
  strcat(buf, gtk_entry_get_text(GTK_ENTRY(deposit.entry.amount)));
  if (buf[0] == '\0') {
    return 1;
  } else if (parse_double(buf, &deposit.value.amount)) {
    return 2;
  } else if ((deposit.value.amount <= 0.0) ||
             (deposit.value.amount > 1000000000000.0)) {
    return 3;
  }

  memset(buf, 0, MAX_LEN);
  strcat(buf, gtk_entry_get_text(GTK_ENTRY(deposit.entry.term)));
  if (buf[0] == '\0') {
    return 1;
  } else if (parse_double(buf, &deposit.value.term)) {
    return 2;
  } else if (trunc(deposit.value.term) != deposit.value.term) {
    return 4;
  } else if ((deposit.value.term < 1.0) || (deposit.value.term > 240.0)) {
    return 4;
  }

  memset(buf, 0, MAX_LEN);
  strcat(buf, gtk_entry_get_text(GTK_ENTRY(deposit.entry.rate)));
  if (buf[0] == '\0') {
    return 1;
  } else if (parse_double(buf, &deposit.value.rate)) {
    return 2;
  } else if ((deposit.value.rate <= 0.0) || (deposit.value.rate > 99.0)) {
    return 5;
  }

  memset(buf, 0, MAX_LEN);
  strcat(buf, gtk_entry_get_text(GTK_ENTRY(deposit.entry.tax)));
  if (buf[0] == '\0') {
    return 1;
  } else if (parse_double(buf, &deposit.value.tax)) {
    return 2;
  } else if ((deposit.value.tax <= 0.0) || (deposit.value.tax > 99.0)) {
    return 5;
  }

  memset(buf, 0, MAX_LEN);
  strcat(buf, gtk_entry_get_text(GTK_ENTRY(deposit.entry.date)));
  if (buf[0] == '\0') {
    return 1;
  } else {
    int i = 0;
    while (is_digit(buf[i])) i++;
    if (buf[i] != '.') {
      return 6;
    }
    i++;
    while (is_digit(buf[i])) i++;
    if (buf[i] != '.') {
      return 6;
    }
    i++;
    while (is_digit(buf[i])) i++;
    if (strlen(buf) != i) {
      return 6;
    }
    sscanf(buf, "%d.%d.%d", &deposit.date.day, &deposit.date.month,
           &deposit.date.year);
    if (deposit.date.month < 1 || deposit.date.month > 12 ||
        deposit.date.day < 1 ||
        deposit.date.day >
            month_days(deposit.date.month, year_days(deposit.date.year)) ||
        deposit.date.year < 1900 || deposit.date.year > 2100) {
      return 6;
    }
  }

  GtkListBoxRow *selected =
      gtk_list_box_get_selected_row(GTK_LIST_BOX(deposit.pay_type_box));
  if (!selected) {
    return 7;
  }
  int index = gtk_list_box_row_get_index(selected);
  if (index == 0)
    deposit.pay_type = 1;
  else if (index == 1)
    deposit.pay_type = 3;
  else if (index == 2)
    deposit.pay_type = 6;
  else if (index == 3)
    deposit.pay_type = 12;
  else if (index == 4)
    deposit.pay_type = deposit.value.term;

  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(deposit.capital_box))) {
    deposit.capital = 1;
  } else {
    deposit.capital = 0;
  }

  return 0;
}

int parse_pay_entry(char *buf, s21_date *date, double *sum) {
  char entry[BUFFER_LENGTH] = {0};
  char temp[BUFFER_LENGTH] = {0};
  int i = 0;
  while (buf[i] && buf[i] != '\n') i++;
  strncat(entry, buf, i);
  if (i < strlen(buf)) {
    strcat(temp, buf + i + 1);
    memset(buf, 0, BUFFER_LENGTH);
    strcat(buf, temp);
  } else {
    memset(buf, 0, BUFFER_LENGTH);
  }
  i = 0;
  while (is_digit(entry[i])) i++;
  if (entry[i] != '.') {
    return 1;
  }
  i++;
  while (is_digit(entry[i])) i++;
  if (entry[i] != '.') {
    return 1;
  }
  i++;
  while (is_digit(entry[i])) i++;
  if (entry[i] != ' ') {
    return 1;
  }
  i++;
  char *dot_point = strchr(entry + i, '.');
  while (is_digit(entry[i])) i++;
  if (entry[i] == '.' && is_digit(entry[i + 1])) {
    i++;
    while (is_digit(entry[i])) i++;
  }
  if (strlen(entry) != i) {
    return 1;
  }

  double test = 0;
  sscanf(entry, "%d.%d.%d %lf", &date->day, &date->month, &date->year, &test);
  if (dot_point) *dot_point = ',';
  sscanf(entry, "%d.%d.%d %lf", &date->day, &date->month, &date->year, sum);
  if (fabs(test) > fabs(*sum)) *sum = test;
  if (date->month < 1 || date->month > 12 || date->day < 1 ||
      date->day > month_days(date->month, year_days(date->year))) {
    return 1;
  }

  return 0;
}

// 2 - дата1 > дата2
// 1 - дата1 <= дата2 НЕ БОЛЕЕ чем на месяц
// 0 - дата1 < дата2 БОЛЕЕ чем на месяц
int compare_dates(s21_date start, s21_date end) {
  if (start.year > end.year) {
    return 2;
  }
  if (start.year < end.year) {
    if (start.year + 1 == end.year && start.month == 12 && end.month == 1 &&
        start.day > end.day) {
      return 1;
    } else {
      return 0;
    }
  }
  if (start.month > end.month) {
    return 2;
  }
  if (start.month < end.month) {
    if (start.month + 1 == end.month && start.day > end.day) {
      return 1;
    } else {
      return 0;
    }
  }
  if (start.day > end.day) {
    return 2;
  }
  return 1;
}

int deposit_fun(char *result) {
  double total_percents = 0;
  double month_percents = 0;
  double period_percents = 0;
  double year_percents = 0;
  double tax = 0;
  double left = deposit.value.amount;
  double delta = 0;
  char buf[MAX_LEN];
  double no_tax_summ = 1000000.0 * deposit.value.tax / 100;
  s21_date current;
  current.day = deposit.date.day;
  current.month = deposit.date.month;
  current.year = deposit.date.year;

  GtkTextIter start, end;
  GtkTextBuffer *buffer;
  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(deposit.text.in_box));
  gtk_text_buffer_get_bounds(buffer, &start, &end);
  char in_buf[BUFFER_LENGTH] = {0};
  strcat(in_buf, gtk_text_buffer_get_text(buffer, &start, &end, FALSE));
  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(deposit.text.out_box));
  gtk_text_buffer_get_bounds(buffer, &start, &end);
  char out_buf[BUFFER_LENGTH] = {0};
  strcat(out_buf, gtk_text_buffer_get_text(buffer, &start, &end, FALSE));

  s21_date in_date, out_date;
  double in_sum, out_sum;
  int flag_in = 0, flag_out = 0;

  int count = 0;
  int code = 0, compare;
  while (count < deposit.value.term) {
    if (out_buf[0] || flag_out) {
      if (!flag_out) code = parse_pay_entry(out_buf, &out_date, &out_sum);
      if (code) return 1;
      compare = compare_dates(current, out_date);
      while (compare == 1 && out_buf[0]) {
        delta -= out_sum;
        code = parse_pay_entry(out_buf, &out_date, &out_sum);
        if (code) return 1;
        compare = compare_dates(current, out_date);
      }
      if (compare == 2) {
        return 1;
      } else if (compare == 1) {
        delta -= out_sum;
        flag_out = 0;
      } else if (compare == 0) {
        flag_out = 1;
      }
    }
    if (in_buf[0] || flag_in) {
      if (!flag_in) code = parse_pay_entry(in_buf, &in_date, &in_sum);
      if (code) return 1;
      compare = compare_dates(current, in_date);
      while (compare == 1 && in_buf[0]) {
        delta += in_sum;
        code = parse_pay_entry(in_buf, &in_date, &in_sum);
        if (code) return 1;
        compare = compare_dates(current, in_date);
      }
      if (compare == 2) {
        return 1;
      } else if (compare == 1) {
        delta += in_sum;
        flag_in = 0;
      } else if (compare == 0) {
        flag_in = 1;
      }
    }
    if (delta < 0) {
      left += delta;
      if (left < 0) return 1;
    }
    month_percents = left * deposit.value.rate / 100 / 12;
    period_percents += month_percents;
    year_percents += month_percents;
    total_percents += month_percents;
    if (deposit.capital) {
      if ((count + 1) % deposit.pay_type == 0) {
        left += period_percents;
        period_percents = 0;
      }
    }
    if (delta > 0) {
      left += delta;
    }
    delta = 0;
    current.month += 1;
    if (current.month > 12) {
      current.month = 1;
      current.year += 1;
      if (year_percents - no_tax_summ > 0) {
        tax += (year_percents - no_tax_summ) * 0.13;
      }
      year_percents = 0;
    }
    count++;
  }
  if (flag_in || flag_out) return 1;
  if (deposit.capital) left += period_percents;
  if (year_percents - no_tax_summ > 0) {
    tax += (year_percents - no_tax_summ) * 0.13;
  }
  memset(buf, 0, MAX_LEN);
  sprintf(buf, "Accrued interest:                        %lf\n",
          total_percents);
  strcat(result, buf);
  memset(buf, 0, MAX_LEN);
  sprintf(buf, "Tax:                                                     %lf\n",
          tax);
  strcat(result, buf);
  memset(buf, 0, MAX_LEN);
  sprintf(buf, "Deposit amount in the end:  %lf\n", left);
  strcat(result, buf);
  return 0;
}

void deposit_calculate(GtkButton *button, gpointer data) {
  GtkTextBuffer *empty_buffer = gtk_text_buffer_new(NULL);
  char empty_text[1] = {0};
  gtk_text_buffer_set_text(empty_buffer, empty_text, strlen(empty_text));
  gtk_text_view_set_buffer(GTK_TEXT_VIEW(deposit.text.result_box),
                           empty_buffer);

  int code = check_input_deposit();
  if (code == 1) {
    gtk_label_set_text(GTK_LABEL(deposit.message), "Please fill all entries");
  } else if (code == 2) {
    gtk_label_set_text(GTK_LABEL(deposit.message),
                       "Incorrect symbols in values");
  } else if (code == 3) {
    gtk_label_set_text(GTK_LABEL(deposit.message),
                       "Deposit amount must be >0 and <=1000000000000 RUB");
  } else if (code == 4) {
    gtk_label_set_text(GTK_LABEL(deposit.message),
                       "Term must be an integer >=1 and <=240");
  } else if (code == 5) {
    gtk_label_set_text(GTK_LABEL(deposit.message),
                       "Rates must be >0 and <=99 \%");
  } else if (code == 6) {
    gtk_label_set_text(GTK_LABEL(deposit.message),
                       "Incorrect date of start of term");
  } else if (code == 7) {
    gtk_label_set_text(GTK_LABEL(deposit.message),
                       "Please choose periodicity of payments");
  } else if (code == 0) {
    gtk_label_set_text(GTK_LABEL(deposit.message), "");
    char result_text[BUFFER_LENGTH] = {0};
    code = deposit_fun(result_text);
    if (code) {
      gtk_label_set_text(GTK_LABEL(deposit.message), "Invalid list entry");
    } else {
      GtkTextBuffer *result_buffer = gtk_text_buffer_new(NULL);
      gtk_text_buffer_set_text(result_buffer, result_text, strlen(result_text));
      gtk_text_view_set_buffer(GTK_TEXT_VIEW(deposit.text.result_box),
                               result_buffer);
    }
  }
}

void deposit_reset(GtkButton *button, gpointer data) {
  gtk_list_box_unselect_all(GTK_LIST_BOX(deposit.pay_type_box));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(deposit.capital_box), FALSE);
  gtk_label_set_text(GTK_LABEL(deposit.message), "");
  gtk_entry_set_text(GTK_ENTRY(deposit.entry.amount), "");
  gtk_entry_set_text(GTK_ENTRY(deposit.entry.date), "");
  gtk_entry_set_text(GTK_ENTRY(deposit.entry.rate), "");
  gtk_entry_set_text(GTK_ENTRY(deposit.entry.tax), "");
  gtk_entry_set_text(GTK_ENTRY(deposit.entry.term), "");
  GtkTextBuffer *empty_buffer_1 = gtk_text_buffer_new(NULL);
  GtkTextBuffer *empty_buffer_2 = gtk_text_buffer_new(NULL);
  GtkTextBuffer *empty_buffer_3 = gtk_text_buffer_new(NULL);
  char empty_text[1] = {0};
  gtk_text_buffer_set_text(empty_buffer_1, empty_text, strlen(empty_text));
  gtk_text_buffer_set_text(empty_buffer_2, empty_text, strlen(empty_text));
  gtk_text_buffer_set_text(empty_buffer_3, empty_text, strlen(empty_text));
  gtk_text_view_set_buffer(GTK_TEXT_VIEW(deposit.text.result_box),
                           empty_buffer_1);
  gtk_text_view_set_buffer(GTK_TEXT_VIEW(deposit.text.in_box), empty_buffer_2);
  gtk_text_view_set_buffer(GTK_TEXT_VIEW(deposit.text.out_box), empty_buffer_3);
  //
}
