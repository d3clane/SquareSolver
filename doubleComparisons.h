//
// Created by Arman on 18.08.2023.
//

/// \file
/// \brief Contains functions for comparisons of double values

#ifndef DOUBLE_COMPARISONS_H
#define DOUBLE_COMPARISONS_H

#include <math.h>

//--------------------------------------------------------------------------------------------------------------------

static const double EPS = 1e-9; ///< comparisons accuracy

//--------------------------------------------------------------------------------------------------------------------

/// \brief enum of comparison of two doubles result
enum ComparisonResult {
    LESS    = -1, ///< first value is less than second one
    EQUAL   =  0, ///< first value is equal to second one
    GREATER =  1, ///< first value is greater than second one
};

//--------------------------------------------------------------------------------------------------------------------

/// \brief Compare two doubles and returns comparison result
///
/// \param [in] a first value
/// \param [in] b second value
/// \return enum ComparisonResult - result of comparing (LESS / EQUAL / GREATER)
ComparisonResult Compare(double a, double b);

//--------------------------------------------------------------------------------------------------------------------

#endif //DOUBLE_COMPARISONS_H
