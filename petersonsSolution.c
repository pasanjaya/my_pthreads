#include <stdio.h>
#include <pthread.h>


#define TRUE 1
#define FALSE 0
#define NUM_THREAD 2

void* runningThreads(void* tId);
void leave_region(int processId);
void enter_region(int processId);
void criticalSection(int processId);

int turn;
int interested[NUM_THREAD];

void* runningThreads(void* tId){
    int processId = *((int*)tId);
    enter_region(processId);
    return NULL;
}

void enter_region(int processId){
    
    int otherProcess;
    otherProcess = 1 - processId;
    interested[processId] = TRUE;
    turn = processId;
    while(turn == processId && interested[otherProcess] == TRUE);
    criticalSection(processId);
    leave_region(processId);
}

void leave_region(int processId){
    interested[processId] = FALSE;
}

void criticalSection(int processId){
    for(int i = 0; i<10; i++)
    printf("Process: %d value %d\n", processId, i);
}

int main(int argc, char const *argv[])
{
    pthread_t threads[NUM_THREAD];
    int threadId[NUM_THREAD];

    for(int i = 0; i<NUM_THREAD; i++){
        threadId[i] = i;
        int tc = pthread_create(&threads[i], NULL, runningThreads, &threadId[i]);
        if(tc != 0) perror("thread creating error\n");
    }

    for(int i = 0; i<NUM_THREAD; i++){
        pthread_join(threads[i], NULL);
    }

    return 0;
}
