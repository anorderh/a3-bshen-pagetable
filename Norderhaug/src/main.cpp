#include <iostream>
#include "bitManipulation.h"
extern "C" {
#include "vaddr_tracereader.h"
}

using namespace std;

void readTrace(const char *filename) {
    FILE* trace_fstream = fopen(filename, "r"); // file stream
    p2AddrTr mtrace;

    while (NextAddress(trace_fstream, &mtrace)) {
        printf("Hex address (printf): %08lx\n", mtrace.addr);
    }

    fclose(trace_fstream);
}

int main(int argc, char **argv) {
//    readTrace(argv[1], int(*argv[2] - '0'));
    unsigned int vAddr = 0x3c654321;
    int lvls[3] = {8, 4, 8};
    printf("Virtual Address: %08x\n\n", vAddr);

    // Declaring args & test structures
    int num_of_lvls = sizeof(lvls)/sizeof(lvls[0]);
    unsigned int bitmasks[num_of_lvls];
    int shifts[num_of_lvls];
    unsigned int entryCount[num_of_lvls];

    // Extracting addr & level info
    getLvlInfo(lvls, num_of_lvls, bitmasks, shifts, entryCount);
    tuple<unsigned int, unsigned int> vals = getAddrInfo(lvls, 3, vAddr);

    // Output to console
    for (int i = 0; i < num_of_lvls; i++) {
        cout << "Level " << i << ":" << endl;
        printf("Bitmask: %08x\n", bitmasks[i]);
        printf("# of Shifts: %d\n", shifts[i]);
        printf("Entry Count: %d\n\n", entryCount[i]);
    }
    printf("VPN: %08x\n", get<0>(vals));
    printf("Offset: %08x\n", get<1>(vals));
}

