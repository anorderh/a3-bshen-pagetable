/**
 * Anthony Norderhaug, Anthony Contereras
 * CS 480 - Spring 2023
 * RedID: 823899304, 824089247
 *
 * main.cpp allocates the PageTable & TLB, and parses the trace file for desired output.
 */

#include "BitManipulation.h"
#include "PageTable.h"
#include "PageTableStats.h"
#include "TLB.h"
#include "ArgParser.h"
extern "C" {
#include "print_helpers.h"
#include "vaddr_tracereader.h"
}

using namespace std;

/**
 * parses the trace and searches & allocates VPNs accordingly. Tracks processing's results for output.
 *
 * @param trace_fstream                     trace's file stream
 * @param pt                                PageTable ptr
 * @param tlb                               TLB ptr
 * @param options                           OutputOptionsType, for tracking print mode
 * @param n                                 # of addresses to process
 * @param parse_all                         flag for processing all addresseses
 */
void processTrace(FILE* trace_fstream, PageTable *pt, TLB* tlb, OutputOptionsType *options, int n, bool parse_all) {
    // Console output - BEFORE processing has occurred
    if (options->levelbitmasks) {
        report_levelbitmasks(pt->level_count, pt->bitmasks);
        return;
    }

    p2AddrTr mtrace;
    PageTableStats stats = PageTableStats(pt);

    unsigned int vAddr = 0; unsigned int pAddr = 0;
    unsigned int VPN = 0; unsigned int* lvl_VPNs = nullptr;
    unsigned frame_idx = 0; unsigned int cur_frame = 0;
    TLB_Entry* tlb_map = nullptr; Map* pt_map = nullptr;
    unsigned int access_time = 0; // For tracking Map's reference time
    bool tlb_hit = false; bool page_hit = false;

    // Begin processing addresses... for 'n' iterations or entire trace
    while (NextAddress(trace_fstream, &mtrace) && (n > 0 || parse_all)) {
        vAddr = (unsigned int) mtrace.addr;
        stats.addresses_processed += 1;

        // Lookup VPN using TLB, if allocated
        if (tlb->valid) { // TLB allocated
            // Check for VPN -> PFN mapping
            VPN = pt->virtualAddressToBaseVPN(vAddr);
            tlb_map = tlb->lookup(VPN, access_time);
        }

        // Validate TLB mapping, otherwise walk Page Table
        if (tlb_map != nullptr) {   // vAddr in TLB
            tlb_hit = true;
            cur_frame = tlb_map->PFN;
            stats.cache_hits += 1;
        } else {    // vAddr NOT in TLB or TLB NOT allocated
            tie(pt_map, lvl_VPNs) = pt->lookup_vpn2pfn(vAddr, tlb, access_time);

            if (pt_map != nullptr) {    // vAddr in PageTable
                page_hit = true;
                cur_frame = pt_map->frame_num;
                stats.table_hits += 1;
            } else {                    // vAddr NOT in PageTable
                cur_frame = frame_idx;
                lvl_VPNs = pt->insert_vpn2pfn(vAddr, cur_frame, tlb, access_time);
                frame_idx += 1;
            }
        }

        // Generate physical addr.
        pAddr = pt->frameToPhysicalAddress(vAddr, cur_frame);

        // Console output - DURING processing
        if (options->offset) {
            hexnum(pt->virtualAddressToOffset(vAddr));
        } else if (options->vpn2pfn) {
            report_pagetable_map(pt->level_count, lvl_VPNs, cur_frame);
        } else if (options->va2pa) {
            report_virtualAddr2physicalAddr(vAddr, pAddr);
        } else if (options->va2pa_tlb_ptwalk) {
            report_va2pa_TLB_PTwalk(vAddr, pAddr, tlb_hit, page_hit);
        }

        // Resetting & updating values
        tlb_hit = false; page_hit = false;
        tlb_map = nullptr;
        access_time += 1; // Update access time
        if (not parse_all) { // Decrement iterations if not parsing entire trace
            n -= 1;
        }
    }

    fclose(trace_fstream);

    // Finalize PageTableStats
    stats.page_size = (unsigned int) pow(2, V_ADDR_BIT_SPACE - pt->bit_sum);
    stats.frames_used = frame_idx;
    stats.bytes = pt->calculateSize();

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
    // Parse provided arguments
    Args* fields = new Args();
    parseArgs(argc, argv, fields);

    // Create PageTable & TLB
    PageTable page_table = PageTable(fields->lvl_args, fields->num_of_lvls);
    TLB tlb = TLB(fields->tlb_size);

    // Process trace file
    processTrace(fields->f_stream, &page_table, &tlb, fields->options, fields->iterations, fields->parse_all);
}

