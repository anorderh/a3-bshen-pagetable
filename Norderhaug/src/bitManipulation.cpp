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
