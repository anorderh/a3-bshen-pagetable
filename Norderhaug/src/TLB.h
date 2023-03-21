//
// Created by Anthony Norderhaug on 3/20/23.
//

#ifndef NORDERHAUG_TLB_H
#define NORDERHAUG_TLB_H

#include "Map.h"
#include <map>

using namespace std;

struct TLB_Entry {
    unsigned int PFN;
    unsigned int access_time;
};

class TLB {
public:
    TLB(unsigned int size);

    map<unsigned int, TLB_Entry> cache;

    unsigned int cur_size;
    unsigned int max_size;
    bool valid;

    TLB_Entry* lookup(unsigned int VPN, unsigned int cur_time);
    void insert(Map* entry, unsigned int cur_time);
    void reformat();
};


#endif //NORDERHAUG_TLB_H
