//
// Created by Arman on 15.08.2023.
//

#include "InputAndOutput.h"
#include "Solver.h"
#include "StringAndCharFuncs.h"
#include "StringEquationFuncs.h"

static inline int menuInputCoeffs();
static inline int menuInputEquation();
static inline int tryOpenFile(const char *name, const char *mode, FILE **fp);
static int strToDouble(const char *str, double *val);
CommandLineFlags commandLineFlags = {0};

int ReadInput(int argc, const char *argv[], double *a, double *b, double *c) {
    assert(a != NULL);
    assert(b != NULL);
    assert(c != NULL);
    assert(argv != NULL);

    commandLineFlags = ReadCommandLineFlags(argc, argv);

    FILE *fp = NULL;
    static const size_t FILE_NAME_LENGTH = 64;
    int inputError = 0;
    static char fileName[FILE_NAME_LENGTH];

    if (commandLineFlags.readFromFile) {
        if (commandLineFlags.readFromCommandLine) {
            inputError |= ReadFileNameFromCommandLine(argc, argv, fileName, FILE_NAME_LENGTH);
        } else if (commandLineFlags.readFromStdin) {
            inputError |= ReadFileNameFromStdin(fileName, FILE_NAME_LENGTH);
        } else {
            perror("Unknown command line flag");
            return 1;
        }

        inputError |= tryOpenFile(fileName, "r", &fp);

        if (inputError) {
            fclose(fp);
            return inputError;
        }

        if (commandLineFlags.equationInputMode) inputError |= ReadEquationCoeffsFromFile(a, b, c, fp);
        else inputError |= ReadCoeffsFromFile(a, b, c, fp);

        fclose(fp);

        return inputError;
    }

    if (commandLineFlags.readFromStdin) {
        if (commandLineFlags.equationInputMode) inputError |= ReadEquationCoeffsFromStdin(a, b, c);
        else inputError |= ReadCoeffsFromStdin(a, b, c);
    } else if (commandLineFlags.readFromCommandLine) {
        if (commandLineFlags.equationInputMode) inputError |= ReadEquationCoeffsFromCommandLine(argc, argv, a, b, c);
        else inputError |= ReadCoeffsFromCommandLine(argc, argv, a, b, c);
    } else {
        perror("unknown command line flag");

        fclose(fp);

        return 1;
    }

    fclose(fp);

    return inputError;
}

CommandLineFlags ReadCommandLineFlags(int argc, const char *argv[]) {
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

int ReadCoeffsFromCommandLine(int argc, const char *argv[], double *a, double *b, double *c) {
    assert(a != NULL);
    assert(b != NULL);
    assert(c != NULL);

    int readError = 1;
    for (int i = 0; i < argc - 3; ++i) { // till (argc - 3) cause also reading (i + 3)
        if (strcmp(argv[i], _COMMAND_LINE_FLAG) == 0) {
            readError  = strToDouble(argv[i + 1], a);
            readError |= strToDouble(argv[i + 2], b);
            readError |= strToDouble(argv[i + 3], c);

            break;
        }
    }

    return readError;
}

int ReadEquationCoeffsFromCommandLine(int argc, const char *argv[], double *a, double *b, double *c) {
    assert(a != NULL);
    assert(b != NULL);
    assert(c != NULL);

    int readError = 1;
    for (int i = 0; i < argc - 1; ++i) { // till (argc - 3) cause also reading (i + 3)
        if (strcmp(argv[i], _EQUATION_INPUT_MODE_FLAG) == 0) {
            readError = ParseQuadraticEquation(argv[i + 1], a, b, c);

            break;
        }
    }

    return readError;
}

int ReadFileNameFromCommandLine(int argc, const char *argv[], char *name, size_t size) {
    assert(name != NULL);

    int readError = 1;
    for (int i = 0; i < argc - 1; ++i) {
        if (strcmp(argv[i], _FILE_FlAG) == 0) {
            readError = 0;
            strncpy(name, argv[i + 1], size - 1);

            break;
        }
    }

    return readError;
}

int ReadCoeffsFromStdin(double *a, double *b, double *c) {
    assert(a != NULL);
    assert(b != NULL);
    assert(c != NULL);

    static const int NUMBER_OF_READ_VALUES = 3;

    int quitInput = menuInputCoeffs(); //quitInput - flag to quit the scan program without getting correct input of coeffs
    int scannedVals = fscanf(stdin, "%lf%lf%lf", a, b, c);
    while (scannedVals != NUMBER_OF_READ_VALUES && quitInput != 1 && scannedVals != EOF) {
        printf("not valid input\n");
        skipSymbols();

        quitInput = menuInputCoeffs();
        if (quitInput) {
            printf("Quit the program\n");
            break;
        }

        scannedVals = fscanf(stdin, "%lf%lf%lf", a, b, c);
    }

    return quitInput;
}

int ReadEquationCoeffsFromStdin(double *a, double *b, double *c) {
    assert(a != NULL);
    assert(b != NULL);
    assert(c != NULL);

    static char equation[MAX_EQUATION_SIZE];

    int quitInput = menuInputEquation();
    if (fgets(equation, MAX_EQUATION_SIZE, stdin) == NULL) {
        perror("Error reading input");
        return 1;
    }

    while (quitInput != 1 && ParseQuadraticEquation(equation, a, b, c) != 0) {
        printf("not valid input\n");
        skipSymbols();

        quitInput = menuInputEquation();
        if (quitInput) {
            printf("Quit the program\n");
            break;
        }

        char *tmp = fgets(equation, MAX_EQUATION_SIZE, stdin);
        if (tmp == NULL) {
            perror("Error reading input");

            quitInput = 1;
            return quitInput;
        }
    }

    return quitInput;
}

int ReadFileNameFromStdin(char *name, size_t size) {
    assert(name != NULL);

    printf("Print file name with the length less than %zu (or EOF to quit): \n", size);

    int readError = fgets_s(name, size);
    if (readError) {
        perror("Error reading name of the file");

        return readError;
    }

    return readError;
}

int ReadCoeffsFromFile(double *a, double *b, double *c, FILE *fp) {
    assert(a != NULL);
    assert(b != NULL);
    assert(c != NULL);
    assert(fp != NULL);

    static const int NUMBER_OF_READ_VALUES = 3;

    return fscanf(fp, "%lf%lf%lf", a, b, c) != NUMBER_OF_READ_VALUES;
}

int ReadEquationCoeffsFromFile(double *a, double *b, double *c, FILE *fp) {
    assert(a != NULL);
    assert(b != NULL);
    assert(c != NULL);
    assert(fp != NULL);

    static char equation[MAX_EQUATION_SIZE];

    if (fgets(equation, MAX_EQUATION_SIZE, fp) == NULL) {
        perror("Error reading from file");
        return 1;
    }

    return ParseQuadraticEquation(equation, a, b, c);
}

int PrintRoots(enum NumberOfRoots numberOfRoots, double x1, double x2) {
    int printError = 0;

    switch (numberOfRoots) {
        case ZERO_ROOTS:
            printf("No roots\n");
            break;
        case ONE_ROOT:
            printf("One root: %.8lf\n", x1);
            break;
        case TWO_ROOTS:
            printf("Two roots: %.8lf and %.8lf\n", x1, x2);
            break;
        case INF_ROOTS:
            printf("Infinity number of roots\n");
            break;
        default:
            perror("Not valid number of roots");

            printError = 1;
            break;
    }

    return printError;
}

// returns 0 if OK else not 1, return the converted value to *val
static int strToDouble(const char *str, double *val) {
    const char *endptr = strchr(str, '\0');

    char *ptr = NULL;
    *val = strtod(str, &ptr);

    return (ptr != endptr);
}

static inline int menuInputCoeffs() {
    printf("Print coefficients a, b, c for the equation type ax^2 + bx + c: (or q to quit)\n");

    int ch = getc(stdin);
    ungetc(ch, stdin);

    return ch == 'q';
}

static inline int menuInputEquation() {
    printf("Print equation format: x^2 + 4x - 2x + 3 = 23 - 17x^2: (or <Enter> to quit) \n");

    int ch = getc(stdin);
    ungetc(ch, stdin);

    return (!isalpha(ch) && !isdigit(ch) && !isblank(ch) && ch != '.');
}

static inline int tryOpenFile(const char *name, const char *mode, FILE **fp) {
    assert(name != NULL);
    assert(mode != NULL);
    assert(fp != NULL);

    *fp = fopen(name, mode);
    if (*fp == NULL) {
        perror("Can't open the file");
    }

    return (*fp == NULL);
}