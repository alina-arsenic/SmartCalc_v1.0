#include "s21_smart_calc.h"
#include <stdio.h>

int main() {
    char src[MAX_LEN], dst[MAX_LEN];

    strcpy(src, "((1 * (2 - 2) mod 23324 + 2 / +7))");
    printf("%s\n", src);

    int code = notation_convert(src, dst);
    if (!code) printf("%s\n", dst);
    else printf("ERROR\n");
    return 0;
}
