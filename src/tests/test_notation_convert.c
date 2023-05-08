#include "s21_tests.h"

void compare_convert(char src[], char res[]) {
    char dst[MAX_LEN];
    int code = notation_convert(src, dst);
    printf("|%s| LENGTH %ld\n", dst, strlen(dst));
    ck_assert_int_eq(code, 0);
    ck_assert_str_eq(dst, res);
}

void failure_convert(char src[]) {
    char dst[MAX_LEN];
    int code = notation_convert(src, dst);
    ck_assert_int_ne(code, 0);
}

START_TEST(test_s21_notation_convert_basic) {
    compare_convert("1 + 1", "1 1 +");
    compare_convert("1 - 1", "1 1 -");
    compare_convert("1 * 1", "1 1 *");
    compare_convert("1 / 1", "1 1 /");
    compare_convert("536546 ^ 213", "536546 213 ^");
    compare_convert("1 mod 1", "1 1 mod");
    compare_convert("+ 1", "1 +u");
    compare_convert("- 1", "-1");
    compare_convert("- - 1", "-1 -u");

    compare_convert("1.345 + 342123.3", "1.345 342123.3 +");
    compare_convert("145324.345 - 34212323.3", "145324.345 34212323.3 -");
    compare_convert("145324.345 * 34212323.3", "145324.345 34212323.3 *");
    compare_convert("145324.345 / 34212323.3", "145324.345 34212323.3 /");
    compare_convert("145324.345 ^ 34212323.3", "145324.345 34212323.3 ^");
    compare_convert("145324.345 mod 34212323.3", "145324.345 34212323.3 mod");
    compare_convert("+ 34212323.3", "34212323.3 +u");
    compare_convert("- 34212323.3", "-34212323.3");
    compare_convert("+ - 34212323.3", "-34212323.3 +u");
    compare_convert("- + 34212323.3", "34212323.3 +u -u");

    compare_convert("3423.3", "3423.3");
}
END_TEST

START_TEST(test_s21_notation_convert_multiple) {
    compare_convert("1 + 1 + 1 + 1", "1 1 + 1 + 1 +");
    compare_convert("234.4 + 237 mod 2.3 / 23.4 - 32", "234.4 237 2.3 mod 23.4 / + 32 -");
    compare_convert("3 ^ 3 mod 1 ^ 34 / 23 ^ 1", "3 3 ^ 1 34 ^ mod 23 1 ^ /");
    compare_convert("1 mod 34.4 mod 45.3 mod 345", "1 34.4 mod 45.3 mod 345 mod");
    compare_convert("- - - - 4", "-4 -u -u -u");
    compare_convert("+ 345 * 12", "345 +u 12 *");
    compare_convert("12 / 43 + + 345 - 12 ^ 21", "12 43 / 345 +u + 12 21 ^ -");

}
END_TEST

START_TEST(test_s21_notation_convert_brackets) {
    compare_convert("(123.32 + 2)", "123.32 2 +");
    compare_convert("((123.32 + 2))", "123.32 2 +");
    compare_convert("((123.32)) + 2", "123.32 2 +");
    compare_convert("(12 / ((43 + + 345) - 12)) ^ 21", "12 43 345 +u + 12 - / 21 ^");
    compare_convert("(((((((((((((((((1 mod 34.4))))) mod 45.3))) mod 345)))))))))", "1 34.4 mod 45.3 mod 345 mod");
    compare_convert("- - - - (4)", "4 -u -u -u -u");
}
END_TEST

START_TEST(test_s21_notation_convert_x) {
    compare_convert("(((((((((((((((((x mod x))))) mod x))) mod x)))))))))", "x x mod x mod x mod");
    compare_convert("- - - - x", "-x -u -u -u");
    compare_convert("+ x * x", "x +u x *");
    compare_convert("x / 43 + + x - 12 ^ 21", "x 43 / x +u + 12 21 ^ -");
}
END_TEST

START_TEST(test_s21_notation_convert_long) {
    compare_convert("1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1",
    "1 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 * 1 *");
}
END_TEST

START_TEST(test_s21_notation_convert_functions) {
    compare_convert("(((((((((((((((((cos(x) mod sin(x)))))) mod tan(x)))) mod acos(x))))))))))", "x cos x sin mod x tan mod x acos mod");
    compare_convert("+ asin(acos(atan(x))) * sqrt(++x + --x)", "x atan acos asin +u x +u +u -x -u + sqrt *");
    compare_convert("- - - - log(4)", "4 -log -u -u -u");
    compare_convert("- + ln(ln(1)) * x", "1 ln ln +u -u x *");
    compare_convert("-(log(2 * + 3 * + 8))", "2 3 +u * 8 +u * log -u");
}
END_TEST

START_TEST(test_s21_notation_convert_bad_input) {
    failure_convert("");
    failure_convert("1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*1*11");
    failure_convert("    ");
    failure_convert("0.1234.3 + 12");
    failure_convert("123 + + * 4");
    failure_convert("x + x + xx");
    failure_convert("rfgd");
    failure_convert("324^^234");
    failure_convert("logg(3)");
    failure_convert("xcos(1)");
    failure_convert("((cos(1)");
    failure_convert("0.1234 + 12)");
    failure_convert("4 (0.1234 + 12)");
    failure_convert("(0.1234 + 12 -)");
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
