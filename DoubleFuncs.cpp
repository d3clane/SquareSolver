//
// Created by Arman on 18.08.2023.
//

#include "DoubleFuncs.h"

ComparisonResult Compare(const double a, const double b) {
    ComparisonResult result = EQUAL;

    if (b - a > EPS) {
        result = LESS;
    } else if (a - b > EPS) {
        result = GREATER;
    } else {
        result = EQUAL;
    }

    return result;
}

void Swap(void *aVoid, void *bVoid, const size_t size) {
    char *a = (char *) aVoid;
    char *b = (char *) bVoid;
    
    unsigned long long *a_ll = (unsigned long long *)a;
    unsigned long long *b_ll = (unsigned long long *)b;
    unsigned long long tmp = 0;

    size_t ullSize = sizeof(unsigned long long);

    unsigned long long pos = 0;    

    for (pos = 0; (long long) pos <= (long long) size - 8; 
         pos += ullSize, a += ullSize, b += ullSize, ++a_ll, ++b_ll) {
        tmp = *a_ll;
              *a_ll = *b_ll;
                      *b_ll = tmp;
    }

    char tmp2 = 0;
    for (; (long long) pos < (long long) size; ++pos, ++a, ++b) {                                     
        tmp2 = *a;
               *a = *b;
                    *b = tmp2;
    }

}