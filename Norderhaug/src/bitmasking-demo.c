#include <stdio.h>

/* demo of bit masking and shifting */
int main(int argc, char **argv) {

  int	page;
  
  /* Suppose that we have a three level page table:
   * Level 0:  4 bits
   * Level 1:  8 bits
   * Level 2:  8 bits
   *
   * offset is 32 - (4 + 8 + 8) = 12 bits
   * page size is 2^12 = 4096 bytes
   */

  int	address = 0xFe98f982;

  /* Suppose we are interested in the level 1 page table index:
   * We would have to construct a bit mask where the following bits
   * are set:
   *
   * 3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 
   * 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
   * ===============================================================
   * 0 0 0 0|1 1 1 1|1 1 1 1|0 0 0 0|0 0 0 0|0 0 0 0|0 0 0 0|0 0 0 0 
   * or in hexadecimal:
   *    0   |   F   |   F   |   0   |   0   |   0   |   0   |   0
   *
   * You will have to construct this in an algorithmic fashion, for
   * demonstration purposes we'll just use a constant.
   */
  unsigned int	mask = 0x0FF00000;

  /* Note that since we stored Address as an int instead of an
   * unsigned int, when we print in decimal we think that this is an
   * negative number if bit 31 is set.  When printing in hexadecimal,
   * all numbers are assumed unsigned and it won't make any
   * difference.  (If we had declared this as an unsigned int, it
   * would have printed as positive integer.)
   */
  printf("Address:\t0x%08x (%d decimal)\n", address, address);

  /* Pull out the relevant bits by bit-wise and */
  page = mask & address;
  printf("After masking:\t0x%08x\n", page);

  /* Shift right by the relevant number of bits */
  page = page >> 20;

  printf("Page number = 0x%x\n", page);

  /* example to build a mask 0xF0*/
  unsigned int   aMask = 1;
  unsigned int   numOfMaskBits = 4;
  /* after the following loop, aMask will become 0xF */
  for (int b = 1; b < numOfMaskBits; b++)
  {
    aMask = aMask << 1;
    aMask = aMask | 1;
  }

  // Shift into appropriate position, for this example, shift to left by 4 bits
  int leftShift = 4;
  aMask = aMask << leftShift; // aMask will become 0xF0
}
