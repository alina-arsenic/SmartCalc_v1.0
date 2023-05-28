#include "lib/s21_smart_calc.h"
#include "gui_utils.h"
#include "lib/s21_notation.h"

s21_limits limits;
s21_numpad numpad;
s21_credit credit;

int parse_double(char *str, double *result) {
	int i = 0, dot = 0;
	if (str[i] == '-') i++;
	while (is_digit(str[i])) i++;
	if (str[i] == '.' && is_digit(str[i+1])) {
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

int parse_integer(char *str, int *result) {
	int i = 0;
	if (str[i] == '-') i++;
	while (is_digit(str[i])) i++;
	if (i != strlen(str)) {
		return 1;
	}
    sscanf(str, "%d", result);
	return 0;
}

void check_box_1(GtkButton *button, gpointer data) {
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(credit.button[2]))) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(credit.button[3]), FALSE);
	}
	
}

void check_box_2(GtkButton *button, gpointer data) {
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(credit.button[3]))) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(credit.button[2]), FALSE);
	}
}

int check_input() {
	char buf[MAX_LEN] = {0};
	strcat(buf, gtk_entry_get_text(GTK_ENTRY(credit.entry_amount)));
	if (buf[0] == '\0') {
		return 1;
	} else if (parse_double(buf, &credit.value_amount)) {
		return 2;
	} else if ((credit.value_amount <= 0.0) || (credit.value_amount > 1000000000000.0)) {
		return 3;
	}
	memset(buf, 0, MAX_LEN);
	strcat(buf, gtk_entry_get_text(GTK_ENTRY(credit.entry_term)));
	if (buf[0] == '\0') {
		return 1;
	} else if (parse_integer(buf, &credit.value_term)) {
		return 2;
	} else if ((credit.value_term < 1) || (credit.value_term > 600)) {
		return 4;
	}
	memset(buf, 0, MAX_LEN);
	strcat(buf, gtk_entry_get_text(GTK_ENTRY(credit.entry_rate)));
	if (buf[0] == '\0') {
		return 1;
	} else if (parse_double(buf, &credit.value_rate)) {
		return 2;
	} else if ((credit.value_rate <= 0.0) || (credit.value_rate > 999.0)) {
		return 5;
	}
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(credit.button[2])) == FALSE
	 && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(credit.button[3])) == FALSE) {
		return 6;
	}
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(credit.button[2]))) {
		credit.credit_type = 1;
	}
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(credit.button[3]))) {
		credit.credit_type = 2;
	}
	return 0;
}

void credit_calculate(GtkButton *button, gpointer data) {
	int code = check_input();
	if (code == 1) {
		gtk_label_set_text(GTK_LABEL(credit.message), "Please fill all entries");
	} else if (code == 2) {
		gtk_label_set_text(GTK_LABEL(credit.message), "Incorrect symbols in values");
	} else if (code == 3) {
		gtk_label_set_text(GTK_LABEL(credit.message), "Credit amount must be >0 and <=1000000000000 RUB");
	} else if (code == 4) {
		gtk_label_set_text(GTK_LABEL(credit.message), "Term must be >=1 and <=600 mounth");
	} else if (code == 5) {
		gtk_label_set_text(GTK_LABEL(credit.message), "Interest rate must be >0 and <=999 \%");
	} else if (code == 6) {
		gtk_label_set_text(GTK_LABEL(credit.message), "Please choose type of monthly payments");
	} else if (code == 0) {
		gtk_label_set_text(GTK_LABEL(credit.message), "");
		//
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
}

void activate(GtkApplication *app, gpointer user_data) {

	GtkWidget *space;
	
	// создание окна приложения
	numpad.window = gtk_application_window_new(app);
	gtk_window_set_position(GTK_WINDOW(numpad.window), GTK_WIN_POS_CENTER);
	gtk_window_set_title(GTK_WINDOW(numpad.window), "Smart Calculator");
	gtk_window_set_default_size(GTK_WINDOW(numpad.window), APP_WIDTH, APP_HEIGHT);
	gtk_container_set_border_width(GTK_CONTAINER(numpad.window), BORDER_SIZE);

	// создание решетки-каркаса для виджетов
	numpad.notebook = gtk_notebook_new();
	gtk_notebook_set_show_border(GTK_NOTEBOOK(numpad.notebook), FALSE);
	numpad.grid = gtk_grid_new();
	GtkWidget *notebook_label = gtk_label_new("Math");
	gtk_notebook_append_page(GTK_NOTEBOOK(numpad.notebook), numpad.grid, notebook_label);
	gtk_container_add(GTK_CONTAINER(numpad.window), numpad.notebook);

	credit.grid = gtk_grid_new();
	notebook_label = gtk_label_new("Credit");
	gtk_notebook_append_page(GTK_NOTEBOOK(numpad.notebook), credit.grid, notebook_label);
	credit.entry_amount = gtk_entry_new();
	credit.entry_rate = gtk_entry_new();
	credit.entry_term = gtk_entry_new();
	credit.label[0] = gtk_label_new("Credit amount");
	credit.label[1] = gtk_label_new("Credit term");
	credit.label[2] = gtk_label_new("Interest rate");
	credit.label[3] = gtk_label_new("RUB");
	credit.label[4] = gtk_label_new("Months");
	credit.label[5] = gtk_label_new("\%");
	credit.label[6] = gtk_label_new("Type of monthly payments");
	credit.message = gtk_label_new("Message");
	for (int i = 0; i < 7; i++) {
		gtk_label_set_xalign(GTK_LABEL(credit.label[i]), 0.0);
	}
	gtk_label_set_xalign(GTK_LABEL(credit.message), 0.0);
	credit.text_wall = gtk_text_view_new();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(credit.text_wall), FALSE);
	gtk_grid_attach(GTK_GRID(credit.grid), credit.label[0], 0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(credit.grid), credit.label[1], 0, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(credit.grid), credit.label[2], 0, 2, 1, 1);
	gtk_grid_attach(GTK_GRID(credit.grid), credit.entry_amount, 1, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(credit.grid), credit.entry_term, 1, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(credit.grid), credit.entry_rate, 1, 2, 1, 1);
	gtk_grid_attach(GTK_GRID(credit.grid), credit.label[3], 2, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(credit.grid), credit.label[4], 2, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(credit.grid), credit.label[5], 2, 2, 1, 1);
	gtk_grid_attach(GTK_GRID(credit.grid), credit.label[6], 0, 3, 1, 1);
	gtk_grid_attach(GTK_GRID(credit.grid), credit.message, 0, 6, 4, 1);
	gtk_grid_attach(GTK_GRID(credit.grid), credit.text_wall, 0, 7, 1, 1);

	credit.button[0] = gtk_button_new_with_label("Calculate");
	credit.button[1] = gtk_button_new_with_label("Reset");
	credit.button[2] = gtk_check_button_new_with_label("annuity");
	credit.button[3] = gtk_check_button_new_with_label("differentiated");
	credit.credit_type = 0;
	g_signal_connect(credit.button[0], "clicked", G_CALLBACK(credit_calculate), NULL);
	g_signal_connect(credit.button[1], "clicked", G_CALLBACK(credit_reset), NULL);
	g_signal_connect(credit.button[2], "clicked", G_CALLBACK(check_box_1), NULL);
	g_signal_connect(credit.button[3], "clicked", G_CALLBACK(check_box_2), NULL);

	gtk_grid_attach(GTK_GRID(credit.grid), credit.button[0], 1, 5, 1, 1);
	gtk_grid_attach(GTK_GRID(credit.grid), credit.button[1], 0, 5, 1, 1);
	gtk_grid_attach(GTK_GRID(credit.grid), credit.button[2], 1, 3, 1, 1);
	gtk_grid_attach(GTK_GRID(credit.grid), credit.button[3], 1, 4, 1, 1);
	

	// создание окна ввода выражения и вывода результата
  	numpad.box = gtk_entry_new();
	gtk_entry_set_max_length(GTK_ENTRY(numpad.box), MAX_USER_LEN);
  	gtk_grid_attach(GTK_GRID(numpad.grid), numpad.box, 0, 0, 7, 1);

	// создание кнопок
	initialize_buttons(&numpad);
	for (int i = 0; i < 33; i++) {
		g_signal_connect(numpad.button[i],"clicked",G_CALLBACK(calculate), NULL);
	}
  	
	// создание области для отрисовки графиков
	numpad.drawingArea = (GtkDrawingArea*) gtk_drawing_area_new();
	gtk_widget_set_size_request ((GtkWidget*)numpad.drawingArea, DISPLAY_WIDTH, DISPLAY_HEIGHT);
	memset(numpad.expr, 0, MAX_LEN);
    g_signal_connect((GtkWidget*)numpad.drawingArea, "draw", G_CALLBACK(on_draw), NULL);
	gtk_grid_insert_row(GTK_GRID(numpad.grid), 0);
	gtk_grid_attach(GTK_GRID(numpad.grid), (GtkWidget*)numpad.drawingArea, 0, 0, 7, 1);

	// создание области с областью определения и областью значений
	limits.x1_box = gtk_entry_new();
	limits.x2_box = gtk_entry_new();
	limits.y1_box = gtk_entry_new();
	limits.y2_box = gtk_entry_new();
	GtkWidget *upper_grid = gtk_grid_new();
	initialize_upper_grid(upper_grid, &limits);
	gtk_grid_attach(GTK_GRID(upper_grid), numpad.button[31], 5, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(upper_grid), numpad.button[32], 5, 1, 1, 1);

	// создание области с текстовыми сообщениями
	space = gtk_label_new("");
	gtk_grid_attach(GTK_GRID(upper_grid), space, 0, 2, 7, 1);
	numpad.label = gtk_label_new("");
	gtk_label_set_xalign(GTK_LABEL(numpad.label), 0.0);
	gtk_grid_attach(GTK_GRID(upper_grid), numpad.label, 0, 3, 7, 1);

	// создание картинки
	GtkWidget *image = gtk_image_new_from_file("images/kitty2.png");
    gtk_grid_attach(GTK_GRID(upper_grid), image, 7, 0, 1, 4);

	// скрепление областей
	gtk_grid_insert_row(GTK_GRID(numpad.grid), 1);
	gtk_grid_attach(GTK_GRID(numpad.grid), upper_grid, 0, 1, 7, 1);
	gtk_grid_insert_row(GTK_GRID(numpad.grid), 2);
	space = gtk_label_new("");
	gtk_grid_attach(GTK_GRID(numpad.grid), space, 0, 2, 1, 1);
	gtk_grid_insert_row(GTK_GRID(numpad.grid), 1);
	space = gtk_label_new("");
	gtk_grid_attach(GTK_GRID(numpad.grid), space, 0, 1, 1, 1);

	gtk_label_set_text(GTK_LABEL(numpad.label), "Welcome to the Smart Calc by weaveryu!");
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
