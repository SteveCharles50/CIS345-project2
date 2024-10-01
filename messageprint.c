#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>


//mutex var creation not init
pthread_mutex_t mutex;


//routine for thread creation
void* routine(void* arg){
    //getting threadID will work off of this to process composite and prime threadID
    printf("Thread id: %lu\n", (unsigned long)pthread_self());
};


int main(int argc, char* argv[]){
    //variable to track thread
    int i;

    //mutex initialization
    pthread_mutex_init(&mutex, NULL);

    //prompting and reading integer from console
    printf("Enter desired number of threads:\n");
    scanf("%d", &i);

    //creating threads
    pthread_t thread[i];
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
    pthread_mutex_destroy(&mutex);
    return 0;
}

