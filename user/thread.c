#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/spinlock.h"
#include "user/thread.h"
#include "user/user.h"

//wrapper function of clone to allocate a user stack and create a child thread
int thread_create(void *(*start_routine)(void*), void *arg)
{
    int thread_id;        
    int stack_size = 4096*sizeof(void);           //size is PGSIZE 
    void* stack = (void*)malloc(stack_size);      //creates user stack of PGSIZE bytes
    thread_id = clone(stack);                           //creates a new thread, thread_id is returned
    if(thread_id == 0)                                  //check if the reurned value is 0
    {
        (*start_routine)(arg);                    //if child thread is successfuly created then,
        exit(0);                                  //start_routine is called to continue execution of child
    }
    return 0;                                     //returns 0 if child creation is a failure
}

//init function to set the locked vairable as 0 for every thread
void lock_init(struct lock_t *lock)
{   
    lock->locked = 0;       //set locked as 0 initially
}

//function to acquire the lock is locked is 0
//if it is 1 then it'll wait till it becomes 0
//then the lock is acquired and locked is changed to 1
void lock_acquire(struct lock_t *lock)
{
    while(__sync_lock_test_and_set(&lock->locked, 1) != 0);
    __sync_synchronize();
}

//function to release the lock 
void lock_release(struct lock_t *lock)
{
    __sync_synchronize();
    __sync_lock_release(&lock->locked, 0); //locked is set to 0 once lock is released
}