#include "s21_tests.h"

void compare(char src[], char res[]) {
    char dst[MAX_LEN];
    int code = notation_convert(src, dst);
    printf("|%s| LENGTH %ld\n|%s| LENGTH %ld\n", dst, strlen(dst), res, strlen(res));
    ck_assert_int_eq(code, 0);
    ck_assert_str_eq(dst, res);
}

START_TEST(test_s21_notation_convert_basic) {
    compare("1 + 1", "1 1 +");
    compare("1 - 1", "1 1 -");
    compare("1 * 1", "1 1 *");
    compare("1 / 1", "1 1 /");
    compare("536546 ^ 213", "536546 213 ^");
    compare("1 mod 1", "1 1 mod");
    compare("+ 1", "1 +");
    compare("- 1", "1 -");

    compare("1.345 + 342123.3", "1.345 342123.3 +");
    compare("145324.345 - 34212323.3", "145324.345 34212323.3 -");
    compare("145324.345 * 34212323.3", "145324.345 34212323.3 *");
    compare("145324.345 / 34212323.3", "145324.345 34212323.3 /");
    compare("145324.345 ^ 34212323.3", "145324.345 34212323.3 ^");
    compare("145324.345 mod 34212323.3", "145324.345 34212323.3 mod");
    compare("+ 34212323.3", "34212323.3 +");
    compare("- 34212323.3", "34212323.3 -");

    compare("3423.3", "3423.3");
}
END_TEST

START_TEST(test_s21_notation_convert_multiple) {
    compare("1 + 1 + 1 + 1", "1 1 + 1 + 1 +");
    compare("234.4 + 237 mod 2.3 / 23.4 - 32", "234.4 237 2.3 mod 23.4 / + 32 -");
    compare("3 ^ 3 mod 1 ^ 34 / 23 ^ 1", "3 3 ^ 1 34 ^ mod 23 1 ^ /");
    compare("1 mod 34.4 mod 45.3 mod 345", "1 34.4 mod 45.3 mod 345 mod");
    compare("- - - - 4", "4 - - - -");
    compare("+ 345 - 12", "345 + 12 -");
    compare("12 / 43 + + 345 - 12 ^ 21", "12 43 / + 345 + 12 21 ^ -");

}
END_TEST

START_TEST(test_s21_notation_convert_brackets) {
    compare("(123.32 + 2)", "123.32 2 +");
    compare("((123.32 + 2))", "123.32 2 +");
    compare("((123.32)) + 2", "123.32 2 +");
}
END_TEST

Suite *s21_notation_convert_suite(void) {
  Suite *notation_convert = suite_create("s21_notation_convert");
  TCase *tc_s21_notation_convert = tcase_create("test_notation_convert");
  tcase_add_test(tc_s21_notation_convert, test_s21_notation_convert_basic);
  tcase_add_test(tc_s21_notation_convert, test_s21_notation_convert_multiple);
  tcase_add_test(tc_s21_notation_convert, test_s21_notation_convert_brackets);
  suite_add_tcase(notation_convert, tc_s21_notation_convert);
  return notation_convert;
}
