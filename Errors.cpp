//
// Created by Arman on 21.08.2023.
//

#include "Errors.h"

//---------------------------------------------------------------------------------------------------------------------

int PrintErrors(const Errors errors) {
    switch (errors) {
        case Errors::NO_ERRORS:
            return 0;
        case Errors::FILE_OPENING_ERROR:
            perror("Error opening the file");
            break;
        case Errors::INVALID_NUMBER_OF_ROOTS:
            perror("Invalid number of roots given");
            break;
        case Errors::READING_COEFFS_FROM_STDIN_ERROR:
            perror("Error while reading coefficients of quadratic equation from stdin");
            break;
        case Errors::READING_FILE_NAME_FROM_STDIN_ERROR:
            perror("Error while reading file name from stdin");
            break;
        case Errors::READING_EQUATION_FROM_STDIN_ERROR:
            perror("Error while reading equation from stdin");
            break;
        case Errors::READING_COEFFS_FROM_FILE_ERROR:
            perror("Error while reading coefficients of quadratic equation from file");
            break;
        case Errors::READING_EQUATION_FROM_FILE_ERROR:
            perror("Error while reading equation from file");
            break;
        case Errors::READING_EQUATION_FROM_COMMAND_LINE_ERROR:
            perror("Error while reading equation from command line");
            break;
        case Errors::READING_COEFFS_FROM_COMMAND_LINE_ERROR:
            perror("Error while reading coefficients of quadratic equation from command line");
            break;
        case Errors::READING_FILE_NAME_FROM_COMMAND_LINE_ERROR:
            perror("Error while reading file name from command line");
            break;
        case Errors::INVALID_EQUATION_FORMAT:
            perror("Invalid equation format given. Format should be\n"
                   "5x^2 + 3.21x - 17.22 + 17x^2 = 9.1x + 3.788");
            break;
        case Errors::INVALID_POWER_OF_X:
            perror("Invalid power of x is given in equation\n"
                   "power of x should be less than two");
            break;
        case Errors::QUIT_THE_PROGRAM_WITHOUT_INPUT:
            printf("User quit the program\n");
            break;
        case Errors::UNKNOWN_COMMAND_LINE_FLAG:
            perror("Unknown command line flag given\n"
                   "read available command line flags in README");
            break;
        case Errors::READING_FROM_STDIN_ERROR:
            perror("Error reading from stdin");
            break;
        case Errors::INVALID_STRING_NO_ENDING:
            perror("Invalid string: no ending ('\\0')");
            break;
        case Errors::DOUBLE_IS_OUT_OF_RANGE:
            perror("Double is out of range");
            break;
        case Errors::EXTRA_SYMBOLS_IN_LINE:
            perror("Extra symbols printed by user");
            break;
        default:
            perror("No such error");
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
