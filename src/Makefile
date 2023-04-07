PLATFORM := $(shell uname)
CFLAGS = -std=c11 -Wall -Werror -Wextra
TFLAGS = -lc --coverage
PKG = `pkg-config --cflags --libs check`
SRC = s21_*.c
OBJ = *.o
TESTS_SRC = tests/*.c

ifeq ($(PLATFORM), Linux)
OPEN = xdg-open 
else
OPEN = open
endif

all: clean s21_smart_calc.a test gcov_report

#install
#uninstall

clean:
	rm -f *.o *.a *.gcda *.gcno *.info
	rm -fr gcov_report ./tests/all_tests*
	rm -f ./a.out
	rm -rf ./tests/*.o ./tests/*.a ./tests/*.gcda ./tests/*.gcno ./tests/*.info
	rm -rf VALGRIND.txt

#dvi
#dist

s21_smart_calc.a:
	gcc $(CFLAGS) -c $(SRC)
	ar rcs s21_smart_calc.a $(OBJ)
	ranlib s21_smart_calc.a
	rm -f *.o

test: s21_smart_calc.a
	gcc $(TESTS_SRC) $(SRC) -o ./tests/all_tests $(CFLAGS) $(PKG) $(TFLAGS) s21_smart_calc.a
	./tests/all_tests

gcov_report: test
	lcov -t "./tests/all_tests" -o gcov.info -c -d .
	genhtml -o gcov_report gcov.info
	@rm -rf ./*.gc* ./tests/*.gc*
	$(OPEN) gcov_report/index.html

debug:
	gcc debug.c $(SRC) $(PKG); ./a.out
