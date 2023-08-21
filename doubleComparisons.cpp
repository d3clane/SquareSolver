//
// Created by Arman on 18.08.2023.
//

#include "DoubleComparisons.h"

int equal(double a, double b) {
    return fabs(a - b) < EPS;
}

int less(double a, double b) {
    return b - a > EPS;
}

int greater(double a, double b) {
    return a - b > EPS;
}
