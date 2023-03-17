//
// Created by Anthony Contreras on 3/17/23.
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

    // Allocating nextLevels and explicitly set them all to null
    if(depth != pt->level_count - 1) {
        nextLevels = new Level *[count]();
        for (int i = 0; i < count; i++) {
            nextLevels[i] = nullptr;

        }
    }
    else {
        // Allocating map, only in the leaf node case
        maps = new Map *[count]();
    }

}