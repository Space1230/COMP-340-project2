/**
 * Header file for dining philosophers
 */

#include <pthread.h>
#include <semaphore.h>  
#include <stdlib.h>
#include <unistd.h>

// the number of philosophers
#define NUMBER 		5

// the maximum number of random numbers
#define MAX_LENGTH 500

// the array holding the list of random numbers 
int rand_numbers[MAX_LENGTH];

//function for getting a random number from the list
int get_next_number();

//mutex lock to use in order to protect the order of random numbers
pthread_mutex_t mutex_rand;

//position of next random number 
int rand_position;

// the state of each philosopher (THINKING, HUNGRY, EATING)
enum {THINKING, HUNGRY, EATING} state[NUMBER];

// the id of each philosopher (0 .. NUMBER - 1)
int thread_id[NUMBER];

// semaphore variables and associated mutex lock
sem_t		sem_vars[NUMBER];
pthread_mutex_t 	mutex_lock;

//function that simulates the philosopher operation
void *philosopher(void *param);

//function for the philosopher to pickup the chopsticks
void pickup_chopsticks(int number);

//function for the philosopher to return the chopsticks
void return_chopsticks(int number);
