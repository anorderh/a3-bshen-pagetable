//
// Created by Anthony Contreras on 3/17/23.
//

#ifndef PAGETREE_LEVEL_H
#define PAGETREE_LEVEL_H

class PageTable; // Forward declaration to break circular dependency

#include "Map.h"
#include "PageTable.h"

class Level {
public:
    Level(int lvl, PageTable *pageTable);

    //for our inner levels
    Level** nextLevels;
    //for leaf level
    Map** maps;
    //need to know how deep we are
    int depth;
    //our associated bitmask and shift for the given level
    unsigned int bitmask;
    int shift;
    //to know what number to assign the frame
    unsigned int count;
};



#endif //PAGETREE_LEVEL_H
