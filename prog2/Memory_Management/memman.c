/**
 * @file memman.c
 *
 * @brief Takes in a 32-bit virtual address and outputs the page number and 
 * offset
 *
 * This program performs bitwise operations on the virtual address passed in. 
 * Based on the information provided regarding the page size, we can build a 
 * mask to extract the bits we need, in order to calculate the page number and
 * offset, as required.
 *
 * Compilation Instuctions: make
 * Run: ./memman virtual-address
 *
 * @author Aaron Alphonsus
 * 
 * @date 3 April 2017 
 */

#include <stdio.h>
#include <stdlib.h>

/// Mask to extract lower n bits
#define MASK 4095

/// Value to shift by to capture higher-order bits 
#define BIT_SHIFT 12

/**
 * Function takes in a single decimal value from the command line. It performs
 * bitwise operations to extract the lower 12 bits for the offset and the 
 * higher-order bits for the page number. It then prints this information to 
 * the console.
 *
 * @param[in] argc Integer count of the command-line arguments 
 * @param[in] argv Vector of the command-line arguments
 *
 * @return 0 Indicates normal termination of main.
 */
int main(int argc, char *argv[])
{    
	/// Declare variables to store address, page number and offset    
	unsigned long long virtual_address, pagenum, offset;

	/// Check for correct number of command-line arguments
    if (argc != 2)
    {
        printf("Usage: ./memman virtual-address\n");
        exit(0);
    }

	/// Convert command line argument to long long
    virtual_address = atoll(argv[1]);

	/// Make sure address is within range
    if (virtual_address < 0 || virtual_address >= 4294967296)
    {
        printf("Please enter a virtual address within the range 0 to 2^32 - 1."
            "\n");
        exit(1);
    }
   
	/// Perform bitwise operations to calculate offset and page number
    offset = virtual_address & MASK;
    pagenum = virtual_address >> BIT_SHIFT;
 
	/// Output page number and offset to console  
    printf("The address %llu contains:\n", virtual_address); 
    printf("page number = %llu\n", pagenum);
    printf("offset = %llu\n", offset); 
 
    return 0;
}
