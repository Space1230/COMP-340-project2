#include "dp.h"
#include <stdio.h> // for debuging
#include <ctype.h>
#include <stdbool.h>
#include <sys/time.h>

//function for getting a random number from the list
int get_next_number() {
    if (rand_position == MAX_LENGTH) {
        fprintf(stderr, "Exhasted random numbers\n");
        exit(EXIT_FAILURE);
    }
    return rand_numbers[rand_position++];
}

//function that simulates the philosopher operation
void *philosopher(void *param) {
    int philosopher_number = *(int*)param;
    while (1) {
        think(philosopher_number);
        pickup_chopsticks(philosopher_number);
        eat(philosopher_number);
        return_chopsticks(philosopher_number);

    }
    return NULL;
}

//function for the philosopher to think
void think(int number) {
    state[number] = THINKING;
    int wait_time = get_next_number();
    printf("%d: thinking for %d seconds\n", number, wait_time);
    sleep(wait_time);
}

//function for the philosopher to pickup the chopsticks
void pickup_chopsticks(int number) {
    printf("%d: pickup chopsticks\n", number);
    pthread_mutex_lock(&mutex_lock);
    state[number] = HUNGRY;
    printf("%d: hungry\n", number);
    test(number);
    pthread_mutex_unlock(&mutex_lock);
    sem_wait(&sem_vars[number]);
}

//helper function for pickup_chopsticks and return_chopsticks
void test(int number) {
    bool eat = false;
    bool lock_middle = false;
    //pthread_mutex_lock(&mutex_middle); 

    if (pthread_mutex_trylock(&mutex_middle) == 0) { // try this for now to solve error
        lock_middle = true;
    }

    // do a check if the philosopher can eat with both adjacent 
    //chopsticks or one adjacent and the middle one
    if (((state[(number + NUMBER - 1) % NUMBER] != EATING) && 
    (state[(number + 1) % NUMBER] != EATING)) || // if this passes then adjacent chopsticks are available
        (lock_middle && ((state[(number + NUMBER - 1) % NUMBER] != EATING) || 
        (state[(number + 1) % NUMBER] != EATING)))) { // otherwise middle chopstick is locked and one adjacent is available
        eat = true;
    }

    if (eat && state[number] == HUNGRY) {
        gettimeofday(&time_after[number], NULL); // Time recorded when philosopher starts eating
        state[number] = EATING;
        sem_post(&sem_vars[number]); // wake up phi[i] if it is blocked
    }

    if (!eat && lock_middle) {
        pthread_mutex_unlock(&mutex_middle);
    }


    // if (state[number] == HUNGRY && // want to compete
    //     state[(number + NUMBER - 1) % NUMBER] != EATING && // LEFT
    //     state[(number + 1) % NUMBER] != EATING) { // RIGHT

    //     state[number] = EATING; // phi[i] can eat
    //     printf("%d: eating\n", number);
    //     sem_post(&sem_vars[number]); // wake up phi[i] if it is blocked
    // }
}
//function for the philosopher to eat
void eat(int number) {
    int eating_time = get_next_number();
    printf("%d: eating for %d seconds\n", number, eating_time);
    waitTime[number] = 
    (double)(time_after[number].tv_usec - time_before[number].tv_usec) / 1000 +
    (double)(time_after[number].tv_sec - time_before[number].tv_sec) * 1000;
    printf("%d: waited for %f milliseconds\n", number, waitTime[number]);
    sleep(eating_time);
}

//function for the philosopher to return the chopsticks
void return_chopsticks(int number) {
    printf("%d: putdown chopsticks\n", number);
    pthread_mutex_lock(&mutex_lock);
    state[number] = THINKING;
    printf("%d: thinking\n", number);
    test((number + NUMBER - 1) % NUMBER); // test LEFT
    test((number + 1) % NUMBER); // test RIGHT
    //sem_post(&sem_vars[number]);
    pthread_mutex_unlock(&mutex_lock);
}

int main(int argc, char* argv[]) {
    // Argument Error Checking
    if (argc < 2) {
        fprintf(stderr, "Please insert your random nums file\n");
        exit(EXIT_FAILURE);
    }
    else if (argc > 2) {
        fprintf(stderr, "Too many commands, only include your random nums file\n");
        exit(EXIT_FAILURE);
    }

    // Parsing Random Num File
    char* filename = argv[1];
    FILE* random_num_file = fopen(filename, "r");

    if (random_num_file == NULL) {
        fprintf(stderr, "File name doesn't exist, maybe try putting it in quotes\n");
        exit(EXIT_FAILURE);
    }

    char input;
    rand_position = 0;

    while ((input = fgetc(random_num_file)) != EOF) {
        if (!isblank(input) && rand_position < MAX_LENGTH){
            rand_numbers[rand_position++] = input - '0';
        }
    }

    rand_position = 0; // resetting for future use

    // testing
    /* for (int i = 0; i < MAX_LENGTH; i++) { */
    /*     printf("%i ", rand_numbers[i]); */
    /* } */
    /* printf("\n"); */

    //Initalizing for the Philosophers
    pthread_mutex_init(&mutex_rand, NULL);
    pthread_mutex_init(&mutex_lock, NULL);
    for (int i = 0; i < NUMBER; i++) {
        sem_init(&sem_vars[i], 0, 0);
        printf("thinking\n");
        state[i] = THINKING;
    }

    //Creating the Philosophers
    pthread_t philosophers[NUMBER];

    for (int i = 0; i < NUMBER; i++) {
        thread_id[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &thread_id[i]);
    }

    // Collecting the Philosophers
    for (int i = 0; i < NUMBER; i++) {
        pthread_join(philosophers[i], NULL);
    }
}
