//
// Created by Arman on 19.08.2023.
//

#include "StringEquationFuncs.h"

//---------------------------------------------------------------------------------------------------------------------

static void TransposeOneSymbol(char **tmpPos, char **posPtr, bool haveToChangeSign);

//---------------------------------------------------------------------------------------------------------------------

static int ValidAlpha(const char *now);

static int ValidDigit(const char *now);

static int ValidPow(const char *nowt);

static int ValidDot(const char *now);

static int ValidSign(const char *now);

static int ValidEqual(const char *now);

static int ValidE(const char *now);

static int ValidSymbol(const char now);

static int ValidStart(const char now);

static int ValidCoeff(char **posInEquation);

static int ValidPowCoeff(char **posInEquation);

static Errors CheckStringOnValidSymbols(char **posInEquation);

static Errors CheckStringPowsAndCoeffs(char **posInEquation);

//---------------------------------------------------------------------------------------------------------------------

Errors ParseQuadraticEquation(const char *equation, double *a, double *b, double *c) {
    assert(a);
    assert(b);
    assert(c);
    assert(equation);

    static char copyEquation[2 * MAX_EQUATION_SIZE] = "";
    strncpy(copyEquation, equation, MAX_EQUATION_SIZE);

    DeleteSpaces(copyEquation);
    StringToLower(copyEquation);

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
        char *tmpPos = nullptr;

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
                UpdateError(Errors::INVALID_POWER_OF_X);
                return Errors::INVALID_POWER_OF_X;
        }

        posInCopyEquation = tmpPos;

        if (!IsSign(*posInCopyEquation) && *posInCopyEquation != '\0') {
            posInCopyEquation++;
        }
    }

    if (errors != Errors::NO_ERRORS)
        return errors;

    errors = CheckCoeffsIsFinite(*a, *b, *c);

    if (errors != Errors::NO_ERRORS) {
        UpdateError(errors);
    }

    return errors;
}

//---------------------------------------------------------------------------------------------------------------------

//endPtr = pointer to the last symbol of the addend, equation have to be after
// 1) deleting spaces with DeleteSpaces()
// 2) transposing with function TransposeEquation()
PowerOfX GetXPowAndCoeff(const char *x, double *target, char **endPtr) {
    assert(x);
    assert(target);
    assert(endPtr);

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
    assert(equation);

    char *equalPointer = strchr(equation, '=');
    char *endPointer = strchr(equation, '\0');

    if (!endPointer) {
        UpdateError(Errors::INVALID_STRING_NO_ENDING);
        return      Errors::INVALID_STRING_NO_ENDING;
    }

    if (!equalPointer) {
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
    assert(equationToCheck);

    char *posInEquation = equationToCheck;

    if (!ValidStart(*posInEquation)) {
        UpdateError(Errors::INVALID_EQUATION_FORMAT);
        return      Errors::INVALID_EQUATION_FORMAT;
    }

    posInEquation++;

    Errors errors = Errors::NO_ERRORS;

    errors = CheckStringOnValidSymbols(&posInEquation);
    
    if (errors != Errors::NO_ERRORS) return errors;

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

static Errors CheckStringOnValidSymbols(char **posInEquation) {
    int cntEquals = 0;

    while (**posInEquation != '\0') {
        if (                             !ValidSymbol(**posInEquation)                     ||
            (**posInEquation == 'e'   && !ValidE(*posInEquation))                                ||
            (**posInEquation == '.'   && !ValidDot(*posInEquation))                              ||
            (**posInEquation == '='   && !ValidEqual(*posInEquation))                            ||
            (**posInEquation == '^'   && !ValidPow(*posInEquation))                              ||
            (isalpha(**posInEquation) &&  **posInEquation != 'e' && !ValidAlpha(*posInEquation)) ||
            (isdigit(**posInEquation) && !ValidDigit(*posInEquation))                            ||
            (IsSign(**posInEquation)  && !ValidSign(*posInEquation))) {  

            UpdateError(Errors::INVALID_EQUATION_FORMAT);
            return      Errors::INVALID_EQUATION_FORMAT;
        }

        if (**posInEquation == '=') cntEquals++;

        (*posInEquation)++;
    }

    if (cntEquals > 1) {
        UpdateError(Errors::INVALID_EQUATION_FORMAT);
    }

    return cntEquals > 1 ? Errors::INVALID_EQUATION_FORMAT : Errors::NO_ERRORS;
}

//---------------------------------------------------------------------------------------------------------------------

static Errors CheckStringPowsAndCoeffs(char **posInEquation) {
    while (**posInEquation != '\0') {
        if (!ValidCoeff(posInEquation)) {
            UpdateError(Errors::INVALID_EQUATION_FORMAT);
            return      Errors::INVALID_EQUATION_FORMAT;
        }

        if (**posInEquation == '\0') break;
        (*posInEquation)++; //go to the next 

        if (!ValidPowCoeff(posInEquation)) {
            UpdateError(Errors::INVALID_EQUATION_FORMAT);
            return      Errors::INVALID_EQUATION_FORMAT;
        }   

    }  

    return Errors::NO_ERRORS;
}

//---------------------------------------------------------------------------------------------------------------------

static inline int ValidStart(const char now) {
    return IsSign(now) || now == '.' || isdigit(now) || isalpha(now);
}

static int ValidE(const char *now) {
    bool prevVal = isdigit(*(now - 1)) || *(now - 1) == '.';

    if (*(now + 1) == '-') 
        return prevVal &&
               isdigit(*(now + 2));
    
    return prevVal &&
           isdigit(*(now + 1));
}

static int ValidAlpha(const char *now) {
    return (strchr("+-=.", *(now - 1)) || isdigit(*(now - 1))) &&
           (strchr("+-=^", *(now + 1)));
}

static int ValidDigit(const char *now) {
    return (strchr("+-^=e.", *(now - 1)) || isdigit(*(now - 1))) &&
           (strchr("+-.e=",  *(now + 1)) || isdigit(*(now + 1))  || isalpha(*(now + 1)));
}

static int ValidPow(const char *now) {
    return isalpha(*(now - 1)) &&
           isdigit(*(now + 1));
}

static int ValidDot(const char *now) {
    return (strchr("+-=", *(now - 1)) || isdigit(*(now - 1))) &&
           (strchr("+-=", *(now + 1)) || isdigit(*(now + 1))  || isalpha(*(now + 1))) &&
           (isdigit(*(now - 1))       || isdigit(*(now + 1)));
}

static int ValidSign(const char *now) {
    return (isalpha(*(now - 1)) || isdigit(*(now - 1)) || *(now - 1) == '.' || *(now - 1) == '=') &&
           (isalpha(*(now + 1)) || isdigit(*(now + 1)) || *(now + 1) == '.');
}

static int ValidEqual(const char *now) {
    return (isdigit(*(now - 1)) || isalpha(*(now - 1)) || *(now - 1) == '.') &&
           (isdigit(*(now + 1)) || isalpha(*(now + 1)) || *(now + 1) == '.'  || IsSign(*(now + 1)));
}

static int ValidSymbol(const char now) {
    return isdigit(now) || isalpha(now) || strchr("+-.=^", now);
}

//chagnes posInEquation position on the last symbol not read
static int ValidCoeff(char **posInEquation) {
    if (**posInEquation == '\0') return 1;

    strtod(*posInEquation, posInEquation);

    if (**posInEquation == '\0') return 1;

    if (**posInEquation == 'e'    || 
        !(IsSign(**posInEquation) || isalpha(**posInEquation) || **posInEquation == '=')) { 
        return 0;
        }
    
    return 1;
}

//change posInEquation position on the last symbol not read
static int ValidPowCoeff(char **posInEquation) {
    if (**posInEquation != '^') return 1;
    
    (*posInEquation)++;
    strtol(*posInEquation, posInEquation, 10);
    
    if (!(IsSign(**posInEquation) || **posInEquation == '=' || **posInEquation == '\0')) // x^2+ x^2= no other cases
        return 0;

    return 1;
}
