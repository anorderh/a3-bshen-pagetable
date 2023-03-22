/**
 * Anthony Norderhaug, Anthony Contreras
 * CS 480 - Spring 2023
 * RedID: 823899304, 824089247
 *
 * ArgParser.cpp implements optional & mandatory arg parsing.
 */

#include "ArgParser.h"

/**
 * prints out resulting fields from CLI arguments. for debugging purposes.
 * @param fields                        Args, holding PageTable & TLB attrs
 */
void debugArgs(Args* fields) {
    for (int i = 0; i < fields->num_of_lvls; i++) {
        cout << "Level " << i << ": " << fields->lvl_args[i] << " bits" << endl;
    }

    cout << "\n# of Levels: " << fields->num_of_lvls << endl;
    cout << "TLB Size: " << fields->tlb_size << endl;
    cout << "Iterations: " << fields->iterations << endl;
    cout << "Parse_all Flag: " << fields->parse_all << "\n" << endl;

    cout << "PRINT MODE FLAGS:" << endl;
    cout << "Summary: " << fields->options->summary << endl;
    cout << "Bitmasks: " << fields->options->levelbitmasks << endl;
    cout << "Offset: " << fields->options->offset << endl;
    cout << "vpn2pfn " << fields->options->vpn2pfn << endl;
    cout << "va2pa: " << fields->options->va2pa << endl;
    cout << "va2patlb_walk: " << fields->options->va2pa_tlb_ptwalk<< endl;
}


/**
 * parses argv for iterations, TLB capacity, print modes, lvl_args, & file stream
 * @param argc                          # of args
 * @param argv                          ptr to arg char* array
 * @param fields                        Args, holding PageTable & TLB attrs
 */
void parseArgs(int argc, char **argv, Args* fields) {
    // Output options' defaults
    auto* options = fields->options;
    options->summary = true;
    options->levelbitmasks = false;
    options->offset = false;
    options->vpn2pfn = false;
    options->va2pa = false;
    options->va2pa_tlb_ptwalk = false;

    // PROCESS OPTIONAL ARGS
    int option = 0;
    while ((option = getopt(argc, argv, "n:c:p:")) != -1) {
        switch (option) {
            // Iterations
            case 'n': {
                int amount = atoi(optarg);
                if (amount < 0) {
                    cout << "Number of memory accesses must be a number, greater than or equal to 0" << endl;
                    exit(1);
                }

                fields->iterations = amount;
                fields->parse_all = false;
                break;
            }
            // TLB Size
            case 'c': {
                int tlb_size = atoi(optarg);
                if (tlb_size < 0) {
                    cout << "Cache capacity must be a number, greater than or equal to 0" << endl;
                    exit(1);
                }

                fields->tlb_size = tlb_size;
                break;
            }
            // Print mode
            case 'p': {
                string mode(optarg);

                // Disabling default 'summary' mode if not enabled
                if (mode != "summary") {
                    options->summary = false;
                }

                // Checking & enabling output modes
                if (mode == "levelbitmasks"){
                    options->levelbitmasks = true;
                } else if (mode == "va2pa"){
                    options->va2pa = true;
                } else if (mode == "va2pa_tlb_ptwalk"){
                    options->va2pa_tlb_ptwalk = true;
                } else if (mode == "vpn2pfn"){
                    options->vpn2pfn = true;
                } else if (mode == "offset"){
                    options->offset = true;
                }
                break;
            }
            case ':': {
                cout << "Option '" << optopt << "' was not given a parameter.";
                exit(1);
            }
            case '?': {
                cout << "Unknown argument: " << optopt;
                exit(1);
            }
            default: {
                printf("Usage: traceFile {bits_per_lvl} -n -c -p");
                exit(1);
            }
        }
    }

    // PROCESS MANDATORY ARGS
    if (argc - optind < 2) { // Usage check - if < 2, not enough args provided
        printf("Usage: traceFile {bits_per_lvl} -n -c -p");
        exit(1);
    }

    // Verify trace file
    const char* filename = argv[optind];
    fields->f_stream = fopen(filename, "r"); // stream
    if (fields->f_stream == nullptr) {
        cout << "Unable to open <<" << filename << ">>" << endl;
        exit(1);
    }
    optind++;

    // Processing bits per_level
    fields->num_of_lvls = argc - optind;
    fields->lvl_args = new int[fields->num_of_lvls];
    int bits = 0; int bit_sum = 0;
    for (int i = 0; i < argc-optind; i++) {
        bits = atoi(argv[optind+i]);

        if (bits <= 0) { // Check to ensure bits >= 1
            cout << "Level " << i << " page table must be at least 1 bit" << endl;
            exit(1);
        }
        fields->lvl_args[i] = bits;
        bit_sum += bits;
    }

    if (bit_sum > 28) { // Check to ensure bits_sum is <= 28
        cout << "Too many bits used in page tables" << endl;
        exit(1);
    }
}