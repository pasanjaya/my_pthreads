#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BUF_SIZE 3


int buffer[BUF_SIZE];  

int addPoint = 0;
int remPoint = 0;
int count = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c_cons = PTHREAD_COND_INITIALIZER;


void* producer(void* param);
void* consumer(void* param);

int main()
{   
    pthread_t tid1, tid2;
    
    int tc = pthread_create(&tid1, NULL, producer, NULL);
    if(tc !=0){
        fprintf(stderr, "Unable to create\n");
        exit(1);
    }

    int tc1 = pthread_create(&tid2, NULL, consumer, NULL);
    if(tc1 != 0){
        fprintf(stderr, "Unable to create\n");
        exit(1);
    }

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);


    return 0;
}

void* producer(void*param){
    printf("im pro\n");
    for(int i = 0; i<=20; i++){
        pthread_mutex_lock(&mutex);
        if(count > BUF_SIZE) exit(1); // overflow
        if(count == BUF_SIZE){
            pthread_cond_wait(&c_cons, &mutex);
        }
        buffer[addPoint] = i;
        addPoint = (addPoint+1) %BUF_SIZE;
        count++;
        printf("buffer filled with %d\n", i); fflush(stdout);
        pthread_mutex_unlock(&mutex);
        if(count==1){
            pthread_cond_signal(&c_cons);
        }
    }
    printf("------Finished Producing-------\n");
}

void* consumer(void* param){
    printf("im con\n");
    int i;
    while(1){
        pthread_mutex_lock(&mutex);
        if(count < 0) exit(1); //downflow
        if( count == 0){
            pthread_cond_wait(&c_cons, &mutex);
        }
        i = buffer[remPoint];
        remPoint = (remPoint+1)%BUF_SIZE;
        count--;
        printf("Consumer got %d\n", i);
        fflush(stdout);
        pthread_mutex_unlock(&mutex);
        if(count==BUF_SIZE-1){
            pthread_cond_signal(&c_cons);
        }
    }
    
}
