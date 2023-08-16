//
// Created by Arman on 15.08.2023.
//

#ifndef QUADRIC_SOLVER_H
#define QUADRIC_SOLVER_H


enum NumberOfRoots {
    ZERO_ROOTS = 0,
    ONE_ROOT = 1,
    TWO_ROOTS = 2,
    INF_ROOTS = -1,
};

//solves Quadratic Equation
int SolveQuadraticEquation(double a, double b, double c, double *x1, double *x2);

#endif //QUADRIC_SOLVER_H
