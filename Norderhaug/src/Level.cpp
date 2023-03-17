//
// Created by Anthony Norderhaug on 3/17/23.
//

#include "Level.h"

/**
 * constructor tracking lvl, lvl attributes, and allocating nextLevels & maps arrays
 * @param lvl
 * @param pt
 */
Level::Level(int lvl, PageTable* pt) {
    cur_level = lvl;

    bitmask = pt->bitmasks[cur_level];
    shift = pt->bit_shifts[cur_level];
    count = pt->entryCount[cur_level];

    // Allocating nextLevels
    nextLevels = new Level*[count]();
    // Allocating map, incase leaf node
    map = new Map*[count]();
}
