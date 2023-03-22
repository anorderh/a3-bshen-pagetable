//
// Created by Anthony Norderhaug on 3/21/23.
//

#ifndef NORDERHAUG_ARGPARSER_H
#define NORDERHAUG_ARGPARSER_H

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
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




#endif //NORDERHAUG_ARGPARSER_H
