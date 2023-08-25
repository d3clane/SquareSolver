//
// Created by Arman on 18.08.2023.
//

/*! \file
 * \brief contains functions and info for testing program
 *
 * contains functions for testing and info about file name and opening mode
 * all functions should be used only after using -t command line mode
 */
#ifndef TESTING_MODE_H
#define TESTING_MODE_H

#include <stdio.h>
#include <string.h>

#include "Colors.h"
#include "DoubleFuncs.h"
#include "Solver.h"

//---------------------------------------------------------------------------------------------------------------------

extern const char *const FILE_NAME;
extern const char *const FILE_MODE;   

//---------------------------------------------------------------------------------------------------------------------

/*!
 *\brief  function reads input from test file and checks if answers are correct
 */
 /*!
  * function read input from file $FILE_NAME$ with mode $FILE_MODE$ and compares calculated answers with answers from file \n
  * format of test file input : \n
  * int1 double1 double2 double3 double4 double5 (different quadratic equation input should be on different string) \n
  * int1 - number of roots. Could be 0, 1, 2. Any other number counts as infinite number of roots \n
  * double1 - quadratic coefficient a \n
  * double2 - linear coefficient b \n
  * double3 - free coefficient c \n
  * double4 - root1 (could be any value if number of roots is 0) \n
  * double5 - root2 (could be any value if number of roots is 1 or 0) \n
  * if number of roots is infinite any 2 values should be mentioned as roots. \n
  * \return Errors
  */
 Errors Testing();

//---------------------------------------------------------------------------------------------------------------------

/// \brief tests only one equation
/// \param [in] a quadratic coefficeints 
/// \param [in] b linear coefficients 
/// \param [in] c free coefficient
/// \param [in] testX1 reference X1 from tests
/// \param [in] testX2 reference X2 from tests
/// \param [in] numberOfRoots reference number of roots from tests
/// \param [in] testNumber number of test running right now
/// \return errors it they occur
Errors TestOneEquation(double a, double b, double c, 
                       double testX1, double testX2,
                       int numberOfRoots, int testNumber);

#endif //TESTING_MODE_H
