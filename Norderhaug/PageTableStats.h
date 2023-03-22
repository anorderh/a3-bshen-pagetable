/**
 * Anthony Norderhaug, Anthony Contreras
 * CS 480 - Spring 2023
 * RedID: 823899304, 824089247
 *
 * PageTableStats.h implements PageTableStats members
 */

#ifndef PAGETABLESTATS_H
#define PAGETABLESTATS_H

#include "PageTable.h"
#include "math.h"

class PageTableStats {
public:
    explicit PageTableStats(PageTable* pt);

    PageTable* table;

    unsigned int page_size = 0;
    unsigned int cache_hits = 0;
    unsigned int table_hits = 0;
    unsigned int addresses_processed = 0;
    unsigned int frames_used = 0;
    unsigned int bytes = 0;
};


#endif //PAGETABLESTATS_H
