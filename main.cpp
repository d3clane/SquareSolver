#include <stdio.h>
#include "InputAndOutput.h"
#include "solver.h"
#include <stdlib.h>
#include <math.h>
int main() {
    double a = NAN, b = NAN, c = NAN;
    double x1 = NAN, x2 = NAN;

    /* scanning coeffs for quadratic equation */
    if (ScanCoeffs(&a, &b, &c) != 0) {
        return EXIT_FAILURE;
    }

    enum NumberOfRoots numberOfRoots = SolveQuadraticEquation(a, b, c, &x1, &x2);
    PrintRoots(numberOfRoots, x1, x2);
}
