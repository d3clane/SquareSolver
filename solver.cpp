#include "Solver.h"

enum NumberOfRoots SolveQuadraticEquation(double a, double b, double c, double *x1, double *x2) {
    assert(x1 != NULL);
    assert(x2 != NULL);
    enum NumberOfRoots numberOfRoots = ZERO_ROOTS;
    if (equal(a, 0.0)) {
        numberOfRoots = SolveLinearEquation(b, c, x1);
    } else {
        double D = b * b - 4 * a * c;
        if (less(D, 0.0)) {
            numberOfRoots = ZERO_ROOTS;
        } else if (equal(D, 0.0)) {
            numberOfRoots = ONE_ROOT;
            *x1 = -b / (2 * a);
        } else {
            numberOfRoots = TWO_ROOTS;
            double sqrtD = sqrt(D);
            *x1 = (-b - sqrtD) / (2 * a);
            *x2 = (-b + sqrtD) / (2 * a);
        }
    }
    return numberOfRoots;
}

enum NumberOfRoots SolveLinearEquation(double a, double b, double *x1) {
    if (equal(a, 0.0)) {
        return equal(b, 0.0) ? INF_ROOTS : ZERO_ROOTS;
    }
    *x1 = -b / a;
    return ONE_ROOT;
}

enum NumberOfRoots convertNumberOfRootsToEnum(int numberOfRoots) {
    enum NumberOfRoots myNumberOfRoots = ZERO_ROOTS;
    switch (numberOfRoots) {
        case 0:
            return ZERO_ROOTS;
        case 1:
            return ONE_ROOT;
        case 2:
            return TWO_ROOTS;
        default:
            return INF_ROOTS;
    }
}

const char *convertEnumToString(enum NumberOfRoots numberOfRoots) {
    switch (numberOfRoots) {
        case ZERO_ROOTS:
            return "zero roots";
        case ONE_ROOT:
            return "one root";
        case TWO_ROOTS:
            return "two roots";
        default:
            return "infinite number of roots";
    }
}

