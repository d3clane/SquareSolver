//
// Created by Arman on 15.08.2023.
//


/*! \file
 * \brief Contains functions for working with input and output
 */

/*! \defgroup InputOutputFuncs
 * \brief Functions for working with input and output
 */
#ifndef QUADRIC_INPUT_AND_OUTPUT_H
#define QUADRIC_INPUT_AND_OUTPUT_H

#include "solver.h"

/// \brief scans coefficients for quadratic equation ax^2 + bx + c from standard input
///
/// \param [out] a pointer to the storage for a coefficient
/// \param [out] b pointer to the storage for b coefficient
/// \param [out] c pointer to the storage for c coefficient
/// \return 0 if scanning is successful or not 0 if the user decided to quit the input
int ScanCoeffs(double *a, double *b, double *c);

/// \brief print roots to the standard output
///
/// \param [in] numberOfRoots number of roots to be printed
/// \param [in] x1 root number 1
/// \param [in] x2 root number 2
/// \return 0 if printing is successful otherwise not 0
int PrintRoots(const enum NumberOfRoots numberOfRoots, const double x1, const double x2);
#endif
