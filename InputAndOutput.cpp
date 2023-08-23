//
// Created by Arman on 15.08.2023.
//

#include "InputAndOutput.h"

//---------------------------------------------------------------------------------------------------------------------

static inline Errors MenuInputCoeffs();
static inline Errors MenuInputEquation();
static inline Errors TryOpenFile(const char *name, const char *mode, FILE **fp);

//---------------------------------------------------------------------------------------------------------------------

const char *FILE_FlAG                =  "-f";

const char *COMMAND_LINE_FLAG        =  "-c";

const char *STDIN_FLAG               =  "-s";

#ifdef TEST
const char *TEST_MODE_FLAG           =  "-t";
#endif

const char *EQUATION_INPUT_MODE_FLAG = "-eq";

//---------------------------------------------------------------------------------------------------------------------

Errors ReadInput(const int argc, const char *argv[], double *a, double *b, double *c) {
    assert(a    != NULL);
    assert(b    != NULL);
    assert(c    != NULL);
    assert(argv != NULL);

    CommandLineFlags commandLineFlags = ReadCommandLineFlags(argc, argv);

    FILE *fp = NULL;
    static const size_t FILE_NAME_LENGTH = 64;
    static char fileName[FILE_NAME_LENGTH] = "";
    Errors inputError = Errors::NO_ERRORS;

    if (commandLineFlags.readFromFile) {
        if (commandLineFlags.readFromCommandLine) {
            inputError = ReadFileNameFromCommandLine(argc, argv, fileName, FILE_NAME_LENGTH);
        } else if (commandLineFlags.readFromStdin) {
            inputError = ReadFileNameFromStdin(fileName, FILE_NAME_LENGTH);
            printf("HERE7 %d\n", (int) inputError);
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

CommandLineFlags ReadCommandLineFlags(const int argc, const char *argv[]) {
    assert(argv != NULL);

    CommandLineFlags commandLineFlags = {0};

    for (int i = 0; i < argc; ++i) {
        if (strcmp(argv[i], FILE_FlAG) == 0)
            commandLineFlags.readFromFile = 1;
        else if (strcmp(argv[i], COMMAND_LINE_FLAG) == 0)
            commandLineFlags.readFromCommandLine = 1;
        else if (strcmp(argv[i], STDIN_FLAG) == 0)
            commandLineFlags.readFromStdin = 1;

        else if (strcmp(argv[i], EQUATION_INPUT_MODE_FLAG) == 0)
            commandLineFlags.equationInputMode = 1;

#ifdef TEST
        if (strcmp(argv[i], TEST_MODE_FLAG) == 0)
            commandLineFlags.testMode = 1;
#endif

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

Errors ReadCoeffsFromCommandLine(const int argc, const char *argv[], double *a, double *b, double *c) {
    assert(a    != NULL);
    assert(b    != NULL);
    assert(c    != NULL);
    assert(argv != NULL);

    Errors readError = Errors::READING_COEFFS_FROM_COMMAND_LINE_ERROR;
    for (int i = 0; i < argc - 3; ++i) { // till (argc - 3) cause also reading (i + 3)
        if (strcmp(argv[i], COMMAND_LINE_FLAG) == 0) {
            if (sscanf(argv[i + 1], "%lf%*c", a) != 1) readError = Errors::READING_COEFFS_FROM_COMMAND_LINE_ERROR;
            if (sscanf(argv[i + 2], "%lf%*c", b) != 1) readError = Errors::READING_COEFFS_FROM_COMMAND_LINE_ERROR;
            if (sscanf(argv[i + 3], "%lf%*c", c) != 1) readError = Errors::READING_COEFFS_FROM_COMMAND_LINE_ERROR;

            break;
        }
    }

    return readError != Errors::NO_ERRORS ? readError : CheckCoeffsIsFinite(*a, *b, *c);
}

//---------------------------------------------------------------------------------------------------------------------

Errors ReadEquationCoeffsFromCommandLine(const int argc, const char *argv[], double *a, double *b, double *c) {
    assert(a    != NULL);
    assert(b    != NULL);
    assert(c    != NULL);
    assert(argv != NULL);

    Errors readError = Errors::READING_EQUATION_FROM_COMMAND_LINE_ERROR;

    for (int i = 0; i < argc - 1; ++i) { // till (argc - 3) cause also reading (i + 3)
        if (strcmp(argv[i], EQUATION_INPUT_MODE_FLAG) == 0) {
            if (strlen(argv[i + 1]) + 1 >= MAX_EQUATION_SIZE)
                return Errors::EXTRA_SYMBOLS_IN_LINE;

            readError = ParseQuadraticEquation(argv[i + 1], a, b, c);
            
            break;
        }
    }

    return readError != Errors::NO_ERRORS ? readError : CheckCoeffsIsFinite(*a, *b, *c);
}

//---------------------------------------------------------------------------------------------------------------------

Errors ReadFileNameFromCommandLine(const int argc, const char *argv[], char *name, const size_t size) {
    assert(name != NULL);
    assert(argv != NULL);

    Errors readError = Errors::READING_FILE_NAME_FROM_COMMAND_LINE_ERROR;

    for (int i = 0; i < argc - 1; ++i) {
        if (strcmp(argv[i], FILE_FlAG) == 0) {
            readError = Errors::NO_ERRORS;
            strncpy(name, argv[i + 1], size - 1);

            if (strlen(argv[i + 1]) > size - 1) 
                return Errors::EXTRA_SYMBOLS_IN_LINE;
            
            name[size - 1] = '\0';
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

    Errors quitInput = Errors::NO_ERRORS;

    int scannedVals = 0;
    while (true) {
        quitInput = MenuInputCoeffs();
        scannedVals = fscanf(stdin, "%lf %lf %lf", a, b, c);
        
        if (SkipSymbols(stdin) == 0) {
            if (scannedVals == NUMBER_OF_READ_VALUES || 
                quitInput != Errors::NO_ERRORS       || scannedVals == EOF) {
                break;
                }
        } 

        printf("Not valid input\n");
    }


    if (scannedVals == EOF) {
        return Errors::READING_COEFFS_FROM_STDIN_ERROR;
    }

    return quitInput != Errors::NO_ERRORS ? quitInput : CheckCoeffsIsFinite(*a, *b, *c);
}

//---------------------------------------------------------------------------------------------------------------------

Errors ReadEquationCoeffsFromStdin(double *a, double *b, double *c) {
    assert(a != NULL);
    assert(b != NULL);
    assert(c != NULL);

    static char equation[MAX_EQUATION_SIZE] = {0};

    Errors quitInput = Errors::NO_ERRORS;
    while (true) {
        quitInput = MenuInputEquation();

        if (fgets(equation, MAX_EQUATION_SIZE, stdin) == NULL) 
            return Errors::READING_EQUATION_FROM_STDIN_ERROR;

        //printf("%s\n", equation);
        if (HasReadAllStringWithFgets(equation, MAX_EQUATION_SIZE, stdin)) {
            if (quitInput == Errors::QUIT_THE_PROGRAM_WITHOUT_INPUT ||  ParseQuadraticEquation(equation, a, b, c) == Errors::NO_ERRORS) {
                break;
            }
        } else {
            printf("Equation is too long\n");
        }

        printf("Not valid input\n");
    }

    return quitInput != Errors::NO_ERRORS ? quitInput : CheckCoeffsIsFinite(*a, *b, *c);
}

//---------------------------------------------------------------------------------------------------------------------

Errors ReadFileNameFromStdin(char *name, const size_t size) {
    assert(name != NULL);

    printf("Print file name with the length less than %zu (or EOF to quit): \n", size);

    Errors readError = Fgets_s(name, size, stdin);

    if (readError == Errors::READING_FROM_STDIN_ERROR)
        readError = Errors::READING_FILE_NAME_FROM_STDIN_ERROR;

    if (!HasReadAllStringWithFgets(name, size, stdin)) {
        return Errors::EXTRA_SYMBOLS_IN_LINE;
    }


    return readError;
}

//---------------------------------------------------------------------------------------------------------------------

Errors ReadCoeffsFromFile(double *a, double *b, double *c, FILE *fp) {
    assert(a  != NULL);
    assert(b  != NULL);
    assert(c  != NULL);

    static const int NUMBER_OF_READ_VALUES = 3;

    Errors error = Errors::NO_ERRORS;

    if (fscanf(fp, "%lf %lf %lf", a, b, c) != NUMBER_OF_READ_VALUES) {
        error = Errors::READING_COEFFS_FROM_FILE_ERROR;
    }
    int cnt = 0;
    if ((cnt = SkipSymbols(fp)) != 0)
        return Errors::EXTRA_SYMBOLS_IN_LINE;
    
    return error != Errors::NO_ERRORS ? error : CheckCoeffsIsFinite(*a, *b, *c);
}

//---------------------------------------------------------------------------------------------------------------------

Errors ReadEquationCoeffsFromFile(double *a, double *b, double *c, FILE *fp) {
    assert(a  != NULL);
    assert(b  != NULL);
    assert(c  != NULL);
    assert(fp != NULL);

    static char equation[MAX_EQUATION_SIZE] = "";

    Errors error = Fgets_s(equation, MAX_EQUATION_SIZE, fp);
    if (error == Errors::READING_FROM_STDIN_ERROR)
        error = Errors::READING_EQUATION_FROM_FILE_ERROR;
    
    if (!HasReadAllStringWithFgets(equation, MAX_EQUATION_SIZE, fp)) 
        return Errors::EXTRA_SYMBOLS_IN_LINE;

    return ParseQuadraticEquation(equation, a, b, c);
}

//---------------------------------------------------------------------------------------------------------------------

Errors PrintRoots(const NumberOfRoots numberOfRoots, const double x1, const double x2) {

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

static inline Errors MenuInputCoeffs() {
    printf("Print coefficients a, b, c for the equation type ax^2 + bx + c: (or q to quit)\n");

    int ch = getc(stdin);
    ungetc(ch, stdin);

    return ch == 'q' ? Errors::QUIT_THE_PROGRAM_WITHOUT_INPUT: Errors::NO_ERRORS;
}

//---------------------------------------------------------------------------------------------------------------------

static inline Errors MenuInputEquation() {
    printf("Print equation format: x^2 + 4x - 2x + 3 = 23 - 17x^2: (or <Enter> to quit) \n");
    printf("Equation size have to be less than %lu\n", MAX_EQUATION_SIZE);

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

    return (*fp == NULL) ? Errors::FILE_OPENING_ERROR :
                           Errors::NO_ERRORS;
}

//---------------------------------------------------------------------------------------------------------------------

Errors Fgets_s(char *name, const size_t size, FILE *fp) {
    assert(name != NULL);

    if (fgets(name, (int) size, fp) == NULL) {
        return Errors::READING_FROM_STDIN_ERROR;
    }

    char *ptr = strchr(name, '\n');
    if (ptr != NULL) {
        *ptr = '\0';
    }
    printf("Here5\n");
    return Errors::NO_ERRORS;
}

//---------------------------------------------------------------------------------------------------------------------

bool HasReadAllStringWithFgets(const char *str, const size_t size, FILE *fp) {
    if (strlen(str) + 1 < size || str[size - 2] == '\0') {
        printf("here2\n");
        return 1;
    }  
    
    printf("Here1\n");
    return SkipSymbols(fp) == 0 ? 1 : 0;
}

//---------------------------------------------------------------------------------------------------------------------