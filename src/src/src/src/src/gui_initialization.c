#include "gui_utils.h"

gboolean on_draw (GtkWidget *widget, GdkEventExpose *event, gpointer data) {
	make_plot();
    memset(numpad.expr, 0, MAX_LEN);
	return FALSE;
}

void calculator_initialization() {
    // создание окна ввода выражения и вывода результата
  	numpad.box = gtk_entry_new();
	gtk_entry_set_max_length(GTK_ENTRY(numpad.box), MAX_USER_LEN);
  	gtk_grid_attach(GTK_GRID(numpad.grid), numpad.box, 0, 0, 7, 1);

	// создание кнопок
	numpad.button[0] = gtk_button_new_with_label("C");
	numpad.button[1] = gtk_button_new_with_label("(");
	numpad.button[2] = gtk_button_new_with_label(")");
	numpad.button[3] = gtk_button_new_with_label("^");
	numpad.button[4] = gtk_button_new_with_label("x");
	numpad.button[5] = gtk_button_new_with_label("sqrt");
	numpad.button[6] = gtk_button_new_with_label("7");
	numpad.button[7] = gtk_button_new_with_label("8");
	numpad.button[8] = gtk_button_new_with_label("9");
	numpad.button[9] = gtk_button_new_with_label("/");
	numpad.button[10] = gtk_button_new_with_label("log");
	numpad.button[11] = gtk_button_new_with_label("ln");
	numpad.button[12] = gtk_button_new_with_label("4");
	numpad.button[13] = gtk_button_new_with_label("5");
	numpad.button[14] = gtk_button_new_with_label("6");
	numpad.button[15] = gtk_button_new_with_label("*");
	numpad.button[16] = gtk_button_new_with_label("cos");
	numpad.button[17] = gtk_button_new_with_label("acos");
	numpad.button[18] = gtk_button_new_with_label("1");
	numpad.button[19] = gtk_button_new_with_label("2");
	numpad.button[20] = gtk_button_new_with_label("3");
	numpad.button[21] = gtk_button_new_with_label("-");
	numpad.button[22] = gtk_button_new_with_label("sin");
	numpad.button[23] = gtk_button_new_with_label("asin");
	numpad.button[24] = gtk_button_new_with_label("0");
	numpad.button[25] = gtk_button_new_with_label(".");
	numpad.button[26] = gtk_button_new_with_label("mod");
	numpad.button[27] = gtk_button_new_with_label("+");
	numpad.button[28] = gtk_button_new_with_label("tan");
	numpad.button[29] = gtk_button_new_with_label("atan");
	numpad.button[30] = gtk_button_new_with_label("=");
	numpad.button[31] = gtk_button_new_with_label("clear X");
	numpad.button[32] = gtk_button_new_with_label("clear Y");

	for (int i = 0; i < 33; i++) {
		gtk_widget_set_can_focus(numpad.button[i], FALSE);
	}

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 6; j++) {
			gtk_grid_attach(GTK_GRID(numpad.grid), numpad.button[i*6 + j], j, i+1, 1, 1);
		}
	}

	gtk_grid_attach(GTK_GRID(numpad.grid), numpad.button[30], 6, 1, 1, 5);
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
	gtk_entry_set_max_length(GTK_ENTRY(limits.x1_box), MAX_USER_LEN);
	gtk_entry_set_max_length(GTK_ENTRY(limits.x2_box), MAX_USER_LEN);
	gtk_entry_set_max_length(GTK_ENTRY(limits.y1_box), MAX_USER_LEN);
	gtk_entry_set_max_length(GTK_ENTRY(limits.y2_box), MAX_USER_LEN);
	gtk_entry_set_width_chars(GTK_ENTRY(limits.x1_box), 8);
	gtk_entry_set_width_chars(GTK_ENTRY(limits.x2_box), 8);
	gtk_entry_set_width_chars(GTK_ENTRY(limits.y1_box), 8);
	gtk_entry_set_width_chars(GTK_ENTRY(limits.y2_box), 8);
	
	gtk_grid_attach(GTK_GRID(upper_grid), x1_label, 0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(upper_grid), limits.x1_box, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(upper_grid), delim2, 2, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(upper_grid), limits.x2_box, 3, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(upper_grid), x2_label, 4, 0, 1, 1);

	gtk_grid_attach(GTK_GRID(upper_grid), y1_label, 0, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(upper_grid), limits.y1_box, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(upper_grid), delim1, 2, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(upper_grid), limits.y2_box, 3, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(upper_grid), y2_label, 4, 1, 1, 1);

	GtkWidget *space = gtk_label_new("             ");
	gtk_grid_attach(GTK_GRID(upper_grid), space, 6, 0, 1, 2);
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

void credit_initialization() {
    GtkWidget *space;
    credit.entry_amount = gtk_entry_new();
	credit.entry_rate = gtk_entry_new();
	credit.entry_term = gtk_entry_new();
	credit.label[0] = gtk_label_new("Credit amount");
	credit.label[1] = gtk_label_new("Credit term");
	credit.label[2] = gtk_label_new("Interest rate");
	credit.label[3] = gtk_label_new(" RUB");
	credit.label[4] = gtk_label_new(" Months");
	credit.label[5] = gtk_label_new(" \%");
	credit.label[6] = gtk_label_new("Type of monthly payments  ");
	credit.message = gtk_label_new("");
	for (int i = 0; i < 7; i++) {
		gtk_label_set_xalign(GTK_LABEL(credit.label[i]), 0.0);
	}
	gtk_label_set_xalign(GTK_LABEL(credit.message), 0.0);

	credit.text_wall = gtk_text_view_new();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(credit.text_wall), FALSE);

	credit.scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_container_add (GTK_CONTAINER(credit.scrolled_window), credit.text_wall);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(credit.scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

	gtk_widget_set_size_request(credit.scrolled_window, APP_WIDTH - BORDER_SIZE*2, APP_HEIGHT*3/4-BORDER_SIZE);

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
	gtk_grid_attach(GTK_GRID(credit.grid), credit.scrolled_window, 0, 7, 3, 1);

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
	
	gtk_grid_insert_row(GTK_GRID(credit.grid), 7);
	space = gtk_label_new("");
	gtk_grid_attach(GTK_GRID(credit.grid), space, 0, 7, 1, 1);
	gtk_grid_insert_row(GTK_GRID(credit.grid), 6);
	space = gtk_label_new("");
	gtk_grid_attach(GTK_GRID(credit.grid), space, 0, 6, 1, 1);
	gtk_grid_insert_row(GTK_GRID(credit.grid), 5);
	space = gtk_label_new("");
	gtk_grid_attach(GTK_GRID(credit.grid), space, 0, 5, 1, 1);
	gtk_grid_insert_row(GTK_GRID(credit.grid), 3);
	space = gtk_label_new("");
	gtk_grid_attach(GTK_GRID(credit.grid), space, 0, 3, 1, 1);
	gtk_grid_insert_row(GTK_GRID(credit.grid), 0);
	space = gtk_label_new("");
	gtk_grid_attach(GTK_GRID(credit.grid), space, 0, 0, 1, 1);

	GtkWidget *image = gtk_image_new_from_file("images/kitty1.png");
    gtk_grid_attach(GTK_GRID(credit.grid), image, 2, 5, 1, 5);
}

void deposit_initialization() {
    GtkWidget *space;

    deposit.entry.amount = gtk_entry_new();
	deposit.entry.date = gtk_entry_new();
    deposit.entry.rate = gtk_entry_new();
    deposit.entry.tax = gtk_entry_new();
    deposit.entry.term = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(deposit.grid), deposit.entry.amount, 1, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(deposit.grid), deposit.entry.date, 1, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(deposit.grid), deposit.entry.term, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(deposit.grid), deposit.entry.rate, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(deposit.grid), deposit.entry.tax, 1, 4, 1, 1);

    deposit.label = gtk_label_new("Deposit amount");
    gtk_label_set_xalign(GTK_LABEL(deposit.label), 0.0);
    gtk_grid_attach(GTK_GRID(deposit.grid), deposit.label, 0, 0, 1, 1);
	deposit.label = gtk_label_new("Start of term");
    gtk_label_set_xalign(GTK_LABEL(deposit.label), 0.0);
    gtk_grid_attach(GTK_GRID(deposit.grid), deposit.label, 0, 1, 1, 1);
    deposit.label = gtk_label_new("Deposit term");
    gtk_label_set_xalign(GTK_LABEL(deposit.label), 0.0);
    gtk_grid_attach(GTK_GRID(deposit.grid), deposit.label, 0, 2, 1, 1);
    deposit.label = gtk_label_new("Interest rate");
    gtk_label_set_xalign(GTK_LABEL(deposit.label), 0.0);
    gtk_grid_attach(GTK_GRID(deposit.grid), deposit.label, 0, 3, 1, 1);
    deposit.label = gtk_label_new("Key rate");
    gtk_label_set_xalign(GTK_LABEL(deposit.label), 0.0);
    gtk_grid_attach(GTK_GRID(deposit.grid), deposit.label, 0, 4, 1, 1);
    deposit.label = gtk_label_new("Periodicity of payments    ");
    gtk_label_set_xalign(GTK_LABEL(deposit.label), 0.0);
    gtk_label_set_yalign(GTK_LABEL(deposit.label), 0.0);
    gtk_grid_attach(GTK_GRID(deposit.grid), deposit.label, 0, 5, 1, 1);

    deposit.label = gtk_label_new(" RUB");
    gtk_label_set_xalign(GTK_LABEL(deposit.label), 0.0);
    gtk_grid_attach(GTK_GRID(deposit.grid), deposit.label, 2, 0, 1, 1);
	deposit.label = gtk_label_new(" dd.mm.yyyy");
    gtk_label_set_xalign(GTK_LABEL(deposit.label), 0.0);
    gtk_grid_attach(GTK_GRID(deposit.grid), deposit.label, 2, 1, 1, 1);
    deposit.label = gtk_label_new(" Months");
    gtk_label_set_xalign(GTK_LABEL(deposit.label), 0.0);
    gtk_grid_attach(GTK_GRID(deposit.grid), deposit.label, 2, 2, 1, 1);
    deposit.label = gtk_label_new(" \%");
    gtk_label_set_xalign(GTK_LABEL(deposit.label), 0.0);
    gtk_grid_attach(GTK_GRID(deposit.grid), deposit.label, 2, 3, 1, 1);
    deposit.label = gtk_label_new(" \%");
    gtk_label_set_xalign(GTK_LABEL(deposit.label), 0.0);
    gtk_grid_attach(GTK_GRID(deposit.grid), deposit.label, 2, 4, 1, 1);

    deposit.pay_type_box = gtk_list_box_new();

    deposit.label = gtk_label_new("end of term");
    gtk_label_set_xalign(GTK_LABEL(deposit.label), 0.0);
    gtk_list_box_prepend(GTK_LIST_BOX(deposit.pay_type_box), deposit.label);
    deposit.label = gtk_label_new("year");
    gtk_label_set_xalign(GTK_LABEL(deposit.label), 0.0);
    gtk_list_box_prepend(GTK_LIST_BOX(deposit.pay_type_box), deposit.label);
    deposit.label = gtk_label_new("half-year");
    gtk_label_set_xalign(GTK_LABEL(deposit.label), 0.0);
    gtk_list_box_prepend(GTK_LIST_BOX(deposit.pay_type_box), deposit.label);
    deposit.label = gtk_label_new("quarter");
    gtk_label_set_xalign(GTK_LABEL(deposit.label), 0.0);
    gtk_list_box_prepend(GTK_LIST_BOX(deposit.pay_type_box), deposit.label);
    deposit.label = gtk_label_new("month");
    gtk_label_set_xalign(GTK_LABEL(deposit.label), 0.0);
    gtk_list_box_prepend(GTK_LIST_BOX(deposit.pay_type_box), deposit.label);

    gtk_grid_attach(GTK_GRID(deposit.grid), deposit.pay_type_box, 1, 5, 1, 1);

    deposit.capital_box = gtk_check_button_new_with_label("capitalization");
    gtk_grid_attach(GTK_GRID(deposit.grid), deposit.capital_box, 1, 6, 1, 1);

    deposit.label = gtk_label_new("Replenishment list ->");
    gtk_label_set_xalign(GTK_LABEL(deposit.label), 0.0);
    gtk_label_set_yalign(GTK_LABEL(deposit.label), 0.0);
    gtk_grid_attach(GTK_GRID(deposit.grid), deposit.label, 0, 7, 1, 1);
    deposit.label = gtk_label_new("Withdrawals list ->");
    gtk_label_set_xalign(GTK_LABEL(deposit.label), 0.0);
    gtk_label_set_yalign(GTK_LABEL(deposit.label), 0.0);
    gtk_grid_attach(GTK_GRID(deposit.grid), deposit.label, 0, 8, 1, 1);

    GtkWidget *in_window = gtk_scrolled_window_new(NULL, NULL);
    deposit.text.in_box = gtk_text_view_new();
	gtk_container_add (GTK_CONTAINER(in_window), deposit.text.in_box);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(in_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_widget_set_size_request(in_window, APP_WIDTH/2, APP_HEIGHT/6);
    gtk_grid_attach(GTK_GRID(deposit.grid), in_window, 1, 7, 2, 1);
    
    GtkWidget *out_window = gtk_scrolled_window_new(NULL, NULL);
    deposit.text.out_box = gtk_text_view_new();
	gtk_container_add (GTK_CONTAINER(out_window), deposit.text.out_box);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(out_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_widget_set_size_request(out_window, APP_WIDTH/2, APP_HEIGHT/6);
    gtk_grid_attach(GTK_GRID(deposit.grid), out_window, 1, 8, 2, 1);

    deposit.message = gtk_label_new("The format of list entries is: 'dd.mm.yyyy  summ'");
    gtk_label_set_xalign(GTK_LABEL(deposit.message), 0.0);
    gtk_grid_attach(GTK_GRID(deposit.grid), deposit.message, 0, 10, 4, 1);

    deposit.button[0] = gtk_button_new_with_label("Reset");
	deposit.button[1] = gtk_button_new_with_label("Calculate");
    g_signal_connect(deposit.button[0], "clicked", G_CALLBACK(deposit_reset), NULL);
	g_signal_connect(deposit.button[1], "clicked", G_CALLBACK(deposit_calculate), NULL);
    gtk_grid_attach(GTK_GRID(deposit.grid), deposit.button[0], 0, 9, 1, 1);
    gtk_grid_attach(GTK_GRID(deposit.grid), deposit.button[1], 1, 9, 1, 1);

    GtkWidget *result_window = gtk_scrolled_window_new(NULL, NULL);
    deposit.text.result_box = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(deposit.text.result_box), FALSE);
	gtk_container_add (GTK_CONTAINER(result_window), deposit.text.result_box);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(result_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_widget_set_size_request(result_window, APP_WIDTH - BORDER_SIZE*2, APP_HEIGHT/8);
    gtk_grid_attach(GTK_GRID(deposit.grid), result_window, 0, 11, 4, 1);

	GtkWidget *image = gtk_image_new_from_file("images/kitty3.png");
    gtk_grid_attach(GTK_GRID(deposit.grid), image, 2, 5, 1, 2);

	gtk_grid_insert_row(GTK_GRID(deposit.grid), 10);
	space = gtk_label_new("");
	gtk_grid_attach(GTK_GRID(deposit.grid), space, 0, 10, 1, 1);
	gtk_grid_insert_row(GTK_GRID(deposit.grid), 6);
	space = gtk_label_new("");
	gtk_grid_attach(GTK_GRID(deposit.grid), space, 0, 6, 1, 1);
	gtk_grid_insert_row(GTK_GRID(deposit.grid), 5);
	space = gtk_label_new("");
	gtk_grid_attach(GTK_GRID(deposit.grid), space, 0, 5, 1, 1);
	gtk_grid_insert_row(GTK_GRID(deposit.grid), 0);
	space = gtk_label_new("");
	gtk_grid_attach(GTK_GRID(deposit.grid), space, 0, 0, 1, 1);
	gtk_grid_insert_row(GTK_GRID(deposit.grid), 10);
	space = gtk_label_new("");
	gtk_grid_attach(GTK_GRID(deposit.grid), space, 0, 10, 1, 1);
	
}