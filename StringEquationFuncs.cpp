//
// Created by Arman on 19.08.2023.
//

#include "StringEquationFuncs.h"

static const char *VALID_NON_DIGITS = "+-.^=";

Errors ParseQuadraticEquation(const char *equation, double *a, double *b, double *c) {
    assert(a != NULL);
    assert(b != NULL);
    assert(c != NULL);
    assert(equation != NULL);

    *a = *b = *c = 0;
    static char copyEquation[2 * MAX_EQUATION_SIZE];

    Errors errors = CheckEquation(equation);
    if (errors != Errors::NO_ERRORS) {
        return errors;
    }

    strcpy(copyEquation, equation);

    DeleteSpaces(copyEquation);

    errors = TransposeEquation(copyEquation);
    if (errors != Errors::NO_ERRORS) {
        return errors;
    }

    char *posPtr = copyEquation;
    while (*posPtr != '\0') {
        double tmpVal = NAN;
        char *tmpPos = NULL;

        PowerOfX powerOfX = GetXPowAndCoeff(posPtr, &tmpVal, &tmpPos);

        switch (powerOfX) {
            case POWER_ZERO:
                *c += tmpVal;
                break;
            case POWER_ONE:
                *b += tmpVal;
                break;
            case POWER_TWO:
                *a += tmpVal;
                break;
            default:
                return Errors::INVALID_POWER_OF_X;;
        }

        posPtr = tmpPos;

        if (!IsSign(*posPtr) && *posPtr != '\0') {
            posPtr++;
        }
    }

    return errors;
}


//endPtr = pointer to the last symbol of the addend, equation have to be after
// 1) deleting spaces with DeleteSpaces()
// 2) transposing with function TransposeEquation()
enum PowerOfX GetXPowAndCoeff(const char *x, double *target, char **endPtr) {
    static const int BASE = 10;
    *target = strtod(x, endPtr); //endPtr could be on 'x' or on next sign (in case of free coefficient) or ending

    if (!isalpha(**endPtr)) {
        return POWER_ZERO;
    }

    (*endPtr)++;
    if (**endPtr != '^') {
        return POWER_ONE;
    }

    (*endPtr)++;
    long power = strtol(*endPtr, endPtr, BASE);
    return (power == 2 ? POWER_TWO : INVALID_POWER);
}

//spaces have to be deleted
Errors TransposeEquation(char *copyEquation) {
    assert(copyEquation != NULL);

    char *equalPointer = strchr(copyEquation, '=');
    char *endPointer = strchr(copyEquation, '\0');

    if (endPointer == NULL) {
        return Errors::INVALID_STRING_NO_ENDING;
    }

    if (equalPointer == NULL) {
        equalPointer = endPointer;
    }

    static char tmpEquation[2 * MAX_EQUATION_SIZE];

    char *tmpPos = tmpEquation;
    char *posPtr = copyEquation;

    if (isalpha(*posPtr) || isdigit(*posPtr)) {
        *tmpPos = '+';
        tmpPos++;
    }
    // TODO обернуть алгоритм из вайла внутри в функцию, так как вставляется дважды (в следующем вайле тоже)
    while (posPtr < equalPointer) {
        if (IsSign(*posPtr) || isdigit(*posPtr)) { //example: +2x
            *tmpPos = *posPtr;
        } else if (isalpha(*posPtr) && isdigit(*(posPtr - 1))) { //example: 2x
            *tmpPos = *posPtr;
        } else if (isalpha(*posPtr) && !isdigit(*(posPtr - 1))) { //example: +x, x
            *tmpPos = '1';
            ++tmpPos;
            *tmpPos = *posPtr;
        } else {
            *tmpPos = *posPtr;
        }
        ++tmpPos;
        ++posPtr;
    }

    posPtr++;

    if (isalpha(*posPtr) || isdigit(*posPtr)) {
        *tmpPos = '-';
        tmpPos++;
    }
    while (posPtr < endPointer) {
        if (IsSign(*posPtr) || isdigit(*posPtr)) { //example: +2x
            *tmpPos = ChangeSign(*posPtr);
        } else if (isalpha(*posPtr) && isdigit(*(posPtr - 1))) { //example: 2x
            *tmpPos = *posPtr;
        } else if (isalpha(*posPtr) && !isdigit(*(posPtr - 1))) { //example: +x, x
            *tmpPos = '1';
            ++tmpPos;
            *tmpPos = *posPtr;
        } else {
            *tmpPos = *posPtr;
        }
        ++tmpPos;
        ++posPtr;
    }

    *tmpPos = '\0';

    strcpy(copyEquation, tmpEquation);

    return Errors::NO_ERRORS;
}

Errors CheckEquation(const char *equation) {
    while (*equation != '\0') {
        if (
                (strchr("^.", *equation) != NULL && !isdigit(*(equation + 1)))
                ||
                (!isnumber(*equation) && !isalpha(*equation) && !isspace(*equation) &&
                 strchr(VALID_NON_DIGITS, *equation) == NULL)
                ||
                (strchr(VALID_NON_DIGITS, *equation) != NULL && strchr(VALID_NON_DIGITS, *(equation + 1)) != NULL)
                ) {
            return Errors::INVALID_EQUATION_FORMAT;
        }

        equation++;
    }

    return Errors::NO_ERRORS;
}
