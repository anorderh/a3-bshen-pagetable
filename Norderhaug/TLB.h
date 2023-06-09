/**
 * Anthony Norderhaug, Anthony Contreras
 * CS 480 - Spring 2023
 * RedID: 823899304, 824089247
 *
 * TLB.h implements struct TLB_Entry for storing entry access_time & TLB.cpp's members and method signatures.
 */
#ifndef TLB_H
#define TLB_H

# define LRU_MAX_LIMIT 8

#include "Map.h"
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

struct TLB_Entry {
    unsigned int PFN;
    unsigned int access_time;
};

class TLB {
public:
    TLB(unsigned int size);

    map<unsigned int, TLB_Entry> cache;
    vector<unsigned int> LRU;

    unsigned int cur_size;
    unsigned int max_size; unsigned int LRU_max;
    bool valid;

    TLB_Entry* lookup(unsigned int VPN, unsigned int cur_time);
    void insert(Map* entry, unsigned int cur_time);
    void update(unsigned int VPN, unsigned int cur_time);
    void reformat(bool removeFromTLB);
};


#endif //TLB_H
