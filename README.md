# SquareSolver
This program can solve quadratic equations.
It has different modes of working (command line flags):
1) -f - reading from file.
2) -c - reading from command line.
3) -s - reading from standard input
4) -t - testing mode
5) -eq - program reads equation and solves it

[!NOTE] user may mix flags and use them together
[!NOTE] testing mode can't be mixed with other mods.
[!IMPORTANT] -s flag is flag by standard (used if no reading flags are specified)
[!IMPORTANT] when using -c flag with -f user have to write file name right after -f flag in command line
[!IMPORTANT] when using -c flag with -eq user have to write file name right after -eq flag in command line
[!IMPORTANT] when using -c flag with -f user have to write file name right after -f flag in command line

## Examples:
1) `./quadratic_Solver -c -f filename.txt` (reading file name from command line)
2) `./quadratic_Solver -c -eq "5x^2 + 3x + 2 + 9x^2 = 78x - 21"` (reading equation from command line)
[!IMPORTANT] equation have to be surrounded my quotes
3) `./quadratic_Solver` (program will ask about coefficients in standard input)
4) `./quadric_Solver` (program will ask about file name in standard input)
5) `./quadric_Solver -t`
6) 
