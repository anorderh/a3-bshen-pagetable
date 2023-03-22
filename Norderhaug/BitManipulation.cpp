/**
 * Anthony Norderhaug, Anthony Contreras
 * CS 480 - Spring 2023
 * RedID: 823899304, 824089247
 *
 * BitManipulation.cpp implements int shifting and bitmask generation.
 */

#include "BitManipulation.h"

using namespace std;

/**
 * shifting unsigned int & populating with either '1' or '0'
 * @param value                         unsigned int to be shifted
 * @param shifts                        # of shifts
 * @param populate                      true - '1', false, - '0'
 */
void shift(unsigned int& value, int shifts, bool populate) {
    for (int i = 0; i < shifts; i++) {
        value = value << 1;
        if (populate) {
            value = value | 1;
        }
    }
}

/**
 * generates bitmask to extract bits within [start, end)
 * @param start                         bit range start
 * @param end                           bit range end
 * @return                              resulting bitmask
 */
unsigned int generateBitmask(int start, int end) {
    unsigned int mask = 0;

    shift(mask, start-0, false); // Shift for prepending 0 bits
    shift(mask, end-start, true); // Populate with bit range
    shift(mask, V_ADDR_BIT_SPACE-end, false); // Shift for ending 0 bits

    return mask;
}
