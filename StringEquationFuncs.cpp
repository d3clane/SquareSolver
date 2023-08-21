//
// Created by Arman on 19.08.2023.
//

#include "StringAndCharFuncs.h"
#include "StringEquationFuncs.h"

int ParseQuadraticEquation(const char *equation, double *a, double *b, double *c) {
    assert(a != NULL);
    assert(b != NULL);
    assert(c != NULL);
    assert(equation != NULL);

    int parseError = 0;
    *a = *b = *c = 0;
    static char copyEquation[2 * MAX_EQUATION_SIZE];

    strcpy(copyEquation, equation);
    DeleteSpaces(copyEquation);
    TransposeEquation(copyEquation);

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
                perror("Invalid power of x");
                parseError = 1;
                return parseError;
        }

        posPtr = tmpPos;

        if (!isSign(*posPtr) && *posPtr != '\0') {
            posPtr++;
        }
    }

    return parseError;
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
void TransposeEquation(char *copyEquation) {
    assert(copyEquation != NULL);

    char *equalPointer = strchr(copyEquation, '=');
    char *endPointer = strchr(copyEquation, '\0');

    if (endPointer == NULL) {
        perror("Not valid string equation");
        return;
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
        if (isSign(*posPtr) || isdigit(*posPtr)) { //example: +2x
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
        if (isSign(*posPtr) || isdigit(*posPtr)) { //example: +2x
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

    *tmpPos= '\0';

    strcpy(copyEquation, tmpEquation);
}


