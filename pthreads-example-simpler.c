//-------------------------------
// jdh CS201 Spring 2023
// simple example of pthreads
//-------------------------------

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define NUM_THREADS 2
#define NUM_ELEMENTS 1000

void *runner(void *param);

typedef struct {
  int lowVal;
  int highVal;
  int maxVal;
} ThreadInfo;

int A[NUM_ELEMENTS];

//---------------------------------------------------------------------------

void *runner(void *param) {
  ThreadInfo *data;
  int i, maxVal;

  data = (ThreadInfo *) param;
  printf("I am runner; will do max for the range %d to %d\n",
          data->lowVal, data->highVal);

  maxVal = A[data->lowVal];

  for (i=data->lowVal; i<=data->highVal; ++i) {
    if (A[i] > maxVal)
      maxVal = A[i];
  }

  data->maxVal = maxVal;
  printf("my max is %d\n", data->maxVal);
  pthread_exit(NULL);
} // runner()

//---------------------------------------------------------------------------

int main() {
  ThreadInfo tdata[NUM_THREADS]; // holds data we want to give to each thread
  pthread_t tids[NUM_THREADS];    // thread identifier for child thread #1
  int maxVal;
  int i, pos, chunkSize;

  // initialize the array with random integers in the range 0..NUM_ELEMENTS
  for (i=0; i<NUM_ELEMENTS; ++i) {
    A[i] = (int) NUM_ELEMENTS * drand48();
  }

  // set up bounds for the threads
  chunkSize = NUM_ELEMENTS / NUM_THREADS;
  pos = 0;
  for (i=0; i<NUM_THREADS; ++i) {
    tdata[i].lowVal = pos;
    tdata[i].highVal = tdata[i].lowVal + chunkSize;
    pos = pos + chunkSize + 1;
  }

  // adjust last region if necessary
  if (tdata[NUM_THREADS-1].highVal < NUM_ELEMENTS-1)
    tdata[NUM_THREADS-1].highVal = NUM_ELEMENTS - 1;
  else if (tdata[NUM_THREADS-1].highVal > NUM_ELEMENTS-1)
    tdata[NUM_THREADS-1].highVal = NUM_ELEMENTS - 1;

  // create child threads
  for (i=0; i<NUM_THREADS; ++i)
    pthread_create(&tids[i], NULL, runner, &tdata[i]);

  // wait for the child threads to terminate
  for (i=0; i<NUM_THREADS; ++i)
    pthread_join(tids[i], NULL);

  // gather data from the individual results
  maxVal = tdata[0].maxVal;
  for (i=1; i<NUM_THREADS; ++i) {
    if (tdata[i].maxVal > maxVal)
      maxVal = tdata[i].maxVal;
  }

  printf("overall max is %d\n", maxVal);
  return 0;
} // main()
