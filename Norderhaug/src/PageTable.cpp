//
// Created by Anthony Norderhaug on 3/17/23.
//

#include "PageTable.h"

/**
 * initializes bitmasks, shifts, and entryCount arrays according to levels
 * @param lvls
 * @param num_of_lvls
 * @param bitmasks
 * @param shifts
 * @param entryCount
 */
void getPageTableInfo(int lvls[], int num_of_lvls, unsigned int* bitmasks, int* shifts, unsigned int* entryCount) {
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

/**
 * constructor tracking # of levels, calculating all needed values, and allocating root level
 * @param lvl_args
 * @param num_of_lvls
 */
PageTable::PageTable(int *lvl_args, int num_of_lvls) {
    level_count = num_of_lvls;

    bitmasks = new unsigned int[num_of_lvls];
    bit_shifts = new int[num_of_lvls];
    entryCount = new unsigned int[num_of_lvls];

    getPageTableInfo(lvl_args, num_of_lvls, bitmasks, bit_shifts, entryCount);

    root = new Level[0];
}
