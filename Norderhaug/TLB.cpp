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

    // If size < LRU_MAX_LIMIT, accommodate lru_max field
    if (size < LRU_MAX_LIMIT) {
        LRU_max = size;
    } else {
        LRU_max = LRU_MAX_LIMIT;
    }

    // Flag indicating valid TLB or not
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
        update(VPN, cur_time);
        return &cache[VPN];
    } else {
        return nullptr;     // Return 'nullptr'
    }
}

/**
 * updates VPN access time & insert into LRU, if not present.
 * @param VPN
 * @param cur_time
 */
void TLB::update(unsigned int VPN, unsigned int cur_time) {
    cache[VPN].access_time = cur_time; // Update access time

    if (find(LRU.begin(), LRU.end(), VPN) ==  LRU.end()) { // Not present in LRU
        reformat(false);

        // Insert mapping into LRU
        LRU.push_back(VPN);
    }
}

/**
 * traverses & removes least recently used element LRU. If specified, removes from TLB.
 * @param removeFromTLB
 */
void TLB::reformat(bool removeFromTLB) {
    unsigned int min_access_time = -1; unsigned int cur_access_time = -1;

    unsigned int LRU_VPN;
    vector<unsigned int>::iterator LRU_VPN_iter;

    // Traversing map for lowest access time & associated VPN - O(n)
    for (auto vpn = LRU.begin(); vpn != LRU.end(); vpn++) {
        cur_access_time = cache[*vpn].access_time;

        // Entry referenced less is found OR no min set yet
        if (min_access_time == -1 || cur_access_time < min_access_time) {
            LRU_VPN = *vpn;
            min_access_time = cur_access_time;
            LRU_VPN_iter = vpn;
        }
    }

    // Removing VPN from LRU
    LRU.erase(LRU_VPN_iter);

    // If specified, remove VPN from TLB & decrement cur_size
    if (removeFromTLB) {
        cache.erase(LRU_VPN);
        cur_size -= 1;
    }
}

/**
 * insert Map into LRU & TLB. Reformats both structures depending on capacity.
 * @param entry                             Mapping to be inserted
 * @param cur_time                          virtual time
 */
void TLB::insert(Map* entry, unsigned int cur_time) {
    if (cur_size >= max_size) { // TLB Capacity reached
        reformat(true);
    } else if (cur_size >= LRU_max) { // LRU Capacity reached
        reformat(false);
    }

    // Insert to LRU & cache
    LRU.push_back(entry->VPN);
    cache[entry->VPN] = {entry->frame_num, cur_time};

    // Increment size
    cur_size += 1;
}