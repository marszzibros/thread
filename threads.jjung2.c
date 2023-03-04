/*
 * Jay Hwasung Jung
 * CS 201 - Operating Systems
 * threads.jjung2.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define BUFLEN 10100
#define LINELEN 256 
#define NUM_THREADS 4

typedef struct {
    char *A;     // the digits themselves--this is a pointer to your big buffer
    int start;   // first position that thread should look at
    int end;     // last position that this thread should look at
    int bestpos; // ending position of the best sequence found by this thread
    int max;     // length of the best sequence found by this thread
} ThreadInfo;

/**
 * readFile 
 * filename
 * numChars
 * buffer
 * return
*/
int readFile(char *filename, int *numChars, char *buffer) {
    int len = 0;
    char *chp;
    char line[LINELEN];

    FILE *fp = fopen(filename, "r"); 

    if (fp == NULL) {
        printf("ERROR: cannot open file '%s'\n", filename);
        return 1;
    }

    chp = fgets(line, LINELEN, fp);
    
    if (chp == NULL) {
        printf ("file is empty\n");
        fclose(fp);
        return 8;
    }

    while (chp != NULL ) {
        len = strlen(line);
        if (line[len - 1] == '\n') {
            line[len - 1] = '\0';
            len = len - 1;
        }
        for (int i = 0; i < len; ++ i) {
            // ignore the decimal point in the input
            // by substracting 48, make ASCII digits to integers
            if (line[i] != '.') buffer[strlen(buffer)] = line[i] - 48;
            
        }
        chp = fgets(line, LINELEN, fp);
    }
    fclose(fp);
}

void *findMaxSumSeq(void *param) {

    ThreadInfo *data;

    int currLen;        // lengh of the sequece
    int seqSum;         // sequence sum so far
    int startInd;       // starting index
    int endInd;         // ending index

    data = (ThreadInfo *)param;

    if (data->start == 0) {
        startInd = 0;
        endInd = data->end + 10;
    }
    else if (strlen(data->A) - 1 == data->end) {
        startInd = data->start - 10;
        endInd = data->end;

    }
    else {
        startInd = data->start - 10;
        endInd = data->end + 10;
    }

    for (int i = startInd + 1; i < endInd; i ++) {
        
        currLen = 0;
        seqSum = 0;

        while(i - currLen >= startInd) {

        }

        for (int j = i - 1; j >= i - currLen; j --) {
            if ()
        }


        // when sequence sum equals to the next number
        if ((int)(data->A[i]) == seqSum) {
            data->bestpos = i;

        }
        else {
            
        }
    }



}
int main (int argc, char *argv[]) {
    int rc, numChars;
    char buffer[BUFLEN];

    if (argc < 2) { 
        printf ("ERROR: need a filename\n");
        return 8;
    }

    rc = readFile (argv[1], &numChars, buffer);
    if (rc != 0) return 8;

    printf("%s\n", buffer);

    ThreadInfo tdata[NUM_THREADS]; 
    pthread_t tids[NUM_THREADS]; 

    // set up bounds for the threads
    int chunkSize = strlen(buffer) / NUM_THREADS;
    int pos = 0;

    for (int i = 0; i < NUM_THREADS; ++i) {
        strcpy(tdata[i].A, buffer);  
        tdata[i].start = pos;
        tdata[i].end = tdata[i].start + chunkSize;
        pos = pos + chunkSize + 1;
    }
    // create child threads
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_create(&[i], NULL, findMaxSumSeq, &tdata[i]);
    }


    return 0;
}