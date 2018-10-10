/*

Fill an array using by assigning random values 1 - 10 using threads

*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define ARRAY_SIZE 50
#define NUM_THREADS 2
int randomizer();

int array[ARRAY_SIZE];
int wsize;
pthread_mutex_t mutex;

int randomizer(){
    int r = rand()%10 + 1; // create a random value 1 - 10;
    return r;
}

void* insert(void* tid){
    int start, end;

    start = *((int*)tid) * wsize;
    end = start + wsize;

    
    for(int i = start; i < end; i++){
        
        pthread_mutex_lock(&mutex);
        int r = randomizer();
        printf("tid: %d randomVal: %d ", *((int*)tid), r);
        array[i] = r;
        pthread_mutex_unlock(&mutex);
    }
    
    printf("\n");
}


int main(int argc, char const *argv[])
{
    wsize = ARRAY_SIZE/NUM_THREADS;

    pthread_t threads[NUM_THREADS];
    pthread_mutex_init(&mutex, NULL);

    int tIdNum[NUM_THREADS];

    for(int i = 0; i < NUM_THREADS; i++){
        tIdNum[i] = i;
        int tc = pthread_create(&threads[i], NULL, insert, &tIdNum[i]);
        if(tc) perror("Error in creation\n");
    }

    for(int i = 0; i < NUM_THREADS; i++){
        int tj = pthread_join(threads[i], NULL);
        if(tj) perror("Error in creation\n");
    }

    int *p;
    p = array;
    while(p != array+ARRAY_SIZE){
        printf("%d ", *p);
        p++;
    }
    printf("\n");



    return 0;
}
