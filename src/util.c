#include <stdlib.h>

#include "../include/util.h"


unsigned int rand_between(unsigned int lower, unsigned int upper) {
    /* Return a random number between `lower` and `upper`.
    ** Note that we don't seed. Additionally, we are working with 
    ** the naturals mod upper.
    */
    return (rand() % (upper - lower + 1)) + lower;
}


unsigned int rand_below(unsigned int max) {
    /* Return a random number below `max` but 
    ** greater than or equal to 0.
    */
    return (rand() % max);
}
