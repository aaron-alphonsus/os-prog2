#include <stdio.h>
#include "bank.h"

void *customer_loop( void *param )
{
    int customer_id = *(int *)param;
    printf("Hey hey hey, it's customer %d\n", customer_id);
    
    

    // loop while (remaining > 0)
    // while (remaining() > 0)

    // create random request array (% need[customer_id][])

    // request resources
    // if > available, sleep and try again (continuously)
    // otherwise, sleep
    
    // create random release array (% alloc[customer_id][])
    // release_resources
    // sleep   
}

int request_resources( int customer_num, int request[] )
{
    // request for random number of resources (`
    if(1)
        return 0;
    return -1;
}

int release_resources( int customer_num, int release[] )
{
    if(1)
        return 0;
    return -1;
}

int safety_test ()
{

}

int remaining(int customer_id)
{
    int sum = 0;    

    for(int i = 0; i < NUMBER_OF_RESOURCES; i++)
        sum += need[customer_id][i];
    
    return sum;
}
