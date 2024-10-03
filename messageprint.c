#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>


//mutex var creation not init
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t CV = PTHREAD_COND_INITIALIZER;
int number_prime_finished = 0;

//tracker to keep track of where we are with total number of threads in execution
int tracker = 0;

//tracks total num of threads
int numOfThreads;

//keeps track of the prime thread--> 0 means composite 1 means prime
int prime = 0;

//function used to check if the number is prime or composite
int isPrime(int num){
    //assumes number is not prime
    int checkPrime = 0;
    if(num <= 1){
        return 0;
    }
    for(int b = 2; b < num; b++){
        if(num % b == 0){
            checkPrime = 0;
            break;
        }
        else{
            checkPrime = 1;
        };
    }
    return checkPrime;
}


//routine for thread creation
void* routine(void* arg){
    pthread_mutex_lock(&lock);
    printf("This is a new thread processing\n");
    printf("Here is the tracker val: %d\n", tracker);
    //calls function to check if thread is prime or composite
    unsigned long tid = (unsigned long)pthread_self();
    prime = isPrime((int)tid);
    printf("prime val: %d\n", prime);
    if(prime == 1){
        printf("Here is the prime: %d\n", prime);

    }
    //getting threadID will work off of this to process composite and prime threadID
    if(prime == 0 && tracker == numOfThreads){
        tracker++;
        pthread_mutex_unlock(&lock);
        pthread_cond_broadcast(&CV);
    }
    else if(prime == 0){
        printf("Here is the thread ID: %lu \n", (long unsigned) pthread_self());
        tracker++;
        pthread_mutex_unlock(&lock);
    }
    else{
        while(number_prime_finished == 0){
            printf("ThreadID in the else block: %lu\n", (long unsigned) pthread_self());
            tracker++;
            printf("Executing Prime threads first...\n");
            pthread_cond_wait(&CV, &lock);
            pthread_mutex_unlock(&lock);
        }
    }
    return 0;
}



int main(int argc, char* argv[]){
    //variable to track thread
    int i;

    //prompting and reading integer from console
    printf("Enter desired number of threads:\n");
    scanf("%d", &i);

    //creating threads
    pthread_t thread[i];

    numOfThreads = i-1;

    for(int x = 0; x < i; x++){
        if(pthread_create((&thread[x]), NULL, &routine, NULL) != 0){
            return 1;
        }
    }
    for(int x = 0; x < i; x++){
        if(pthread_join(thread[x], NULL) != 0){
            return 2;
        }
    }
    //destroying mutex
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&CV);
    printf("tracker: %d\n", tracker);
    return 0;
}


