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
    depth = lvl;

    bitmask = pt->bitmasks[lvl];
    shift = pt->bit_shifts[lvl];
    count = pt->entryCount[lvl];

    if (lvl == pt->level_count-1) { // Leaf node - allocate Map[]
        maps = new Map*[count]();
    } else {                        // Allocate nextLevels
        nextLevels = new Level*[count]();
    }
}
