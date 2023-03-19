#include <iostream>
#include "bitManipulation.h"
#include "PageTable.h"
#include "PageTableStats.h"
extern "C" {
#include "print_helpers.h"
#include "vaddr_tracereader.h"
}

using namespace std;

void processTrace(const char *filename, PageTable* pt, OutputOptionsType* options) {
    // Console output - BEFORE processing has occurred
    if (options->levelbitmasks) {
        report_levelbitmasks(pt->level_count, pt->bitmasks);
        return;
    }

    FILE* trace_fstream = fopen(filename, "r"); // file stream
    p2AddrTr mtrace;
    PageTableStats stats = PageTableStats(pt);

    // Console output - DURING processing
    // Begin processing addresses...
    unsigned int vAddr = 0;
    unsigned frame_idx = 0;
    while (NextAddress(trace_fstream, &mtrace)) { // REMEMBER TO IMPLEMENT N-LENGTH PROCESSING
        vAddr = (unsigned int)mtrace.addr;
        stats.addresses_processed += 1;

        if (options->offset) { // Outputting offset if desired
            hexnum(pt->virtualAddressToOffset(vAddr));
        }

        // Lookup addr - if not present, insert & increment frame_idx
        if (pt->lookup_vpn2pfn(vAddr, options) == nullptr) {
            pt->insert_vpn2pfn(vAddr, frame_idx, options);
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

    // Console output - AFTER processing has occurred
    if (options->summary) {
        report_summary(stats.page_size,
                       stats.cache_hits,
                       stats.table_hits,
                       stats.addresses_processed,
                       stats.frames_used,
                       stats.bytes);
    }
}

int main(int argc, char **argv) {
    int args[] = {20};
    int size = sizeof(args)/sizeof(args[0]);

    // Create PageTable
    PageTable page_table = PageTable(args, size);

    // Specify arguments
    auto* options = new OutputOptionsType();
    options->summary = true;
    options->levelbitmasks = false;
    options->offset = false;
    options->vpn2pfn = false;
    options->va2pa = false;
    options->va2pa_tlb_ptwalk = false;

    // Process
    processTrace("trace.tr", &page_table, options);
}

