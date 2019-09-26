/* pclock.h
** Abstract representation of a shared clock.
** We need to create atomic access to the clock (sounds funny). 
** We will have n-many processes. We need to ensure that only
** the parent can modify the clock. All children can read it. 
*/
#ifndef P_CLOCK_H_
#define P_CLOCK_H_

#define NANO_SEC_IN_SEC 1000000000

/* Abstract representation of a system clock.
*/
typedef struct {
    unsigned int seconds;
    unsigned int nanoseconds;
    unsigned long total_tick;
} pclock_t;


pclock_t get_copy();
void tick(int);
pclock_t add(pclock_t, unsigned int);
void add_in_place(pclock_t*, unsigned int);
int is_equal_to_sys_clock(pclock_t);


#endif
