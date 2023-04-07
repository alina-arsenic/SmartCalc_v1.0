#include "s21_tests.h"

START_TEST(test_s21_aAA_1) {
    ck_assert_int_eq(0, 0);
}
END_TEST

Suite *s21_aAA_suite(void) {
  Suite *aAA = suite_create("s21_aAA");
  TCase *tc_s21_aAA = tcase_create("test_aAA");
  tcase_add_test(tc_s21_aAA, test_s21_aAA_1);
  suite_add_tcase(aAA, tc_s21_aAA);
  return aAA;
}
