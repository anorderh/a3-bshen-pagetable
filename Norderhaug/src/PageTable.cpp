//
// Created by Anthony Norderhaug on 3/17/23.
//

#include "PageTable.h"

using namespace std;

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
 * generates VPN based on virtualAddress (groups all Levels together)
 * @param vAddr
 * @return
 */
unsigned int PageTable::virtualAddressToBaseVPN(unsigned int vAddr) {
    unsigned int mask = generateBitmask(0, bit_sum);
    unsigned int shift = 32 - bit_sum;

    return virtualAddressToVPN(vAddr, mask, shift);
}

/**
 * generates offset based on virtual address & bitmask
 * @param vAddr
 * @param mask
 * @return
 */
unsigned int PageTable::virtualAddressToOffset(unsigned int vAddr) {
    unsigned int offset;
    unsigned int offset_mask = generateBitmask(bit_sum, 32);

    // Applying bitmask
    offset = vAddr & offset_mask;

    return offset;
}

/**
 * generate physical addr. using vAddr's offset and mapped frame
 * @param vAddr
 * @param frame
 * @return
 */
unsigned int PageTable::frameToPhysicalAddress(unsigned int vAddr, unsigned int frame) {
    unsigned int offset = virtualAddressToOffset(vAddr);

    unsigned int frame_val = frame; // Factor in frame idx using Page's size
    shift(frame_val, 32 - bit_sum, false);

    return frame_val + offset;
}

/**
 * uses vAddr to traverse tree iteratively unto leaf level. Checks if vAddr is mapped.
 * Stores sequential Level VPNs. Returns tuple holding Map* and Level's VPNs.
 * @param vAddr
 * @return
 */
tuple<Map*, unsigned int*> PageTable::lookup_vpn2pfn(unsigned int vAddr, TLB* tlb, unsigned int access_time) {
    Level* lvl = root;
    unsigned int lvl_vpn = 0; unsigned int offset = 0;
    int leaf_lvl = level_count-1;
    auto* VPNs = new unsigned int[level_count]();

    // Process unto leaf level
    for (int depth = 0; depth < leaf_lvl; depth++) {
        // Apply lvl's bitmask & shift for VPN
        lvl_vpn = virtualAddressToVPN(vAddr, lvl->bitmask, lvl->shift);
        VPNs[depth] = lvl_vpn; // Tracking VPNs per leve

        // Checking if table at lvl exists...
        // Y - Update lvl and continue, N - VPN not mapped yet, return nullptr
        if (lvl->nextLevels[lvl_vpn] == nullptr) {
            return make_tuple(nullptr, nullptr);
        }
        lvl = lvl->nextLevels[lvl_vpn];
    }

    // Getting leaf lvl idx & retrieving Map present
    unsigned int leaf_map_idx = virtualAddressToVPN(vAddr, bitmasks[leaf_lvl], bit_shifts[leaf_lvl]);
    Map* entry = lvl->maps[leaf_map_idx];
    VPNs[leaf_lvl] = leaf_map_idx;

    // Save to TLB if 1) TLB is allocated and 2) Map is present
    if (tlb->valid && entry != nullptr) {
        tlb->insert(entry, access_time);
    }

    // Return tuple holding Map* and processed Level VPNs
    return make_tuple(entry, VPNs);
}

/**
 * uses vAddr to traverse and allocate necessary Level and Map objs.
 * Stores & returns sequential Level VPNs.
 * @param vAddr
 * @param frame
 */
unsigned int* PageTable::insert_vpn2pfn(unsigned int vAddr, unsigned int frame, TLB* tlb, unsigned int access_time) {
    // Generating masks, extracting for VPN & offset
    unsigned int VPN = virtualAddressToBaseVPN(vAddr);

    Level* lvl = root;
    unsigned int lvl_vpn = 0;
    auto* VPNs = new unsigned int[level_count]();
    int leaf_lvl = level_count-1;

    // Process unto leaf level
    for (int depth = lvl->depth; depth < leaf_lvl; depth++) {

        // Apply lvl's bitmask & shift for VPN
        lvl_vpn = virtualAddressToVPN(vAddr, lvl->bitmask, lvl->shift);
        VPNs[depth] = lvl_vpn; // Tracking VPNs per leve

        // Checking if table at lvl exists...
        // Y - Update lvl and continue, N - Allocate new Level, update and continue
        if (lvl->nextLevels[lvl_vpn] == nullptr) {
            lvl->nextLevels[lvl_vpn] = new Level(depth + 1, this);
        }
        lvl = lvl->nextLevels[lvl_vpn];
    }

    // Getting leaf lvl idx & allocating new Map
    unsigned int leaf_map_idx = virtualAddressToVPN(vAddr, bitmasks[leaf_lvl], bit_shifts[leaf_lvl]);
    Map* entry = new Map(VPN, frame);
    lvl->maps[leaf_map_idx] = entry;
    VPNs[leaf_lvl] = leaf_map_idx; // Storing final VPN

    // Save to TLB if TLB is allocated
    if (tlb->valid) {
        tlb->insert(entry, access_time);
    }

    // Return processed Level VPNs
    return VPNs;
}

/**
 * calculate size by iterating through all present Levels an Maps
 * @return
 */
unsigned int PageTable::calculateSize() {
    Level* cur = nullptr; Level* next = nullptr; Map* mapping = nullptr;

    unsigned int size = sizeof(*this) + sizeof(*root); // PageTable + Root size
    queue<Level*> unprocessed;
    unprocessed.push(root);

    // Iterate through unprocessed
    while (!unprocessed.empty()) {
        cur = unprocessed.front();
        unprocessed.pop();

        // Iterate through cur's nodes
        for (int i = 0; i < entryCount[cur->depth]; i++) {

            // Leaf node found - update size using Map ptrs
            if (cur->depth == level_count-1) {
                mapping = cur->maps[i];

                if (mapping != nullptr) {
                    size += sizeof(*mapping); // sizeof(Map)
                } else {
                    size += sizeof(mapping); // sizeof(nullptr)
                }
            // Non-leaf node found - update size using Level ptrs
            } else {
                next = cur->nextLevels[i];

                if (next != nullptr) { //
                    size += sizeof(*next); // sizeof(Level)
                    unprocessed.push(next);
                } else {
                    size += sizeof(next); // sizeof(nullptr)
                }
            }
        }
    }

    return size;
}