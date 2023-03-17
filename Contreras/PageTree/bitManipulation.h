//
// Created by Anthony Contreras on 3/17/23.
//

#ifndef PAGETREE_BITMANIPULATION_H
#define PAGETREE_BITMANIPULATION_H

#include <tuple>
#include <vector>
#include <iostream>
using namespace std;

void shift(unsigned int& value, int shifts, bool populate);
unsigned int generateBitmask(int start, int end);
tuple<unsigned int, unsigned int> getAddrInfo(int lvls[], int numOfLvls, unsigned int vAddr);



#endif //PAGETREE_BITMANIPULATION_H
