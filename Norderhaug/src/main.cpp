#include <iostream>
extern "C" {
#include "vaddr_tracereader.h"
}

using namespace std;

void readTrace(const char *filename, int iterations) {
    FILE* trace_fstream = fopen(filename, "r"); // file stream
    p2AddrTr mtrace;

    if (iterations != 0)  { // Parse 'iterations' amount of records
        for (int i = 0; i < iterations; i++) {
            if (NextAddress(trace_fstream, &mtrace)) {
                printf("Hex address (printf): %08lx\n", mtrace.addr);
                fflush(stdout);
                cout << "Hex address (cout): " << hex << mtrace.addr << endl;
                fflush(stdout);
            }
        }
    } else { // Parse entire file
        while (NextAddress(trace_fstream, &mtrace)) {
            printf("Hex address (printf): %08lx\n", mtrace.addr);
            fflush(stdout);
            cout << "Hex address (cout): " << hex << mtrace.addr << endl;
            fflush(stdout);
        }
    }

    fclose(trace_fstream);
}

int main(int argc, char **argv) {
    readTrace(argv[1], int(*argv[2] - '0'));
}

