#include "gui_utils.h"

/*
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
*/

int check_input_deposit() {
    return 0;
}

void deposit_calculate(GtkButton *button, gpointer data) {
    //
}

void deposit_reset(GtkButton *button, gpointer data) {
    //
}
