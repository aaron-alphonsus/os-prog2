/**
 * @file bank.h
 * 
 * @brief Bank header file. Contains data structures to keep track of the 
 * resources
 *
 * @author Aaron Alphonsus
 *
 * @date 3 April 2017
 */

/* these may be any values >= 0 */

/// Number of customers
#define NUMBER_OF_CUSTOMERS 5

/// Number of resources
#define NUMBER_OF_RESOURCES 3

/// Maximum time (in seconds) to sleep. 
#define MAX_SLEEP_TIME 5

/// Mutex lock
pthread_mutex_t mutex_lock;

/// The available amount of each resource 
int available[NUMBER_OF_RESOURCES];

/// The maximum demand of each customer
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

/// The amount currently allocated to each customer 
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

/// The remaining need of each customer
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

/// Numeric id of each customer
int customer_id[NUMBER_OF_CUSTOMERS];
