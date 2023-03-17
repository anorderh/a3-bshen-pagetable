//
// Created by Anthony Norderhaug on 3/16/23.
//

#include <tuple>
#include <vector>
#include <iostream>

#ifndef NORDERHAUG_BITMANIPULATION_H
#define NORDERHAUG_BITMANIPULATION_H

using namespace std;

void shift(unsigned int& value, int shifts, bool populate);
unsigned int generateBitmask(int start, int end);
tuple<unsigned int, unsigned int> getAddrInfo(int lvls[], int numOfLvls, unsigned int vAddr);


#endif //NORDERHAUG_BITMANIPULATION_H
