#include "solver.h"
#include <stdio.h>
#include <assert.h>
#include <math.h>

//solves linear equation
static int SolveLinearEquation(double a, double b, double *x1);

//comprasions of double numbers with accuracy EPS
#define EPS 1e-9;
static int eq(double a, double b);
static int less(double a, double b);
static int more(double a, double b);

int SolveQuadraticEquation(double a, double b, double c, double *x1, double *x2) {
    assert(x1 != NULL);
    assert(x2 != NULL);
    int numberOfRoots = ZERO_ROOTS;
    if (a == 0) {
        numberOfRoots = SolveLinearEquation(b, c, x1);
    } else {
        double D = b * b - 4 * a * c;
        if (less(D, 0.0)) {
            numberOfRoots = ZERO_ROOTS;
        } else if (eq(D, 0.0)) {
            numberOfRoots = ONE_ROOT;
            *x1 = (-b - sqrt(D)) / (2 * a);
        } else {
            numberOfRoots = TWO_ROOTS;
            *x1 = (-b - sqrt(D)) / (2 * a);
            *x2 = (-b + sqrt(D)) / (2 * a);
        }
    }
    return numberOfRoots;
}

static int eq(double a, double b) {
    return fabs(a - b) < EPS;
}

static int less(double a, double b) {
    return b - a > EPS;
}

static int more(double a, double b) {
    return a - b > EPS;
}

static int SolveLinearEquation(double a, double b, double *x1) {
    if (eq(a, 0.0) && eq(b, 0.0)) {
        return INF_ROOTS;
    }
    if (eq(a, 0.0)) {
        return ZERO_ROOTS;
    }
    *x1 = -b / a;
    return ONE_ROOT;
}
