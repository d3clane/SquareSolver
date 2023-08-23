//
// Created by Arman on 19.08.2023.
//

#include "StringEquationFuncs.h"

//---------------------------------------------------------------------------------------------------------------------

static void TransposeOneSymbol(char **tmpPos, char **posPtr, bool haveToChangeSign);

//---------------------------------------------------------------------------------------------------------------------

static int ValidAlpha(char previous, char next);

static int ValidDigit(char previous, char next);

static int ValidPow(char previous, char next);

static int ValidDot(char previous, char next);

static int ValidSign(char previous, char next);

static int ValidEqual(char previous, char next);

static int ValidE(char previous, char next);

static int ValidSymbol(char now);

static int ValidStart(char now);

static int ValidCoeff(char **posInEquation);

static int ValidPowCoeff(char **posInEquation);

static Errors CheckStringOnValidSymbols(char **posInEquation);

static Errors CheckStringPowsAndCoeffs(char **posInEquation);

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

    char *posInCopyEquation = copyEquation;

    *a = *b = *c = 0;

    while (*posInCopyEquation != '\0') {
        double tmpVal = NAN;
        char *tmpPos = NULL;

        PowerOfX powerOfX = GetXPowAndCoeff(posInCopyEquation, &tmpVal, &tmpPos);

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

        posInCopyEquation = tmpPos;

        if (!IsSign(*posInCopyEquation) && *posInCopyEquation != '\0') {
            posInCopyEquation++;
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
Errors TransposeEquation(char *equation) {
    assert(equation != NULL);

    char *equalPointer = strchr(equation, '=');
    char *endPointer = strchr(equation, '\0');

    if (endPointer == NULL) {
        return Errors::INVALID_STRING_NO_ENDING;
    }

    if (equalPointer == NULL) {
        equalPointer = endPointer;
    }

    static char transposedEquation[2 * MAX_EQUATION_SIZE] = "";

    char *posInTransposed = transposedEquation;
    char *posInGivenEquation = equation;
    
    if (!IsSign(*posInGivenEquation)) {
        *posInTransposed = '+';
        posInTransposed++;
    }
    while (posInGivenEquation < endPointer) {
        if      (posInGivenEquation < equalPointer)                                 //норм делать фигурные скобки только у одного элса
            TransposeOneSymbol(&posInTransposed, &posInGivenEquation, false);
        else if (posInGivenEquation > equalPointer) 
            TransposeOneSymbol(&posInTransposed, &posInGivenEquation, true);
        else {
            posInGivenEquation++;

            if (!IsSign(*posInGivenEquation)) {
                *posInTransposed = '-';
                posInTransposed++;
            }
        }
    }

    *posInTransposed = '\0';

    strcpy(equation, transposedEquation);

    return Errors::NO_ERRORS;
}


//---------------------------------------------------------------------------------------------------------------------

// spaces have to be deleted
Errors CheckEquation(char *equationToCheck) {
    assert(equationToCheck != NULL);

    char *posInEquation = equationToCheck;

    if (!ValidStart(*posInEquation)) {
        return Errors::INVALID_EQUATION_FORMAT;
    }

    posInEquation++;

    Errors errors = Errors::NO_ERRORS;

    errors = CheckStringOnValidSymbols(&posInEquation);
    
    if (errors != Errors::NO_ERRORS)

    //Check valid pows and coeffs:
    posInEquation = equationToCheck;
    errors = CheckStringPowsAndCoeffs(&posInEquation);

    return errors;
}

//---------------------------------------------------------------------------------------------------------------------

static void TransposeOneSymbol(char **posInTransposed, char **posInGiven, bool haveToChangeSign) {
    if (isalpha(**posInGiven) && !isdigit(*(*posInGiven - 1)) && *(*posInGiven - 1) != '.') {
        **posInTransposed = '1';
        ++(*posInTransposed);
    }

    if (haveToChangeSign) 
         **posInTransposed = ChangeSign(**posInGiven);
    else **posInTransposed = **posInGiven;

    ++(*posInTransposed);
    ++(*posInGiven);
}

//---------------------------------------------------------------------------------------------------------------------

#define ValidArg *(*posInEquation - 1), *(*posInEquation + 1)

static Errors CheckStringOnValidSymbols(char **posInEquation) {
    int cntEquals = 0;

    while (**posInEquation != '\0') {
        if (                            !ValidSymbol(**posInEquation)                     ||
            (**posInEquation == 'e'   && !ValidE(ValidArg))                               ||
            (**posInEquation == '.'   && !ValidDot(ValidArg))                             ||
            (**posInEquation == '='   && !ValidEqual(ValidArg))                           ||
            (**posInEquation == '^'   && !ValidPow(ValidArg))                             ||
            (isalpha(**posInEquation) &&  **posInEquation != 'e' && !ValidAlpha(ValidArg)) ||
            (isdigit(**posInEquation) && !ValidDigit(ValidArg))                           ||
            (IsSign(**posInEquation)  && !ValidSign(ValidArg))) {        
            return Errors::INVALID_EQUATION_FORMAT;
        }
        
        if (**posInEquation == '=') cntEquals++;
        (*posInEquation)++;
    }

    return cntEquals > 1 ? Errors::INVALID_EQUATION_FORMAT : Errors::NO_ERRORS;
}

#undef ValidArg

//---------------------------------------------------------------------------------------------------------------------

static Errors CheckStringPowsAndCoeffs(char **posInEquation) {
    while (**posInEquation != '\0') {
        if (!ValidCoeff(posInEquation)) {
            return Errors::INVALID_EQUATION_FORMAT;
        }
    
        posInEquation++; //go to the next 

        if (!ValidPowCoeff(posInEquation)) {
            return Errors::INVALID_EQUATION_FORMAT;
        }   
    }  

    return Errors::NO_ERRORS;
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
           (isalpha(next)           || isdigit(next) || IsSign(next) || next == '\0' || next == '=') &&
           (isdigit(previous)       || isdigit(next));
}

static int ValidSign(char previous, char next) {
    return (isalpha(previous) || isdigit(previous) || previous == '.' || previous == '=') &&
           (isalpha(next)     || isdigit(next)     || next     == '.');
}

static int ValidEqual(char previous, char next) {
    return (isdigit(previous) || isalpha(previous) || previous == '.') &&
           (isdigit(next)     || isalpha(next)     || next     == '.' || IsSign(next));
}

static int ValidSymbol(char now) {
    return isdigit(now) || isalpha(now) || strchr("+-.=^", now);
}

//chagnes posInEquation position on the last symbol not read
static int ValidCoeff(char **posInEquation) {
    strtod(*posInEquation, posInEquation);

    if (**posInEquation != '\0') {
        if (**posInEquation == 'e' || !(IsSign(**posInEquation) || isalpha(**posInEquation) || **posInEquation == '=')) { 
            return 1;
        }
    }

    return 0;
}

//change posInEquation position on the last symbol not read
static int ValidPowCoeff(char **posInEquation) {
    if (**posInEquation != '\0') {
        if (**posInEquation == '^') {
            (*posInEquation)++;
            strtol(*posInEquation, posInEquation, 10);
            
            if (!(IsSign(**posInEquation) || **posInEquation == '=' || **posInEquation == '\0')) // x^2+ x^2= no other cases
                    return 1;
        }
    }

    return 0;
}
