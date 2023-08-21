//
// Created by Arman on 21.08.2023.
//

/// \file
/// \brief Contains errors that may occur during the program working.
/// \details Contains errors enum and function to print these errors.

#ifndef ERRORS_H
#define ERRORS_H

#include <stdio.h>


/// \brief Errors than may occur during the program working
enum class Errors {
    NO_ERRORS = 0, ///< no errors occurred
    FILE_OPENING_ERROR, ///< error occurred while opening file
    CONVERTING_STRING_TO_DOUBLE_ERROR, ///< error occurred while converting string to double
    INVALID_NUMBER_OF_ROOTS, ///< error occurred because of incorrect number of roots given
    READING_COEFFS_FROM_STDIN_ERROR, ///< error occurred while reading coefficients from stdin
    READING_FILE_NAME_FROM_STDIN_ERROR, ///< error occurred while reading file name from stdin
    READING_EQUATION_FROM_STDIN_ERROR, ///< error occurred while reading equation from stdin
    READING_COEFFS_FROM_FILE_ERROR, ///< error occurred while reading coefficients from file
    READING_EQUATION_FROM_FILE_ERROR, ///< error occurred while reading equation from file
    READING_EQUATION_FROM_COMMAND_LINE_ERROR, ///< error occurred while reading equation from command line
    READING_COEFFS_FROM_COMMAND_LINE_ERROR, ///< error occurred while reading coefficients from command line
    READING_FILE_NAME_FROM_COMMAND_LINE_ERROR, ///< error occurred while reading file name from command line
    INVALID_EQUATION_FORMAT, ///< error occurred because of incorrect equation format
    INVALID_POWER_OF_X, ///< error occurred because of incorrect power of x in the equation
    QUIT_THE_PROGRAM_WITHOUT_INPUT, ///< user decided to quit the program
    UNKNOWN_COMMAND_LINE_FLAG, ///< error occurred because of unknown command line flag given
    READING_FROM_STDIN_ERROR, ///< error occurred while reading from stdin
    INVALID_STRING_NO_ENDING, ///< error occurs when given string has no ending
};

/// \brief print errors
///
/// \param [in] errors error to print
/// \return 0 if program doesn't have serious errors otherwise 1
int PrintErrors(Errors errors);

#endif
