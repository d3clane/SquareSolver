//
// Created by Arman on 21.08.2023.
//

/// \file
/// \brief Contains errors that may occur during the program working.
/// \details Contains errors info and function to print these errors.

#ifndef ERRORS_H
#define ERRORS_H

//#define NDEBUG

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "Colors.h"

//---------------------------------------------------------------------------------------------------------------------

/// \brief Errors than may occur during the program working. 
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

    DOUBLE_IS_OUT_OF_RANGE                    = 19,
};

//---------------------------------------------------------------------------------------------------------------------

/// \brief max length file name in my directory
const int MAX_MY_FILE_NAME = 64;

//---------------------------------------------------------------------------------------------------------------------

/// \brief Contains info about errors - File with error, line with error, error code. 
/// \warning Have to be updated with UpdateError() only
struct ErrorInfo_t {
    Errors error; ///< error code
    char fileWithError[MAX_MY_FILE_NAME]; ///< __FILE__ (file name with error)
    int lineWithError; ///< __LINE__ (line with error)
};

/// \brief global errorInfo constant with error info
/// \warning this variable have to be changes only with UpdateError()
extern ErrorInfo_t ErrorInfo;

//---------------------------------------------------------------------------------------------------------------------

#ifndef NDEBUG

/// \brief updates special struct with errors errorInfo 
/// \details copyFileName copy of __FILE__ define at the moment macros is called 
/// \details copyLineNumber __LINE__ define at the moment macros is valled
/// \param [in] ERROR Errors enum with error occurred in program
/// \attention macros is multiline and so in if clause have to be in brackets
#define UpdateError(ERROR) {                   \
    strcpy(ErrorInfo.fileWithError, __FILE__); \
    ErrorInfo.lineWithError = __LINE__;        \
    ErrorInfo.error = ERROR;                   \
    }

#else

/// \brief updates only error code without debug info
/// \param [in] ERROR
#define UpdateError(ERROR) ErrorInfo.error = ERROR

#endif

//---------------------------------------------------------------------------------------------------------------------

/// \brief print errors from special struct with errors errorInfo.
/// \attention errorInfo have to be updated only with UpdateError()
/// \return 0 if program doesn't have serious errors otherwise 1
int PrintErrors();

//---------------------------------------------------------------------------------------------------------------------

/// \brief check all three coefficients isfinite()
/// \param [in] a   quadratic coefficient
/// \param [in] b   linear coefficent
/// \param [in] c   free coefficient
/// \return error if one of the coeffs is not finite
Errors CheckCoeffsIsFinite(const double a, const double b, const double c);

//---------------------------------------------------------------------------------------------------------------------

#endif // ERRORS_H
