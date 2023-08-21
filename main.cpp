#include "InputAndOutput.h"
#include "Solver.h"
#include "TestingMode.h"

int main(int argc, const char *argv[]) {
    //setbuf(stdout, NULL);

    CommandLineFlags commandLineFlags = ReadCommandLineFlags(argc, argv);

    if (commandLineFlags.testMode) {
        Testing();
        return 0;
    }

    double a = NAN, b = NAN, c = NAN;
    double x1 = NAN, x2 = NAN;

    Errors errors = ReadInput(argc, argv, &a, &b, &c);

    if (PrintErrors(errors) != 0) {
        return (int) errors;
    }

    enum NumberOfRoots numberOfRoots = SolveQuadraticEquation(a, b, c, &x1, &x2);

    PrintRoots(numberOfRoots, x1, x2);
}
