#include "s21_tests.h"

START_TEST(test_s21_bBB_1) {
    ck_assert_int_eq(0, 0);
}
END_TEST

Suite *s21_bBB_suite(void) {
  Suite *bBB = suite_create("s21_bBB");
  TCase *tc_s21_bBB = tcase_create("test_bBB");
  tcase_add_test(tc_s21_bBB, test_s21_bBB_1);
  suite_add_tcase(bBB, tc_s21_bBB);
  return bBB;
}
