//
// Created by Arman on 18.08.2023.
//

#include "TestingMode.h"

static inline void Swap(double *a, double *b);

#define TESTFAILED "Test %d failed:\nprogram result: "
#define TESTOK "Test %d: test is OK\n"
//---------------------------------------------------------------------------------------------------------------------

Errors Testing() {
    FILE *fp = fopen(FILE_NAME, FILE_MODE);

    if (fp == NULL) {
        return Errors::FILE_OPENING_ERROR;
    }

    static const int HAVE_TO_READ_VALUES_AT_ONCE = 6;

    int numberOfRoots = 0;

    double a = NAN, b = NAN, c = NAN;
    double testX1 = NAN, testX2 = NAN;

    int testNumber = 1;

    while (true) {
        int numberOfSuccessfullyReadValues = fscanf(fp, "%d %lf %lf %lf %lf %lf", &numberOfRoots, &a, &b, &c, &testX1, &testX2);
    
        if (numberOfSuccessfullyReadValues != HAVE_TO_READ_VALUES_AT_ONCE) 
            break;
        
        TestOneEquation(a, b, c, testX1, testX2, numberOfRoots, testNumber);

        testNumber++;
    }

    return Errors::NO_ERRORS;
}

//---------------------------------------------------------------------------------------------------------------------

Errors TestOneEquation(double a, double b, double c, 
                       double testX1, double testX2,
                       int numberOfRoots, int testNumber) {
    double myX1 = NAN, myX2 = NAN;

    NumberOfRoots myNumberOfRoots = SolveQuadraticEquation(a, b, c, &myX1, &myX2);
    NumberOfRoots testNumberOfRoots = (numberOfRoots >= 0 && numberOfRoots <= 2) ? (NumberOfRoots) numberOfRoots : INF_ROOTS;

    if (testNumberOfRoots != myNumberOfRoots) {
        printf("Test %d: number of roots do not match\n", testNumber);

        printf("program expected %s, test has %s\n",
                ConvertEnumToString(myNumberOfRoots),
                ConvertEnumToString(testNumberOfRoots));
    } else {
        switch (myNumberOfRoots) {
            case ZERO_ROOTS:
                printf(TESTOK, testNumber);
                break;
            case ONE_ROOT:
                if (Compare(testX1, myX1) == EQUAL) {
                    printf(TESTOK, testNumber);
                } else {
                    printf(TESTFAILED "%lf  test value: %lf\n",
                            testNumber, myX1, testX1);
                }

                break;
            case TWO_ROOTS:
                if (Compare(testX1, testX2) == GREATER) Swap(&testX1, &testX2); // testX1 <= testX2

                if (Compare(testX1, myX1) == EQUAL && Compare(testX2, myX2) == EQUAL) {
                    printf(TESTOK, testNumber);
                } else {
                    printf(TESTFAILED "%lf and %lf  test values: %lf and %lf\n",
                            testNumber, myX1, myX2, testX1, testX2);
                }

                break;
            case INF_ROOTS:
                printf(TESTOK, testNumber);
                break;
            default:
                return Errors::INVALID_NUMBER_OF_ROOTS;
        }
    }

    return Errors::NO_ERRORS;    
}

//---------------------------------------------------------------------------------------------------------------------

static inline void Swap(double *a, double *b) {
    double tmp = *a;
                 *a = *b;
                      *b = tmp;
}

#undef TESTOK
#undef TESTFAILED
