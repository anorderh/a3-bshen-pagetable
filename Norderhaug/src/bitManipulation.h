//
// Created by Anthony Norderhaug on 3/16/23.
//

#include <tuple>
#include <vector>
#include <iostream>

#ifndef NORDERHAUG_BITMANIPULATION_H
#define NORDERHAUG_BITMANIPULATION_H

using namespace std;

void getLvlIndices(int lvls[], int numOfLvls);
void getLvlInfo(int lvls[], int num_of_lvls, unsigned int* bitmasks, int* shifts, unsigned int* entryCount);
tuple<unsigned int, unsigned int> getAddrInfo(int lvls[], int numOfLvls, unsigned int vAddr);


#endif //NORDERHAUG_BITMANIPULATION_H
