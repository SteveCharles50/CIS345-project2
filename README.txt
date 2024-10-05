Steve Charles 2850400 50%
Michael Hylton 2788154 50%


part 1:


part 2: 

Design ideaas:
  -The program prompts the user to input the number of threads they want to process. The method takes any amount of threads 
  -The idea of the program is to accept user input and pass each thread a thread id that is assinged through the for loop. 
  -These threads all call routine where a isPrime function checks if the number is either prime or composite. 
  -Once this is determined the the threads execute through a series of for loops determining when the prime threads are ready for execution
  -Once all prime threads have executed all composite threads will execute and the program will terminate.

Code Logic
  -pthreads library contains functions such as mutex lock and unlock as well as wait and broadcast function. 
  -The lock is set at the beginning of the if statements and waits for one of the if statements to process before unlocking and allowing another thread to edit the global tracker variable. This functionality is used to prevent race conditions
  -The tracker variable is used to track how many threads have been processed and allows for the program to determine when to start executing the composite threads
  -the pthread_cond_wait(&CV, &lock); is used to stop composite threads from executing as the point of the program is to process all prime threads first. This function puts the thread to sleeps and holds it in the current state and waits for pthread_cond_broadcast(&CV); to signal the threads to pick up and execute. 
  -Once these composite threads have executed the program will terminate.


Compilation instructions:
  -The makefile for part 2 is name Makefile1 to avoid confusion with the makefile in part 1. In order to compile the program all you need to type in your shell is the statement make1.
  -Once you type make1 you will be prompted with a statement like this:

                -$Enter desired number of threads:
                  20 <----- This is where you are prompted to enter a number. Once you have entered the desired number make sure to press enter.
  -OUTPUT: 
waiting for all prime threads to process. Thread ID: 0
waiting for all prime threads to process. Thread ID: 1
This is a new thread processing, threadID: 2
This is a new thread processing, threadID: 3
waiting for all prime threads to process. Thread ID: 4
This is a new thread processing, threadID: 5
waiting for all prime threads to process. Thread ID: 6
This is a new thread processing, threadID: 7
waiting for all prime threads to process. Thread ID: 8
waiting for all prime threads to process. Thread ID: 9
waiting for all prime threads to process. Thread ID: 10
This is a new thread processing, threadID: 11
waiting for all prime threads to process. Thread ID: 12
This is a new thread processing, threadID: 13
waiting for all prime threads to process. Thread ID: 14
waiting for all prime threads to process. Thread ID: 15
waiting for all prime threads to process. Thread ID: 16
This is a new thread processing, threadID: 17
waiting for all prime threads to process. Thread ID: 18
This is a new thread processing, threadID: 19
Thread has been broadcasted, here is the composite thread being processed: 1
Thread has been broadcasted, here is the composite thread being processed: 8
Thread has been broadcasted, here is the composite thread being processed: 12
Thread has been broadcasted, here is the composite thread being processed: 15
Thread has been broadcasted, here is the composite thread being processed: 6
Thread has been broadcasted, here is the composite thread being processed: 10
Thread has been broadcasted, here is the composite thread being processed: 0
Thread has been broadcasted, here is the composite thread being processed: 16
Thread has been broadcasted, here is the composite thread being processed: 18
Thread has been broadcasted, here is the composite thread being processed: 14
Thread has been broadcasted, here is the composite thread being processed: 4
Thread has been broadcasted, here is the composite thread being processed: 9
Threads executed: 20

-Each thread is producing at max 2 lines of out put. Most are just giving one. 



