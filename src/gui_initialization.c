#include "gui_utils.h"

void initialize_buttons(s21_numpad *numpad) {
	
	numpad->button[0] = gtk_button_new_with_label("C");
	numpad->button[1] = gtk_button_new_with_label("(");
	numpad->button[2] = gtk_button_new_with_label(")");
	numpad->button[3] = gtk_button_new_with_label("^");
	numpad->button[4] = gtk_button_new_with_label("x");
	numpad->button[5] = gtk_button_new_with_label("sqrt");
	numpad->button[6] = gtk_button_new_with_label("7");
	numpad->button[7] = gtk_button_new_with_label("8");
	numpad->button[8] = gtk_button_new_with_label("9");
	numpad->button[9] = gtk_button_new_with_label("/");
	numpad->button[10] = gtk_button_new_with_label("log");
	numpad->button[11] = gtk_button_new_with_label("ln");
	numpad->button[12] = gtk_button_new_with_label("4");
	numpad->button[13] = gtk_button_new_with_label("5");
	numpad->button[14] = gtk_button_new_with_label("6");
	numpad->button[15] = gtk_button_new_with_label("*");
	numpad->button[16] = gtk_button_new_with_label("cos");
	numpad->button[17] = gtk_button_new_with_label("acos");
	numpad->button[18] = gtk_button_new_with_label("1");
	numpad->button[19] = gtk_button_new_with_label("2");
	numpad->button[20] = gtk_button_new_with_label("3");
	numpad->button[21] = gtk_button_new_with_label("-");
	numpad->button[22] = gtk_button_new_with_label("sin");
	numpad->button[23] = gtk_button_new_with_label("asin");
	numpad->button[24] = gtk_button_new_with_label("0");
	numpad->button[25] = gtk_button_new_with_label(".");
	numpad->button[26] = gtk_button_new_with_label("mod");
	numpad->button[27] = gtk_button_new_with_label("+");
	numpad->button[28] = gtk_button_new_with_label("tan");
	numpad->button[29] = gtk_button_new_with_label("atan");
	numpad->button[30] = gtk_button_new_with_label("=");
	numpad->button[31] = gtk_button_new_with_label("clear X");
	numpad->button[32] = gtk_button_new_with_label("clear Y");

	for (int i = 0; i < 33; i++) {
		gtk_widget_set_can_focus(numpad->button[i], FALSE);
	}

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 6; j++) {
			gtk_grid_attach(GTK_GRID(numpad->grid), numpad->button[i*6 + j], j, i+1, 1, 1);
		}
	}

	gtk_grid_attach(GTK_GRID(numpad->grid), numpad->button[30], 6, 1, 1, 5);
}

void initialize_upper_grid(GtkWidget *upper_grid, s21_limits *limits) {

	GtkWidget *x1_label = gtk_label_new("X ∈ [ ");
	GtkWidget *x2_label = gtk_label_new(" ]       ");
    GtkWidget *delim1 = gtk_label_new(" ; ");
	GtkWidget *y1_label = gtk_label_new("Y ∈ [ ");
	GtkWidget *y2_label = gtk_label_new(" ]       ");
    GtkWidget *delim2 = gtk_label_new(" ; ");
	gtk_label_set_xalign(GTK_LABEL(x2_label), 0.1);
	gtk_label_set_xalign(GTK_LABEL(y2_label), 0.1);
	gtk_label_set_xalign(GTK_LABEL(x1_label), 0.9);
	gtk_label_set_xalign(GTK_LABEL(y1_label), 0.9);
	gtk_entry_set_max_length(GTK_ENTRY(limits->x1_box), MAX_USER_LEN);
	gtk_entry_set_max_length(GTK_ENTRY(limits->x2_box), MAX_USER_LEN);
	gtk_entry_set_max_length(GTK_ENTRY(limits->y1_box), MAX_USER_LEN);
	gtk_entry_set_max_length(GTK_ENTRY(limits->y2_box), MAX_USER_LEN);
	gtk_entry_set_width_chars(GTK_ENTRY(limits->x1_box), 8);
	gtk_entry_set_width_chars(GTK_ENTRY(limits->x2_box), 8);
	gtk_entry_set_width_chars(GTK_ENTRY(limits->y1_box), 8);
	gtk_entry_set_width_chars(GTK_ENTRY(limits->y2_box), 8);
	
	gtk_grid_attach(GTK_GRID(upper_grid), x1_label, 0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(upper_grid), limits->x1_box, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(upper_grid), delim2, 2, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(upper_grid), limits->x2_box, 3, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(upper_grid), x2_label, 4, 0, 1, 1);

	gtk_grid_attach(GTK_GRID(upper_grid), y1_label, 0, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(upper_grid), limits->y1_box, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(upper_grid), delim1, 2, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(upper_grid), limits->y2_box, 3, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(upper_grid), y2_label, 4, 1, 1, 1);

	GtkWidget *space = gtk_label_new("             ");
	gtk_grid_attach(GTK_GRID(upper_grid), space, 6, 0, 1, 2);

}
