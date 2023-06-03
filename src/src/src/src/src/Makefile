PLATFORM := $(shell uname)

FLAGS = `pkg-config --cflags gtk+-3.0`
LIBS = `pkg-config --libs gtk+-3.0` -lm

CONTROL_FLAGS = -std=c11 -Wall -Werror -Wextra
TEST_FLAGS = `pkg-config --cflags --libs check` -lc --coverage

OBJ = *.o
LIB_SRC = lib/*.c
GUI_SRC = gui_*.c
TEST_SRC = tests/*.c

INSTALL_DIR = ~
DISTRIBUTIVE_DIR = ~

ifeq ($(PLATFORM), Linux)
OPEN = xdg-open 
else
OPEN = open
endif

all: clean gcov_report install

dist: clean
	tar -cvzf $(DISTRIBUTIVE_DIR)/Calculator.tar.gz ../*

install: dist
	tar -xvf $(DISTRIBUTIVE_DIR)/Calculator.tar.gz && rm -rf $(DISTRIBUTIVE_DIR)/Calculator.tar.gz

	gcc $(FLAGS) -o app $(LIB_SRC) $(GUI_SRC) $(LIBS)
	install -D app $(INSTALL_DIR)/Calculator
	rm -rf app

uninstall:
	rm -rf Calculator

clean: uninstall
	rm -f *.o *.a *.gcda *.gcno *.info
	rm -fr gcov_report ./tests/all_tests*
	rm -f ./a.out
	rm -rf ./tests/*.o ./tests/*.a ./tests/*.gcda ./tests/*.gcno ./tests/*.info
	rm -rf VALGRIND.txt

s21_smart_calc.a:
	gcc $(CONTROL_FLAGS) $(FLAGS) -c $(LIB_SRC) $(LIBS)
	ar rcs s21_smart_calc.a $(OBJ)
	ranlib s21_smart_calc.a
	rm -f *.o

test: s21_smart_calc.a
	gcc $(CONTROL_FLAGS) $(FLAGS) $(TEST_SRC) $(LIB_SRC) -o ./tests/all_tests $(LIBS) $(TEST_FLAGS) s21_smart_calc.a
	./tests/all_tests

gcov_report: test
	lcov -t "./tests/all_tests" -o gcov.info -c -d .
	genhtml -o gcov_report gcov.info
	@rm -rf ./*.gc* ./tests/*.gc*
	$(OPEN) gcov_report/index.html

debug:
#	gcc debug.c $(FLAGS) $(LIB_SRC) $(LIBS); ./a.out
	gcc debug.c $(FLAGS) $(LIBS); ./a.out
