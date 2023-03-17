#include <iostream>
#include "bitManipulation.h"
#include "PageTable.h"
#include "Map.h"
extern "C" {
#include "vaddr_tracereader.h"
}

using namespace std;

void processTrace(const char *filename, PageTable* pt) {
    FILE* trace_fstream = fopen(filename, "r"); // file stream
    p2AddrTr mtrace;

    unsigned int vAddr;
    unsigned frame_idx = 0;
    while (NextAddress(trace_fstream, &mtrace)) {
        vAddr = (unsigned int)mtrace.addr;

        // Lookup addr - if not present, insert & increment frame_idx
        if (pt->lookup_vpn2pfn(vAddr) == nullptr) {
            pt->insert_vpn2pfn(vAddr, frame_idx);
            frame_idx++;
        }
    }

    fclose(trace_fstream);
}

int main(int argc, char **argv) {
    int args[] = {8, 4, 8};
    int size = sizeof(args)/sizeof(args[0]);

    PageTable page_table = PageTable(args, size);
    processTrace("trace.tr", &page_table);
}

