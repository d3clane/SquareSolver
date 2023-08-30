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

    uint8_t *a = (uint8_t *) aVoid;
    uint8_t *b = (uint8_t *) bVoid;

    uint64_t *a_ll = (uint64_t *) a;
    uint64_t *b_ll = (uint64_t *) b;
    uint64_t tmp = 0;

    assert(sizeof(uint64_t) == 8);

    size_t sizeInQWords = size >> 3;    
    
    for (size_t i = 0; i < sizeInQWords; ++i) {
        memcpy(&tmp, a_ll + i, sizeof(tmp));
              memcpy(a_ll + i, b_ll + i, sizeof(*a_ll));
                        memcpy(b_ll + i, &tmp, sizeof(*b_ll));
    }

    a = (uint8_t *)(a_ll + sizeInQWords);
    b = (uint8_t *)(b_ll + sizeInQWords);

    if (size & 4) {
        uint32_t temp = 0;

        assert(sizeof(temp) == 4);

        memcpy(&temp, a, sizeof(temp));
               memcpy(a, b, sizeof(temp));
                  memcpy(b, &temp, sizeof(temp));
        
        a += sizeof(uint32_t);
        b += sizeof(uint32_t);
    }

    if (size & 2) {
        uint16_t temp = 0;

        assert(sizeof(temp) == 2);

        memcpy(&temp, a, sizeof(temp));
               memcpy(a, b, sizeof(temp));
                  memcpy(b, &temp, sizeof(temp));

        a += sizeof(uint16_t);
        b += sizeof(uint16_t);
    }

    if (size & 1) {
        uint8_t temp = 0;
        
        assert(sizeof(temp) == 1);

        memcpy(&temp, a, sizeof(temp));
               memcpy(a, b, sizeof(temp));
                  memcpy(b, &temp, sizeof(temp));
    }
}