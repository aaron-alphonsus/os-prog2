#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "bank.h"

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

int request_resources(int customer_num, int request[])
{
    int flag = 0;    

    // acquire mutex lock    
    pthread_mutex_lock(&mutex_lock); 
 
    // Print request array
    printf("Request P%d <", customer_num);
    for(int i = 0; i < NUMBER_OF_RESOURCES - 1; i++)
        printf("%d, ", request[i]);
    printf("%d>\n", request[NUMBER_OF_RESOURCES - 1]);

    // Check if request is less than available resources
    for(int i = 0; i < NUMBER_OF_RESOURCES; i++)
    {
        if (request[i] > available[i])
           flag = 1;
    }
    if(flag)
    {
        printf("Resources unavailable.\n\n");    
        pthread_mutex_unlock(&mutex_lock); // Release mutex lock
        
        return -1; 
    }
    else
    {
        printf("Good to go. Allocate and safety test.\n\n");
        
        for(int i = 0; i < NUMBER_OF_RESOURCES; i++)
        {
            available[i] = available[i] - request[i];
            allocation[customer_num][i] = allocation[customer_num][i] + 
                request[i];
            need[customer_num][i] = need[customer_num][i] - request[i];
        }

        display();

        // safety test
        // if(!safety_test())
            // return -1
        // else
            // return 0            

        // release mutex lock
        pthread_mutex_unlock(&mutex_lock); 
        return 0; 
    }      
}

int release_resources(int customer_num, int release[])
{
    // acquire mutex lock    
    pthread_mutex_lock(&mutex_lock); 
 
    // Print release array
    printf("Release P%d <", customer_num);
    for(int i = 0; i < NUMBER_OF_RESOURCES - 1; i++)
        printf("%d, ", release[i]);
    printf("%d>\n\n", release[NUMBER_OF_RESOURCES - 1]);

    for(int i = 0; i < NUMBER_OF_RESOURCES; i++)
    {
        available[i] = available[i] + release[i];
        allocation[customer_num][i] = allocation[customer_num][i] - release[i];
        need[customer_num][i] = need[customer_num][i] + release[i];
    }
        
    display(); 

    // release mutex lock
    pthread_mutex_unlock(&mutex_lock); 
    return 0;     
}

void *customer_loop( void *param )
{
    int customer_id = *(int *)param;
    int request[NUMBER_OF_RESOURCES], release[NUMBER_OF_RESOURCES];
    int request_sum = 0, release_sum = 0;
    
    // printf("Hey hey hey, it's customer %d\n", customer_id);
 
    srandom((unsigned) time(NULL));
   
    // loop continuously
    while (1)
    {
        // create random request array (% need[customer_id][])
        for(int i = 0; i < NUMBER_OF_RESOURCES; i++)
        {
            request[i] = rand() % (need[customer_id][i] + 1);
            request_sum += request[i];
        }        
          
        if(request_sum > 0)
        {                    
            // request resources
            request_resources(customer_id, request);
        
            // sleep for random amount of time    
            sleep(rand() % MAX_SLEEP_TIME);
        }    
        
        // create random release array (% alloc[customer_id][])
        for(int i = 0; i < NUMBER_OF_RESOURCES; i++)
        {
            release[i] = rand() % (allocation[customer_id][i] + 1);
            release_sum += release[i];
        }        
          
        if(release_sum > 0)
        {                    
            // release resources
            release_resources(customer_id, release);
        
            // sleep for random amount of time    
            sleep(rand() % MAX_SLEEP_TIME);
        }   
    
        request_sum = 0; 
        release_sum = 0;
    } 
}

int safety_test()
{
    
}
