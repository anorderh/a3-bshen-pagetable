#include <iostream>
#include "bitManipulation.h"
#include "PageTable.h"
#include "PageTableStats.h"
#include "TLB.h"
extern "C" {
#include "print_helpers.h"
#include "vaddr_tracereader.h"
}

using namespace std;

void processTrace(const char *filename, PageTable *pt, TLB* tlb, OutputOptionsType *options) {
    // Console output - BEFORE processing has occurred
    if (options->levelbitmasks) {
        report_levelbitmasks(pt->level_count, pt->bitmasks);
        return;
    }

    FILE *trace_fstream = fopen(filename, "r"); // file stream
    p2AddrTr mtrace;
    PageTableStats stats = PageTableStats(pt);

    unsigned int vAddr = 0; unsigned int pAddr = 0;
    unsigned int VPN = 0; unsigned int* lvl_VPNs = nullptr;
    unsigned frame_idx = 0;
    TLB_Entry* tlb_map = nullptr; Map* pt_map = nullptr;
    unsigned int access_time = 0; // For tracking Map's reference time

    // Begin processing addresses...
    for (int i = 0; i < 20; i++) {          // REMEMBER TO IMPLEMENT N-LENGTH PROCESSING
        NextAddress(trace_fstream, &mtrace);
        vAddr = (unsigned int) mtrace.addr;
        stats.addresses_processed += 1;

        // Check TLB for VPN -> PFN mapping, otherwise walk Page_Table
        VPN = pt->virtualAddressToBaseVPN(vAddr);
        tlb_map = tlb->lookup(VPN, access_time);

        if (tlb_map != nullptr) {   // vAddr in TLB
            stats.cache_hits += 1;
            pAddr = pt->frameToPhysicalAddress(vAddr, tlb_map->PFN);
        } else {    // vAddr NOT in TLB
            tie(pt_map, lvl_VPNs) = pt->lookup_vpn2pfn(vAddr, options);

            if (pt_map != nullptr) {    // vAddr in PageTable
                pAddr = pt->frameToPhysicalAddress(vAddr, pt_map->frame_num);
                stats.table += 1;
            } else {                    // vAddr NOT in PageTable
                lvl_VPNs = pt->insert_vpn2pfn(vAddr, frame_idx, options);
                pAddr = pt->frameToPhysicalAddress(vAddr, frame_idx);
                frame_idx += 1;
            }
        }

        // NOW IMPLEMENT PRINT_HELPER SECTION HERE
        // USING lvl_VPNs, vAddr, pAddr, frame_idx, etc.
        // Console output - DURING processing
//        if (options->offset) { // Outputting offset if desired
//            hexnum(pt->virtualAddressToOffset(vAddr));
//        }
//
//        if (options->va2pa_tlb_ptwalk) {
//            report_va2pa_TLB_PTwalk(vAddr, pAddr, true, false);
//        }

        access_time += 1; // Update access time
    }

    // Finalize PageTableStats
    stats.page_size = (unsigned int) pow(2, 32 - pt->bit_sum);
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
    int args[] = {4, 8, 8};
    int size = sizeof(args) / sizeof(args[0]);

    // Create PageTable & TLB
    PageTable page_table = PageTable(args, size);
    TLB tlb = TLB(8);

    // Specify arguments
    auto *options = new OutputOptionsType();
    options->summary = false;
    options->levelbitmasks = false;
    options->offset = false;
    options->vpn2pfn = false;
    options->va2pa = true;
    options->va2pa_tlb_ptwalk = false;

    // Process
    processTrace("trace.tr", &page_table, &tlb, options);
}

