//
// Created by Arman on 18.08.2023.
//

/// \file
/// \brief Contains functions for comparisons of double values

#ifndef DOUBLE_COMPARISONS_H
#define DOUBLE_COMPARISONS_H

#include <math.h>

//--------------------------------------------------------------------------------------------------------------------

static const double EPS = 1e-6; ///< comparisons accuracy

//--------------------------------------------------------------------------------------------------------------------
/// \brief checks if two double values are equal
///
/// \param [in] a first value
/// \param [in] b second value
/// \return true (1) if a equals b otherwise false (0)
int equal(double a, double b);

//--------------------------------------------------------------------------------------------------------------------
/// \brief checks if the first value (a) is less than the second one (b)
///
/// \param [in] a first value
/// \param [in] b second value
/// \return true (1) if a is less than b otherwise false (0)
int less(double a, double b);

//--------------------------------------------------------------------------------------------------------------------
/// \brief checks if the first value (a) is greater than the second one (b)
///
/// \param [in] a first value
/// \param [in] b second value
/// \return true (1) if a is greater b otherwise false (0)
int greater(double a, double b);

//--------------------------------------------------------------------------------------------------------------------
#endif //DOUBLE_COMPARISONS_H
