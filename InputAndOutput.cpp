//
// Created by Arman on 15.08.2023.
//

#include "InputAndOutput.h"

//---------------------------------------------------------------------------------------------------------------------

static inline Errors MenuInputCoeffs();
static inline Errors MenuInputEquation();

//---------------------------------------------------------------------------------------------------------------------

static inline Errors PrintFlagHelp(const char *flag);
static inline void PrintStandartHelp();

static inline void PrintFileHelp();
static inline void PrintCommandLineHelp();
static inline void PrintStdinHelp();
static inline void PrintEquationHelp();
static inline void PrintHelpFuncHelp();

#ifdef TEST
static inline void PrintTestHelp();
#endif

//---------------------------------------------------------------------------------------------------------------------

static inline Errors TryOpenFile(const char *name, const char *mode, FILE **fp);

//---------------------------------------------------------------------------------------------------------------------

const char *const FILE_FlAG                =  "-f";

const char *const COMMAND_LINE_FLAG        =  "-c";

const char *const STDIN_FLAG               =  "-s";

#ifdef TEST
const char *const TEST_MODE_FLAG           =  "-t";
#endif

const char *const EQUATION_INPUT_MODE_FLAG = "-eq";

const char *const HELP_FLAG                = "-h";

const char *const ALL_FLAGS                = "-f-c-s-t-eq-h";

//---------------------------------------------------------------------------------------------------------------------

Errors ReadInput(const int argc, const char *argv[], double *a, double *b, double *c) {
    assert(a);
    assert(b);
    assert(c);
    assert(argv);

    CommandLineFlags commandLineFlags = ReadCommandLineFlags(argc, argv);

    FILE *fp = nullptr;
    static const size_t FILE_NAME_LENGTH = 64;
    static char fileName[FILE_NAME_LENGTH] = "";
    Errors inputError = Errors::NO_ERRORS;

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

CommandLineFlags ReadCommandLineFlags(const int argc, const char *argv[]) {
    assert(argv);

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
        else if (strcmp(argv[i], HELP_FLAG) == 0)
            commandLineFlags.helpMode = 1;

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
    assert(a);
    assert(b);
    assert(c);
    assert(argv);

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
    assert(a);
    assert(b);
    assert(c);
    assert(argv);

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
    assert(name);
    assert(argv);

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
    assert(a);
    assert(b);
    assert(c);

    static const int NUMBER_OF_READ_VALUES = 3;

    Errors quitInput = Errors::NO_ERRORS;

    int scannedVals = 0;
    while (true) {
        quitInput = MenuInputCoeffs();
        scannedVals = fscanf(stdin, "%lf %lf %lf", a, b, c);
        
        if (SkipSymbols(stdin) == 0) {
            if (scannedVals == NUMBER_OF_READ_VALUES || 
                quitInput != Errors::NO_ERRORS       || 
                scannedVals == EOF)
                    break;
                
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
    assert(a);
    assert(b);
    assert(c);

    static char equation[MAX_EQUATION_SIZE] = "";

    Errors quitInput = Errors::NO_ERRORS;
    while (true) {
        quitInput = MenuInputEquation();

        if (fgets(equation, MAX_EQUATION_SIZE, stdin) == nullptr) 
            return Errors::READING_EQUATION_FROM_STDIN_ERROR;

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
    assert(name);

    printf("Print file name with the length less than %zu (or EOF to quit): \n", size);

    Errors readError = Fgets_s(name, size, stdin);

    if (readError == Errors::READING_FROM_STDIN_ERROR)
        readError = Errors::READING_FILE_NAME_FROM_STDIN_ERROR;

    if (!HasReadAllStringWithFgets(name, size, stdin))
        return Errors::EXTRA_SYMBOLS_IN_LINE;
    
    return readError;
}

//---------------------------------------------------------------------------------------------------------------------

Errors ReadCoeffsFromFile(double *a, double *b, double *c, FILE *fp) {
    assert(a);
    assert(b);
    assert(c);

    static const int NUMBER_OF_READ_VALUES = 3;

    Errors error = Errors::NO_ERRORS;

    if (fscanf(fp, "%lf %lf %lf", a, b, c) != NUMBER_OF_READ_VALUES)
        error = Errors::READING_COEFFS_FROM_FILE_ERROR;

    if (SkipSymbols(fp) != 0)
        return Errors::EXTRA_SYMBOLS_IN_LINE;
    
    return error != Errors::NO_ERRORS ? error : CheckCoeffsIsFinite(*a, *b, *c);
}

//---------------------------------------------------------------------------------------------------------------------

Errors ReadEquationCoeffsFromFile(double *a, double *b, double *c, FILE *fp) {
    assert(a);
    assert(b);
    assert(c);
    assert(fp);

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

Errors Help(const int argc, const char *argv[]) {
    Errors error = Errors::NO_HELPING_FLAG;

    for (int i = 0; i < argc; ++i) {

        if (strcmp(argv[i], HELP_FLAG) == 0) {
            if (i == argc - 1) 
                PrintStandartHelp();
            else {
                for (++i; i < argc; ++i)
                    error = PrintFlagHelp(argv[i]); 

            }

            break;
        }
    }

    return error;
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

static inline void PrintStandartHelp() {
    printf("\n\n\nThis program can solve quadratic equation.\n"
           "\tFlags that can be specified in program:\n"
           "\t\treading input modes:\n"
           "\t\t\t-f - file input\n"
           "\t\t\t-c - command line input\n"
           "\t\t\t-s - stdin input\n\n"
           "\t\tflag that specify another input type:\n"
           "\t\t\t-eq - reading equation type input and solve it\n\n"
           "\t\tOther flags:\n"
           "\t\t\t-h - flag used for receiving help and get info about program\n"
           "\t\t\tafter using -h flag another flags can be specified to get info about them\n");

#ifdef TEST
    printf("\t\t\t-t flag activated testing mode\n");
#endif 

    printf("User can mix different flags\n");

    printf("\tExamples of usage:\n"
           "\t\t-c -f filename\n"
           "\t\t-f\n"
           "\t\t-c -eq -f filename\n"
           "\t\t-c -eq \"5.2x^2 + 2x + 3 = 0\"\n"
           "\t\t-h -f -eq -c\n");

}

//---------------------------------------------------------------------------------------------------------------------

static inline Errors PrintFlagHelp(const char *flag) {

#ifdef TEST
    if (strcmp(flag, TEST_MODE_FLAG) == 0) {
        PrintTestHelp();
        return Errors::NO_ERRORS;
    }
#endif

    if (strcmp(flag, FILE_FlAG) == 0)
        PrintFileHelp();
    else if (strcmp(flag, COMMAND_LINE_FLAG) == 0)
        PrintCommandLineHelp();
    else if (strcmp(flag, STDIN_FLAG) == 0)
        PrintStdinHelp();
    else if (strcmp(flag, EQUATION_INPUT_MODE_FLAG) == 0)
        PrintEquationHelp();
    else if (strcmp(flag, HELP_FLAG) == 0)
        PrintHelpFuncHelp();
    else 
        return Errors::HELPING_ERROR;

    return Errors::NO_ERRORS;
}
//---------------------------------------------------------------------------------------------------------------------

static inline void PrintFileHelp() {
    printf("Getting input from file\n"
           "\t-f flag (shortcut)\n"
           "\tif -c is used with -f file name have to be entered in command line right after -f flag\n"
           "\tif -c is not used program will ask to enter file name in stdin\n");
        
    printf("Examples of usage:\n"
           "\t-c -f filename.txt\n"
           "\t-c -eq -f filename.txt\n"
           "\t-s -f\n"
           "\t-f\n\n\n\n");
}

//---------------------------------------------------------------------------------------------------------------------

static inline void PrintCommandLineHelp() {
    printf("Getting input from command line\n"
           "\t-c flag (shortcut)\n"
           "\tif -c is used program expects input from command line (coefficients) right after -c\n");

    printf("Examples of usage: \n"
           "\t-c -eq \"2x^2 + 2x - 3\"\n"
           "\t-c -eq -f filename.txt\n"
           "\t-c -f filename.txt\n"
           "\t-c 7 2 3\n\n\n\n");
}

//---------------------------------------------------------------------------------------------------------------------

static inline void PrintStdinHelp() {
    printf("Getting input from stdin\n"
           "\t-s flag (shortcut)\n"
           "\tif -s flag is used program expects input from stdandard input\n"
           "\tif -s and -c flags are not specified -s flag is used\n"
           "\tif -s and -c flags are specified at once -s flag is used\n");
    
    printf("Examples of usage:\n"
           "\t-s -f\n"
           "\t-s\n\n\n\n");
}

//---------------------------------------------------------------------------------------------------------------------

static inline void PrintEquationHelp() {
    printf("Getting equation type info\n"
           "\t-eq flag (shortcut)\n"
           "\tif -c is used with -eq equation have to be entered right after -eq flag\n");
    
    printf("Example: \n"
           "\t-c -eq \"2x^2 + 3.5x - 2\"\n\n\n\n");
}

//---------------------------------------------------------------------------------------------------------------------

static inline void PrintHelpFuncHelp() {
    printf("Help flag\n"
           "\t-h (shortcut)\n"
           "\tflag is used for receiving help and get info about program\n"
           "\tafter using -h flag other flags can be specified to get info about them\n");

    printf("Example of usage:\n"
           "\t-h -f -eq\n\n\n\n");
}

#ifdef TEST
static inline void PrintTestHelp() {
    printf("Program enters testing mode\n"
           "\t-t (shortcut)\n"
           "\tprogram reads tests from %s file\n\n\n\n", FILE_NAME);
}
#endif

//---------------------------------------------------------------------------------------------------------------------

static inline Errors TryOpenFile(const char *name, const char *mode, FILE **fp) {
    assert(name);
    assert(mode);
    assert(fp);

    *fp = fopen(name, mode);

    return (*fp) ? Errors::NO_ERRORS : Errors::FILE_OPENING_ERROR;
}

//---------------------------------------------------------------------------------------------------------------------

Errors Fgets_s(char *name, const size_t size, FILE *fp) {
    assert(name);

    if (!fgets(name, (int) size, fp)) {
        return Errors::READING_FROM_STDIN_ERROR;
    }

    char *ptr = strchr(name, '\n');
    if (ptr)
        *ptr = '\0';
    
    return Errors::NO_ERRORS;
}

//---------------------------------------------------------------------------------------------------------------------

bool HasReadAllStringWithFgets(const char *str, const size_t size, FILE *fp) {
    if (strlen(str) + 1 < size || str[size - 2] == '\0') {
        return 1;
    }  
    
    return SkipSymbols(fp) == 0 ? 1 : 0;
}

//---------------------------------------------------------------------------------------------------------------------
