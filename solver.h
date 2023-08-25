//
// Created by Arman on 15.08.2023.
//

/*! \file
 * \brief Contains functions for solving equations
 *
 * supports solving quadratic and linear equations
 */

#ifndef SOLVER_H
#define SOLVER_H

#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "DoubleFuncs.h"
#include "Errors.h"

//--------------------------------------------------------------------------------------------------------------------
/// \brief enum for number of roots of the equation
enum NumberOfRoots {
    ZERO_ROOTS = 0, ///< case there is no solutions for the equation
    ONE_ROOT   = 1, ///< case there is one solution for the equation
    TWO_ROOTS  = 2, ///< case there is two different solutions for the equation
    INF_ROOTS  = -1, ///< case there is infinite number of solutions for the equation
};

//--------------------------------------------------------------------------------------------------------------------
/// \brief converts enum NumberOfRoots to string
///
/// \param [in] numberOfRoots number of roots
/// \return string number of roots
/// ConvertEnumToString(numberOfRoots) returns "INF_ROOTS"
const char *ConvertEnumToString(const NumberOfRoots numberOfRoots);

//--------------------------------------------------------------------------------------------------------------------

/// \brief solves quadratic equation
///
/// \param [in] a quadratic coefficient of x^2
/// \param [in] b linear coefficient of x
/// \param [in] c free coefficient
/// \param [out] x1 pointer to storage for root1
/// \param [out] x2 pointer to storage for root2
/// \return number of roots
NumberOfRoots SolveQuadraticEquation(const double a, const double b, const double c, double *x1, double *x2);

//--------------------------------------------------------------------------------------------------------------------
/// \brief solves linear equation
///
/// \param [in] a linear coefficient of x
/// \param [in] b free coefficient
/// \param [out] x1 pointer to the storage for the root
/// \return number of roots
NumberOfRoots SolveLinearEquation(const double a, const double b, double *x1);

//--------------------------------------------------------------------------------------------------------------------
#endif //SOLVER_H
