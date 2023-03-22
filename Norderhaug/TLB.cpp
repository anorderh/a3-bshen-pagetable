/**
 * Anthony Norderhaug, Anthony Contreras
 * CS 480 - Spring 2023
 * RedID: 823899304, 824089247
 *
 * TLB.cpp implements TLB constructor, VPN lookup & insertion, and reformatting per LRU policy.
 */

#include "TLB.h"

/**
 * constructor initializing size. 'valid' flag for if size is > 0.
 * @param size                              TLB size
 */
TLB::TLB(unsigned int size) {
    cur_size = 0;
    max_size = size;

    if (size > 0) {
        valid = true;
    } else {
        valid = false;
    }
}

/**
 * search cache for VPN. If found, update access_time and return entry ptr. Else, return nullptr.
 * @param VPN                               VPN to be checked
 * @param cur_time                          virtual time
 * @return                                  derived TLB entry
 */
TLB_Entry* TLB::lookup(unsigned int VPN, unsigned int cur_time) {
    if (cache.find(VPN) != cache.end()) { // VPN found in cache
        cache[VPN].access_time = cur_time; // Update reference time
        return &cache[VPN];
    } else {
        return nullptr;     // Return 'nullptr'
    }
}

/**
 * traverse cache for least, recently referenced Mapping. Remove from cache.
 */
void TLB::reformat() {
    auto start = cache.begin();
    unsigned int cur_access_time = 0;

    // Grabbing first VPN & access_time as base
    unsigned int LRU_VPN = start->first;
    unsigned int min_access_time = start->second.access_time;

    // Traversing map for lowest access time & associated VPN - O(n)
    for (auto entry: cache) {
        cur_access_time = entry.second.access_time;

        if (cur_access_time < min_access_time) { // Entry referenced less is found.
            LRU_VPN = entry.first;
            min_access_time = cur_access_time;
        }
    }

    // Removing least recently referenced VPN, decrement cur_size
    cache.erase(LRU_VPN);
    cur_size -= 1;
}

/**
 * insert Map into cache, track insertion's time
 * @param entry                             Mapping to be inserted
 * @param cur_time                          virtual time
 */
void TLB::insert(Map* entry, unsigned int cur_time) {
    if (cur_size >= max_size) { // Capacity reached, reformat cache via LRU policy
        reformat();
    }

    // Insert mapping into cache & increment size
    cache[entry->VPN] = {entry->frame_num, cur_time};
    cur_size += 1;
}