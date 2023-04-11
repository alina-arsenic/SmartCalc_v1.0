#include "s21_tests.h"

START_TEST(test_s21_notation_convert_number_only) {
    char src[MAX_LEN], dst[MAX_LEN];
    int code;

    strcpy(src, "1");
    code = notation_convert(src, dst);
    ck_assert_int_eq(code, 0);

    strcpy(src, "1.234345");
    code = notation_convert(src, dst);
    ck_assert_int_eq(code, 0);

    strcpy(src, "1.1234.3");
    code = notation_convert(src, dst);
    ck_assert_int_eq(code, 1);

    strcpy(src, "1 1");
    code = notation_convert(src, dst);
    ck_assert_int_eq(code, 1);

    strcpy(src, ".1");
    code = notation_convert(src, dst);
    ck_assert_int_eq(code, 1);

    strcpy(src, "00.999");
    code = notation_convert(src, dst);
    ck_assert_int_eq(code, 0);

    strcpy(src, "3254. 345");
    code = notation_convert(src, dst);
    ck_assert_int_eq(code, 1);
}
END_TEST

Suite *s21_notation_convert_suite(void) {
  Suite *notation_convert = suite_create("s21_notation_convert");
  TCase *tc_s21_notation_convert = tcase_create("test_notation_convert");
  tcase_add_test(tc_s21_notation_convert, test_s21_notation_convert_number_only);
  suite_add_tcase(notation_convert, tc_s21_notation_convert);
  return notation_convert;
}
