/**
 * @file customer.c
 *
 * @brief Defines the behavior of the customer threads. 
 *
 * The customer threads spin in a continuous loop, requesting and releasing 
 * random numbers of resources. The banker evaluates the request and makes sure
 * that the resulting system is not unsafe. A display function helps keep track
 * of the changes that happen at each step.
 *
 * @author Aaron Alphonsus
 * 
 * @date 3 April 2017 
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "bank.h"

/**
 * Function to print the state of the system out to the console in a tabular 
 * format.
 */
void display()
{
	/// Declare array for resource type
    char resource_type[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	/// Print header
    printf("\tAllocation\tNeed\t Available\n\t   ");

	/// Print resource type header
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < NUMBER_OF_RESOURCES; j++)
        {
            printf("%c ", resource_type[j]); 
        }
        printf("      ");
    }
    printf("\n");

	/// Print data structures keeping track of resources
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

/**
 * Contains the algorithm that determines whether the system is in a safe state
 * or not. (Reference: 7.5.3.1 - Operating System Concepts - Silberschatz)
 * 
 * @return 0 if safe, -1 if not
 */
int safety_test()
{
	/// Declare work and finish arrays (along with helper variables)
    int work[NUMBER_OF_RESOURCES];
    int finish[NUMBER_OF_CUSTOMERS];
    int i = 0, flag = 0, unsafe = 0;

	/// Initialize work and finish arrays
    for(int i = 0; i < NUMBER_OF_RESOURCES; i++)
        work[i] = available[i];
    for(int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
        finish[i] = 0;

	/// For each 'false' finish element, check if need <= work
    i = 0;
    while(i < NUMBER_OF_CUSTOMERS)
    {
        if(finish[i] == 0)
        {
            for(int j = 0; j < NUMBER_OF_RESOURCES; j++)
            {
                if(need[i][j] > work[j])
                    flag = 1;
            }
            if(flag == 0)
            {
                for(int j = 0; j < NUMBER_OF_RESOURCES; j++)
                    work[j] += allocation[i][j];

                finish[i] = 1;
                i = 0;
            }
        }
        flag = 0;
        i++;
    }
    /// Once the algorithm is finished, look for false values in the finish 
	/// array which indicates whether the state is unsafe
    for(int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
    {
        if(finish[i] == 0)
            unsafe = 1;
    }
 
    if(unsafe == 1)
        return -1;       
    else
        return 0;
}

/**
 * This function requests for a random number of resources bounded by values 
 * in the need array. Since this function modifies shared resources, we make 
 * sure that we acquire the mutex lock before making changes, and release it 
 * once we are done. The request for resources can fail if there aren't enough
 * resources available or if granting the resources causes the system to be 
 * placed into an unsafe state. Appropriate messages are provided in each case.
 *
 * @param[in] customer_num Holds the customer id
 * @param[in] request The random request array generated
 * 
 * @return 0 if granted, -1 if not
 */
int request_resources(int customer_num, int request[])
{
    int flag = 0;    

    /// acquire mutex lock    
    pthread_mutex_lock(&mutex_lock); 
 
    /// Print request statement and array
    printf("Request P%d <", customer_num);
    for(int i = 0; i < NUMBER_OF_RESOURCES - 1; i++)
        printf("%d, ", request[i]);
    printf("%d>\n", request[NUMBER_OF_RESOURCES - 1]);

    /// Check if request is less than available resources
    for(int i = 0; i < NUMBER_OF_RESOURCES; i++)
    {
        if (request[i] > available[i])
           flag = 1;
    }
    if(flag)
    {
		/// Display message if resources unavailable and return -1
        printf("Resources unavailable.\n\n");    
        pthread_mutex_unlock(&mutex_lock);  
        return -1; 
    }
    else
    {
		/// Make changes to available, allocation and need
        for(int i = 0; i < NUMBER_OF_RESOURCES; i++)
        {
            available[i] = available[i] - request[i];
            allocation[customer_num][i] = allocation[customer_num][i] + 
                request[i];
            need[customer_num][i] = need[customer_num][i] - request[i];
        }
        
        /// Call safety test
        if(safety_test() == 0)
        {
			/// If request granted, print message.
            printf("Safe, request granted.\n\n");
            display();

            pthread_mutex_unlock(&mutex_lock);
            return 0;
        }
        else
        {
            /// If unsafe, rollback changes
            printf("Unsafe, request denied.\n\n");
            for(int i = 0; i < NUMBER_OF_RESOURCES; i++)
            {
                available[i] = available[i] + request[i];
                allocation[customer_num][i] = allocation[customer_num][i] - 
                    request[i];
                need[customer_num][i] = need[customer_num][i] + request[i];
            }
    
			/// Release mutex lock before returning
            pthread_mutex_unlock(&mutex_lock);
            return -1;           
        } 
    }      
}

/**
 * This function releases a random number of resources bounded by values 
 * in the allocation array. Since this function modifies shared resources, we 
 * make sure that we acquire the mutex lock before making changes, and release 
 * it once we are done.
 * 
 * @param[in] customer_num Holds the customer id
 * @param[in] release The random release array generated
 * 
 * @return 0 if successful
 */
int release_resources(int customer_num, int release[])
{
    /// acquire mutex lock    
    pthread_mutex_lock(&mutex_lock); 
 
    /// Print release array
    printf("Release P%d <", customer_num);
    for(int i = 0; i < NUMBER_OF_RESOURCES - 1; i++)
        printf("%d, ", release[i]);
    printf("%d>\n\n", release[NUMBER_OF_RESOURCES - 1]);

	/// Update resource arrays 
    for(int i = 0; i < NUMBER_OF_RESOURCES; i++)
    {
        available[i] = available[i] + release[i];
        allocation[customer_num][i] = allocation[customer_num][i] - release[i];
        need[customer_num][i] = need[customer_num][i] + release[i];
    }
        
	/// Display current state
    display(); 

    /// Release mutex lock before returning
    pthread_mutex_unlock(&mutex_lock); 
    return 0;     
}

/**
 * This function simulates the behavoir of each customer. The customers request
 * some amount of resources, wait and release some amount of resources. This 
 * happens in a continuous loop
 *
 * @param[in] param Pointer to an int pointer to the student id
 */

void *customer_loop( void *param )
{
	/// Declare local helper variables
    int customer_id = *(int *)param;
    int request[NUMBER_OF_RESOURCES], release[NUMBER_OF_RESOURCES];
    int request_sum = 0, release_sum = 0;
 
	/// Seed random generator
    srandom((unsigned) time(NULL));
   
    /// Loop continuously
    while (1)
    {
        /// Create random request array (% need[customer_id][])
        for(int i = 0; i < NUMBER_OF_RESOURCES; i++)
        {
            request[i] = rand() % (need[customer_id][i] + 1);
            request_sum += request[i];
        }        
          
        if(request_sum > 0)
        {                    
            /// Request resources
            request_resources(customer_id, request);
        
            /// Sleep for random amount of time    
            sleep(rand() % MAX_SLEEP_TIME);
        }    
        
        /// Create random release array (% alloc[customer_id][])
        for(int i = 0; i < NUMBER_OF_RESOURCES; i++)
        {
            release[i] = rand() % (allocation[customer_id][i] + 1);
            release_sum += release[i];
        }        
          
        if(release_sum > 0)
        {                    
            /// Release resources
            release_resources(customer_id, release);
        
            /// Sleep for random amount of time    
            sleep(rand() % MAX_SLEEP_TIME);
        }   
 
		/// Reset local variables   
        request_sum = 0; 
        release_sum = 0;
    } 
}
