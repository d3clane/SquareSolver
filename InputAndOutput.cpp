//
// Created by Arman on 15.08.2023.
//

#include "InputAndOutput.h"
//---------------------------------------------------------------------------------------------------------------------

static inline Errors MenuInputCoeffs();
static inline Errors MenuInputEquation();

//---------------------------------------------------------------------------------------------------------------------

static inline void PrintStandartHelp();

static inline void PrintFileHelp();
static inline void PrintCommandLineHelp();
static inline void PrintStdinHelp();
static inline void PrintEquationHelp();
static inline void PrintHelpFuncHelp();

#ifndef NDEBUG
static inline void PrintTestHelp();
#endif

//---------------------------------------------------------------------------------------------------------------------

static inline Errors TryOpenFile(const char *name, const char *mode, FILE **fp);

//---------------------------------------------------------------------------------------------------------------------

const static CommandLineFlags_t FILE_FLAG           = {.short_flag = "-f", .long_flag = "--file",
                                                       .Help = PrintFileHelp,};

const static CommandLineFlags_t COMMAND_LINE_FLAG   = {.short_flag = "-c", .long_flag = "--cmd",
                                                       .Help = PrintCommandLineHelp,};

const static CommandLineFlags_t STDIN_FLAG          = {.short_flag = "-s", .long_flag = "--stdin",
                                                       .Help = PrintStdinHelp,};

#ifndef NDEBUG
const static CommandLineFlags_t TEST_MODE_FLAG      = {.short_flag = "-t", .long_flag = "--test",
                                                       .Help = PrintTestHelp,};
#endif

const static CommandLineFlags_t EQUATION_INPUT_FLAG = {.short_flag = "-eq", .long_flag = "--equation",
                                                       .Help = PrintEquationHelp,};          
                                                                                                                                                                                     
const static CommandLineFlags_t HELP_FLAG           = {.short_flag = "-h", .long_flag = "--help",
                                                       .Help = PrintHelpFuncHelp,};

//---------------------------------------------------------------------------------------------------------------------

/// \brief array with command line flags
#ifndef DEBUG
const static CommandLineFlags_t commandLineFlags[FLAGS_NUMBER] = {COMMAND_LINE_FLAG, STDIN_FLAG, 
                                                                  FILE_FLAG, 
                                                                  EQUATION_INPUT_FLAG,
                                                                  HELP_FLAG,
                                                                  TEST_MODE_FLAG,};
#else
const static CommandLineFlags_t commandLineFlags[FLAGS_NUMBER] = {COMMAND_LINE_FLAG, STDIN_FLAG,
                                                                  FILE_FLAG,
                                                                  EQUATION_INPUT_FLAG,
                                                                  HELP_FLAG,};
#endif          

//---------------------------------------------------------------------------------------------------------------------

/// \brief array converts enum ReadingID to enum FlagsIdInArray
/// \attention all enums ReadingId have to be different and less than MAX_FUNC_NUMBER
const int convertReadingId[MAX_FUNC_NUMBER] = {
    [TypesReadFuncs_t::FILE]  = (int) FlagsIdInArray::FILE_FLAG,
    [TypesReadFuncs_t::STDIN] = (int) FlagsIdInArray::STDIN_FLAG, 
    [TypesReadFuncs_t::CMD]   = (int) FlagsIdInArray::COMMAND_LINE_FLAG,
};

//---------------------------------------------------------------------------------------------------------------------

/// \brief funcs that can be used with -eq flag
const TypesReadFuncs_t EquationReadingFuncs = {
    .flagFuncs = {
        [TypesReadFuncs_t::FILE]  = ReadEquationCoeffsFromFile,
        [TypesReadFuncs_t::STDIN] = ReadEquationCoeffsFromStdin,
        [TypesReadFuncs_t::CMD]   = ReadEquationCoeffsFromCommandLine,
        }
};

/// \brief funcs that can be used with no input type flag                                                
const TypesReadFuncs_t CoeffsReadingFuncs   = {
    .flagFuncs = {
        [TypesReadFuncs_t::FILE]  = ReadCoeffsFromFile,
        [TypesReadFuncs_t::STDIN] = ReadCoeffsFromStdin,
        [TypesReadFuncs_t::CMD]   = ReadCoeffsFromCommandLine,
        }
};

/// \brief funcs that can be used with file reading input type
const TypesReadFuncs_t FileReadingFuncs     = {
    .flagFuncs = {
        [TypesReadFuncs_t::FILE]  = nullptr,
        [TypesReadFuncs_t::STDIN] = ReadFileNameFromStdin,
        [TypesReadFuncs_t::CMD]   = ReadFileNameFromCommandLine,
        }
};                                              

//---------------------------------------------------------------------------------------------------------------------

Errors ReadInput(const int argc, const char *argv[], double *a, double *b, double *c) {
    assert(a);
    assert(b);
    assert(c);
    assert(argv);

    unsigned int flagsActivated = ReadCommandLineFlags(argc, argv);

    if (!GetFlag(flagsActivated, (int) FlagsIdInArray::COMMAND_LINE_FLAG))
        AddFlag(&flagsActivated, (int) FlagsIdInArray::STDIN_FLAG);

    if (GetFlag(flagsActivated, (int) FlagsIdInArray::COMMAND_LINE_FLAG) &&
        GetFlag(flagsActivated, (int) FlagsIdInArray::STDIN_FLAG)) 
        DeleteFlag(&flagsActivated, (int) FlagsIdInArray::COMMAND_LINE_FLAG);

    FILE *fp = nullptr;
    static char fileName[FILE_NAME_LENGTH] = "";
    Errors inputError = Errors::NO_ERRORS;

    if (GetFlag(flagsActivated, (int) FlagsIdInArray::FILE_FLAG)) {

        DeleteFlag(&flagsActivated, (int) FlagsIdInArray::FILE_FLAG);
        
        inputError = CallFlagFunc(flagsActivated, 
                                  a, b, c, argc, argv, 
                                  fileName, FILE_NAME_LENGTH, nullptr, 
                                  &FileReadingFuncs);

        if (inputError != Errors::NO_ERRORS) return inputError;

        inputError = TryOpenFile(fileName, "r", &fp);

        if (inputError != Errors::NO_ERRORS) return inputError;                          

        AddFlag(&flagsActivated, (int) FlagsIdInArray::FILE_FLAG);
        DeleteFlag(&flagsActivated, (int) FlagsIdInArray::COMMAND_LINE_FLAG);
        DeleteFlag(&flagsActivated, (int) FlagsIdInArray::STDIN_FLAG);
    }

    if (GetFlag(flagsActivated, (int) FlagsIdInArray::EQUATION_INPUT_FLAG)) {
        inputError = CallFlagFunc(flagsActivated, 
                                  a, b, c, argc, argv, 
                                  nullptr, 0, fp, 
                                  &EquationReadingFuncs);        
    } else {
        inputError = CallFlagFunc(flagsActivated, 
                                  a, b, c, argc, argv, 
                                  nullptr, 0, fp, 
                                  &CoeffsReadingFuncs);
    }

    fclose(fp);

    return inputError;
}

//---------------------------------------------------------------------------------------------------------------------

//returns int with byte flags (like 101000) 1 - flag is on
unsigned int ReadCommandLineFlags(const int argc, const char *argv[]) {
    assert(argv);

    unsigned int flagsActivated = 0;
    for (int argvPos = 0; argvPos < argc; ++argvPos) {
        for (size_t flagNumber = 0; flagNumber < FLAGS_NUMBER; ++flagNumber) {

            if (CompareWithFlag(argv[argvPos], commandLineFlags + flagNumber) == 0) {
                flagsActivated |= (1u << flagNumber);
                break;
            }

        }
    }

    return flagsActivated;    
}

//---------------------------------------------------------------------------------------------------------------------

Errors ReadCoeffsFromCommandLine(double *a, double *b, double *c,
                                 const int argc, const char *argv[],
                                 char *name, const size_t size,
                                 FILE *fp) {

    assert(a);
    assert(b);
    assert(c);
    assert(argv);

    Errors readError = Errors::READING_COEFFS_FROM_COMMAND_LINE_ERROR;
    for (int i = 0; i < argc - 3; ++i) { // till (argc - 3) because also reading (i + 3)
        if (CompareWithFlag(argv[i], 
                            commandLineFlags + (int) FlagsIdInArray::COMMAND_LINE_FLAG) == 0) {
            readError = Errors::NO_ERRORS;
            char tmpchar = 0;

            if (sscanf(argv[i + 1], "%lf%c", a, &tmpchar) != 1)
                readError = Errors::READING_COEFFS_FROM_COMMAND_LINE_ERROR;
            if (sscanf(argv[i + 2], "%lf%c", b, &tmpchar) != 1) 
                readError = Errors::READING_COEFFS_FROM_COMMAND_LINE_ERROR;
            if (sscanf(argv[i + 3], "%lf%c", c, &tmpchar) != 1) 
                readError = Errors::READING_COEFFS_FROM_COMMAND_LINE_ERROR;

            break;
        }
    }
        
    if (readError != Errors::NO_ERRORS) {
        UpdateError(readError);
        return readError;
    }
    
    readError = CheckCoeffsIsFinite(*a, *b, *c);

    if (readError != Errors::NO_ERRORS)
        UpdateError(readError);
    
    return readError;
}

//---------------------------------------------------------------------------------------------------------------------

Errors ReadEquationCoeffsFromCommandLine(double *a, double *b, double *c,
                                         const int argc, const char *argv[],
                                         char *name, const size_t size,
                                         FILE *fp) {
    assert(a);
    assert(b);
    assert(c);
    assert(argv);                                            

    Errors readError = Errors::READING_EQUATION_FROM_COMMAND_LINE_ERROR;

    for (int i = 0; i < argc - 1; ++i) { // till (argc - 3) cause also reading (i + 3)
        if (CompareWithFlag(argv[i], 
                            commandLineFlags + (int) FlagsIdInArray::EQUATION_INPUT_FLAG) == 0) {
            
            if (strlen(argv[i + 1]) + 1 >= MAX_EQUATION_SIZE) { //TODO check >= or > 
                UpdateError(Errors::EXTRA_SYMBOLS_IN_LINE);
                return      Errors::EXTRA_SYMBOLS_IN_LINE;
            }
            printf("%s\n", argv[i + 1]);
            readError = ParseQuadraticEquation(argv[i + 1], a, b, c);

            break;
        }
    }

    if (readError == Errors::READING_EQUATION_FROM_COMMAND_LINE_ERROR)
         UpdateError(Errors::READING_EQUATION_FROM_COMMAND_LINE_ERROR);
    
    if (readError != Errors::NO_ERRORS) 
        return readError;
    
    readError = CheckCoeffsIsFinite(*a, *b, *c);

    UpdateError(readError);

    return readError;    
}

//---------------------------------------------------------------------------------------------------------------------

Errors ReadFileNameFromCommandLine(double *a, double *b, double *c,
                                   const int argc, const char *argv[],
                                   char *name, const size_t size,
                                   FILE *fp) {

    assert(name);
    assert(argv);

    Errors readError = Errors::READING_FILE_NAME_FROM_COMMAND_LINE_ERROR;

    for (int i = 0; i < argc - 1; ++i) {
        if (CompareWithFlag(argv[i],
                            commandLineFlags + (int) FlagsIdInArray::FILE_FLAG) == 0) {
            readError = Errors::NO_ERRORS;
            strncpy(name, argv[i + 1], size - 1);

            if (strlen(argv[i + 1]) > size - 1) {
                UpdateError(Errors::EXTRA_SYMBOLS_IN_LINE);
                return      Errors::EXTRA_SYMBOLS_IN_LINE;
            }
            
            name[size - 1] = '\0';
            break;
        }
    }

    UpdateError(readError);

    return readError;
}

//---------------------------------------------------------------------------------------------------------------------

Errors ReadCoeffsFromStdin(double *a, double *b, double *c,
                           const int argc, const char *argv[],
                           char *name, const size_t size,
                           FILE *fp) {
    assert(a);
    assert(b);
    assert(c);

    static const int NUMBER_OF_READ_VALUES = 3;

    Errors quitInput = Errors::NO_ERRORS;

    int scannedVals = 0;
    while (true) {
        quitInput = MenuInputCoeffs();
        scannedVals = fscanf(stdin, "%lf %lf %lf", a, b, c);

        if (quitInput != Errors::NO_ERRORS) break;
        if (SkipSymbols(stdin) == 0) {
            if (scannedVals == NUMBER_OF_READ_VALUES || 
                scannedVals == EOF)
                    break;
                
        } 

        printf("Not valid input\n");
    }

    if (scannedVals == EOF) {
        UpdateError(Errors::READING_COEFFS_FROM_STDIN_ERROR);
        return      Errors::READING_COEFFS_FROM_STDIN_ERROR;
    }

    if (quitInput != Errors::NO_ERRORS) 
        return quitInput;

    Errors error = CheckCoeffsIsFinite(*a, *b, *c);

    UpdateError(error);

    return error;
}

//---------------------------------------------------------------------------------------------------------------------

Errors ReadEquationCoeffsFromStdin(double *a, double *b, double *c,
                                   const int argc, const char *argv[],
                                   char *name, const size_t size,
                                   FILE *fp) {
    assert(a);
    assert(b);
    assert(c);

    static char equation[MAX_EQUATION_SIZE] = "";

    Errors quitInput = Errors::NO_ERRORS;
    while (true) {
        quitInput = MenuInputEquation();

        if (fgets(equation, MAX_EQUATION_SIZE, stdin) == nullptr) {
            UpdateError(Errors::READING_EQUATION_FROM_STDIN_ERROR);
            return      Errors::READING_EQUATION_FROM_STDIN_ERROR;
        }

        if (HasReadAllStringWithFgets(equation, MAX_EQUATION_SIZE, stdin)) {
            if (quitInput == Errors::QUIT_THE_PROGRAM_WITHOUT_INPUT || 
                ParseQuadraticEquation(equation, a, b, c) == Errors::NO_ERRORS) {
                break;
            }
        } else {
            printf("Equation is too long\n");
        }

        printf("Not valid input\n");
    }

    if (quitInput != Errors::NO_ERRORS) 
        return quitInput;

    Errors error = CheckCoeffsIsFinite(*a, *b, *c);

    UpdateError(error);

    return error;    
}

//---------------------------------------------------------------------------------------------------------------------

Errors ReadFileNameFromStdin(double *a, double *b, double *c,
                             const int argc, const char *argv[],
                             char *name, const size_t size,
                             FILE *fp) {
    assert(name);

    printf("Print file name with the length less than %zu (or EOF to quit): \n", size);

    Errors readError = Fgets_s(name, size, stdin);

    if (readError == Errors::READING_FROM_STDIN_ERROR) {
        UpdateError( Errors::READING_FILE_NAME_FROM_STDIN_ERROR);
        return       Errors::READING_FILE_NAME_FROM_STDIN_ERROR;
    }

    if (!HasReadAllStringWithFgets(name, size, stdin)) {
        UpdateError(Errors::EXTRA_SYMBOLS_IN_LINE);
        return      Errors::EXTRA_SYMBOLS_IN_LINE;
    }
    
    return readError;
}

//---------------------------------------------------------------------------------------------------------------------

Errors ReadCoeffsFromFile(double *a, double *b, double *c,
                          const int argc, const char *argv[],
                          char *name, const size_t size,
                          FILE *fp) {
    assert(a);
    assert(b);
    assert(c);

    static const int NUMBER_OF_READ_VALUES = 3;

    Errors error = Errors::NO_ERRORS;
    
    if (fscanf(fp, "%lf %lf %lf", a, b, c) != NUMBER_OF_READ_VALUES) {
        UpdateError(Errors::READING_COEFFS_FROM_FILE_ERROR);
        return      Errors::READING_COEFFS_FROM_FILE_ERROR;
    }
    if (SkipSymbols(fp) != 0) {
        UpdateError(Errors::EXTRA_SYMBOLS_IN_LINE);
        return      Errors::EXTRA_SYMBOLS_IN_LINE;
    }

    if (error != Errors::NO_ERRORS) 
        return error;
    
    error = CheckCoeffsIsFinite(*a, *b, *c);

    UpdateError(error);

    return error;    
}

//---------------------------------------------------------------------------------------------------------------------

Errors ReadEquationCoeffsFromFile(double *a, double *b, double *c,
                                  const int argc, const char *argv[],
                                  char *name, const size_t size,
                                  FILE *fp) {
    assert(a);
    assert(b);
    assert(c);
    assert(fp);

    static char equation[MAX_EQUATION_SIZE] = "";

    Errors error = Fgets_s(equation, MAX_EQUATION_SIZE, fp);

    if (error ==    Errors::READING_FROM_STDIN_ERROR) {
        UpdateError(Errors::READING_EQUATION_FROM_FILE_ERROR);
        return      Errors::READING_EQUATION_FROM_FILE_ERROR;
    }
    
    if (!HasReadAllStringWithFgets(equation, MAX_EQUATION_SIZE, fp)) {
        UpdateError(Errors::EXTRA_SYMBOLS_IN_LINE);
        return      Errors::EXTRA_SYMBOLS_IN_LINE;
    }

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
            UpdateError(Errors::INVALID_NUMBER_OF_ROOTS);
            return      Errors::INVALID_NUMBER_OF_ROOTS;
    }

    UpdateError(Errors::NO_ERRORS)
    return Errors::NO_ERRORS;
}

Errors Help(unsigned int flagsActivated) {
    Errors error = Errors::NO_HELPING_FLAG;

    if (!GetFlag(flagsActivated, (int) FlagsIdInArray::HELP_FLAG)) {
        UpdateError(error);
        return error;
    }

    bool foundAnyFlag = 0;
    for (size_t flagPos = 0; flagPos < FLAGS_NUMBER; ++flagPos) {
        if (flagPos == (int) FlagsIdInArray::HELP_FLAG) continue;

        if (GetFlag(flagsActivated, (int) flagPos)) {
            commandLineFlags[flagPos].Help();
            foundAnyFlag = 1;
        }
    }

    if (!foundAnyFlag)
        PrintStandartHelp();

    if (error != Errors::NO_ERRORS)
        UpdateError(error);

    return error;
}

//---------------------------------------------------------------------------------------------------------------------

static inline Errors MenuInputCoeffs() {
    printf("Print coefficients a, b, c for the equation type ax^2 + bx + c: (or q to quit)\n");

    int ch = getc(stdin);
    ungetc(ch, stdin);

    if (ch == 'q') {
        UpdateError(Errors::QUIT_THE_PROGRAM_WITHOUT_INPUT);
    }
    
    return ch == 'q' ? Errors::QUIT_THE_PROGRAM_WITHOUT_INPUT: Errors::NO_ERRORS;
}

//---------------------------------------------------------------------------------------------------------------------

static inline Errors MenuInputEquation() {
    printf("Print equation format: x^2 + 4x - 2x + 3 = 23 - 17x^2: (or <Enter> to quit) \n");
    printf("Equation size have to be less than %lu\n", MAX_EQUATION_SIZE);

    int ch = getc(stdin);
    ungetc(ch, stdin);

    if (ch == '\n') {
        UpdateError(Errors::QUIT_THE_PROGRAM_WITHOUT_INPUT);
    }
    
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

#ifndef NDEBUG
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

static inline void PrintFileHelp() {
    printf("Getting input from file\n"
           "\t --file (shortcut: -f) flag\n"
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
           "\t --cmd (shortcut: -c) flag)\n"
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
           "\t --stdin (shortcut: -s)\n"
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
           "\t --equation (shortcut: -eq) flag\n"
           "\tif -c is used with -eq equation have to be entered right after -eq flag\n");
    
    printf("Example: \n"
           "\t-c -eq \"2x^2 + 3.5x - 2\"\n\n\n\n");
}

//---------------------------------------------------------------------------------------------------------------------

static inline void PrintHelpFuncHelp() {
    printf("Help flag\n"
           "\t --help (shotrcut: -h)\n"
           "\tflag is used for receiving help and get info about program\n"
           "\tafter using -h flag other flags can be specified to get info about them\n");

    printf("Example of usage:\n"
           "\t-h -f -eq\n\n\n\n");
}

#ifndef NDEBUG
static inline void PrintTestHelp() {
    printf("Program enters testing mode\n"
           "\t --test (shortcut: -t)\n"
           "\tprogram reads tests from %s file\n\n\n\n", FILE_NAME);
}
#endif

//---------------------------------------------------------------------------------------------------------------------

static inline Errors TryOpenFile(const char *name, const char *mode, FILE **fp) {
    assert(name);
    assert(mode);
    assert(fp);

    *fp = fopen(name, mode);

    if (*fp == nullptr) {
        UpdateError(Errors::FILE_OPENING_ERROR);
    }
    
    return (*fp) ? Errors::NO_ERRORS : Errors::FILE_OPENING_ERROR;
}

//---------------------------------------------------------------------------------------------------------------------

Errors Fgets_s(char *name, const size_t size, FILE *fp) {
    assert(name);

    if (!fgets(name, (int) size, fp)) {
        UpdateError(Errors::READING_FROM_STDIN_ERROR);
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

int CompareWithFlag(const char *str, const CommandLineFlags_t *flags) {
    return strcmp(str, flags->short_flag) &&
           strcmp(str, flags->long_flag);
}

//---------------------------------------------------------------------------------------------------------------------

//null numeration
int GetFlag(unsigned int flagsActivated, int flagID) {
    return (flagsActivated >> flagID) & 1;
}

//---------------------------------------------------------------------------------------------------------------------

//null numeration
void DeleteFlag(unsigned int *flagsActivated, int flagID) {
    *flagsActivated &= ~(1u << flagID);
}

//---------------------------------------------------------------------------------------------------------------------

//null numeration
void AddFlag(unsigned int *flagsActivated, int flagID) {
    *flagsActivated |= (1u << flagID);
}

//---------------------------------------------------------------------------------------------------------------------

Errors CallFlagFunc(unsigned int flagsActivated,
                    double *a, double *b, double *c,
                    const int argc, const char *argv[],
                    char *name, const size_t size, FILE *fp, 
                    const TypesReadFuncs_t *Funcs) {
    
    for (size_t funcPos = 0; funcPos < MAX_FUNC_NUMBER; ++funcPos) {

            if (GetFlag(flagsActivated, convertReadingId[funcPos])) {
                if ((Funcs->flagFuncs)[funcPos])
                    return (Funcs->flagFuncs)[funcPos](a, b, c, argc, argv, 
                                                     name, size, fp);
                
                break;
            }
        }

    UpdateError(Errors::UNKNOWN_COMMAND_LINE_FLAG);
    return      Errors::UNKNOWN_COMMAND_LINE_FLAG;
}                    