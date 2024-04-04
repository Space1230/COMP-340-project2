#include "dp.h"
#include <stdio.h> // for debuging
#include <ctype.h>

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
    }
    return NULL;
}

//function for the philosopher to think
void think(int number) {
    state[number] = THINKING;
    sleep(get_next_number());
}

//function for the philosopher to pickup the chopsticks
void pickup_chopsticks(int number) {
    pthread_mutex_lock(&mutex_lock);
    state[number] = HUNGRY;
    test(number);
    pthread_mutex_unlock(&mutex_lock);
    sem_wait(&sem_vars[number]);
}

//helper function for pickup_chopsticks and return_chopsticks
void test(int number) {
    if (state[number] == HUNGRY && // want to compete
        state[(number + NUMBER - 1) % NUMBER] != EATING && // LEFT
        state[(number + 1) % NUMBER] != EATING) { // RIGHT

        state[number] = EATING; // phi[i] can eat
        sem_post(&sem_vars[number]); // wake up phi[i] if it is blocked
    }
}

//function for the philosopher to return the chopsticks
void return_chopsticks(int number) {
    pthread_mutex_lock(&mutex_lock);
    state[number] = THINKING;
    test((number + NUMBER - 1) % NUMBER); // test LEFT
    test((number + 1) % NUMBER); // test RIGHT
    sem_post(&sem_vars[number]);
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
            rand_numbers[rand_position++] = input;
        }
    }

    rand_position = 0; // resetting for future use

    // testing
    /* for (int i = 0; i < MAX_LENGTH; i++) { */
    /*     printf("%c ", rand_numbers[i]); */
    /* } */
    /* printf("\n"); */

    //Initalizing for the Philosophers
    pthread_mutex_init(&mutex_rand, NULL);
    pthread_mutex_init(&mutex_lock, NULL);
    for (int i = 0; i < NUMBER; i++) {
        sem_init(&sem_vars[i], 0, 0);
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
