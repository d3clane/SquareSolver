#include "Solver.h"

//---------------------------------------------------------------------------------------------------------------------

enum NumberOfRoots SolveQuadraticEquation(const double a, const double b, const double c, double *x1, double *x2) {
    assert(isfinite(a));
    assert(isfinite(b));
    assert(isfinite(c));

    assert(x1);
    assert(x2);
    assert(x1 != x2);

    NumberOfRoots numberOfRoots = ZERO_ROOTS;
    if (Compare(a, 0.0) == EQUAL) {
        numberOfRoots = SolveLinearEquation(b, c, x1);

    } else {

        if (Compare(c, 0.0) == EQUAL) {

            if (Compare(b, 0.0) == EQUAL) {
                numberOfRoots = ONE_ROOT;
                *x1 = 0;
            } else {
                numberOfRoots = TWO_ROOTS;
                *x1 = 0;
                *x2 = -b / a;
            }

            if (numberOfRoots == TWO_ROOTS && Compare(*x1, *x2) != LESS)
                Swap((void *) x1, (void *) x2, sizeof(*x1));

            return numberOfRoots;       
        }

        double D = b * b - 4 * a * c; //case c = 0 check 
        ComparisonResult compareDiscriminantAndZero = Compare(D, 0.0);

        switch (compareDiscriminantAndZero) {
            case LESS:
                numberOfRoots = ZERO_ROOTS;
                break;

            case EQUAL:
                numberOfRoots = ONE_ROOT;
                *x1 = -b / (2 * a);
                break;
            
            case GREATER:
            default:
                numberOfRoots = TWO_ROOTS;
                double sqrtD = sqrt(D);
                *x1 = (-b - sqrtD) / (2 * a);
                *x2 = (-b + sqrtD) / (2 * a);
                break;
        }
    }

    if (Compare(*x1, *x2) != LESS) 
        Swap((void *) x1, (void *) x2, sizeof(*x1));
    
    return numberOfRoots;
}

//---------------------------------------------------------------------------------------------------------------------

NumberOfRoots SolveLinearEquation(const double a, const double b, double *x1) {
    assert(isfinite(a));
    assert(isfinite(b));

    assert(x1);

    if (Compare(a, 0.0) == EQUAL) {
        return Compare(b, 0.0) == EQUAL ? INF_ROOTS : ZERO_ROOTS;
    }

    *x1 = -b / a;

    return ONE_ROOT;
}

//---------------------------------------------------------------------------------------------------------------------

const char *ConvertEnumToString(const NumberOfRoots numberOfRoots) {
    switch (numberOfRoots) {
        case ZERO_ROOTS: return "zero roots";
        case ONE_ROOT:   return "one root";
        case TWO_ROOTS:  return "two roots";
        case INF_ROOTS:  return "infinite number of roots";
        default:         return "invalid number of roots";
    }
}

//---------------------------------------------------------------------------------------------------------------------
