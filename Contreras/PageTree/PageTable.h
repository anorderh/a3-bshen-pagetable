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

class PageTable {
public:
    //lvl_args: array of details about bits per level
    PageTable(int lvl_args[], int num_of_lvls);

    Level* root;

    int level_count;
    int bit_sum;
    unsigned int* bitmasks;
    int* bit_shifts;
    unsigned int* entryCount;

    unsigned int virtualAddressToVPN(unsigned int vAddr, unsigned int mask, unsigned int shift);
    Map* lookup_vpn2pfn(unsigned int vAddr);
    void insert_vpn2pfn(unsigned int vAddr, unsigned int frame);

};

void getPageTableInfo(int lvls[], int num_of_lvls, unsigned int* bitmasks, int* shifts, unsigned int* entryCount);

#endif //PAGETREE_PAGETABLE_H
