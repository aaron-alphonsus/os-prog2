#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{    
    if (argc != 2)
    {
        printf("Usage: ./memman virtual-address\n");
        exit(0);
    }

    printf("%s\n", argv[1]);  
    return 0;
}
