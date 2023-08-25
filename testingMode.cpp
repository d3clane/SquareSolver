//
// Created by Arman on 18.08.2023.
//

#include "TestingMode.h"

//---------------------------------------------------------------------------------------------------------------------

const char *const FILE_NAME = "test.txt";
const char *const FILE_MODE = "r";   

//---------------------------------------------------------------------------------------------------------------------

Errors Testing() {
    FILE *fp = fopen(FILE_NAME, FILE_MODE);

    if (!fp) {
        UpdateError(Errors::FILE_OPENING_ERROR);
        return      Errors::FILE_OPENING_ERROR;
    }

    static const int HAVE_TO_READ_VALUES_AT_ONCE = 6;

    int numberOfRoots = 0;

    double a = NAN, b = NAN, c = NAN;
    double testX1 = NAN, testX2 = NAN;

    int testNumber = 1;

    while (true) {
        int numberOfSuccessfullyReadValues = fscanf(fp, "%d %lf %lf %lf %lf %lf", 
                                                    &numberOfRoots, &a, &b, &c, &testX1, &testX2);
    
        if (numberOfSuccessfullyReadValues != HAVE_TO_READ_VALUES_AT_ONCE) 
            break;
        
        TestOneEquation(a, b, c, testX1, testX2, numberOfRoots, testNumber);

        testNumber++;
    }

    return Errors::NO_ERRORS;
}

//---------------------------------------------------------------------------------------------------------------------

#define TEST_FAILED "Test %d failed:\nprogram result: "
#define TEST_OK "Test %d: test is OK\n"

Errors TestOneEquation(double a, double b, double c, 
                       double testX1, double testX2,
                       int numberOfRoots, int testNumber) {
    assert(isfinite(a));
    assert(isfinite(b));
    assert(isfinite(c));
    assert(isfinite(testX1));
    assert(isfinite(testX2));

    double myX1 = NAN, myX2 = NAN;

    NumberOfRoots myNumberOfRoots = SolveQuadraticEquation(a, b, c, &myX1, &myX2);
    NumberOfRoots testNumberOfRoots = (numberOfRoots >= 0 && numberOfRoots <= 2) ? 
                                      (NumberOfRoots) numberOfRoots : INF_ROOTS;

    if (testNumberOfRoots != myNumberOfRoots) {
        printf("Test %d: number of roots do not match\n", testNumber);

        printf("program expected %s, test has %s\n",
                ConvertEnumToString(myNumberOfRoots),
                ConvertEnumToString(testNumberOfRoots));
    } else {
        switch (myNumberOfRoots) {
            case ZERO_ROOTS:
                printf(TEST_OK, testNumber);
                break;

            case ONE_ROOT:
                if (Compare(testX1, myX1) == EQUAL) {
                    printf(TEST_OK, testNumber);
                } else {
                    printf(TEST_FAILED "%lf  test value: %lf\n",
                            testNumber, myX1, testX1);
                }

                break;

            case TWO_ROOTS:
                if (Compare(testX1, testX2) == GREATER) 
                    Swap((void *) &testX1, (void *) &testX2, sizeof(testX1)); // testX1 <= testX2

                if (Compare(testX1, myX1) == EQUAL && Compare(testX2, myX2) == EQUAL) {
                    printf(TEST_OK, testNumber);
                } else {
                    printf(TEST_FAILED "%lf and %lf  test values: %lf and %lf\n",
                            testNumber, myX1, myX2, testX1, testX2);
                }

                break;
            case INF_ROOTS:
                printf(TEST_OK, testNumber);
                break;

            default:
                UpdateError(Errors::INVALID_NUMBER_OF_ROOTS);
                return      Errors::INVALID_NUMBER_OF_ROOTS;
        }
    }

    return Errors::NO_ERRORS;    
}

#undef TEST_OK
#undef TEST_FAILED


//---------------------------------------------------------------------------------------------------------------------
