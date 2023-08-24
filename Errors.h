//
// Created by Arman on 21.08.2023.
//

/// \file
/// \brief Contains errors that may occur during the program working.
/// \details Contains errors enum and function to print these errors.

#ifndef ERRORS_H
#define ERRORS_H

#include <stdio.h>
#include <math.h>

//---------------------------------------------------------------------------------------------------------------------

/// \brief Errors than may occur during the program working
enum class Errors {
    NO_ERRORS                                 =  0,
    QUIT_THE_PROGRAM_WITHOUT_INPUT            =  1, ///< user decided to quit the program

    FILE_OPENING_ERROR                        =  2,

    UNKNOWN_COMMAND_LINE_FLAG                 =  3, ///< user used unknown flag

    READING_FROM_STDIN_ERROR                  =  4,
    READING_COEFFS_FROM_STDIN_ERROR           =  5,
    READING_FILE_NAME_FROM_STDIN_ERROR        =  6,
    READING_EQUATION_FROM_STDIN_ERROR         =  7,
    READING_COEFFS_FROM_FILE_ERROR            =  8,
    READING_EQUATION_FROM_FILE_ERROR          =  9,
    READING_EQUATION_FROM_COMMAND_LINE_ERROR  = 10,
    READING_COEFFS_FROM_COMMAND_LINE_ERROR    = 11,
    READING_FILE_NAME_FROM_COMMAND_LINE_ERROR = 12,

    HELPING_ERROR                             = 20, ///< no flag to be described by flag 
    NO_HELPING_FLAG                           = 22, ///< no -h flag but help functions are used

    INVALID_EQUATION_FORMAT                   = 13,
    INVALID_POWER_OF_X                        = 14, ///< power of x is out of the permitted range
    INVALID_NUMBER_OF_ROOTS                   = 15, ///< number of roots is out of the permitted range

    INVALID_STRING_NO_ENDING                  = 17,
    EXTRA_SYMBOLS_IN_LINE                     = 18, ///< user printed extra symbols

    DOUBLE_IS_OUT_OF_RANGE                     = 19,
};

//---------------------------------------------------------------------------------------------------------------------

/// \brief print errors
///
/// \param [in] errors error to print
/// \return 0 if program doesn't have serious errors otherwise 1
int PrintErrors(const Errors errors);

//---------------------------------------------------------------------------------------------------------------------

/// \brief check all three coefficients isfinite()
/// \param [in] a   quadratic coefficient
/// \param [in] b   linear coefficent
/// \param [in] c   free coefficient
/// \return
Errors CheckCoeffsIsFinite(const double a, const double b, const double c);


#endif //ERRORS_H
