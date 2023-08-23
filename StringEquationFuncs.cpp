//
// Created by Arman on 19.08.2023.
//

#include "StringEquationFuncs.h"

//---------------------------------------------------------------------------------------------------------------------

static void TransposeOneSymbol(char **tmpPos, char **posPtr, bool haveToChangeSign);

static int ValidAlpha(char previous, char next);

static int ValidDigit(char previous, char next);

static int ValidPow(char previous, char next);

static int ValidDot(char previous, char next);

static int ValidSign(char previous, char next);

static int ValidEqual(char previous, char next);

static int ValidE(char previous, char next);

static int ValidSymbol(char now);

static int ValidStart(char now);

//---------------------------------------------------------------------------------------------------------------------

Errors ParseQuadraticEquation(const char *equation, double *a, double *b, double *c) {
    assert(a != NULL);
    assert(b != NULL);
    assert(c != NULL);
    assert(equation != NULL);

    static char copyEquation[2 * MAX_EQUATION_SIZE] = "";
    strncpy(copyEquation, equation, MAX_EQUATION_SIZE);

    DeleteSpaces(copyEquation);
    
    Errors errors = CheckEquation(copyEquation);
    if (errors != Errors::NO_ERRORS) {
        return errors;
    }

    errors = TransposeEquation(copyEquation);
    if (errors != Errors::NO_ERRORS) {
        return errors;
    }

    char *posPtr = copyEquation;

    *a = *b = *c = 0;

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

            case INVALID_POWER:
            default:
                return Errors::INVALID_POWER_OF_X;;
        }

        posPtr = tmpPos;

        if (!IsSign(*posPtr) && *posPtr != '\0') {
            posPtr++;
        }

    }

    if (errors != Errors::NO_ERRORS)
        return errors;

    return CheckCoeffsIsFinite(*a, *b, *c);
}

//---------------------------------------------------------------------------------------------------------------------

//endPtr = pointer to the last symbol of the addend, equation have to be after
// 1) deleting spaces with DeleteSpaces()
// 2) transposing with function TransposeEquation()
PowerOfX GetXPowAndCoeff(const char *x, double *target, char **endPtr) {
    assert(x != NULL);
    assert(target != NULL);
    assert(endPtr != NULL);

    static const int BASE = 10;
    *target = strtod(x, endPtr);

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

//---------------------------------------------------------------------------------------------------------------------

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

    static char tmpEquation[2 * MAX_EQUATION_SIZE] = "";

    char *tmpPos = tmpEquation;
    char *posPtr = copyEquation;
    
    if (!IsSign(*posPtr)) {
        *tmpPos = '+';
        tmpPos++;
    }
    while (posPtr < equalPointer) {
        TransposeOneSymbol(&tmpPos, &posPtr, 0);
    }

    if (posPtr >= endPointer) {
        strcpy(copyEquation, tmpEquation);
        return Errors::NO_ERRORS;
    }

    posPtr++;

    if (!IsSign(*posPtr)) {
        *tmpPos = '-';
        tmpPos++;
    }
    while (posPtr < endPointer) {
        TransposeOneSymbol(&tmpPos, &posPtr, 1);
    }

    *tmpPos = '\0';

    strcpy(copyEquation, tmpEquation);

    return Errors::NO_ERRORS;
}


//---------------------------------------------------------------------------------------------------------------------

#define ValidArg *(equation - 1), *(equation + 1)

// spaces have to be deleted
Errors CheckEquation(char *equationToCheck) {
    assert(equationToCheck != NULL);

    char *equation = equationToCheck;

    if (!ValidStart(*equation)) {
        return Errors::INVALID_EQUATION_FORMAT;
    }

    equation++;
    int cntEquals = 0;

    while (*equation != '\0') {
        if                        (                       !ValidSymbol(*equation)                    ||
                                   (*equation == 'e'   && !ValidE(ValidArg))                         ||
                                   (*equation == '.'   && !ValidDot(ValidArg))                       ||
                                   (*equation == '='   && !ValidEqual(ValidArg))                     ||
                                   (*equation == '^'   && !ValidPow(ValidArg))                       ||
                                   (isalpha(*equation) && !ValidAlpha(ValidArg) && *equation != 'e') ||
                                   (isdigit(*equation) && !ValidDigit(ValidArg))                     ||
                                   (IsSign(*equation)  && !ValidSign(ValidArg))) {        
            return Errors::INVALID_EQUATION_FORMAT;
        }
        
        if (*equation == '=') cntEquals++;
        equation++;
    }

    if (cntEquals > 1) {
        return Errors::INVALID_EQUATION_FORMAT;
    }

    //Check valid pows and coeffs:
    equation = equationToCheck;
    while (*equation != '\0') {
            strtod(equation, &equation);
            
            if (*equation == '\0') break;

            //printf("%c %d\n", *equation, IsSign(*equation) || isalpha(*equation) || *equation == '=' );
            if (*equation == 'e' || !(IsSign(*equation) || isalpha(*equation) || *equation == '=')) { //case 2.2.2 or 1e5.2
                printf("HERE3: %c\n", *equation);
                return Errors::INVALID_EQUATION_FORMAT;
            }

            equation++;

            if (*equation == '\0') break;

            if (*equation == '^') {
                equation++;
                strtol(equation, &equation, 10);
                if (!(IsSign(*equation) || *equation == '=' || *equation == '\0')) { // x^2+ x^2= no other cases
                    printf("HERE4: %c\n", *equation);
                    return Errors::INVALID_EQUATION_FORMAT;
                }
            }

            printf("%s\n", equation);
    }  

    return Errors::NO_ERRORS;
}

#undef ValidArg

// =

//---------------------------------------------------------------------------------------------------------------------

static void TransposeOneSymbol(char **tmpPos, char **posPtr, bool haveToChangeSign) {
    if (IsSign(**posPtr) || isdigit(**posPtr)) { //example: +2x

        if (haveToChangeSign) **tmpPos = ChangeSign(**posPtr);
        else **tmpPos = **posPtr;

    } else if (isalpha(**posPtr) && isdigit(*(*posPtr - 1))) { //example: 2x
        **tmpPos = **posPtr;
    } else if (isalpha(**posPtr) && !isdigit(*(*posPtr - 1)) && *(*posPtr - 1) != '.') { //example: +x, x
        **tmpPos = '1';
        ++(*tmpPos);
        **tmpPos = **posPtr;
    } else {
        **tmpPos = **posPtr;
    }

    ++(*tmpPos);
    ++(*posPtr);
}

//---------------------------------------------------------------------------------------------------------------------

static int ValidStart(char now) {
    return IsSign(now) || now == '.' || isdigit(now) || isalpha(now);
}

static int ValidE(char previous, char next) {
    return (isdigit(previous) || previous == '.') &&
            isdigit(next);
}
static int ValidAlpha(char previous, char next) {
    return (strchr("+-=.", previous) || isdigit(previous)) &&
           (strchr("+-=^", next));
}

static int ValidDigit(char previous, char next) {
    return (strchr("+-^=e.", previous) || isdigit(previous)) &&
           (strchr("+-.e=",  next)     || isdigit(next) || isalpha(next));
}

static int ValidPow(char previous, char next) {
    return isalpha(previous) &&
           isdigit(next);
}

static int ValidDot(char previous, char next) {
    return (strchr("+-=", previous) || isdigit(previous)) &&
           (isdigit(next) || isalpha(next) || IsSign(next) || next == '\0' || next == '=') &&
           (isdigit(previous) || isdigit(next));
}

static int ValidSign(char previous, char next) {
    return (isalpha(previous) || isdigit(previous) || previous == '.' || previous == '=') &&
           (isalpha(next)     || isdigit(next)     || next     == '.');
}

static int ValidEqual(char previous, char next) {
    return (isdigit(previous) || isalpha(previous) || previous == '.') &&
           (strchr("+-.", next) || isdigit(next) || isalpha(next));
}

static int ValidSymbol(char now) {
    return isdigit(now) || isalpha(now) || strchr("+-.=^", now);
}
