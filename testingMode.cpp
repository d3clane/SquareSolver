//
// Created by Arman on 18.08.2023.
//

#include "DoubleComparisons.h"
#include "Solver.h"
#include "TestingMode.h"

static inline void swap(double *a, double *b);

void Testing() {
    FILE *fp = fopen(FILE_NAME, FILE_MODE);

    if (fp == NULL) {
        perror("Error opening file");
        return;
    }

    static const int haveToReadValuesAtOnce = 6;

    int numberOfRoots = 0;

    double a = NAN, b = NAN, c = NAN;
    double testX1 = NAN, testX2 = NAN;
    double myX1 = NAN, myX2 = NAN;

    int numberOfSuccessfullyReadValues = fscanf(fp, "%d%lf%lf%lf%lf%lf", &numberOfRoots, &a, &b, &c, &testX1, &testX2);
    int testNumber = 1;
    while (numberOfSuccessfullyReadValues == haveToReadValuesAtOnce) {
        enum NumberOfRoots myNumberOfRoots = SolveQuadraticEquation(a, b, c, &myX1, &myX2);
        enum NumberOfRoots testNumberOfRoots = convertNumberOfRootsToEnum(numberOfRoots);

        if (testNumberOfRoots != myNumberOfRoots) {
            printf("Test %d: number of roots do not match\n", testNumber);

            printf("program expected %s, test has %s\n",
                    convertEnumToString(myNumberOfRoots),
                    convertEnumToString(testNumberOfRoots));
        } else {
            switch (myNumberOfRoots) {
                case ZERO_ROOTS:
                    printf("Test %d: test is OK\n", testNumber);
                    break;
                case ONE_ROOT:
                    if (equal(testX1, myX1)) {
                        printf("Test %d: test is OK\n", testNumber);
                    } else {
                        printf("Test %d failed:\nprogram result: %lf  test value: %lf\n",
                                testNumber, myX1, testX1);
                    }

                    break;
                case TWO_ROOTS:
                    if (less(testX1, testX2)) swap(&testX1, &testX2); // testX1 <= testX2

                    if (equal(testX1, myX1) && equal(testX2, myX2)) {
                        printf("Test %d: test is OK\n", testNumber);
                    } else {
                        printf("Test %d failed:\nprogram result: %lf and %lf  test value: %lf and %lf\n",
                                testNumber, myX1, myX2, testX1, testX2);
                    }

                    break;
                case INF_ROOTS:
                    printf("Test %d: test is OK\n", testNumber);
                    break;
                default:
                    perror("Not valid number of roots");
                    break;
            }
        }

        numberOfSuccessfullyReadValues = fscanf(fp, "%d%lf%lf%lf%lf%lf", &numberOfRoots, &a, &b, &c, &testX1, &testX2);

        testNumber++;
    }
}

static inline void swap(double *a, double *b) {
    double tmp = NAN;
    tmp = *a;
    *a = *b;
    *b = tmp;
}
