/**
 * Anthony Norderhaug, Anthony Contreras
 * CS 480 - Spring 2023
 * RedID: 823899304, 824089247
 *
 * ArgParser.h implements struct Args for storing process fields. Also stores members & method signatures.
 */

#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <iostream>
#include <unistd.h>
#include "print_helpers.h"

using namespace std;

struct Args {
    int* lvl_args = nullptr;
    FILE* f_stream = nullptr;
    int num_of_lvls = 0; int tlb_size = 0;
    int iterations = 0; bool parse_all = true;
    OutputOptionsType* options = new OutputOptionsType();
};

void debugArgs(Args* fields);
void parseArgs(int argc, char **argv, Args* fields);




#endif //ARGPARSER_H
