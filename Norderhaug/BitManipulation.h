/**
 * Anthony Norderhaug, Anthony Contreras
 * CS 480 - Spring 2023
 * RedID: 823899304, 824089247
 *
 * BitManipulation.h implements method signatures for bit shifting & mask generation
 */

#ifndef BITMANIPULATION_H
#define BITMANIPULATION_H

#include "AddressConstants.h"
#include <tuple>
#include <vector>
#include <iostream>

using namespace std;

void shift(unsigned int& value, int shifts, bool populate);
unsigned int generateBitmask(int start, int end);


#endif //BITMANIPULATION_H
