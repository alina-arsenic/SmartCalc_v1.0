#include "s21_tests.h"

#define TOL 1e-7

void compare_calc(char src[], double answer) {
    char dst[MAX_LEN];
    double result;
    int code = notation_convert(src, dst);
    printf("%s\n", dst);
    if (!code) code += calculation(dst, &result);
    printf("%s = %.7lf\n\n", src, result);
    ck_assert_int_eq(code, 0);
    ck_assert_double_eq_tol(result, answer, TOL);
}

void failure_calc(char src[]) {
    char dst[MAX_LEN];
    double result;
    int code = notation_convert(src, dst);
    if (!code) code += calculation(dst, &result);
    ck_assert_int_ne(code, 0);
}

START_TEST(test_s21_calculation_basic) {
    compare_calc("2+2", 4.0);
    compare_calc("2-2", 0.0);
    compare_calc("2-(-2)", 4.0);
    compare_calc("2--2", 4.0);
    compare_calc("2*-2", -4.0);
    compare_calc("-2", -2.0);
    compare_calc("-(2)", 1.0);
    
    compare_calc("-(--(5))", -3.0);
    compare_calc("---(5)", 2.0);
    compare_calc("1+-(--(5))", -2.0);
    compare_calc("1--(--(5))", 4.0);
    compare_calc("1----(5)", -1.0);
    compare_calc("1----(5.3)", -1.3);

    failure_calc("1---(--(5))");
    failure_calc("--2");
    failure_calc("4(-2)");
}
END_TEST

Suite *s21_calculation_suite(void) {
  Suite *calculation = suite_create("s21_calculation");
  TCase *tc_s21_calculation = tcase_create("test_calculation");
  tcase_add_test(tc_s21_calculation, test_s21_calculation_basic);
  suite_add_tcase(calculation, tc_s21_calculation);
  return calculation;
}
