#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "bank.h"

int sum_need(int customer_id)
{
    int sum = 0;    

    for(int i = 0; i < NUMBER_OF_RESOURCES; i++)
        sum += need[customer_id][i];
    
    return sum;
}

void display()
{
    char resource_type[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    printf("\tAllocation\tNeed\t Available\n\t   ");

    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < NUMBER_OF_RESOURCES; j++)
        {
            printf("%c ", resource_type[j]); 
        }
        printf("      ");
    }
    printf("\n");

    for(int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
    {
        printf("P%d\t   ", i);
        
        for(int j = 0; j < NUMBER_OF_RESOURCES; j++)
            printf("%d ", allocation[i][j]);
        printf("      ");
        for(int j = 0; j < NUMBER_OF_RESOURCES; j++)
            printf("%d ", need[i][j]);
        
        if(i == 0)
        {
            printf("\t   ");
            for(int j = 0; j < NUMBER_OF_RESOURCES; j++)
                printf("%d ", available[j]);
        }
        printf("\n");
    }
    printf("\n");
}

void *customer_loop( void *param )
{
    int customer_id = *(int *)param;
    int request[NUMBER_OF_RESOURCES];
    int remaining = 0;
    int request_sum = 0;
    int try_again = 0;
    int success = 0;
    // printf("Hey hey hey, it's customer %d\n", customer_id);
 
    srandom((unsigned) time(NULL));
   
    remaining = sum_need(customer_id);

    pthread_mutex_lock(&mutex_lock);
    display();
    pthread_mutex_unlock(&mutex_lock);
    // printf(">P%d remaining = %d\n", customer_id, remaining);

    // // loop while (remaining > 0)
    // while (remaining > 0)
    // {
    //     // create random request array (% need[customer_id][])
    //     for(int i = 0; i < NUMBER_OF_RESOURCES; i++)
    //     {
    //         request[i] = rand() % (need[customer_id][i] + 1);
    //         request_sum += request[i];
    //     }        

    //     printf("Request P%d <", customer_id);
    //     for(int i = 0; i < NUMBER_OF_RESOURCES - 1; i++)
    //         printf("%d, ", request[i]);
    //     printf("%d>\n", request[NUMBER_OF_RESOURCES - 1]);
    //     
    //     printf("Request sum = %d\n", request_sum);

    //     // if(request_sum > 0)
    //     // {
    //     //     for(int i = 0; i < NUMBER_OF_RESOURCES; i++)
    //     //         need[customer_id][i] -= request[i];
    //     //     remaining = sum_need(customer_id);
    //     //     printf("P%d remaining = %d\n\n", customer_id, remaining);
    //     // }
    //                  
    //     // request resources
    //     // if > available, sleep and try again (continuously)
    //     success = request_resources(customer_id, request, &try_again);
    //     while (success == -1 && try_again)
    //     {
    //         sleep(rand() % MAX_SLEEP_TIME);
    //         success = request_resources(customer_id, request);
    //     }    
    //     // otherwise, sleep
    //     sleep(rand() % MAX_SLEEP_TIME);
    // 
    //     // create random release array (% alloc[customer_id][])
    //     // release_resources
    //     // sleep   
    // 
    //     remaining = sum_need(customer_id);

    //     request_sum = 0; 
    //     sleep(rand() % MAX_SLEEP_TIME);
    // }
    // // release allocation[customer_id] 
}

int request_resources(int customer_num, int request[], int *try_again)
{
    *try_again = 0;

    // acquire mutex lock    
    pthread_mutex_lock(&mutex_lock); 

    for(int i = 0; i < NUMBER_OF_RESOURCES; i++)
    {
        if (request[i] > available[i])
           *try_again = 1;
    }
    if(!try_again)
    {
        printf("Good to go. Allocate and safety test.");
        
        for(int i = 0; i < NUMBER_OF_RESOURCES; i++)
        {
            available[i] = available[i] - request[i];
            allocation[customer_num][i] = allocation[customer_num][i] + 
                request[i];
            need[customer_num][i] = need[customer_num][i] - request[i];
        }

        // safety test
        // if(!safety_test())

        return 0;
    }
    else
    {
        printf("Resources unavailable. Wait and try again");    
        return -1;    
    }
    
    // release mutex lock
    pthread_mutex_unlock(&mutex_lock);    
}

int release_resources(int customer_num, int release[])
{
    if(1)
        return 0;
    return -1;
}

int safety_test()
{

}
