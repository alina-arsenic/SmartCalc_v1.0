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
FOLDER_NAME = SmartCalculator

APP_DIR = $(INSTALL_DIR)/$(FOLDER_NAME)

ifeq ($(PLATFORM), Linux)
OPEN = xdg-open 
else
OPEN = open
endif

all: clean gcov_report dist install

dist:
	mkdir -p $(APP_DIR)/dist
	tar -cvzf $(APP_DIR)/dist/Calculator.tar.gz ../*

install:
	mkdir -p $(APP_DIR)/images
	gcc $(FLAGS) -o $(APP_DIR)/app $(LIB_SRC) $(GUI_SRC) $(LIBS)
	cp images/* $(APP_DIR)/images/
	install $(APP_DIR)/app $(APP_DIR)/Calculator
	rm -rf $(APP_DIR)/app

uninstall:
	rm -rf $(APP_DIR)

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

style:
	cppcheck *.c *.h
	cp ../materials/linters/.clang-format ./
#clang-format -i *.c *.h tests/*.c
	clang-format -n *.c *.h tests/*.c
	rm -f ./.clang-format

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=VALGRIND.txt ./tests/all_tests
