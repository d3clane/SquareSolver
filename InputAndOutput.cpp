//
// Created by Arman on 15.08.2023.
//
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "InputAndOutput.h"
#include "solver.h"

static inline void skipSymbols();
static inline int menuInputCoeffs();
static inline int fgets_s(char *name, size_t size);
static inline int tryOpenFile(const char *name, const char *mode, FILE **fp);
static int strToDouble(const char *str, double *val);

int ReadInput(int argc, const char *argv[], double *a, double *b, double *c) {
    assert(a != NULL);
    assert(b != NULL);
    assert(c != NULL);
    assert(argv != NULL);
    CommandLineFlags commandLineFlags = ReadCommandLineFlags(argc, argv);
    FILE *fp = NULL;
    const size_t FILE_NAME_LENGTH = 64;
    int inputSuccess = 0; ///< returning value
    char fileName[FILE_NAME_LENGTH];
    if (commandLineFlags.readFromStdin) {

        if (commandLineFlags.readFromFile) {
            inputSuccess |= ReadFileNameFromStdin(fileName, FILE_NAME_LENGTH);
            inputSuccess |= tryOpenFile(fileName, "r", &fp);
            if (inputSuccess != 0) {
                return inputSuccess;
            }
            inputSuccess |= ReadCoeffsFromFile(a, b, c, fp);
        } else {
            inputSuccess |= ReadCoeffsFromStdin(a, b, c);
        }
    } else if (commandLineFlags.readFromCommandLine) {

        if (commandLineFlags.readFromFile) {
            inputSuccess |= ReadFileNameFromCommandLine(argc, argv, fileName, FILE_NAME_LENGTH);
            inputSuccess |= tryOpenFile(fileName, "r", &fp);
            if (inputSuccess != 0) {
                return inputSuccess;
            }
            inputSuccess |= ReadCoeffsFromFile(a, b, c, fp);
        } else {
            inputSuccess |= ReadCoeffsFromCommandLine(argc, argv, a, b, c);
        }

    } else {
        fprintf(stderr, "Wrong command line flags\n");
        inputSuccess = 1;
    }
    fclose(fp);
    return inputSuccess;
}

CommandLineFlags ReadCommandLineFlags(int argc, const char *argv[]) {
    CommandLineFlags flags = {0, 0, 0};
    for (int i = 0; i < argc; ++i) {
        if (strcmp(argv[i], FILEFlAG) == 0) {
            flags.readFromFile = 1;
        } else if (strcmp(argv[i], COMMANDLINEFLAG) == 0) {
            flags.readFromCommandLine = 1;
        } else if (strcmp(argv[i], STDINFLAG) == 0) {
            flags.readFromStdin = 1;
        }
    }

    if (!flags.readFromCommandLine) {
        flags.readFromStdin = 1;
    }
    if (flags.readFromStdin) {
        flags.readFromCommandLine = 0;
    }
    return flags;
}

int ReadCoeffsFromCommandLine(int argc, const char *argv[], double *a, double *b, double *c) {
    assert(a != NULL);
    assert(b != NULL);
    assert(c != NULL);
    int readSuccess = 1;
    for (int i = 0; i < argc - 3; ++i) { // till (argc - 3) cause also reading (i + 3)
        if (strcmp(argv[i], COMMANDLINEFLAG) == 0) {
            readSuccess = strToDouble(argv[i + 1], a);
            readSuccess |= strToDouble(argv[i + 2], b);
            readSuccess |= strToDouble(argv[i + 3], c);
        }
    }
    return readSuccess;
}

int ReadFileNameFromCommandLine(int argc, const char *argv[], char *name, size_t size) {
    assert(name != NULL);
    int readSuccess = 1; //returned val
    for (int i = 0; i < argc - 1; ++i) {
        if (strcmp(argv[i], FILEFlAG) == 0) {
            readSuccess = 0;
            strncpy(name, argv[i + 1], size - 1);
        }
    }
    return readSuccess;
}

int ReadCoeffsFromStdin(double *a, double *b, double *c) {
    assert(a != NULL);
    assert(b != NULL);
    assert(c != NULL);
    const int NUMBER_OF_READ_VALUES = 3;

    int quitInput = menuInputCoeffs(); //quitInput - flag to quit the scan program without getting correct input of coeffs

    int scannedVals = fscanf(stdin, "%lf%lf%lf", a, b, c);
    while (scannedVals != NUMBER_OF_READ_VALUES && quitInput != 1) {
        fprintf(stdout, "not valid input\n");
        skipSymbols();
        quitInput = menuInputCoeffs();
        scannedVals = fscanf(stdin, "%lf%lf%lf", a, b, c);
    }

    return quitInput;
}

int ReadFileNameFromStdin(char *name, size_t size) {
    assert(name != NULL);
    fprintf(stdout, "Print file name with the length less than %zu (or EOF to quit): \n", size);
    int readSuccess = fgets_s(name, size);
    if (readSuccess == 1) {
        fprintf(stderr, "Error reading name of the file\n");
    }
    return readSuccess;
}

int ReadCoeffsFromFile(double *a, double *b, double *c, FILE *fp) {
    assert(a != NULL);
    assert(b != NULL);
    assert(c != NULL);
    assert(fp != NULL);
    const int NUMBER_OF_READ_VALUES = 3;
    return fscanf(fp, "%lf%lf%lf", a, b, c) != NUMBER_OF_READ_VALUES;
}



int PrintRoots(enum NumberOfRoots numberOfRoots, double x1, double x2) {
    int printSuccess = 0;
    switch (numberOfRoots) {
        case ZERO_ROOTS:
            fprintf(stdout, "No roots\n");
            break;
        case ONE_ROOT:
            fprintf(stdout, "One root: %.8lf\n", x1);
            break;
        case TWO_ROOTS:
            fprintf(stdout, "Two roots: %.8lf and %.8lf\n", x1, x2);
            break;
        case INF_ROOTS:
            fprintf(stdout, "Infinity number of roots\n");
            break;
        default:
            fprintf(stderr, "Not valid number of roots\n");
            printSuccess = 1;
            break;
    }
    return printSuccess;
}

// returns 0 if OK else not 1, return the converted value to *val
static int strToDouble(const char *str, double *val) {
    const char *endptr = strchr(str, '\0');
    char *ptr = NULL;
    *val = strtod(str, &ptr);
    return (ptr != endptr);
}

static inline int menuInputCoeffs() {
    fprintf(stdout, "Print coefficients a, b, c for the equation type ax^2 + bx + c: (or q to quit)\n");
    int ch = getc(stdin);
    ungetc(ch, stdin);
    return ch == 'q';
}

static inline int fgets_s(char *name, size_t size) {
    int readSuccess = 0;
    readSuccess = (fgets(name, (int) size, stdin) == NULL); //
    char *ptr = strchr(name, '\n');
    if (ptr != NULL) {
        *ptr = '\0';
    }
    return readSuccess;
}

static inline int tryOpenFile(const char *name, const char *mode, FILE **fp) {
    assert(name != NULL);
    assert(mode != NULL);
    *fp = fopen(name, mode);
    if (*fp == NULL) {
        fprintf(stderr, "Can't open the file %s\n", name);
    }
    return (*fp == NULL);
}

static inline void skipSymbols() {
    while (getchar() != '\n') {
        continue;
    }
}
