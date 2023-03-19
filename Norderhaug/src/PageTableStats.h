//
// Created by Anthony Norderhaug on 3/18/23.
//

#ifndef NORDERHAUG_PAGETABLESTATS_H
#define NORDERHAUG_PAGETABLESTATS_H

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


#endif //NORDERHAUG_PAGETABLESTATS_H
