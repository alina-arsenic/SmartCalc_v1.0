#include "gui_utils.h"
#include "lib/s21_notation.h"

#define BUFFER_LENGTH 4096*2

int year_days(int n) {
	if ((n % 4 == 0 && n % 100 !=0 ) || n % 400 == 0) {
		return 366;
	} else {
		return 356;
	}
}

int month_days(int n, int leap) {
	if (n == 2) {
		if (leap) return 29;
		else return 28;
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
	} else if ((deposit.value.amount <= 0.0) || (deposit.value.amount > 1000000000000.0)) {
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
		sscanf(buf, "%d.%d.%d", &deposit.date.day, &deposit.date.month, &deposit.date.year);
		if (deposit.date.month < 1 || deposit.date.month > 12 || deposit.date.day < 1
		|| deposit.date.day > month_days(deposit.date.month, year_days(deposit.date.year)) || 
		deposit.date.year < 1900 || deposit.date.year > 2100) {
			return 6;
		}
	}

	GtkListBoxRow* selected = gtk_list_box_get_selected_row(GTK_LIST_BOX(deposit.pay_type_box));
	if (!selected) {
		return 7;
	}
	deposit.pay_type = gtk_list_box_row_get_index(selected);

	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(deposit.capital_box))) {
		deposit.capital = 1;
	} else {
		deposit.capital = 0;
	}

    return 0;
}

int deposit_fun(char *result) {
	//
	//
	return 0;
}

void deposit_calculate(GtkButton *button, gpointer data) {
    GtkTextBuffer *empty_buffer = gtk_text_buffer_new(NULL);
	char empty_text[1] = {0};
	gtk_text_buffer_set_text(empty_buffer, empty_text, strlen(empty_text));
	gtk_text_view_set_buffer(GTK_TEXT_VIEW(deposit.text.result_box), empty_buffer);

	int code = check_input_deposit();
	if (code == 1) {
		gtk_label_set_text(GTK_LABEL(deposit.message), "Please fill all entries");
	} else if (code == 2) {
		gtk_label_set_text(GTK_LABEL(deposit.message), "Incorrect symbols in values");
	} else if (code == 3) {
		gtk_label_set_text(GTK_LABEL(deposit.message), "Deposit amount must be >0 and <=1000000000000 RUB");
	} else if (code == 4) {
		gtk_label_set_text(GTK_LABEL(deposit.message), "Term must be >=1 and <=240 mounth");
	} else if (code == 5) {
		gtk_label_set_text(GTK_LABEL(deposit.message), "Rates must be an integer >0 and <=99 \%");
	} else if (code == 6) {
		gtk_label_set_text(GTK_LABEL(deposit.message), "Incorrect date of start of term");
	} else if (code == 7) {
		gtk_label_set_text(GTK_LABEL(deposit.message), "Please choose periodicity of payments");
	} else if (code == 0) {
		gtk_label_set_text(GTK_LABEL(deposit.message), "");
		char result_text[BUFFER_LENGTH] = {0};
		code = deposit_fun(result_text);
		if (code) {
			gtk_label_set_text(GTK_LABEL(deposit.message), "Invalid list entry");
		} else {
			GtkTextBuffer *result_buffer = gtk_text_buffer_new(NULL);
			gtk_text_buffer_set_text(result_buffer, result_text, strlen(result_text));
			gtk_text_view_set_buffer(GTK_TEXT_VIEW(deposit.text.result_box), result_buffer);
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
	gtk_text_view_set_buffer(GTK_TEXT_VIEW(deposit.text.result_box), empty_buffer_1);
	gtk_text_view_set_buffer(GTK_TEXT_VIEW(deposit.text.in_box), empty_buffer_2);
	gtk_text_view_set_buffer(GTK_TEXT_VIEW(deposit.text.out_box), empty_buffer_3);
	//
}
