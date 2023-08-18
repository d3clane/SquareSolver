//
// Created by Arman on 18.08.2023.
//
#include <stdio.h>
#include <math.h>
#include "testingMode.h"
#include "InputAndOutput.h"
#include "solver.h"
#include "doubleComparisons.h"

static inline enum NumberOfRoots convertNumberOfRootsToEnum(int numberOfRoots);
static inline void swap(double *a, double *b);
static inline char *convertEnumToString(enum NumberOfRoots numberOfRoots);

void Testing() {
    FILE *fp = fopen(FILE_NAME, FILE_MODE);
    int numberOfRoots = 0;
    double a = NAN, b = NAN, c = NAN;
    double testX1 = NAN, testX2 = NAN;
    double myX1 = NAN, myX2 = NAN;
    const int haveToReadValuesAtOnce = 6;
    int numberOfSuccessfullyReadValues = fscanf(fp, "%d%lf%lf%lf%lf%lf", &numberOfRoots, &a, &b, &c, &testX1, &testX2);
    int testNumber = 1;
    while (numberOfSuccessfullyReadValues == haveToReadValuesAtOnce) {
        enum NumberOfRoots myNumberOfRoots = SolveQuadraticEquation(a, b, c, &myX1, &myX2);
        char *stringNumberOfRoots = convertEnumToString(myNumberOfRoots);
        enum NumberOfRoots testNumberOfRoots = convertNumberOfRootsToEnum(numberOfRoots);
        if (testNumberOfRoots != myNumberOfRoots) {
            fprintf(stdout, "Test %d: number of roots do not match\n", testNumber);
            fprintf(stdout, "program expected %s, test has %s\n", stringNumberOfRoots,
                    convertEnumToString(testNumberOfRoots));
        } else {

            switch (myNumberOfRoots) {
                case ZERO_ROOTS:
                    fprintf(stdout, "Test %d: test is OK\n", testNumber);
                    break;
                case ONE_ROOT:
                    if (equal(testX1, myX1)) {
                        fprintf(stdout, "Test %d: test is OK\n", testNumber);
                    } else {
                        fprintf(stdout, "Test %d failed:\nprogram result: %lf  test value: %lf\n", testNumber, myX1,
                                testX1);
                    }
                    break;
                case TWO_ROOTS:
                    if (equal(testX1, myX1) && equal(testX2, myX2)) {
                        fprintf(stdout, "Test %d: test is OK\n", testNumber);
                    } else {
                        fprintf(stdout, "Test %d failed:\nprogram result: %lf and %lf  test value: %lf and %lf\n",
                                testNumber, myX1, myX2, testX1, testX2);
                    }
                    break;
                case INF_ROOTS:
                    fprintf(stdout, "Test %d: test is OK\n", testNumber);
                    break;
                default:
                    fprintf(stderr, "not valid number of roots\n");
                    break;
            }
        }
        numberOfSuccessfullyReadValues = fscanf(fp, "%d%lf%lf%lf%lf%lf", &numberOfRoots, &a, &b, &c, &testX1, &testX2);
        testNumber++;
    }
}

static inline enum NumberOfRoots convertNumberOfRootsToEnum(int numberOfRoots) {
    enum NumberOfRoots myNumberOfRoots = ZERO_ROOTS;
    if (numberOfRoots == 0) {
        myNumberOfRoots = ZERO_ROOTS;
    } else if (numberOfRoots == 1) {
        myNumberOfRoots = ONE_ROOT;
    } else if (numberOfRoots == 2) {
        myNumberOfRoots = TWO_ROOTS;
    } else {
        myNumberOfRoots = INF_ROOTS;
    }
    return myNumberOfRoots;
}

static inline char *convertEnumToString(enum NumberOfRoots numberOfRoots) {
    if (numberOfRoots == ZERO_ROOTS) {
        return (char *)"zero roots";
    } else if (numberOfRoots == ONE_ROOT) {
        return (char *)"one root";
    } else if (numberOfRoots == TWO_ROOTS) {
        return (char *)"two roots";
    } else {
        return (char *)"infinite number of roots";
    }
}

static inline void swap(double *a, double *b) {
    double tmp = 0;
    tmp = *a;
    *a = *b;
    *b = tmp;
}
