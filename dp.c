#include "dp.h"
#include <stdio.h> // for debuging
#include <ctype.h>

//function for getting a random number from the list
int get_next_number() {
    return 1;
}

//function that simulates the philosopher operation
void *philosopher(void *param) {
    return (void*) 0;
}

//function for the philosopher to pickup the chopsticks
void pickup_chopsticks(int number) {

}

//function for the philosopher to return the chopsticks
void return_chopsticks(int number) {

}

#define BUFFER_LEN MAX_LENGTH * 2

int main(int argc, char* argv[]) {
    // Argument Error Checking
    if (argc < 2) {
        fprintf(stderr, "Please insert your random nums file\n");
        return -1;
    }
    else if (argc > 2) {
        fprintf(stderr, "Too many commands, only include your random nums file\n");
        return -1;
    }

    // Parsing Random Num File
    char* filename = argv[1];
    FILE* random_num_file = fopen(filename, "r");
    if (random_num_file == NULL) {
        fprintf(stderr, "File name doesn't exist, maybe try putting it in quotes\n");
        return -1;
    }
    char input;
    rand_position = 0;
    while ((input = fgetc(random_num_file)) != EOF) {
        if (isblank(input) && rand_position < MAX_LENGTH){
            rand_numbers[rand_position++] = (char) fgetc(random_num_file);
        }
    }
    rand_position = 0; // resetting for future use

    // testing parsing
    /* for (int i = 0; i < MAX_LENGTH; i++) { */
    /*     printf("%c ", rand_numbers[i]); */
    /* } */
    /* printf("\n"); */
}
