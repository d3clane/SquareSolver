//
// Created by Arman on 21.08.2023.
//

#include "Errors.h"


#define RED "\x1b[31;1m" // в отдельный файл

#ifndef NDEBUG
    #define PrintError(X) fprintf(stderr, RED X \
                          "\nError occured in file %s in line %d\n", \
                          ErrorInfo.fileWithError, ErrorInfo.lineWithError)
#else
    #define PrintError(X) fprintf(stderr, RED X);
#endif

ErrorInfo_t ErrorInfo = {.error = Errors::NO_ERRORS, .fileWithError = "aboba", .lineWithError = -1};

//getopt 
//---------------------------------------------------------------------------------------------------------------------

int PrintErrors() {
    switch (ErrorInfo.error) {
        case Errors::NO_ERRORS:
            return 0;
        case Errors::QUIT_THE_PROGRAM_WITHOUT_INPUT:
            printf("User quit the program\n");
            break;      


        case Errors::FILE_OPENING_ERROR:
            PrintError("Error opening the file");
            break;


        case Errors::UNKNOWN_COMMAND_LINE_FLAG:
            PrintError("Unknown command line flag given\n"
                       "read available command line flags in README or -h");
            break;  
            
        //-----------------------------------

        case Errors::READING_FROM_STDIN_ERROR:
            PrintError("Error reading from stdin");
            break;          
        case Errors::READING_COEFFS_FROM_STDIN_ERROR:
            PrintError("Error while reading coefficients of quadratic equation from stdin");
            break;
        case Errors::READING_FILE_NAME_FROM_STDIN_ERROR:
            PrintError("Error while reading file name from stdin");
            break;
        case Errors::READING_EQUATION_FROM_STDIN_ERROR:
            PrintError("Error while reading equation from stdin");
            break;
        case Errors::READING_COEFFS_FROM_FILE_ERROR:
            PrintError("Error while reading coefficients of quadratic equation from file");
            break;
        case Errors::READING_EQUATION_FROM_FILE_ERROR:
            PrintError("Error while reading equation from file");
            break;
        case Errors::READING_EQUATION_FROM_COMMAND_LINE_ERROR:
            PrintError("Error while reading equation from command line");
            break;
        case Errors::READING_COEFFS_FROM_COMMAND_LINE_ERROR:
            PrintError("Error while reading coefficients of quadratic equation from command line");
            break;
        case Errors::READING_FILE_NAME_FROM_COMMAND_LINE_ERROR:
            PrintError("Error while reading file name from command line");
            break;


        case Errors::HELPING_ERROR:
            PrintError("Help can't print info about this flag :\n"
                   "No such flag");
            return 0;
        case Errors::NO_HELPING_FLAG:
            PrintError("No helping flag specified. Help functions should not be used");
            return 0;


        case Errors::INVALID_EQUATION_FORMAT:
            PrintError("Invalid equation format given. Format should be\n"
                   "5x^2 + 3.21x - 17.22 + 17x^2 = 9.1x + 3.788");
            break;
        case Errors::INVALID_POWER_OF_X:
            PrintError("Invalid power of x is given in equation\n"
                   "power of x should be less than two");
            break;
        case Errors::INVALID_NUMBER_OF_ROOTS:
            PrintError("Invalid number of roots given");
            break;


        case Errors::INVALID_STRING_NO_ENDING:
            PrintError("Invalid string: no ending ('\\0')");
            break;
        case Errors::EXTRA_SYMBOLS_IN_LINE:
            PrintError("Extra symbols printed by user");
            break;

        case Errors::DOUBLE_IS_OUT_OF_RANGE:
            PrintError("Double is out of range");
            break;
        default:
            PrintError("No such error");
            break;
    }   

    return 1;
}

//---------------------------------------------------------------------------------------------------------------------

Errors CheckCoeffsIsFinite(const double a, const double b, const double c) {
    if (!isfinite(a) || !isfinite(b) || !isfinite(c)) {
        return Errors::DOUBLE_IS_OUT_OF_RANGE;
    }

    return Errors::NO_ERRORS;
}

//---------------------------------------------------------------------------------------------------------------------
