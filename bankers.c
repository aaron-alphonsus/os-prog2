#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include "bank.h"
#include "customer.h"

pthread_t customers[NUMBER_OF_CUSTOMERS];

void init(char *argv[])
{
    if(pthread_mutex_init(&mutex_lock, NULL) != 0)
       printf("%s\n", strerror(errno));

    // Test only two arguments.
    for(int i = 0; i < NUMBER_OF_RESOURCES; i++)
        available[i] = atoi(argv[i+1]);

    for(int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
    {
        for(int j = 0; j < NUMBER_OF_RESOURCES; j++)
           allocation[i][j] = 0;
    }

    for(int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
    {
        for(int j = 0; j < NUMBER_OF_RESOURCES; j++)
           maximum[i][j] = rand() % (available[j] + 1);
    }

    // need = maximum initially since allocation = 0
    for(int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
    {
        for(int j = 0; j < NUMBER_OF_RESOURCES; j++)
           need[i][j] = maximum[i][j] - allocation[i][j]; 
    }

    for(int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
        customer_id[i] = i;   
}

void create_customers()
{
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
    {
        pthread_create(&customers[i], NULL, customer_loop, 
            (void *)&customer_id[i] );
    }
}

int main(int argc, char *argv[])
{
    srandom((unsigned) time(NULL));

    init(argv);
    // printf("The amount of each resource: %d, %d, & %d\n", available[0], 
        // available[1], available[2]);
    // for(int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
    // {
    //     for(int j = 0; j < NUMBER_OF_RESOURCES; j++)
    //         printf("%d ", allocation[i][j]);
    //     printf("\n");
    // }
    // for(int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
    // {
    //     for(int j = 0; j < NUMBER_OF_RESOURCES; j++)
    //         printf("%d ", maximum[i][j]);
    //     printf("\n");
    // }
    // for(int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
    // {
    //     for(int j = 0; j < NUMBER_OF_RESOURCES; j++)
    //         printf("%d ", need[i][j]);
    //     printf("\n");
    // }


    display();

    create_customers();   

    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
        pthread_join(customers[i], NULL);

    return 0;
}
