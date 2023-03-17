//
// Created by Anthony Norderhaug on 3/16/23.
//

#include "bitManipulation.h"
#include "math.h"

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

/**
 * retrieves tuple containing (VPN, offset)
 * @param lvls
 * @param numOfLvls
 * @param vAddr
 * @return
 */
tuple<unsigned int, unsigned int> getAddrInfo(int lvls[], int numOfLvls, unsigned int vAddr) {
    unsigned int VPN = vAddr;
    unsigned int offset = vAddr;

    // Get sum of levels' bits
    int bit_sum = 0;
    for (int i = 0; i < numOfLvls; i++) {
        bit_sum += lvls[i];
    }

    // Mask and shift for VPN
    unsigned int vpn_mask = generateBitmask(0, bit_sum);
    int shift = 32 - bit_sum;
    VPN = vAddr & vpn_mask;
    VPN = VPN >> shift;

    // Mask for offset
    unsigned int offset_mask = generateBitmask(bit_sum, 32);
    offset = offset & offset_mask;

    return make_tuple(VPN, offset);
}

/**
 * initializes input bitmasks, shifts, and entryCount arrays with values
 * @param lvls
 * @param num_of_lvls
 * @param bitmasks
 * @param shifts
 * @param entryCount
 */
void getLvlInfo(int lvls[], int num_of_lvls, unsigned int* bitmasks, int* shifts, unsigned int* entryCount) {
    int offset = 0;

    // Create tuples holding levels' start_idx & ending_idx
    tuple<int, int> bit_ranges[num_of_lvls];
    for (int i = 0; i < num_of_lvls; i++) {
        bit_ranges[i] = make_tuple(offset, offset + lvls[i]);           // Calculate lvl's desired bit range
        entryCount[i] = (unsigned int)(pow(2, lvls[i])); // Calculate lvl's entry count
        shifts[i] = 32 - get<1>(bit_ranges[i]);                     // Calculate lvl's bit shift

        offset += lvls[i]; // Update offset
    }

    // Iterate through lvls & use bit range to generate bitmasks
    for (int i = 0; i < num_of_lvls; i++) {
        bitmasks[i] = generateBitmask(get<0>(bit_ranges[i]), get<1>(bit_ranges[i]));
    }
}
