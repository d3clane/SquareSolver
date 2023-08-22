//
// Created by Arman on 18.08.2023.
//

#include "TestingMode.h"

static inline void Swap(double *a, double *b);

//---------------------------------------------------------------------------------------------------------------------

Errors Testing() {
    FILE *fp = fopen(FILE_NAME, FILE_MODE);

    if (fp == NULL) {
        return Errors::FILE_OPENING_ERROR;
    }

    static const int haveToReadValuesAtOnce = 6;

    int numberOfRoots = 0;

    double a = NAN, b = NAN, c = NAN;
    double testX1 = NAN, testX2 = NAN;
    double myX1 = NAN, myX2 = NAN;

    int numberOfSuccessfullyReadValues = fscanf(fp, "%d%lf%lf%lf%lf%lf", &numberOfRoots, &a, &b, &c, &testX1, &testX2);
    int testNumber = 1;
    while (numberOfSuccessfullyReadValues == haveToReadValuesAtOnce) {
        NumberOfRoots myNumberOfRoots = SolveQuadraticEquation(a, b, c, &myX1, &myX2);
        NumberOfRoots testNumberOfRoots = ConvertNumberOfRootsToEnum(numberOfRoots);

        if (testNumberOfRoots != myNumberOfRoots) {
            printf("Test %d: number of roots do not match\n", testNumber);

            printf("program expected %s, test has %s\n",
                   ConvertEnumToString(myNumberOfRoots),
                   ConvertEnumToString(testNumberOfRoots));
        } else {
            switch (myNumberOfRoots) {
                case ZERO_ROOTS:
                    printf("Test %d: test is OK\n", testNumber);
                    break;
                case ONE_ROOT:
                    if (Compare(testX1, myX1) == EQUAL) {
                        printf("Test %d: test is OK\n", testNumber);
                    } else {
                        printf("Test %d failed:\nprogram result: %lf  test value: %lf\n",
                               testNumber, myX1, testX1);
                    }

                    break;
                case TWO_ROOTS:
                    if (Compare(testX1, testX2) == GREATER) Swap(&testX1, &testX2); // testX1 <= testX2

                    if (Compare(testX1, myX1) == EQUAL && Compare(testX2, myX2) == EQUAL) {
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
                    return Errors::INVALID_NUMBER_OF_ROOTS;
            }
        }

        numberOfSuccessfullyReadValues = fscanf(fp, "%d%lf%lf%lf%lf%lf", &numberOfRoots, &a, &b, &c, &testX1, &testX2);

        testNumber++;
    }

    return Errors::NO_ERRORS;
}

//---------------------------------------------------------------------------------------------------------------------

static inline void Swap(double *a, double *b) {
    double tmp = NAN;
    tmp = *a;
    *a = *b;
    *b = tmp;
}
