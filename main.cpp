#include "InputAndOutput.h"
#include "Solver.h"

#ifndef NDEBUG
#include "TestingMode.h"
#endif

int main(int argc, const char *argv[]) {
    //setbuf(stdout, NULL);
    unsigned int commandLineFlags = ReadCommandLineFlags(argc, argv);

#ifndef NDEBUG
    if (GetFlag(commandLineFlags, (int) FlagsIdInArray::TEST_MODE_FLAG))
        return (int) Testing();
#endif

    if (GetFlag(commandLineFlags, (int) FlagsIdInArray::HELP_FLAG))
        return (int) Help(commandLineFlags);
    
    double a = NAN, b = NAN, c = NAN;
    double x1 = NAN, x2 = NAN;

    Errors errors = ReadInput(argc, argv, &a, &b, &c);

    if (PrintErrors() != 0)
        return (int) errors;

    enum NumberOfRoots numberOfRoots = SolveQuadraticEquation(a, b, c, &x1, &x2);

    PrintRoots(numberOfRoots, x1, x2);
}
