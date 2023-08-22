//
// Created by Arman on 19.08.2023.
//

/// \file
/// \brief Contains functions for working and strings and chars
#ifndef STRING_FUNCS_H
#define STRING_FUNCS_H

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "Errors.h"

//---------------------------------------------------------------------------------------------------------------------

/// \brief delete spaces from the string
/// \warning this function changes the string itself
/// \param [out] string the string with deleted spaces
void DeleteSpaces(char *string);

//---------------------------------------------------------------------------------------------------------------------

/// \brief skip line symbols until the new line or EOF
void SkipSymbols();

//---------------------------------------------------------------------------------------------------------------------

/// \brief checks if the char is a sign (+ or -)
///
/// \param [in] c
/// \return true (1) if c is a sign otherwise false (0)
int IsSign(const char c);

//---------------------------------------------------------------------------------------------------------------------

/// \brief Changes the sign on the opposite
/// \details Function changes the sign on the opposite (+ on minus and vice versa)
/// If the char is not a sign returns char without changes
/// \param [in] c character to change
/// \return the opposite sign if c is a sign otherwise return c
char ChangeSign(const char c);

//---------------------------------------------------------------------------------------------------------------------

#endif
