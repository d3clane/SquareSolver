#include <stdio.h>
#include "InputAndOutput.h"
#include "solver.h"
int main() {
    double a = 0, b = 0, c = 0;
    double x1, x2;
    ScanCoeffs(&a, &b, &c);
    int numberOfRoots = SolveQuadraticEquation(a, b, c, &x1, &x2);
    PrintRoots(numberOfRoots, x1, x2);
}
