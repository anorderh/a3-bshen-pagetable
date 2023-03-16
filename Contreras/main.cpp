#include <iostream>
#include <unistd.h>

using namespace std;
extern "C" {
#include "vaddr_tracereader.h"
}

void readTrace(const char* filename, int iterations){
    FILE* trace_fstream = fopen(filename, "r");
    if(trace_fstream == NULL){
        cout << "Not valid";
        exit(-1);
    }
    int isValid = 0;

    p2AddrTr mtrace;
    //in the case that we're given specified amount to parse
    if(iterations != 0) {
        for (int i = 0; i < iterations; i++) {
            if (NextAddress(trace_fstream, &mtrace)) {
                printf("Hex address (printf): %08x\n", mtrace.addr);
                fflush(stdout);
                cout << "Hex address (cout): " << hex << mtrace.addr << endl;
                fflush(stdout);
            }
        }
    }
    //in the case no number of iterations are specified
    else{
            while(NextAddress(trace_fstream, &mtrace)){
                printf("Hex address (printf): %08lx\n", mtrace.addr);
                fflush(stdout);
                cout << "Hex address (cout): " << hex << mtrace.addr << endl;
                fflush(stdout);
            }
        }
    fclose(trace_fstream);
}
//given virtual address, apply bitmask and shift right by
// given num of bits to get VPN of any level
unsigned int virtualAddressToVPN(unsigned int virtualAddress, unsigned int mask, unsigned int shift){
//use bitwise AND with virtual address and mask
virtualAddress = virtualAddress & mask;
//shift right to get our VPN
virtualAddress = virtualAddress>>shift;
cout << virtualAddress;
return virtualAddress;
}

int main(int argc, char **argv) {
    readTrace(argv[1], int (*argv[2]));
    virtualAddressToVPN(0x3c654321,0x0FC00000,22);
}
