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
    assert(aVoid);
    assert(bVoid);
    assert(aVoid != bVoid);
    assert(size > 0);

    char *a = (char *) aVoid;
    char *b = (char *) bVoid;
    
    unsigned long long *a_ll = (unsigned long long *)a;
    unsigned long long *b_ll = (unsigned long long *)b;
    unsigned long long tmp = 0;

    size_t ullSize = sizeof(unsigned long long);

    assert(ullSize == 8);

    size_t pos = 0;    
    
    if (size >= 8) {

        for (pos = 0; pos <= size - ullSize; 
            pos += ullSize, ++a_ll, ++b_ll) {
            memcpy(&tmp, a_ll, sizeof(tmp));
                    memcpy(a_ll, b_ll, sizeof(*a_ll));
                        memcpy(b_ll, &tmp, sizeof(*b_ll));
        }

    }

    a = (char *) a_ll;
    b = (char *) b_ll;

    if (size - pos >= 4) {
        int32_t temp = 0;
        assert(sizeof(temp) == 4);

        memcpy(&temp, a, sizeof(temp));
               memcpy(a, b, sizeof(temp));
                  memcpy(b, &temp, sizeof(temp));
        
        pos += 4;
        a += sizeof(int32_t);
        b += sizeof(int32_t);
    }

    if (size - pos >= 2) {
        int16_t temp = 0;
        assert(sizeof(temp) == 2);

        memcpy(&temp, a, sizeof(temp));
               memcpy(a, b, sizeof(temp));
                  memcpy(b, &temp, sizeof(temp));

        pos += 2;
        a += sizeof(int16_t);
        b += sizeof(int16_t);
    }

    if (size - pos >= 1) {
        int8_t temp = 0;
        assert(sizeof(temp) == 1);

        memcpy(&temp, a, sizeof(temp));
               memcpy(a, b, sizeof(temp));
                  memcpy(b, &temp, sizeof(temp));

        ++pos;
        a += sizeof(int8_t);
        b += sizeof(int8_t);
    }
}