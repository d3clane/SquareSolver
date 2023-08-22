//
// Created by Arman on 19.08.2023.
//

/// \file
/// \brief Contains functions that works with string form of the equation

#ifndef STRING_EQUATION_FUNCS_H
#define STRING_EQUATION_FUNCS_H

#include "StringAndCharFuncs.h"

//---------------------------------------------------------------------------------------------------------------------

/// \brief enum returning power of the x value in the equation
enum PowerOfX {
    POWER_ZERO    =  0, ///< free coefficient
    POWER_ONE     =  1, ///< linear coefficient
    POWER_TWO     =  2, ///< quadratic coefficient
    INVALID_POWER = -1, ///< coefficient is non of previous ones
};

//---------------------------------------------------------------------------------------------------------------------

static const size_t MAX_EQUATION_SIZE = 100; ///< max size of the equation that can be read

//---------------------------------------------------------------------------------------------------------------------

/// \brief Parsing the quadratic equation and returning coefficients
/// \details This functions is parsing the equation, sums the coefficients of common multipliers and returns them
/// \param [in] equation equation format: 5x^2 + 3x - 2 = 7x + 21
/// \param [out] a  quadratic coefficient
/// \param [out] b  linear coefficient
/// \param [out] c  free coefficient
/// \return 0 if parsing is successful otherwise not 0
Errors ParseQuadraticEquation(const char *equation, double *a, double *b, double *c);

//---------------------------------------------------------------------------------------------------------------------

/// \brief Transposing the equation
/// \warning Spaces have to be deleted before using this function.
/// \details This function: \n
/// 1) adds coefficients '1' before x-s with no coefficients. \n
/// 2) adds signs character before coefficients with no sign. \n
/// 3) delete equal (=) sign and transposes coefficients to the left side of the equation
/// \param [out] copyEquation string that will be transposed
Errors TransposeEquation(char *copyEquation);

//---------------------------------------------------------------------------------------------------------------------

/// \brief Get coefficient and power of x
///
/// \warning before using have to 1) DeleteSpaces() 2) TransposeEquation()
/// \param [in] x pointer to string. Argument have to start with sign / coefficient.
/// \param [out] target pointer to storage for x coefficient
/// \param [out] endPtr pointer to the storage for pointer to the ending of X
/// \return Power of x
/// ### Example
/// ~~~~~~~~~~~~~~~~~~~~~.cpp
/// enum PowOfX powOfX = GetXPowAndCoeff("+2.2x^2-3=0", &tmpVal, &endPtr);
/// // powOfX = POWER_TWO
/// // tmpVal = 2.2
/// // endPtr = "-3=0"
/// ~~~~~~~~~~~~~~~~~~~~~
PowerOfX GetXPowAndCoeff(const char *x, double *target, char **endPtr);

//---------------------------------------------------------------------------------------------------------------------

/// \brief checks equation format
/// checks if the equation has correct format and can be parsed
/// \param [in] equation equation to check
/// \return NO_ERRORS if equation is OK otherwise INVALID_EQUATION_FORMAT
Errors CheckEquation(const char *equation);

//---------------------------------------------------------------------------------------------------------------------

#endif
