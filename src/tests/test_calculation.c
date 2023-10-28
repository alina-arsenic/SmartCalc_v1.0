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
  if (code) {
    printf("%s\nIncorrect expression\n\n", src);
  } else {
    code += calculation(dst, &result);
    if (code) printf("%s\nAnswer is not defined\n\n", src);
  }
  ck_assert_int_ne(code, 0);
}

START_TEST(test_s21_calculation) {
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

  compare_calc("-ln (89.2*6^8)-32-(9 mod 1+(6.02-3^5.1)/(3.7))", 20.8501598);
  compare_calc("-ln (89.2*6^8)-32-(-++++(9) mod 1+(-6.02-3^5.1)/+++-+(3.7))",
               -9.4461611);
  failure_calc("-ln (89.2*6^8)-32-(-++++(-9) mod 1+(-6.02-3^5.1)/+++-+(3.7))");
  failure_calc("-ln (89.2*6^8)-32-(-++++((9) mod 1+(-6.02-3^5.1)/+++-+(3.7))");
  failure_calc("-ln (89.2*6^8)-32-(-++++(9)) mod 1+(-6.02-3^5.1)/+++-+(3.7))");

  compare_calc("cos(sin(tan(-sqrt(99999.999999))))", 0.570365801);
  compare_calc("-cos(sin(tan(-sqrt(99999.999999))))", -0.570365801);
  compare_calc("acos(cos(sin(tan(-sqrt(99999.999999)))))", 0.963845197);
  failure_calc("--cos(sin(tan(-sqrt(99999.999999))))");
  failure_calc("--cos(sin(tan(sqrt((99999.999999))))");
  failure_calc("--cos(sin(tan(sqrt((99999.999999))))))");
  compare_calc("-log(atan(123456789123456789))", -0.196119877);

  failure_calc("asin(-200)");
  failure_calc("0/0");
  failure_calc("-0/0");
  failure_calc("0/-0");
  failure_calc("-0/-0");
  failure_calc("1/----------(10)");
  failure_calc("1/-(----------(10))");
  compare_calc("1/-(-----------(10))", 1);
  compare_calc("1/-(---------(10))", -1);
  failure_calc("0/asin(0)");
  failure_calc("acos(9999^99999)");
  failure_calc(
      "999999999999999999999999999^"
      "99999999999999999999999999999999999999999999999999");
  compare_calc(
      "999999999999999999999999999^-"
      "99999999999999999999999999999999999999999999999999",
      0);
  compare_calc("999^-2", 0.000001002);
  compare_calc("999999999999999999999999999^-(1/4)", 0.000000178);
  compare_calc(
      "999999999999^9",
      999999999991000000000035999999999916000000000125999999999874000000000083999999999964000000000008999999999999.0);
  failure_calc("log(-log(atan(6666666)))");
  failure_calc("ln(-log(atan(6666666)))");
  failure_calc("sqrt(-1111111111)");

  compare_calc(
      "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
      "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
      "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
      "++++++++++++++++++++++++++++++++++++(1)",
      253);
  failure_calc(
      "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
      "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
      "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
      "+++++++++++++++++++++++++++++++++++++(1)");
  failure_calc("345345.3245234.5");
  failure_calc("345345 3245234 5");
  failure_calc("345345 mod 3245234 log(5)");
  failure_calc("-llog(5)");
  failure_calc("jerigferogr");
  failure_calc("cos(345345) sin(3245234) log(5)");
  failure_calc("+ + *");
  failure_calc(")");
  failure_calc("(");
  failure_calc("");
  failure_calc("()");
  failure_calc("cos()");
}
END_TEST

Suite *s21_calculation_suite(void) {
  Suite *calculation = suite_create("s21_calculation");
  TCase *tc_s21_calculation = tcase_create("test_calculation");
  tcase_add_test(tc_s21_calculation, test_s21_calculation);
  suite_add_tcase(calculation, tc_s21_calculation);
  return calculation;
}
