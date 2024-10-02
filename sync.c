/*
 * Michael Hylton.c
 *
 * sync.c
 *
 * Synchronization routines for SThread
 */

#define _REENTRANT

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sched.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <pthread.h>
#include "sthread.h"
#include "sync.h"

/*
 * Atomic operations for x86 architecture.
 */
static inline int test_and_set_bit(volatile unsigned long *addr)
{
	int oldval;
	__asm__ __volatile__("xchgl %0, %1"
			: "=r"(oldval), "+m"(*(addr))	/* output */
			: "0"(1)						/* input */
			: "memory"	/* clobbered: changing contents of memory */
			);
	return oldval;
}
static inline void clear_bit(volatile unsigned long *addr)
{
	unsigned long oldval;
	__asm__ __volatile__("xchgl %0, %1"
			: "=r"(oldval), "+m"(*(addr))	/* output */
			: "0"(0)						/* input */
			: "memory"	/* clobbered: changing contents of memory */
			);
}

/*
 * rwlock routines
 */

int sthread_rwlock_init(sthread_rwlock_t *rwlock)
{
        printf("\nInitiate rwlock\n");
        rwlock->numOfThreads=0;
        pthread_mutex_init(&rwlock->mutex_lock, NULL); 
        pthread_cond_init(&rwlock->readCond, NULL);
        pthread_cond_init(&rwlock->writeCond, NULL);
        return 0;
}

int sthread_rwlock_destroy(sthread_rwlock_t *rwlock)
{
    //reset all variables to their original conditions.
    printf("\nDestroy rwlock\n");
    
    pthread_mutex_destroy(&rwlock->mutex_lock); 
    pthread_cond_destroy(&rwlock->readCond);
    pthread_cond_destroy(&rwlock->writeCond);
    return 0;
}

int sthread_read_lock(sthread_rwlock_t *rwlock)
{
        //keeps the thread lock while writing
        printf("\nread lock\nnumOfThreads=%d", rwlock->numOfThreads);;
        pthread_mutex_lock(&rwlock->mutex_lock);
        while(rwlock->numOfThreads>0){
          // lock based off read condition.
          pthread_mutex_unlock(&rwlock->mutex_lock);
          sched_yield();
          pthread_mutex_lock(&rwlock->mutex_lock);
        }
        rwlock->numOfThreads++; 
        pthread_mutex_unlock(&rwlock->mutex_lock);
        return 0; // returns after the mutex is unlocked.

}

int sthread_read_try_lock(sthread_rwlock_t *rwlock)
{
      printf("\nread try lock\nnumOfThreads=%d", rwlock->numOfThreads);
      pthread_mutex_lock(&rwlock->mutex_lock); //locks the thread while checking.
      if (rwlock->numOfThreads == 0) {
          rwlock->numOfThreads++;
          pthread_mutex_unlock(&rwlock->mutex_lock); //unlocks the thread after check.
          return 0;
      }
      pthread_mutex_unlock(&rwlock->mutex_lock);//unlocks the thread after check.
      return -1; 
      
}

int sthread_read_unlock(sthread_rwlock_t *rwlock)
{
        printf("\nread unlock\nnumOfThreads=%d", rwlock->numOfThreads);
        pthread_mutex_lock(&rwlock->mutex_lock); //locks the thread while checking.
        rwlock->numOfThreads--;
        //if no readers exist after decrementing, wake up the next queued writers
        if (rwlock->numOfThreads == 0) {
            pthread_cond_signal(&rwlock->writeCond); 
        }
        pthread_mutex_unlock(&rwlock->mutex_lock); //unlocks the thread.
        return 0;
}

int sthread_write_lock(sthread_rwlock_t *rwlock)
{
        
        printf("\nwrite lock\nnumOfThreads=%d", rwlock->numOfThreads);
        pthread_mutex_lock(&rwlock->mutex_lock); //lock the thread while checking
        while (rwlock->numOfThreads > 0) {
          pthread_mutex_unlock(&rwlock->mutex_lock);
          sched_yield();  // Yield to allow other threads to run
          pthread_mutex_lock(&rwlock->mutex_lock);
        }
        rwlock->numOfThreads++; //increment the writers count
        pthread_mutex_unlock(&rwlock->mutex_lock); //unlocks the thread.
        return 0;
        
}

int sthread_write_try_lock(sthread_rwlock_t *rwlock)
{
        printf("\nwrite try lock\nnumOfThreads=%d", rwlock->numOfThreads);
        pthread_mutex_lock(&rwlock->mutex_lock); //lock the thread while locking
        if (rwlock->numOfThreads == 0) {
            rwlock->numOfThreads++; 
            pthread_mutex_unlock(&rwlock->mutex_lock); //unlocks the thread upon completion
            return 0; 
        }
        pthread_mutex_unlock(&rwlock->mutex_lock); //unlocks the thread upon completion
        return -1;
        
}

int sthread_write_unlock(sthread_rwlock_t *rwlock)
{
        printf("\nwrite unlock\nnumOfThreads=%d", rwlock->numOfThreads);
        pthread_mutex_lock(&rwlock->mutex_lock);
        rwlock->numOfThreads--;
        //if no readers exist after decrementing, wakeup the next queued writers or queued reader
        if (rwlock->numOfThreads == 0) {
            pthread_cond_broadcast(&rwlock->readCond); // Wake up all readers first before waking up next writer
            pthread_cond_signal(&rwlock->writeCond); // Wake up a writer
        }
        pthread_mutex_unlock(&rwlock->mutex_lock); //unlock the thread.
        return 0;
        
}

