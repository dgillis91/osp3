/* Test for util.c.
** gcc -o random_test test_random.c ../src/util.c
** Need a way to run automated tests for C. I'll look into this
** at another time. Also need a way to test random generation
** more effectively.
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "../include/util.h"

#define UPPER 10
#define LOWER 1
#define TO_GEN 100

int main(void) {
    srand(time(NULL));
    unsigned int freq_table[UPPER];
    unsigned int i, current_rand;

    // Initialize frequencies.
    for (i = 0; i < UPPER; ++i) {
        freq_table[i] = 0;
    }

    // Generate `TO_GEN` randoms, log them
    // in the frequency table. 
    for (i = 0; i < TO_GEN; ++i) {
        current_rand = rand_between(LOWER, UPPER);
        freq_table[current_rand - 1] += 1;
    }

    // For now, I'm going to print them out. However,
    // there are ways to validate that we are generating
    // from the correct distribution. Worth noting, there
    // are no guarantees about the distribution rand(),
    // draws from. 
    for (i = 0; i < UPPER; ++i) {
        printf("%d: %d\n", i + 1, freq_table[i]);
    }

    return EXIT_SUCCESS;
}
