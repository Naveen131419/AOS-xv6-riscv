#include "kernel/types.h"
typedef struct lock_t {                                         //declaring struct lock_t
uint locked; 
}lock_t; 
int thread_create(void *(*start_routine)(void*), void *arg);    //
void lock_init(lock_t *lock);                                   //declaring function to set the locked variable as 0 initially
void lock_acquire(lock_t *);                                    //declaring fucntion to acquire the lock
void lock_release(lock_t *);                                    //declaring function to release the lock