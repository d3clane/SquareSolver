//
// Created by Arman on 19.08.2023.
//

#include "StringAndCharFuncs.h"

//---------------------------------------------------------------------------------------------------------------------

void DeleteSpaces(char *string) {
    assert(string);

    int shift = 0;

    while (*string != '\0') {
        *(string - shift) = *string;

        if (isspace(*string)) {
            shift++;
        }

        string++;
    }

    *(string - shift) = *string; // '\0' on pos (string - shift) (last pos)
}

//---------------------------------------------------------------------------------------------------------------------

int SkipSymbols(FILE *fp) {
    int cnt = 0;
    
    int ch = getc(fp);
    while (ch != '\n' && ch != EOF) {
        if (!isspace(ch)) cnt++;
        ch = getc(fp);
    }

    return cnt;
}

//---------------------------------------------------------------------------------------------------------------------

int IsSign(const char c) {
    return c == '-' || c == '+';
}

//---------------------------------------------------------------------------------------------------------------------

char ChangeSign(const char c) {
    if (c == '-') {
        return '+';
    }
    if (c == '+') {
        return '-';
    }

    return c;
}

//---------------------------------------------------------------------------------------------------------------------

void StringToLower(char *str) {
    while (*str != '\0') {
        *str = (char) tolower(*str);
        str++;
    }
}

//---------------------------------------------------------------------------------------------------------------------