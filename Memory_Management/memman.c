#include <stdio.h>
#include <stdlib.h>

#define MASK 4095
#define BIT_SHIFT 12

int main(int argc, char *argv[])
{    
    unsigned long long virtual_address, pagenum, offset;

    if (argc != 2)
    {
        printf("Usage: ./memman virtual-address\n");
        exit(0);
    }

    virtual_address = atoll(argv[1]);

    if (virtual_address < 0 || virtual_address >= 4294967296)
    {
        printf("Please enter a virtual address within the range 0 to 2^32 - 1."
            "\n");
        exit(1);
    }
   
    offset = virtual_address & MASK;
    pagenum = virtual_address >> 12;
   
    printf("The address %llu contains:\n", virtual_address); 
    printf("page number = %llu\n", pagenum);
    printf("offset = %llu\n", offset); 
 
    return 0;
}
