#include "lib/s21_smart_calc.h"
#include <stdio.h>

int main() {
    char src[MAX_LEN], dst[MAX_LEN];
    double result;

    strcpy(src, "-3.3/(-cos(5.5/1.2 * ++--4) * ln(+++238.4 - 2))");
    printf("\n%s\n", src);

    int code = notation_convert(src, dst);
    if (!code) printf("%s\n\n", dst);
    else printf("ERROR\n");

    code = calculation(dst, &result);
    if (!code) printf("%lf\n\n", result);
    else printf("ERROR\n");
    
    return 0;
}
