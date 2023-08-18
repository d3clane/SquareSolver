#include <stdio.h>
#include "InputAndOutput.h"
#include "solver.h"
#include <stdlib.h>
#include <math.h>
#include "testingMode.h"
int main(int argc, const char *argv[]) {
#ifdef TEST
    Testing();
#else
    double a = NAN, b = NAN, c = NAN;
    double x1 = NAN, x2 = NAN;
    if (ReadInput(argc, argv, &a, &b, &c) != 0) {
        fprintf(stderr, "Error while reading input\n");
        return EXIT_FAILURE;
    }

    enum NumberOfRoots numberOfRoots = SolveQuadraticEquation(a, b, c, &x1, &x2);
    PrintRoots(numberOfRoots, x1, x2);
#endif
}
