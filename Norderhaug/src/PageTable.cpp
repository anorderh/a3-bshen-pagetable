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
 * generates VPN based on virtual address, bitmask, and shift
 * @param vAddr
 * @param mask
 * @param shift
 * @return
 */
unsigned int PageTable::virtualAddressToVPN(unsigned int vAddr, unsigned int mask, unsigned int shift) {
    unsigned int VPN;

    // Applying bitmask && shifting
    VPN = vAddr & mask;
    VPN = VPN >> shift;

    return VPN;
}

/**
 * uses vAddr to traverse tree iteratively unto leaf level, checks Map presence
 * @param vAddr
 * @return
 */
Map* PageTable::lookup_vpn2pfn(unsigned int vAddr) {
    Level* lvl = root;
    unsigned int lvl_vpn;
    int leaf_lvl = level_count-1;

    // Process unto leaf level
    for (int i = 0; i < leaf_lvl; i++) {

        // Apply lvl's bitmask & shift for VPN
        lvl_vpn = virtualAddressToVPN(vAddr, lvl->bitmask, lvl->shift);

        // Checking if table at lvl exists...
        // Y - Update lvl and continue, N - VPN not mapped yet, return nullptr
        if (lvl->nextLevels[lvl_vpn] == nullptr) {
            return nullptr;
        }
        lvl = lvl->nextLevels[lvl_vpn];
    }

    // Getting leaf lvl idx & retrieving Map present
    unsigned int leaf_map_idx = virtualAddressToVPN(vAddr, bitmasks[leaf_lvl], bit_shifts[leaf_lvl]);
    return lvl->maps[leaf_map_idx];
}

/**
 * uses vAddr to traverse and allocate necessary Level and Map objs
 * @param vAddr
 * @param frame
 */
void PageTable::insert_vpn2pfn(unsigned int vAddr, unsigned int frame) {
    // Generating VPN mask & shift, then extracting VPN
    unsigned int vpn_mask = generateBitmask(0, bit_sum);
    int shift = 32 - bit_sum;
    unsigned int VPN = virtualAddressToVPN(vAddr, vpn_mask, shift);

    Level* lvl = root;
    int lvl_depth = lvl->depth; // Starts at 0 given 'root'
    unsigned int lvl_vpn;
    int leaf_lvl = level_count-1;

    // Process unto leaf level
    for (int i = 0; i < leaf_lvl; i++) {

        // Apply lvl's bitmask & shift for VPN
        lvl_vpn = virtualAddressToVPN(vAddr, lvl->bitmask, lvl->shift);

        // Checking if table at lvl exists...
        // Y - Update lvl and continue, N - Allocate new Level, update and continue
        if (lvl->nextLevels[lvl_vpn] == nullptr) {
            lvl->nextLevels[lvl_vpn] = new Level(lvl_depth + 1, this);
        }
        lvl = lvl->nextLevels[lvl_vpn];
    }

    // Getting leaf lvl idx & allocating new Map
    unsigned int leaf_map_idx = virtualAddressToVPN(vAddr, bitmasks[leaf_lvl], bit_shifts[leaf_lvl]);
    lvl->maps[leaf_map_idx] = new Map(VPN, frame);
}

/**
 * constructor tracking # of levels, calculating all needed values, and allocating root level
 * @param lvl_args
 * @param num_of_lvls
 */
PageTable::PageTable(int *lvl_args, int num_of_lvls) {
    // Lvl count & sum of lvl bits
    level_count = num_of_lvls;
    int sum = 0;
    for (int i = 0; i < num_of_lvls; i++) {
        sum += lvl_args[i];
    }
    bit_sum = sum;

    bitmasks = new unsigned int[num_of_lvls];
    bit_shifts = new int[num_of_lvls];
    entryCount = new unsigned int[num_of_lvls];

    getPageTableInfo(lvl_args, num_of_lvls, bitmasks, bit_shifts, entryCount);

    root = new Level(0, this);
}
