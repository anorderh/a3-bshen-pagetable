//
// Created by Anthony Contreras on 3/17/23.
//

#ifndef PAGETREE_PAGETABLE_H
#define PAGETREE_PAGETABLE_H

class Level; // Forward declaration to break circular dependency

#include "bitManipulation.h"
#include "math.h"
#include "Level.h"
#include "Map.h"
#include "TLB.h"
extern "C" {
#include "print_helpers.h"
};
#include <queue>
#include <sstream>

class PageTable {
public:
    PageTable(int lvl_args[], int num_of_lvls);

    Level* root;

    int level_count;
    int bit_sum;
    unsigned int* bitmasks;
    int* bit_shifts;
    unsigned int* entryCount;

    unsigned int virtualAddressToVPN(unsigned int vAddr, unsigned int mask, unsigned int shift);
    unsigned int virtualAddressToBaseVPN(unsigned int vAddr);
    unsigned int virtualAddressToOffset(unsigned int vAddr);
    unsigned int frameToPhysicalAddress(unsigned int vAddr, unsigned int frame);

    tuple<Map*, unsigned int*> lookup_vpn2pfn(unsigned int vAddr, TLB* tlb, unsigned int access_time);
    unsigned int* insert_vpn2pfn(unsigned int vAddr, unsigned int frame, TLB* tlb, unsigned int access_time);
    unsigned int calculateSize();
};

void getPageTableInfo(int lvls[], int num_of_lvls, unsigned int* bitmasks, int* shifts, unsigned int* entryCount);


#endif //PAGETREE_PAGETABLE_H
