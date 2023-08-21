//
// Created by Arman on 19.08.2023.
//

#include "StringAndCharFuncs.h"

void DeleteSpaces(char *string) {
    assert(string != NULL);

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

void SkipSymbols() {
    int ch = getchar();
    while (ch != '\n' && ch != EOF) {
        ch = getchar();
    }
}

int IsSign(char c) {
    return c == '-' || c == '+';
}

char ChangeSign(char c) {
    if (c == '-') {
        c = '+';
    } else if (c == '+') {
        c = '-';
    }

    return c;
}