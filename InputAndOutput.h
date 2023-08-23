//
// Created by Arman on 15.08.2023.
//


/*! \file
 * \brief Contains functions for working with input and output
 */

#ifndef INPUT_AND_OUTPUT_H
#define INPUT_AND_OUTPUT_H

#include <stdio.h>

#include "Errors.h"
#include "Solver.h"
#include "StringAndCharFuncs.h"
#include "StringEquationFuncs.h"

//---------------------------------------------------------------------------------------------------------------------

/// \brief bit fields for command line flags
///
/// flags that indicates different input modes
typedef struct {
    unsigned char readFromFile        : 1; ///< flag indicates that input should be read from the file
    unsigned char readFromStdin       : 1; ///< flag indicates that input should be read from stdin
    unsigned char readFromCommandLine : 1; ///< flag indicates that input should be read from the command line
    unsigned char testMode            : 1; ///< flag indicates that program is started in testing mode
    unsigned char equationInputMode   : 1; ///< flag indicated that program have to read equation not only coefficients
} CommandLineFlags;

//---------------------------------------------------------------------------------------------------------------------
/// \brief command line flags

///< -f getting input from file (if "-c" flag is added, file name have to be in command line,
///< otherwise program reads the file name from the standard input).
const extern char *FILE_FlAG;

///< -c getting input from commandline
const extern char *COMMAND_LINE_FLAG;

///< -s getting input from stdin. this flag is used if "-c" is not specified.
///< Could be used with "-f" to read file name from the standard input
const extern char *STDIN_FLAG;

/// < -t program enters testing mode
const extern char *TEST_MODE_FLAG;

///< program reads equation not only coefficients
const extern char *EQUATION_INPUT_MODE_FLAG;

//---------------------------------------------------------------------------------------------------------------------

/// \brief reading input based on command line flags
///
/// \param [in] argc number of arguments in argv
/// \param [in] argv command line arguments
/// \param [out] a pointer to the storage for a coefficient
/// \param [out] b pointer to the storage for b coefficient
/// \param [out] c pointer to the storage for c coefficient
/// \return 0 if reading is successful otherwise not 0
Errors ReadInput(const int argc, const char *argv[], double *a, double *b, double *c);

//---------------------------------------------------------------------------------------------------------------------

/// \brief reading command line flags
///
/// \param [in] argc number of arguments in argv
/// \param [in] argv command line arguments
/// \return command line flags
CommandLineFlags ReadCommandLineFlags(const int argc, const char *argv[]);

//---------------------------------------------------------------------------------------------------------------------
/// \brief scans coefficients for quadratic equation ax^2 + bx + c from standard input
///
/// \param [out] a pointer to the storage for a coefficient
/// \param [out] b pointer to the storage for b coefficient
/// \param [out] c pointer to the storage for c coefficient
/// \return 0 if reading is successful or not 0 if the user decided to quit the input
Errors ReadCoeffsFromStdin(double *a, double *b, double *c);

//---------------------------------------------------------------------------------------------------------------------

/// \brief read coefficients for quadratic equation ax^2 + bx + c from file
///
/// \param [out] a pointer to the storage for a coefficient
/// \param [out] b pointer to the storage for b coefficient
/// \param [out] c pointer to the storage for c coefficient
/// \param [in] fp pointer to the FILE
/// \return 0 if reading is successful otherwise not 0
Errors ReadCoeffsFromFile(double *a, double *b, double *c, FILE *fp);

//---------------------------------------------------------------------------------------------------------------------

/// \brief read file name from standard input
///
/// \param [out] name storage for file name
/// \param [in] size max size of file name (size of storage)
/// \return 0 if reading is successful otherwise not 0
Errors ReadFileNameFromStdin(char *name, const size_t size);

//---------------------------------------------------------------------------------------------------------------------

/// \brief
///
/// \param [in] argc number of arguments in argv
/// \param [in] argv command line arguments
/// \param [out] name storage for file name
/// \param [in] size max size of file name (size of storage)
/// \return 0 if reading is successful otherwise not 0
Errors ReadFileNameFromCommandLine(const int argc, const char *argv[], char *name, const size_t size);

//---------------------------------------------------------------------------------------------------------------------

/// \brief print roots to the standard output
///
/// \param [in] numberOfRoots number of roots to be printed
/// \param [in] x1 root number 1
/// \param [in] x2 root number 2
/// \return 0 if printing is successful otherwise not 0
Errors PrintRoots(const NumberOfRoots numberOfRoots, const double x1, const double x2);

//---------------------------------------------------------------------------------------------------------------------

/// \brief reads coefficients from the command line
///
/// Read from command line only in case flag "-c" is specified right before the coefficients
/// \param [in] argc number of values in argv
/// \param [in] argv parameters from the command line
/// \param [out] a pointer to the storage for a coefficient
/// \param [out] b pointer to the storage for b coefficient
/// \param [out] c pointer to the storage for c coefficient
/// \return 0 if reading is successful otherwise not 0
Errors ReadCoeffsFromCommandLine(const int argc, const char *argv[], double *a, double *b, double *c);

//---------------------------------------------------------------------------------------------------------------------

/// \brief reads equation from command line and converts to coefficients
/// equation format: 5x^2 + 3x - 1 = 7x + 2
/// \param [in] argc number of argv values
/// \param [in] argv command line arguments
/// \param [out] a  quadratic coefficient
/// \param [out] b  linear coefficient
/// \param [out] c  free coefficient
/// \return 0 if reading is successful otherwise not 0
Errors ReadEquationCoeffsFromCommandLine(const int argc, const char *argv[], double *a, double *b, double *c);

//---------------------------------------------------------------------------------------------------------------------

///\brief read equation from stdin and converts to coefficients
/// equation format: 5x^2 + 3x - 1 = 7x + 2
/// \param [out] a  quadratic coefficient
/// \param [out] b  linear coefficient
/// \param [out] c  free coefficient
/// \return 0 if reading is successful otherwise not 0
Errors ReadEquationCoeffsFromStdin(double *a, double *b, double *c);

//---------------------------------------------------------------------------------------------------------------------

///\brief read equation from file and converts to coefficients
/// equation format: 5x^2 + 3x - 1 = 7x + 2
/// \param [out] a  quadratic coefficient
/// \param [out] b  linear coefficient
/// \param [out] c  free coefficient
/// \param [in] fp pointer to the FILE
/// \return 0 if reading is successful otherwise not 0
Errors ReadEquationCoeffsFromFile(double *a, double *b, double *c, FILE *fp);

//---------------------------------------------------------------------------------------------------------------------

/// \brief fgets with deleting new line symbol
///
/// \param [out] name
/// \param [in] size
/// \return 0 if reading is successful otherwise not 0
Errors Fgets_s(char *name, const size_t size, FILE *fp);

//---------------------------------------------------------------------------------------------------------------------

/// \brief 
/// \param [in] str string with read input from stdin
/// \param [in] size - size of str buffer 
/// \param [in] file from which str was read
/// \return true (1) if fgets has read all string. false (0) if something is in the string (except '\n');
bool HasReadAllStringWithFgets(const char *str, const size_t size, FILE *fp);

#endif // INPUT_AND_OUTPUT_H
