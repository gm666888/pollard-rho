#include <stdio.h>
#include <stdlib.h>
#include "unittests.h"

int numTests;

int main() {
    numTests = 0;
    run_point_tests();
    printf("here 1 \n");
    run_ecc_tests();

    printf("here 2 \n");

    run_pollardrho_tests();

    printf("%d tests executed successfully\n", numTests);
    return 0;
}
