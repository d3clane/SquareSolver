#include "Solver.h"

//---------------------------------------------------------------------------------------------------------------------

enum NumberOfRoots SolveQuadraticEquation(double a, double b, double c, double *x1, double *x2) {
    assert(isfinite(a));
    assert(isfinite(b));
    assert(isfinite(c));

    assert(x1 != NULL);
    assert(x2 != NULL);
    assert(x1 != x2);

    NumberOfRoots numberOfRoots = ZERO_ROOTS;

    if (Compare(a, 0.0) == EQUAL) {
        numberOfRoots = SolveLinearEquation(b, c, x1);
    } else {
        double D = b * b - 4 * a * c;
        ComparisonResult compareDiscriminantAndZero = Compare(D, 0.0);

        switch (compareDiscriminantAndZero) {
            case LESS:
                numberOfRoots = ZERO_ROOTS;
                break;
            case EQUAL:
                numberOfRoots = ONE_ROOT;
                *x1 = -b / (2 * a);
                break;
            default:
                numberOfRoots = TWO_ROOTS;
                double sqrtD = sqrt(D);
                *x1 = (-b - sqrtD) / (2 * a);
                *x2 = (-b + sqrtD) / (2 * a);
                break;
        }
    }
    return numberOfRoots;
}

//---------------------------------------------------------------------------------------------------------------------

NumberOfRoots SolveLinearEquation(double a, double b, double *x1) {
    assert(isfinite(a));
    assert(isfinite(b));

    assert(x1 != NULL);

    if (Compare(a, 0.0) == EQUAL) {
        return Compare(b, 0.0) == EQUAL ? INF_ROOTS : ZERO_ROOTS;
    }

    *x1 = -b / a;

    return ONE_ROOT;
}

//---------------------------------------------------------------------------------------------------------------------

NumberOfRoots ConvertNumberOfRootsToEnum(int numberOfRoots) {
    switch (numberOfRoots) {
        case 0:
            return ZERO_ROOTS;
        case 1:
            return ONE_ROOT;
        case 2:
            return TWO_ROOTS;
        default:
            return INF_ROOTS;
    }
}

//---------------------------------------------------------------------------------------------------------------------

const char *ConvertEnumToString(NumberOfRoots numberOfRoots) {
    switch (numberOfRoots) {
        case ZERO_ROOTS:
            return "zero roots";
        case ONE_ROOT:
            return "one root";
        case TWO_ROOTS:
            return "two roots";
        default:
            return "infinite number of roots";
    }
}

//---------------------------------------------------------------------------------------------------------------------
