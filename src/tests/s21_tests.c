#include "s21_tests.h"

int main(void) {

  SRunner *sr;

  Suite *notation_convert = s21_notation_convert_suite();
  Suite *calculation = s21_calculation_suite();

  sr = srunner_create(notation_convert);
  srunner_add_suite(sr, calculation);

  srunner_run_all(sr, CK_ENV);
  int number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
