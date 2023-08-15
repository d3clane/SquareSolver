#include "solver.h"
#include <stdio.h>
#include <assert.h>
#include <math.h>

#define EPS 1e-9;
static int SolveLinearEquation(double a, double b, double *x1);

static int eq(double a, double b);
static int less(double a, double b);
static int more(double a, double b);

int SolveQuadraticEquation(double a, double b, double c, double *x1, double *x2) {
    assert(x1 != NULL);
    assert(x2 != NULL);
    int numberOfRoots = 0;
    if (a == 0) {
        numberOfRoots = SolveLinearEquation(b, c, x1);
    } else {
        double D = b * b - 4 * a * c;
        if (less(D, 0.0)) {
            numberOfRoots = 0;
        } else if (eq(D, 0.0)) {
            numberOfRoots = 1;
            *x1 = (-b - sqrt(D)) / (2 * a);
        } else {
            numberOfRoots = 2;
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
    if (a == 0 && b == 0) {
        return INF;
    }
    if (a == 0) {
        return 0;
    }
    *x1 = -b / a;
    return 1;
}