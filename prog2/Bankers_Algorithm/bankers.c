/**
 * @file bankers.c
 *
 * @brief Main file for the Banker's Algorithm. 
 *
 * The program begins by initializing the required synchronization objects and
 * data structures. Then, it creates a number of threads, each representing a 
 * customer. These threads run the customer_loop() which contains the main 
 * logic behind the Banker's Algorithm simulation. 
 * 
 * Compilation Instructions: make
 * Run: ./bankers [num_resource1, num_resource2, ..., num_resourceN]
 *
 * @author Aaron Alphonsus
 * 
 * @date 3 April 2017 
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include "bank.h"
#include "customer.h"

/// Threads representing customers
pthread_t customers[NUMBER_OF_CUSTOMERS];

/**
 * Initializes matrices that keep track of the resource utilization. Also 
 * initializes the mutex. 
 *
 * @param[in] argv Vector of the command-line arguments
 */
void init(char *argv[])
{
	/// Initialize mutex
    if(pthread_mutex_init(&mutex_lock, NULL) != 0)
       printf("%s\n", strerror(errno));

	/// Initialize available array with values passed in via command-line
    for(int i = 0; i < NUMBER_OF_RESOURCES; i++)
        available[i] = atoi(argv[i+1]);

	/// Initialize allocation matrix (0 initially)
    for(int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
    {
        for(int j = 0; j < NUMBER_OF_RESOURCES; j++)
           allocation[i][j] = 0;
    }

	/// Initialize maximum matrix using available array as a bound
    for(int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
    {
        for(int j = 0; j < NUMBER_OF_RESOURCES; j++)
           maximum[i][j] = rand() % (available[j] + 1);
    }

    /// Initialize need matrix. (need = maximum) initially since allocation = 0
    for(int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
    {
        for(int j = 0; j < NUMBER_OF_RESOURCES; j++)
           need[i][j] = maximum[i][j] - allocation[i][j]; 
    }
	
	/// Initialize customer id array
    for(int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
        customer_id[i] = i;   
}

/**
 * Creates a thread for each customer to execute customer_loop(). Passes in 
 * the array of customer ids to give an identity to each customer thread.
 */
void create_customers()
{
	/// Loop through defined number of customers, creating customer threads
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
    {
        pthread_create(&customers[i], NULL, customer_loop, 
            (void *)&customer_id[i] );
    }
}

/**
 * Makes function call to initialize data structures, and a call to the display
 * function to print this initial state to the console. It then makes a 
 * function call to create customer threads and execute the threaded function
 * customer_loop(), followed by the joining of these threads. 
 *
 * @param[in] argc Integer count of the command-line arguments 
 * @param[in] argv Vector of the command-line arguments
 * 
 * @return 0 Indicates normal termination of main.
 */
int main(int argc, char *argv[])
{
	/// Seed random generator
    srandom((unsigned) time(NULL));

	/// Call function to initialize resource arrays and the mutex lock  
    init(argv);
	/// Call function to display initial state
    display();

	/// Call function to create customer threads to execute customer_loop()	
    create_customers();
	/// Join customer threads   
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
        pthread_join(customers[i], NULL);

    return 0;
}
