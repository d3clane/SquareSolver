# SquareSolver
This program can solve quadratic equations.
It has different modes of working (command line flags):
1) -f (--file)      - reading from file.
2) -c (--cmd)       - reading from command line.
3) -s (--stdin)     - reading from standard input 
4) -t (--test)      - testing mode
5) -eq (--equation) - program reads equation and solves it
6) -h (--help)      - prints help
   
If -eq is not specified program is reading only 3 coefficients of the equation.

> [!NOTE]
> user may mix flags and use them together

> [!NOTE]
> testing mode can't be mixed with other mods.

> [!NOTE]
> User can omit the flag. -s flag will be used.

> [!IMPORTANT]
> when using -c flag with -f user have to write file name right after -f flag in command line

> [!IMPORTANT]
> when using -c flag with -eq user have to write file name right after -eq flag in command line.
> Equation have to be surrounded by qoutes

> [!IMPORTANT]
> when using -c flag with -f user have to write file name right after -f flag in command line


## Examples:
1) `./quadratic_Solver -c -f filename.txt` (reading file name from command line)
2) `./quadratic_Solver -c -eq "5x^2 + 3x + 2 + 9x^2 = 78x - 21"` (reading equation from command line)
3) `./quadratic_Solver` (program will ask about coefficients in standard input)
4) `./quadratic_Solver -f` (program will ask file name in standard input)
5) `./quadratic_Solver -t` (program will start tests)
6) `./quadratic_Solver -c 5 -2 3` - solves equation 5x^2 - 2x + 3 = 0

### Equation mode:

User can use different namings of x and mix them:  
`./quadratic_Solver -c -eq "5z^2 + 3x - 2 = 5y"`.  
Program will solve this equation as "5x^2 + 3x - 2 = 5x"  

> [!WARNING]
> letter e should not be used as a name. It would lead to undefined behaviour.
