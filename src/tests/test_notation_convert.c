#include "s21_tests.h"

void compare(char src[], char res[]) {
    char dst[MAX_LEN];
    int code = notation_convert(src, dst);
    printf("|%s| LENGTH %ld\n", dst, strlen(dst));
    ck_assert_int_eq(code, 0);
    ck_assert_str_eq(dst, res);
}

void failure(char src[]) {
    char dst[MAX_LEN];
    int code = notation_convert(src, dst);
    ck_assert_int_ne(code, 0);
}

START_TEST(test_s21_notation_convert_basic) {
    compare("1 + 1", "1 1 +");
    compare("1 - 1", "1 1 -");
    compare("1 * 1", "1 1 *");
    compare("1 / 1", "1 1 /");
    compare("536546 ^ 213", "536546 213 ^");
    compare("1 mod 1", "1 1 mod");
    compare("+ 1", "1 +");
    compare("- 1", "-1");
    compare("- - 1", "-1 -");

    compare("1.345 + 342123.3", "1.345 342123.3 +");
    compare("145324.345 - 34212323.3", "145324.345 34212323.3 -");
    compare("145324.345 * 34212323.3", "145324.345 34212323.3 *");
    compare("145324.345 / 34212323.3", "145324.345 34212323.3 /");
    compare("145324.345 ^ 34212323.3", "145324.345 34212323.3 ^");
    compare("145324.345 mod 34212323.3", "145324.345 34212323.3 mod");
    compare("+ 34212323.3", "34212323.3 +");
    compare("- 34212323.3", "-34212323.3");
    compare("+ - 34212323.3", "-34212323.3 +");
    compare("- + 34212323.3", "34212323.3 + -");

    compare("3423.3", "3423.3");
}
END_TEST

START_TEST(test_s21_notation_convert_multiple) {
    compare("1 + 1 + 1 + 1", "1 1 + 1 + 1 +");
    compare("234.4 + 237 mod 2.3 / 23.4 - 32", "234.4 237 2.3 mod 23.4 / + 32 -");
    compare("3 ^ 3 mod 1 ^ 34 / 23 ^ 1", "3 3 ^ 1 34 ^ mod 23 1 ^ /");
    compare("1 mod 34.4 mod 45.3 mod 345", "1 34.4 mod 45.3 mod 345 mod");
    compare("- - - - 4", "-4 - - -");
    compare("+ 345 * 12", "345 + 12 *");
    compare("12 / 43 + + 345 - 12 ^ 21", "12 43 / 345 + 12 21 ^ - +");

}
END_TEST

START_TEST(test_s21_notation_convert_brackets) {
    compare("(123.32 + 2)", "123.32 2 +");
    compare("((123.32 + 2))", "123.32 2 +");
    compare("((123.32)) + 2", "123.32 2 +");
    compare("(12 / ((43 + + 345) - 12)) ^ 21", "12 43 345 + + 12 - / 21 ^");
    compare("(((((((((((((((((1 mod 34.4))))) mod 45.3))) mod 345)))))))))", "1 34.4 mod 45.3 mod 345 mod");
    compare("- - - - (4)", "4 - - - -");
}
END_TEST

START_TEST(test_s21_notation_convert_x) {
    compare("(((((((((((((((((x mod x))))) mod x))) mod x)))))))))", "x x mod x mod x mod");
    compare("- - - - x", "-x - - -");
    compare("+ x * x", "x + x *");
    compare("x / 43 + + x - 12 ^ 21", "x 43 / x + 12 21 ^ - +");
}
END_TEST

START_TEST(test_s21_notation_convert_long) {
    compare("1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1",
    "1 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 *");
}
END_TEST

START_TEST(test_s21_notation_convert_functions) {
    compare("(((((((((((((((((cos(x) mod sin(x)))))) mod tan(x)))) mod acos(x))))))))))", "x cos x sin mod x tan mod x acos mod");
    compare("+ asin(acos(atan(x))) * sqrt(++x + --x)", "x atan acos asin + x + + -x - + sqrt *");
    compare("- - - - log(4)", "4 -log - - -");
    compare("- + ln(ln(1)) * x", "1 ln ln + - x *");
    compare("-(log(2 * + 3 * + 8))", "2 3 + * 8 + * log -");
}
END_TEST

START_TEST(test_s21_notation_convert_bad_input) {
    failure("");
    failure("1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*11");
    failure("    ");
    failure("0.1234.3 + 12");
    failure("123 + + * 4");
    failure("x + x + xx");
    failure("rfgd");
    failure("324^^234");
    failure("logg(3)");
    failure("xcos(1)");
    failure("((cos(1)");
    failure("0.1234 + 12)");
    failure("4 (0.1234 + 12)");
    failure("(0.1234 + 12 -)");
}
END_TEST

Suite *s21_notation_convert_suite(void) {
  Suite *notation_convert = suite_create("s21_notation_convert");
  TCase *tc_s21_notation_convert = tcase_create("test_notation_convert");
  tcase_add_test(tc_s21_notation_convert, test_s21_notation_convert_basic);
  tcase_add_test(tc_s21_notation_convert, test_s21_notation_convert_multiple);
  tcase_add_test(tc_s21_notation_convert, test_s21_notation_convert_brackets);
  tcase_add_test(tc_s21_notation_convert, test_s21_notation_convert_x);
  tcase_add_test(tc_s21_notation_convert, test_s21_notation_convert_long);
  tcase_add_test(tc_s21_notation_convert, test_s21_notation_convert_functions);
    tcase_add_test(tc_s21_notation_convert, test_s21_notation_convert_bad_input);
  suite_add_tcase(notation_convert, tc_s21_notation_convert);
  return notation_convert;
}
