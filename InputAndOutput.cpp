//
// Created by Arman on 15.08.2023.
//

#include "InputAndOutput.h"

//---------------------------------------------------------------------------------------------------------------------

static inline Errors MenuInputCoeffs();
static inline Errors MenuInputEquation();
static inline Errors TryOpenFile(const char *name, const char *mode, FILE **fp);
static Errors StrToDouble(const char *str, double *val);

//---------------------------------------------------------------------------------------------------------------------

CommandLineFlags commandLineFlags = {0};

//---------------------------------------------------------------------------------------------------------------------

Errors ReadInput(int argc, const char *argv[], double *a, double *b, double *c) {
    assert(a    != NULL);
    assert(b    != NULL);
    assert(c    != NULL);
    assert(argv != NULL);

    commandLineFlags = ReadCommandLineFlags(argc, argv);

    FILE *fp = NULL;
    static const size_t FILE_NAME_LENGTH = 64;
    Errors inputError = Errors::NO_ERRORS;
    static char fileName[FILE_NAME_LENGTH];

    if (commandLineFlags.readFromFile) {
        if (commandLineFlags.readFromCommandLine) {
            inputError = ReadFileNameFromCommandLine(argc, argv, fileName, FILE_NAME_LENGTH);
        } else if (commandLineFlags.readFromStdin) {
            inputError = ReadFileNameFromStdin(fileName, FILE_NAME_LENGTH);
        } else {
            return Errors::UNKNOWN_COMMAND_LINE_FLAG;
        }

        if (inputError != Errors::NO_ERRORS) return inputError;

        inputError = TryOpenFile(fileName, "r", &fp);

        if (inputError != Errors::NO_ERRORS) return inputError;

        if (commandLineFlags.equationInputMode) inputError = ReadEquationCoeffsFromFile(a, b, c, fp);
        else inputError = ReadCoeffsFromFile(a, b, c, fp);

        fclose(fp);

        return inputError;
    }

    if (commandLineFlags.readFromStdin) {
        if (commandLineFlags.equationInputMode) inputError = ReadEquationCoeffsFromStdin(a, b, c);
        else inputError = ReadCoeffsFromStdin(a, b, c);
    } else if (commandLineFlags.readFromCommandLine) {
        if (commandLineFlags.equationInputMode) inputError = ReadEquationCoeffsFromCommandLine(argc, argv, a, b, c);
        else inputError = ReadCoeffsFromCommandLine(argc, argv, a, b, c);
    } else {
        return Errors::UNKNOWN_COMMAND_LINE_FLAG;
    }

    return inputError;
}

//---------------------------------------------------------------------------------------------------------------------

CommandLineFlags ReadCommandLineFlags(int argc, const char *argv[]) {
    assert(argv != NULL);

    for (int i = 0; i < argc; ++i) {
        if (strcmp(argv[i], _FILE_FlAG) == 0) {
            commandLineFlags.readFromFile = 1;
        } else if (strcmp(argv[i], _COMMAND_LINE_FLAG) == 0) {
            commandLineFlags.readFromCommandLine = 1;
        } else if (strcmp(argv[i], _STDIN_FLAG) == 0) {
            commandLineFlags.readFromStdin = 1;
        } else if (strcmp(argv[i], _TEST_MODE_FLAG) == 0) {
            commandLineFlags.testMode = 1;
        } else if (strcmp(argv[i], _EQUATION_INPUT_MODE_FLAG) == 0) {
            commandLineFlags.equationInputMode = 1;
        }
    }

    if (!commandLineFlags.readFromCommandLine) {
        commandLineFlags.readFromStdin = 1;
    }
    if (commandLineFlags.readFromStdin) {
        commandLineFlags.readFromCommandLine = 0;
    }

    return commandLineFlags;
}

//---------------------------------------------------------------------------------------------------------------------

Errors ReadCoeffsFromCommandLine(int argc, const char *argv[], double *a, double *b, double *c) {
    assert(a    != NULL);
    assert(b    != NULL);
    assert(c    != NULL);
    assert(argv != NULL);

    Errors readError = Errors::READING_COEFFS_FROM_COMMAND_LINE_ERROR;
    for (int i = 0; i < argc - 3; ++i) { // till (argc - 3) cause also reading (i + 3)
        if (strcmp(argv[i], _COMMAND_LINE_FLAG) == 0) {
            readError                                     = StrToDouble(argv[i + 1], a);

            if (readError == Errors::NO_ERRORS) readError = StrToDouble(argv[i + 2], b);

            if (readError == Errors::NO_ERRORS) readError = StrToDouble(argv[i + 3], c);

            break;
        }
    }

    return CheckQuadraticEquationCoefficientsIsFinite(readError, *a, *b, *c);
}

//---------------------------------------------------------------------------------------------------------------------

Errors ReadEquationCoeffsFromCommandLine(int argc, const char *argv[], double *a, double *b, double *c) {
    assert(a    != NULL);
    assert(b    != NULL);
    assert(c    != NULL);
    assert(argv != NULL);

    Errors readError = Errors::READING_EQUATION_FROM_COMMAND_LINE_ERROR;

    for (int i = 0; i < argc - 1; ++i) { // till (argc - 3) cause also reading (i + 3)
        if (strcmp(argv[i], _EQUATION_INPUT_MODE_FLAG) == 0) {
            readError = ParseQuadraticEquation(argv[i + 1], a, b, c);

            break;
        }
    }

    return CheckQuadraticEquationCoefficientsIsFinite(readError, *a, *b, *c);
}

//---------------------------------------------------------------------------------------------------------------------

Errors ReadFileNameFromCommandLine(int argc, const char *argv[], char *name, size_t size) {
    assert(name != NULL);
    assert(argv != NULL);

    Errors readError = Errors::READING_FILE_NAME_FROM_COMMAND_LINE_ERROR;

    for (int i = 0; i < argc - 1; ++i) {
        if (strcmp(argv[i], _FILE_FlAG) == 0) {
            readError = Errors::NO_ERRORS;
            strncpy(name, argv[i + 1], size - 1);
            break;
        }
    }

    return readError;
}

//---------------------------------------------------------------------------------------------------------------------

Errors ReadCoeffsFromStdin(double *a, double *b, double *c) {
    assert(a != NULL);
    assert(b != NULL);
    assert(c != NULL);

    static const int NUMBER_OF_READ_VALUES = 3;

    Errors quitInput = MenuInputCoeffs(); //quitInput - flag to quit the scan program without getting correct input
    int scannedVals = fscanf(stdin, "%lf%lf%lf", a, b, c);
    while (scannedVals != NUMBER_OF_READ_VALUES && quitInput == Errors::NO_ERRORS && scannedVals != EOF) {
        printf("not valid input\n");
        SkipSymbols();

        quitInput = MenuInputCoeffs();
        if (quitInput != Errors::NO_ERRORS) {
            printf("Quit the program\n");
            break;
        }

        scannedVals = fscanf(stdin, "%lf%lf%lf", a, b, c);
    }

    if (scannedVals == EOF) {
        return Errors::READING_COEFFS_FROM_STDIN_ERROR;
    }

    return CheckQuadraticEquationCoefficientsIsFinite(quitInput, *a, *b, *c);
}

//---------------------------------------------------------------------------------------------------------------------

Errors ReadEquationCoeffsFromStdin(double *a, double *b, double *c) {
    assert(a != NULL);
    assert(b != NULL);
    assert(c != NULL);

    static char equation[MAX_EQUATION_SIZE];

    Errors quitInput = MenuInputEquation();
    if (fgets(equation, MAX_EQUATION_SIZE, stdin) == NULL) {
        return Errors::READING_EQUATION_FROM_STDIN_ERROR;
    }

    while (quitInput == Errors::NO_ERRORS && ParseQuadraticEquation(equation, a, b, c) != Errors::NO_ERRORS) {
        printf("not valid input\n");
        //SkipSymbols();

        quitInput = MenuInputEquation();
        if (quitInput != Errors::NO_ERRORS) {
            printf("Quit the program\n");
            break;
        }

        char *tmp = fgets(equation, MAX_EQUATION_SIZE, stdin);
        if (tmp == NULL) {
            return Errors::READING_EQUATION_FROM_STDIN_ERROR;
        }
    }

    return CheckQuadraticEquationCoefficientsIsFinite(quitInput, *a, *b, *c);
}

//---------------------------------------------------------------------------------------------------------------------

Errors ReadFileNameFromStdin(char *name, size_t size) {
    assert(name != NULL);

    printf("Print file name with the length less than %zu (or EOF to quit): \n", size);

    Errors readError = Fgets_s(name, size);

    return readError == Errors::NO_ERRORS ? readError : Errors::READING_FILE_NAME_FROM_STDIN_ERROR;
}

//---------------------------------------------------------------------------------------------------------------------

Errors ReadCoeffsFromFile(double *a, double *b, double *c, FILE *fp) {
    assert(a  != NULL);
    assert(b  != NULL);
    assert(c  != NULL);
    assert(fp != NULL);

    static const int NUMBER_OF_READ_VALUES = 3;

    Errors error = Errors::NO_ERRORS;

    if (fscanf(fp, "%lf%lf%lf", a, b, c) != NUMBER_OF_READ_VALUES) {
        error = Errors::READING_COEFFS_FROM_FILE_ERROR;
    }

    return CheckQuadraticEquationCoefficientsIsFinite(error, *a, *b, *c);
}

//---------------------------------------------------------------------------------------------------------------------

Errors ReadEquationCoeffsFromFile(double *a, double *b, double *c, FILE *fp) {
    assert(a  != NULL);
    assert(b  != NULL);
    assert(c  != NULL);
    assert(fp != NULL);

    static char equation[MAX_EQUATION_SIZE];

    if (fgets(equation, MAX_EQUATION_SIZE, fp) == NULL) {
        return Errors::READING_EQUATION_FROM_FILE_ERROR;
    }

    return ParseQuadraticEquation(equation, a, b, c);
}

//---------------------------------------------------------------------------------------------------------------------

Errors PrintRoots(NumberOfRoots numberOfRoots, double x1, double x2) {

    switch (numberOfRoots) {
        case ZERO_ROOTS:
            printf("No roots\n");
            break;
        case ONE_ROOT:
            assert(isfinite(x1));

            printf("One root: %.8lf\n", x1);
            break;
        case TWO_ROOTS:
            assert(isfinite(x1));
            assert(isfinite(x2));

            printf("Two roots: %.8lf and %.8lf\n", x1, x2);
            break;
        case INF_ROOTS:
            printf("Infinity number of roots\n");
            break;
        default:
            return Errors::INVALID_NUMBER_OF_ROOTS;
    }


    return Errors::NO_ERRORS;
}

//---------------------------------------------------------------------------------------------------------------------

// returns 0 if OK else not 1, return the converted value to *val
static Errors StrToDouble(const char *str, double *val) {
    assert(str != NULL);
    assert(val != NULL);

    const char *endptr = strchr(str, '\0');

    char *ptr = NULL;
    *val = strtod(str, &ptr);

    return
            ptr != endptr
    ?
            Errors::CONVERTING_STRING_TO_DOUBLE_ERROR
    :
            Errors::NO_ERRORS;
}

//---------------------------------------------------------------------------------------------------------------------

static inline Errors MenuInputCoeffs() {
    printf("Print coefficients a, b, c for the equation type ax^2 + bx + c: (or q to quit)\n");

    int ch = getc(stdin);
    ungetc(ch, stdin);

    return ch == 'q' ? Errors::QUIT_THE_PROGRAM_WITHOUT_INPUT: Errors::NO_ERRORS;
}

//---------------------------------------------------------------------------------------------------------------------

static inline Errors MenuInputEquation() {
    printf("Print equation format: x^2 + 4x - 2x + 3 = 23 - 17x^2: (or <Enter> to quit) \n");

    int ch = getc(stdin);
    ungetc(ch, stdin);

    return ch == '\n' ? Errors::QUIT_THE_PROGRAM_WITHOUT_INPUT : Errors::NO_ERRORS;
}

//---------------------------------------------------------------------------------------------------------------------

static inline Errors TryOpenFile(const char *name, const char *mode, FILE **fp) {
    assert(name != NULL);
    assert(mode != NULL);
    assert(fp   != NULL);

    *fp = fopen(name, mode);

    return
            *fp == NULL
    ?
            Errors::FILE_OPENING_ERROR
    :
            Errors::NO_ERRORS;
}

//---------------------------------------------------------------------------------------------------------------------

Errors Fgets_s(char *name, size_t size) {
    assert(name != NULL);

    if (fgets(name, (int) size, stdin) == NULL) {
        return Errors::READING_FROM_STDIN_ERROR;
    }

    char *ptr = strchr(name, '\n');
    if (ptr != NULL) {
        *ptr = '\0';
    }

    return Errors::NO_ERRORS;
}

//---------------------------------------------------------------------------------------------------------------------
