//
// Created by Anthony Norderhaug on 3/17/23.
//

#ifndef NORDERHAUG_PAGETABLE_H
#define NORDERHAUG_PAGETABLE_H

#include "bitManipulation.h"
#include "math.h"
#include "Level.h"


class PageTable {
public:
    PageTable(int lvl_args[], int num_of_lvls);

    Level* root;

    int level_count;
    unsigned int* bitmasks;
    int* bit_shifts;
    unsigned int* entryCount;
};

void getPageTableInfo(int lvls[], int num_of_lvls, unsigned int* bitmasks, int* shifts, unsigned int* entryCount);


#endif //NORDERHAUG_PAGETABLE_H
