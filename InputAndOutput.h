//
// Created by Arman on 15.08.2023.
//


/*! \file
 * \brief Contains functions for working with input and output
 */

#ifndef QUADRIC_INPUT_AND_OUTPUT_H
#define QUADRIC_INPUT_AND_OUTPUT_H

#include "solver.h"
//---------------------------------------------------------------------------------------------------------------------
/// \brief bit fields for command line flags
///
/// flags that indicates different input modes
typedef struct {
    unsigned int readFromFile        : 1; ///< flag indicates that input should be read from the file
    unsigned int readFromStdin       : 1; ///< flag indicates that input should be read from stdin
    unsigned int readFromCommandLine : 1; ///< flag indicates that input should be read from the command line

} CommandLineFlags;

//---------------------------------------------------------------------------------------------------------------------

/// \brief command line flags
const static char * FILEFlAG        = "-f"; ///< -f getting input from file (if "-c" flag is added, file name should be in command line,
///< otherwise program reads the file name from the standard input).
const static char * COMMANDLINEFLAG = "-c"; ///< -c getting input from commandline
const static char * STDINFLAG       = "-s"; ///< -s getting input from stdin. this flag is used if "-c" is not specified.
///< Could be used with "-f" to read file name from the standard input


//---------------------------------------------------------------------------------------------------------------------

/// \brief reading input based on command line flags
///
/// \param [in] argc number of arguments in argv
/// \param [in] argv command line arguments
/// \param [out] a pointer to the storage for a coefficient
/// \param [out] b pointer to the storage for b coefficient
/// \param [out] c pointer to the storage for c coefficient
/// \return 0 if reading is successful otherwise not 0
int ReadInput(int argc, const char *argv[], double *a, double *b, double *c);
//---------------------------------------------------------------------------------------------------------------------

/// \brief reading command line flags
///
/// \param [in] argc number of arguments in argv
/// \param [in] argv command line arguments
/// \return command line flags
CommandLineFlags ReadCommandLineFlags(int argc, const char *argv[]);

//---------------------------------------------------------------------------------------------------------------------
/// \brief scans coefficients for quadratic equation ax^2 + bx + c from standard input
///
/// \param [out] a pointer to the storage for a coefficient
/// \param [out] b pointer to the storage for b coefficient
/// \param [out] c pointer to the storage for c coefficient
/// \return 0 if reading is successful or not 0 if the user decided to quit the input
int ReadCoeffsFromStdin(double *a, double *b, double *c);
//---------------------------------------------------------------------------------------------------------------------

/// \brief read coefficients for quadratic equation ax^2 + bx + c from file
///
/// \param [out] a pointer to the storage for a coefficient
/// \param [out] b pointer to the storage for b coefficient
/// \param [out] c pointer to the storage for c coefficient
/// \param [in] fp pointer to the FILE
/// \return 0 if reading is successful otherwise not 0
int ReadCoeffsFromFile(double *a, double *b, double *c, FILE *fp);
//---------------------------------------------------------------------------------------------------------------------

/// \brief read file name from standard input
///
/// \param [out] name storage for file name
/// \param [in] size max size of file name (size of storage)
/// \return 0 if reading is successful otherwise not 0
int ReadFileNameFromStdin(char *name, size_t size);
//---------------------------------------------------------------------------------------------------------------------

/// \brief
///
/// \param [in] argc number of arguments in argv
/// \param [in] argv command line arguments
/// \param [out] name storage for file name
/// \param [in] size max size of file name (size of storage)
/// \return 0 if reading is successful otherwise not 0
int ReadFileNameFromCommandLine(int argc, const char *argv[], char *name, size_t size);
//---------------------------------------------------------------------------------------------------------------------

/// \brief print roots to the standard output
///
/// \param [in] numberOfRoots number of roots to be printed
/// \param [in] x1 root number 1
/// \param [in] x2 root number 2
/// \return 0 if printing is successful otherwise not 0
int PrintRoots(enum NumberOfRoots numberOfRoots, double x1, double x2);
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
int ReadCoeffsFromCommandLine(int argc, const char *argv[], double *a, double *b, double *c);

//---------------------------------------------------------------------------------------------------------------------
#endif
