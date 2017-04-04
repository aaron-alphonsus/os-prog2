/**
 * @file customer.h
 * 
 * @brief Customer header file. Contains function prototypes to request and 
 * release resources (along with helper functions)
 *
 * @author Aaron Alphonsus
 *
 * @date 3 April 2017
 */

/// Customer function loops continually, requesting and releasing resources
void *customer_loop(void *param);

/// Check whether request for resources can be granted
int request_resources(int customer_num, int request[]);

/// Returns resources to available pool
int release_resources(int customer_num, int release[]);

/// Checks for safe state
int safety_test();

/// Displays state of the system
void display();
