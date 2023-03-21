#include <iostream>
#include "bitManipulation.h"
#include "PageTable.h"
#include "PageTableStats.h"
#include "TLB.h"
#include <getopt.h>
extern "C" {
#include "print_helpers.h"
#include "vaddr_tracereader.h"
}

using namespace std;

void processTrace(const char *filename, PageTable *pt, TLB* tlb, OutputOptionsType *options, int n, bool parse_all) {
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
        } else {    // vAddr NOT in TLB or NOT allocated
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
        if (not parse_all) { // Update iterations if not parsing entire trace
            n -= 1;
        }
    }

    fclose(trace_fstream);

    // Finalize PageTableStats
    stats.page_size = (unsigned int) pow(2, 32 - pt->bit_sum);
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


    int size = sizeof(args) / sizeof(argv[0]);

    // Create PageTable & TLB
    PageTable page_table = PageTable(args, size);
    TLB tlb = TLB(0);

    // Specify arguments
    auto *options = new OutputOptionsType();
    options->summary = true;
    options->levelbitmasks = false;
    options->offset = false;
    options->vpn2pfn = false;
    options->va2pa = false;
    options->va2pa_tlb_ptwalk = false;

    int iterations = 1;
    bool parse_all = true;

    int option;
    string mode;
    while ((option = getopt(argc, argv, "n:c:p")) != -1){
        switch (option) {
            case 'n': // specifying number of memory accesses to process
                if(atoi(optarg) < 0){
                    cout << "Number of memory accesses must be a number, greater than or equal to 0";
                    exit(-1);
                }
                else{
                    iterations = atoi(optarg);
                    parse_all = false;
                    break;
                }
            case 'c': // setting the TLB Cache Capacity
                if(atoi(optarg) < 0){
                    cout << "Cache capacity must be a number, greater than or eqaul to 0";
                    exit(-2);
                }
                else{
                    tlb.max_size = atoi(optarg);
                }
            case 'p' : // choosing the print mode
                string s(optarg);
                mode = optarg;
                if(mode == "levelbitmasks"){
                    options->levelbitmasks = true;
                    break;
                }
                if(mode == "va2pa"){
                    options->va2pa = true;
                    break;
                }
                if(mode == "va2pa_tlb_ptwalk"){
                    options->va2pa_tlb_ptwalk = true;
                    break;
                }
                if(mode == "vpn2pfn"){
                    options->vpn2pfn = true;
                    break;
                }
                if(mode == "offset"){
                    options->offset = true;
                    break;
                }
                if(mode == "summary"){
                    options->summary = true;
                    break;
                }
        }

    }
    //parsing the mandatory arguments
    int index = optind;
    int isValidFile = 0;
    if(index < argc) { // if there are more arguments to process
        if (access(argv[index], isValidFile) == -1) {
            cout << "Unable to open <<" << argv[index] << ">>";
            exit(-3);
        }
        index++;
        int argIndex = 0;
        int args [(argc - index)];
        while(index < argc - 1){
            args[argIndex] = atoi(argv[index]);
            argIndex++;
        }
    }








    // Process
    processTrace("trace.tr", &page_table, &tlb, options, iterations, parse_all);
}
