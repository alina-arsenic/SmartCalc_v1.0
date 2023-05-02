#include "s21_smart_calc.h"

typedef struct {
  GtkWidget *window;
  GtkWidget *grid;
  GtkWidget *button[32];
} s21_numpad;

GtkWidget *box;

#define SIZE 10

char input_buffer[MAX_LEN] = {0};
char output_buffer[MAX_LEN] = {0};

char result[MAX_LEN] = {0};

static void calculate(GtkButton *button, gpointer data) {

	const gchar* text = gtk_button_get_label(button);

	if (strcmp("=", text) == 0) {

		gtk_entry_set_text(GTK_ENTRY(box), "");

		int code = notation_convert(input_buffer, result);
		if (!code) {
			sprintf(output_buffer, "%s", result);
		} else {
			sprintf(output_buffer, "Incorrect expression");
		}

		gtk_entry_set_text(GTK_ENTRY(box), output_buffer);

		memset(input_buffer, 0, strlen(input_buffer));
		memset(output_buffer, 0, strlen(output_buffer));

	} else if (strcmp("C", text) == 0) {
		gtk_entry_set_text(GTK_ENTRY(box), "");
		memset(input_buffer, 0, strlen(input_buffer));
		memset(output_buffer, 0, strlen(output_buffer));

	} else if (strcmp("<-", text) == 0) {
		//

	} else {
		strcat(input_buffer, text);
		strcat(output_buffer, text);
		gtk_entry_set_text(GTK_ENTRY(box), output_buffer);
	}
}

static void activate(GtkApplication *app, gpointer user_data) {

	s21_numpad numpad;
	numpad.window = gtk_application_window_new(app);

	gtk_window_set_position(GTK_WINDOW(numpad.window), GTK_WIN_POS_CENTER);
	gtk_window_set_title(GTK_WINDOW(numpad.window), "Smart Calculator");
	gtk_window_set_default_size(GTK_WINDOW(numpad.window), 600, 600);
	gtk_container_set_border_width(GTK_CONTAINER(numpad.window), 10);

	numpad.grid = gtk_grid_new();
	gtk_container_add(GTK_CONTAINER(numpad.window), numpad.grid);

  	box = gtk_entry_new();
	gtk_editable_set_editable(GTK_EDITABLE(box), FALSE);
  	gtk_grid_attach(GTK_GRID(numpad.grid), box, 0, 0, 7, 1);

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
	numpad.button[30] = gtk_button_new_with_label("<-");
	numpad.button[31] = gtk_button_new_with_label("=");

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 6; j++) {
			gtk_grid_attach(GTK_GRID(numpad.grid), numpad.button[i*6 + j], j, i+1, 1, 1);
		}
	}

	gtk_grid_attach(GTK_GRID(numpad.grid), numpad.button[30], 6, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(numpad.grid), numpad.button[31], 6, 2, 1, 4);

	for (int i = 0; i < 32; i++) {
		g_signal_connect(numpad.button[i],"clicked",G_CALLBACK(calculate), NULL);
	}
	
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
