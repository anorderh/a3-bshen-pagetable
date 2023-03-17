//
// Created by Anthony Norderhaug on 3/16/23.
//

#include "bitManipulation.h"

using namespace std;

void shift(unsigned int& value, int shifts, bool populate) {
    for (int i = 0; i < shifts; i++) {
        value = value << 1;
        if (populate) {
            value = value | 1;
        }
    }
}

unsigned int generateBitmask(int start, int end) {
    unsigned int mask = 0;

    shift(mask, start-0, false); // Shift for prepending 0 bits
    shift(mask, end-start, true); // Populate with bit range
    shift(mask, 32-end, false); // Shift for ending 0 bits

    return mask;
}
