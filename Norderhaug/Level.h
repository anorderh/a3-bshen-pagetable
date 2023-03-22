/**
 * Anthony Norderhaug, Anthony Contreras
 * CS 480 - Spring 2023
 * RedID: 823899304, 824089247
 *
 * Level.h implements Level members
 */

#ifndef LEVEL_H
#define LEVEL_H

class PageTable; // Forward declaration to break circular dependency

#include "Map.h"
#include "PageTable.h"

class Level {
public:
    Level(int lvl, PageTable *pageTable);

    Level** nextLevels;
    Map** maps;

    int depth;
    unsigned int bitmask;
    int shift;
    unsigned int count;
};


#endif //LEVEL_H