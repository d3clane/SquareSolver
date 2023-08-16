#include <stdio.h>
#include "InputAndOutput.h"
#include "solver.h"
#include <stdlib.h>
int main() {
    double a = 0, b = 0, c = 0;
    double x1, x2;

    /* scanning coeffs for quadratic equation */
    if (ScanCoeffs(&a, &b, &c) != 0) {
        exit(EXIT_FAILURE);
    }

    int numberOfRoots = SolveQuadraticEquation(a, b, c, &x1, &x2);
    PrintRoots(numberOfRoots, x1, x2);
}
