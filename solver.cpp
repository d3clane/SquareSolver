#include "solver.h"
#include <stdio.h>
#include <assert.h>
#include <math.h>

//comparisons of double numbers with accuracy EPS
static const double EPS = 1e-9;
static int equal(const double a, const double b);
static int less(const double a, const double b);
static int more(const double a, const double b);

enum NumberOfRoots SolveQuadraticEquation(const double a, const double b, const double c, double *x1, double *x2) {
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

enum NumberOfRoots SolveLinearEquation(const double a, const double b, double *x1) {
    if (equal(a, 0.0)) {
        if (equal(b, 0.0)) {
            return INF_ROOTS;
        }
        return ZERO_ROOTS;
    }
    *x1 = -b / a;
    return ONE_ROOT;
}

static int equal(const double a, const double b) {
    return fabs(a - b) < EPS;
}

static int less(const double a, const double b) {
    return b - a > EPS;
}

static int more(const double a, const double b) {
    return a - b > EPS;
}
