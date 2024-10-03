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
    int ptid = *((int*)arg);
    //casting manually assigned threadID number ot thread number
    pthread_mutex_lock(&lock);
    printf("This is a new thread processing\n");
    printf("ptid: %d\n", ptid);
    //calls function to check if thread is prime or composite
    prime = isPrime(ptid);
    printf("prime val: %d\n", prime);
    if(prime == 1){
        printf("Executing Prime threads first...\n");
    }
    //getting threadID will work off of this to process composite and prime threadID
    if(prime == 0 && tracker == numOfThreads){
        tracker++;
        printf("executing thread: %d\n", ptid);
        pthread_mutex_unlock(&lock);
        pthread_cond_broadcast(&CV);
    }
    else if(prime == 0){
        tracker++;
        printf("executing thread: %d\n", ptid);
        pthread_mutex_unlock(&lock);
    }
    else{
        while(number_prime_finished == 0){
            tracker++;
            pthread_cond_wait(&CV, &lock);
            printf("Thread is has been broadcasted, here is the prime thread ID: %d\n", ptid);
            pthread_mutex_unlock(&lock);
            return 0;
        }
    }
    return NULL;
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
    printf("Here is the last threadID in execution: %d\n", numOfThreads);


    for(int x = 0; x < i; x++){
        if(pthread_create((&thread[x]), NULL, &routine, &x) != 0){
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


