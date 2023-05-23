#include "lib/s21_smart_calc.h"
#include "gui_utils.h"
#include "lib/s21_notation.h"

GtkDrawingArea *drawingArea;
cairo_t *cr;
GtkWidget *label;
GtkWidget *image;
GtkWidget *box;
s21_limits limits;
s21_numpad numpad;
char output_buffer[MAX_LEN] = {0};
char input_buffer[MAX_LEN] = {0};
char expr[MAX_LEN] = {0};

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

int parse_number(char *str, double *result) {
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
	if (parse_number(lims->x1_buffer, &lims->x1) || parse_number(lims->x2_buffer, &lims->x2) ||
		parse_number(lims->y1_buffer, &lims->y1) || parse_number(lims->y2_buffer, &lims->y2)) {
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

int make_plot(char *expr) {
	GdkWindow *window = gtk_widget_get_window((GtkWidget*)drawingArea);
	cairo_region_t *cairoRegion = cairo_region_create();
	GdkDrawingContext *drawingContext = gdk_window_begin_draw_frame (window, cairoRegion);
	cr = gdk_drawing_context_get_cairo_context (drawingContext);

	fill_area(cr);
	draw_border(cr);

	if (expr[0]) {
		double min = 0, max = 0;
		draw_axis(cr, limits);
		draw_plot(cr, limits, expr, &min, &max);

		if (limits.y1 > max || limits.y2 < min) {
			char buf[MAX_LEN] = {0};
			sprintf(buf, "Graph is not visible, y ∈ [ %g ; %g ]", min, max);
			gtk_label_set_text(GTK_LABEL(label), buf);
		}
	}

	gdk_window_end_draw_frame(window, drawingContext);
	cairo_region_destroy(cairoRegion);

	return 0;
}

void calculate(GtkButton *button, gpointer data) {
	
	double result = 0;
	const gchar* text = gtk_button_get_label(button);
	gtk_label_set_text(GTK_LABEL(label), "");

	memset(expr, 0, MAX_LEN);
	//make_plot(expr);

	if (strcmp("=", text) == 0) {

		memset(input_buffer, 0, MAX_LEN);
		memset(output_buffer, 0, MAX_LEN);
		//memset(expr, 0, MAX_LEN);
		//make_plot(expr);
		strcat(input_buffer, gtk_entry_get_text(GTK_ENTRY(box)));
		//gtk_entry_set_text(GTK_ENTRY(box), "");

		int code = notation_convert(input_buffer, expr);
		if (!code) {
			if (strchr(input_buffer, 'x')) {
				code += check_limits(&limits);
				if (code == 0) {
					code += check_x_count(expr);
					if (code == 0) {
						char buf[MAX_LEN] = {0};
						double x_scale = fabs(limits.x1 - limits.x2) / (double)(DISPLAY_WIDTH);
						double y_scale = fabs(limits.y1 - limits.y2) / (double)(DISPLAY_HEIGHT);
						sprintf(buf, "X   1 : %g      Y   1 : %g", x_scale, y_scale);
						gtk_label_set_text(GTK_LABEL(label), buf);
						make_plot(expr);
					} else {
						gtk_label_set_text(GTK_LABEL(label), "No more than 15 'x' please");
					}
					
				} else {
					if (code == 1) {
						gtk_label_set_text(GTK_LABEL(label), "Please specify limits");
					} else if (code == 2) {
						gtk_label_set_text(GTK_LABEL(label), "Values in limits must be from -1000000 to 1000000");
					} else if (code == 4) {
						gtk_label_set_text(GTK_LABEL(label), "Values in limits must be different");
					} else {
						gtk_label_set_text(GTK_LABEL(label), "Incorrect symbols in limits");
					}
				}
			} else {
				code += calculation(expr, &result);
				memset(expr, 0, MAX_LEN);
				if (!code) {
					sprintf(output_buffer, "%.7lf", result);
					char *dot_point = strchr(output_buffer, ',');
					if (dot_point) *dot_point = '.';
					gtk_entry_set_text(GTK_ENTRY(box), output_buffer);
				} else {
					gtk_label_set_text(GTK_LABEL(label), "Answer is not defined");
				}
			}
		} else {
			gtk_label_set_text(GTK_LABEL(label), "Incorrect expression");
		}

	} else if (strcmp("C", text) == 0) {
		//memset(expr, 0, MAX_LEN);
		//make_plot(expr);
		gtk_entry_set_text(GTK_ENTRY(box), "");

	} else if (strcmp("clear X", text) == 0) {
		gtk_entry_set_text(GTK_ENTRY(limits.x1_box), "");
		gtk_entry_set_text(GTK_ENTRY(limits.x2_box), "");

	} else if (strcmp("clear Y", text) == 0) {
		gtk_entry_set_text(GTK_ENTRY(limits.y1_box), "");
		gtk_entry_set_text(GTK_ENTRY(limits.y2_box), "");

	} else {
		GtkWidget *box_ptr = gtk_window_get_focus(GTK_WINDOW(numpad.window));
		guint position = gtk_editable_get_position(GTK_EDITABLE(box_ptr));
		gtk_entry_buffer_insert_text(gtk_entry_get_buffer(GTK_ENTRY(box_ptr)), position, text, -1);
		gtk_editable_set_position(GTK_EDITABLE(box_ptr), position + strlen(text));

	}
}

gboolean on_draw (GtkWidget *widget, GdkEventExpose *event, gpointer data) {
	make_plot(expr);
	//if (cr) cairo_restore(cr);
	return FALSE;
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
	gtk_notebook_append_page(GTK_NOTEBOOK(numpad.notebook), numpad.grid, NULL);
	gtk_container_add(GTK_CONTAINER(numpad.window), numpad.notebook);

	// создание окна ввода выражения и вывода результата
  	box = gtk_entry_new();
	gtk_entry_set_max_length(GTK_ENTRY(box), MAX_USER_LEN);
  	gtk_grid_attach(GTK_GRID(numpad.grid), box, 0, 0, 7, 1);

	// создание кнопок
	initialize_buttons(&numpad);
	for (int i = 0; i < 33; i++) {
		g_signal_connect(numpad.button[i],"clicked",G_CALLBACK(calculate), NULL);
	}
  	
	// создание области для отрисовки графиков
	drawingArea = (GtkDrawingArea*) gtk_drawing_area_new();
	gtk_widget_set_size_request ((GtkWidget*)drawingArea, DISPLAY_WIDTH, DISPLAY_HEIGHT);
    g_signal_connect((GtkWidget*)drawingArea, "draw", G_CALLBACK(on_draw), NULL);
	gtk_grid_insert_row(GTK_GRID(numpad.grid), 0);
	gtk_grid_attach(GTK_GRID(numpad.grid), (GtkWidget*)drawingArea, 0, 0, 7, 1);

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
	label = gtk_label_new("");
	gtk_label_set_xalign(GTK_LABEL(label), 0.0);
	gtk_grid_attach(GTK_GRID(upper_grid), label, 0, 3, 7, 1);

	// создание картинки
	image = gtk_image_new_from_file("images/kitty2.png");
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

	gtk_label_set_text(GTK_LABEL(label), "Welcome to the Smart Calc by weaveryu!");
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
