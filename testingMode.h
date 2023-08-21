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

#include "DoubleComparisons.h"
#include "Solver.h"

//---------------------------------------------------------------------------------------------------------------------

#define TEST
static const char *FILE_NAME = "test.txt";
static const char *FILE_MODE = "r";

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

#endif //TESTING_MODE_H
