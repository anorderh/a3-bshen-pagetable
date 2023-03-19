#include <iostream>
#include "bitManipulation.h"
#include "PageTable.h"
#include "PageTableStats.h"
extern "C" {
#include "print_helpers.h"
#include "vaddr_tracereader.h"
}

using namespace std;

PageTableStats processTrace(const char *filename, PageTable* pt) {
    FILE* trace_fstream = fopen(filename, "r"); // file stream
    p2AddrTr mtrace;
    PageTableStats stats = PageTableStats(pt);

    // Processing addresses
    unsigned int vAddr = 0;
    unsigned frame_idx = 0;
    while (NextAddress(trace_fstream, &mtrace)) {
        vAddr = (unsigned int)mtrace.addr;
        stats.addresses_processed += 1;

        // Lookup addr - if not present, insert & increment frame_idx
        if (pt->lookup_vpn2pfn(vAddr) == nullptr) {
            pt->insert_vpn2pfn(vAddr, frame_idx);
            frame_idx++;
        } else {
            stats.table_hits += 1;
        }
    }

    // Finalize PageTableStats
    stats.page_size = (unsigned int)pow(2, 32 - pt->bit_sum);
    stats.frames_used = frame_idx;
    stats.bytes = pt->calculateSize();

    fclose(trace_fstream);

    return stats;
}

int main(int argc, char **argv) {
    int args[] = {4, 8, 8};
    int size = sizeof(args)/sizeof(args[0]);

    // Creating PageTable, processing trace, and collecting stats
    PageTable page_table = PageTable(args, size);
    PageTableStats stats = processTrace("trace.tr", &page_table);

    // Outputting stat summary
    report_summary(stats.page_size,
                   stats.cache_hits,
                   stats.table_hits,
                   stats.addresses_processed,
                   stats.frames_used,
                   stats.bytes);
}

