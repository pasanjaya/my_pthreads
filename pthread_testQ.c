/* 
You are a virologist working in a large laboratory. For the last 40 days, you have been analyzing the growth rate of the virus, ebolavirus in primate victims.

Today is the last day of your experimentation. With anticipation, you pull your log book from the shelf, but in your excitement, you knock a bottle of acid from the bench. You watch in despair as it spills across your log book and your precious notes dissolve before your eyes. In desperation, you try to remember any statistic regarding the experiment and recalls that the initial number of virus element was an odd number and the viruses doubled in number each day.

In order to compile a report of your findings, you need two important pieces of information which is the initial count of virus elements and the number of days the experiment was running.

Write a multi-threading program (2 threads) using Java or C to find the total number of virus count if the initial virus count is 1 and the number of days the experiment was running is 40 days.

*/

#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 2
#define ARRAY_SIZE 40

unsigned long virusArray[ARRAY_SIZE];  
unsigned long count = 0; 
unsigned long temp = 0;
int wsize; // working siZe of a thread
pthread_mutex_t mutex; // mutually exclution

void* counter(void* tid){
  int start, end;
  unsigned long tempCount =0;

  start = *((int*)tid) * wsize;
  end = start + wsize;

  for(int i =start; i< end; i++){
    tempCount += virusArray[i];
  }

  pthread_mutex_lock(&mutex); // lock the critical section
  count += tempCount;
  pthread_mutex_unlock(&mutex); // unlock the critical section
  
  return NULL;
}


int main(void) {
  wsize= ARRAY_SIZE/NUM_THREADS; // initialize work size;

  pthread_t threads[NUM_THREADS]; // initilize threads array

  pthread_mutex_init(&mutex, NULL); //initialize mutex

  // populate viraus array
  virusArray[0] = 1; 
  for(int i = 1; i < ARRAY_SIZE; i++){
    virusArray[i] = virusArray[i-1]*2;
  }


  int tidArray[NUM_THREADS]; // create a local thread id array

  // create thread one by one
  for(int i = 0; i<NUM_THREADS; i++){
    tidArray[i] = i; //insert thread id into local thread array
    int tc = pthread_create(&threads[i], NULL, counter, &tidArray[i]);
    if(tc) perror("Error when creating\n");
  }

  //join and wait until thread completion
  for(int i = 0; i<NUM_THREADS; i++){
    int tj = pthread_join(threads[i], NULL);
    if(tj) perror("Error when joinig\n");
  }

  // print population array
  for(int i = 0; i<ARRAY_SIZE; i++){
    printf("day %d virus %ld\n", i+1, virusArray[i]);
  }
  printf("\n");

  printf("Total count of viruses: %ld\n", count);

  return 0;
}
