//
// Created by Arman on 15.08.2023.
//
#include <stdio.h>
#include <assert.h>
#include "InputAndOutput.h"
#include "solver.h"

static int SafeScanfd(double *a);

int ScanCoeffs(double *a, double *b, double *c) {
    assert(a != NULL);
    assert(b != NULL);
    assert(c != NULL);
    int scanfSuccess = 0;
    scanfSuccess = scanfSuccess || SafeScanfd(a);
    scanfSuccess = scanfSuccess || SafeScanfd(b);
    scanfSuccess = scanfSuccess || SafeScanfd(c);
    return scanfSuccess;
}

int PrintRoots(int numberOfRoots, double x1, double x2) {
    int printSuccess = 0;
    switch (numberOfRoots) {
        case ZERO_ROOTS:
            fprintf(stdout, "No roots\n");
            break;
        case ONE_ROOT:
            fprintf(stdout, "One root: %.8lf\n", x1);
            break;
        case TWO_ROOTS:
            fprintf(stdout, "Two roots: %.8lf and %.8lf\n", x1, x2);
            break;
        case INF_ROOTS:
            fprintf(stdout, "Infinity number of roots\n");
            break;
        default:
            fprintf(stderr, "Not valid number of roots\n");
            printSuccess = 1;
            break;
    }
    return printSuccess;
}


static int SafeScanfd(double *a) {
    if (fscanf(stdin, "%lf", a) != 1) {
        fprintf(stderr, "Error reading coefficients");
        return 1;
    }
    return 0;
}
