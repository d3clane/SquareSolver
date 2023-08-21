//
// Created by Arman on 18.08.2023.
//

#include "DoubleComparisons.h"

ComparisonResult Compare(double a, double b) {
    ComparisonResult result = EQUAL;

    if (b - a > EPS) {
        result = LESS;
    } else if (a - b > EPS) {
        result = GREATER;
    } else {
        result = EQUAL;
    }

    return result;
}
