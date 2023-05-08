PLATFORM := $(shell uname)
CFLAGS = -std=c11 -Wall -Werror -Wextra
TFLAGS = -lc --coverage
PKG_FLAGS = `pkg-config --cflags gtk+-3.0`
PKG_LIBS = `pkg-config --libs gtk+-3.0`
PKG_TESTS = `pkg-config --cflags --libs check`
SRC = lib/*.c
OBJ = *.o
TESTS_SRC = tests/*.c

ifeq ($(PLATFORM), Linux)
OPEN = xdg-open 
else
OPEN = open
endif

all: clean s21_smart_calc.a test gcov_report

gui: s21_smart_calc.a
	gcc $(PKG_FLAGS) -o calculator $(SRC) gui.c $(PKG_LIBS)
	./calculator

#install
#uninstall

clean:
	rm -f *.o *.a *.gcda *.gcno *.info
	rm -fr gcov_report ./tests/all_tests*
	rm -f ./a.out
	rm -rf ./tests/*.o ./tests/*.a ./tests/*.gcda ./tests/*.gcno ./tests/*.info
	rm -rf VALGRIND.txt
	rm -rf calculator

#dvi
#dist

s21_smart_calc.a:
	gcc $(CFLAGS) $(PKG_FLAGS) -c $(SRC) $(PKG_LIBS)
	ar rcs s21_smart_calc.a $(OBJ)
	ranlib s21_smart_calc.a
	rm -f *.o

test: s21_smart_calc.a
	gcc $(CFLAGS) $(PKG_FLAGS) $(TESTS_SRC) $(SRC) -o ./tests/all_tests $(PKG_TESTS) $(PKG_LIBS) $(TFLAGS) s21_smart_calc.a
	./tests/all_tests

gcov_report: test
	lcov -t "./tests/all_tests" -o gcov.info -c -d .
	genhtml -o gcov_report gcov.info
	@rm -rf ./*.gc* ./tests/*.gc*
	$(OPEN) gcov_report/index.html

debug:
	gcc debug.c $(PKG_FLAGS) $(SRC) $(PKG_LIBS) -lm; ./a.out
