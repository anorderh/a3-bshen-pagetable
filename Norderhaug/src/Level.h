//
// Created by Anthony Norderhaug on 3/17/23.
//

#ifndef NORDERHAUG_LEVEL_H
#define NORDERHAUG_LEVEL_H

#include "Map.h"
#include "PageTable.h"

class Level {
public:
    Level(int lvl, PageTable* pageTable);

    Level** nextLevels;
    Map** map;

    int cur_level;
    unsigned int bitmask;
    int shift;
    unsigned int count;
};


#endif //NORDERHAUG_LEVEL_H