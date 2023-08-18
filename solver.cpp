#include "solver.h"
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "doubleComparisons.h"

enum NumberOfRoots SolveQuadraticEquation(double a, double b, double c, double *x1, double *x2) {
    assert(x1 != NULL);
    assert(x2 != NULL);
    enum NumberOfRoots numberOfRoots = ZERO_ROOTS;
    if (equal(a, 0.0)) {
        numberOfRoots = SolveLinearEquation(b, c, x1);
    } else {
        double D = b * b - 4 * a * c;
        double sqrtD = sqrt(D);
        if (less(D, 0.0)) {
            numberOfRoots = ZERO_ROOTS;
        } else if (equal(D, 0.0)) {
            numberOfRoots = ONE_ROOT;
            *x1 = -b / (2 * a);
        } else {
            numberOfRoots = TWO_ROOTS;
            *x1 = (-b - sqrtD) / (2 * a);
            *x2 = (-b + sqrtD) / (2 * a);
        }
    }
    return numberOfRoots;
}

enum NumberOfRoots SolveLinearEquation(double a, double b, double *x1) {
    if (equal(a, 0.0)) {
        if (equal(b, 0.0)) {
            return INF_ROOTS;
        }
        return ZERO_ROOTS;
    }
    *x1 = -b / a;
    return ONE_ROOT;
}

