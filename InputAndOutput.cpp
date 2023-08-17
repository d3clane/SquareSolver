//
// Created by Arman on 15.08.2023.
//
#include <stdio.h>
#include <assert.h>
#include "InputAndOutput.h"
#include "solver.h"

static inline void skipSymbols();
static inline int menuInput();

int ScanCoeffs(double *a, double *b, double *c) {
    assert(a != NULL);
    assert(b != NULL);
    assert(c != NULL);
    const int numberOfReadValues = 3;

    int quitInput = menuInput(); //quitInput - flag to quit the scan program without getting correct input of coeffs

    int scannedVals = fscanf(stdin, "%lf%lf%lf", a, b, c);
    while (scannedVals != numberOfReadValues && quitInput != 1) {
        fprintf(stdout, "not valid input\n");
        skipSymbols();
        quitInput = menuInput();
        scannedVals = fscanf(stdin, "%lf%lf%lf", a, b, c);
    }

    return quitInput;
}

int PrintRoots(const enum NumberOfRoots numberOfRoots, const double x1, const double x2) {
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

static inline int menuInput() {
    fprintf(stdout, "Print coefficients a, b, c for the equation type ax^2 + bx + c: (or q to quit)\n");
    int ch = getc(stdin);
    ungetc(ch, stdin);
    return ch == 'q';
}

static inline void skipSymbols() {
    while (getchar() != '\n') {
        continue;
    }
}
