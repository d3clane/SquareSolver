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
#include "TestingMode.h"

//---------------------------------------------------------------------------------------------------------------------

static const size_t FILE_NAME_LENGTH = 64;

static const size_t LONG_FLAG_LENGTH = 20;
static const size_t SHORT_FLAG_LENGTH = 5;

#ifndef NDEBUG
static const size_t FLAGS_NUMBER = 6;
#else
static const size_t FLAGS_NUMBER = 5;
#endif

//---------------------------------------------------------------------------------------------------------------------

/// \brief Contains short and long names of command line flags. Also contains pointer to the flag Help
struct CommandLineFlagsType {
    char short_flag[SHORT_FLAG_LENGTH]; ///< short naming of the flag
    char long_flag[LONG_FLAG_LENGTH];   ///< long naming of the flag

    void (*Help)(); ///< pointer to the function with help for flag
};

//---------------------------------------------------------------------------------------------------------------------

/// \brief max number of funcs can be on one flag
static const int MAX_FUNC_NUMBER = 3;

//---------------------------------------------------------------------------------------------------------------------

/// \brief Contains functions to call for every reading type flag
struct TypesReadFuncsType {
    typedef Errors (*FlagFunc)(const void *const storage);

    /// \brief IDs in flagFuncs array for every input type
    enum ReadingId {
        FILE  = 0,
        STDIN = 1,
        CMD   = 2,
    };

    FlagFunc flagFuncs[MAX_FUNC_NUMBER]; ///< functions that can be used with this flag
};

//---------------------------------------------------------------------------------------------------------------------

/// \brief ids of differrent flags in the array in cpp file
enum class FlagsIdInArray {
    COMMAND_LINE_FLAG   = 0,
    STDIN_FLAG          = 1,
    FILE_FLAG           = 2,
    EQUATION_INPUT_FLAG = 3,
    HELP_FLAG           = 4,

#ifndef NDEBUG
    TEST_MODE_FLAG      = 5,
#endif
};

//---------------------------------------------------------------------------------------------------------------------

/// \brief Contains pointers on quadratic coefficients
struct CoeffsPtrs {
    double *const a, *const b, *const c;
};

/// \brief Conttains pointer on storage for file name and size of this storage
struct FileNameParams {
    char *name;
    const size_t size;
};

/// \brief Contains pointers on quadratic coeffs and command line info (argc, argv)
struct ReadFromCmdParams {
    const CoeffsPtrs coeffsPtrs;
    
    const int argc;
    const char *const *argv;
};

/// \brief Contains pointers on quadratic coeffs and file pointer
struct ReadCoeffsFromFileParams {
    const CoeffsPtrs coeffs;
    
    FILE *fp;
};

/// \brief Contains file name info and command line info (argc, argv)
struct ReadFileNameFromCmdParams {
    FileNameParams fileName;

    const int argc;
    const char *const *argv;
};

//---------------------------------------------------------------------------------------------------------------------

/// \brief compares str with command line flags
///
/// \param [in] str string to compare
/// \param [in] flags flags to compare with
/// \returns 0 if they are equal otherwise not 0
int CompareWithFlag(const char *str, const CommandLineFlagsType *const flags);

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
unsigned int ReadCommandLineFlags(const int argc, const char *argv[]);

//---------------------------------------------------------------------------------------------------------------------
/// \brief scans coefficients for quadratic equation ax^2 + bx + c from standard input
///
/// \details Works with CoeffsPtrs structure with params: 
/// \param [out] a pointer to the storage for a coefficient
/// \param [out] b pointer to the storage for b coefficient
/// \param [out] c pointer to the storage for c coefficient
/// \return 0 if reading is successful or not 0 if the user decided to quit the input
Errors ReadCoeffsFromStdin(const void *const storage);

//---------------------------------------------------------------------------------------------------------------------

/// \brief read coefficients for quadratic equation ax^2 + bx + c from file
///
/// \details Works with ReadCoeffsFromFileParams structure with params: 
/// \param [out] a pointer to the storage for a coefficient
/// \param [out] b pointer to the storage for b coefficient
/// \param [out] c pointer to the storage for c coefficient
/// \param [in] fp pointer to the FILE
/// \return 0 if reading is successful otherwise not 0
Errors ReadCoeffsFromFile(const void *const storage);

//---------------------------------------------------------------------------------------------------------------------

/// \brief read file name from standard input
///
/// \details Works with FileNameParams structure with params:
/// \param [out] name storage for file name
/// \param [in] size max size of file name (size of storage)
/// \return 0 if reading is successful otherwise not 0
Errors ReadFileNameFromStdin(const void *const storage);

//---------------------------------------------------------------------------------------------------------------------

/// \brief read file name from command line
///
/// \details Works with ReadFileNameFromCmdParams structure with params:
/// \param [in] argc number of arguments in argv
/// \param [in] argv command line arguments
/// \param [out] name storage for file name
/// \param [in] size max size of file name (size of storage)
/// \return 0 if reading is successful otherwise not 0
Errors ReadFileNameFromCommandLine(const void *const storage);

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
/// \details Read from command line only in case flag "-c" is specified right before the coefficients
/// \details Works with ReadFromCmdParams structure with params:
/// \param [in] argc number of values in argv
/// \param [in] argv parameters from the command line
/// \param [out] a pointer to the storage for a coefficient
/// \param [out] b pointer to the storage for b coefficient
/// \param [out] c pointer to the storage for c coefficient
/// \return 0 if reading is successful otherwise not 0
Errors ReadCoeffsFromCommandLine(const void *const storage);

//---------------------------------------------------------------------------------------------------------------------

/// \brief reads equation from command line and converts to coefficients
///
/// \details equation format: 5x^2 + 3x - 1 = 7x + 2
/// \details Works with ReadFromCmdParams structure with params:
/// \param [in] argc number of argv values
/// \param [in] argv command line arguments
/// \param [out] a  quadratic coefficient
/// \param [out] b  linear coefficient
/// \param [out] c  free coefficient
/// \return 0 if reading is successful otherwise not 0
Errors ReadEquationCoeffsFromCommandLine(const void *const storage);

//---------------------------------------------------------------------------------------------------------------------

/// \brief read equation from stdin and converts to coefficients
///
/// \details equation format: 5x^2 + 3x - 1 = 7x + 2
/// \details Works with CoeffsPtrs structure with params:
/// \param [out] a  quadratic coefficient
/// \param [out] b  linear coefficient
/// \param [out] c  free coefficient
/// \return 0 if reading is successful otherwise not 0
Errors ReadEquationCoeffsFromStdin(const void *const storage);

//---------------------------------------------------------------------------------------------------------------------

/// \brief read equation from file and converts to coefficients
///
/// \details equation format: 5x^2 + 3x - 1 = 7x + 2
/// \details Works with ReadCoeffsFromFileParams structure with params:
/// \param [out] a  quadratic coefficient
/// \param [out] b  linear coefficient
/// \param [out] c  free coefficient
/// \param [in] fp pointer to the FILE
/// \return 0 if reading is successful otherwise not 0
Errors ReadEquationCoeffsFromFile(const void *const storage);

//---------------------------------------------------------------------------------------------------------------------

/// \brief prints help info based on flags in flagsActivated
///
/// \param [in] flagsActivated unsigned int number with bits indicating flags (0 - off, 1 - on)
/// \return Errors::NO_HELPING_FLAG or Errors::NO_ERRORS
Errors Help(const unsigned int flagsActivated);

//---------------------------------------------------------------------------------------------------------------------

/// \brief fgets with deleting new line symbol
///
/// \param [out] name string to read in
/// \param [in] size  size of the buffer of the string
/// \param [in] fp pointer to the FILE to read from
/// \return 0 if reading is successful otherwise not 0
Errors Fgets_s(char *name, const size_t size, FILE *fp);

//---------------------------------------------------------------------------------------------------------------------

/// \brief checks if there is no extra symbols in the string after using fgets
///
/// \param [in] str string with read input from stdin
/// \param [in] size - size of str buffer 
/// \param [in] file from which str was read
/// \return true (1) if fgets has read all string. false (0) if something is in the string (except '\n');
bool HasReadAllStringWithFgets(const char *str, const size_t size, FILE *fp);

//---------------------------------------------------------------------------------------------------------------------

/// \brief gets if the flag flagID is on in the flagsActivated (gets bit)
///
/// \param [in] flagsActivated unsigned int number with bits indicating flags (0 - off, 1 - on)
/// \param [in] flagID ID of the flag to get (in the null numeration)
/// \return 0 is flag is off otherwise 1
int GetFlag(const unsigned int flagsActivated, const int flagID);

//---------------------------------------------------------------------------------------------------------------------

/// \brief delete flag from flagsActivated (sets bit to 0)
///
/// \param [in] flagsActivated unsigned int number with bits indicating flags (0 - off, 1 - on)
/// \param [in] flagID ID of the flag to get (in the null numeration)
void DeleteFlag(unsigned int *const flagsActivated, const int flagID);

//---------------------------------------------------------------------------------------------------------------------

/// \brief adds flag to flagsActivated (sets bit to 1)
///
/// \param [in] flagsActivated unsigned int number with bits indicating flags (0 - off, 1 - on)
/// \param [in] flagID ID of the flag to get (in the null numeration)
void AddFlag(unsigned int *const flagsActivated, const int flagID);

//---------------------------------------------------------------------------------------------------------------------

/// \brief calls flag funcs from Funcs based on flagsActivated
/// 
/// \details not all params have to be given. It's based on Funcs funcs. 
/// \details Redunant params could be any values
/// \param [in]flagsActivated active command line flags 
/// \param [in]a quadratic coefficient
/// \param [in]b linear coefficient
/// \param [in]c free coefficient
/// \param [in]argc number of values in argv
/// \param [in]argv argv number of values
/// \param [out]name storage to write file name
/// \param [in]size size of name storage
/// \param [in]fp file pointer with opened file
/// \param [in]Funcs funcs to call
/// \return Funcs errors if they occurred, unknown flag error if no funcs called. Otherwise no errors
Errors CallFlagFunc(const unsigned int flagsActivated,
                    double *a, double *b, double *c,
                    const int argc, const char *argv[],
                    FILE *fp, 
                    const TypesReadFuncsType *Funcs);

//---------------------------------------------------------------------------------------------------------------------
#endif // INPUT_AND_OUTPUT_H
