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
 * filename - specified filename by users
 * numChars - number of elements in array
 * buffer   - array
 * return   - return 0 when everything went okay
*/
int readFile(char *filename, int *numChars, char *buffer) {
    int len = 0;
    char *chp;
    char line[LINELEN];
    int count = 0;    
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
            if (line[i] != '.') {
                buffer[count] = line[i] - 48;
                count ++;
            }
        }
        chp = fgets(line, LINELEN, fp);
    }   
    fclose(fp);
    *numChars = count;
    return 0;
}
/**
 * findMaxSumSeq 
*/
void *findMaxSumSeq(void *param) {

    ThreadInfo *data;

    int currLen;        // lengh of the sequece
    int seqSum;         // sequence sum so far
    int startInd;       // starting index
    int endInd;         // ending index

    data = (ThreadInfo *)param;

    startInd = data->start;
    endInd = data->end;

    for (int i = startInd + 1; i < endInd; i ++) {
        currLen = 1;
        seqSum = data->A[i - 1];

        // when it reaches the start index, stop
        while(i - currLen >= startInd) {

            currLen ++;
            seqSum += (int)(data->A[i - currLen]);


            // if the sequence sum exceeds the last digit
            if ((seqSum > data->A[i])) {
                break;
            }

            // if it is the first record, store it
            // if it is a max length, replace it
            else if (data->max < currLen + 1 && seqSum == data->A[i]) {
                data->max = currLen + 1;
                data->bestpos = i;
                break;
            }
        }
    }
}
int main (int argc, char *argv[]) {
    
    int rc, numChars;
    char buffer[BUFLEN];

    // first row - position
    // second row - length
    int result[2][NUM_THREADS] = 
    {{-1, -1, -1, -1},
     {-1, -1, -1, -1}};
    int count = 0; 

    if (argc < 2) { 
        printf ("ERROR: need a filename\n");
        return 8;
    }

    // read file
    rc = readFile (argv[1], &numChars, buffer);
    if (rc != 0) return 8;

    ThreadInfo tdata[NUM_THREADS]; 
    pthread_t tids[NUM_THREADS]; 


    // set up bounds for the threads

    int chunkSize = numChars / NUM_THREADS;
    int pos = 0;

    // +10 and -10 to the start index except for special circumstances (first and last thread)
    for (int i = 0; i < NUM_THREADS; ++i) {
        tdata[i].A = buffer;  
        tdata[i].start = pos;
        tdata[i].end = tdata[i].start + chunkSize;
        if(i == 0) tdata[i].end += 10;
        else if(i == NUM_THREADS - 1) tdata[i].start -= 10;
        else {
            tdata[i].start -= 10;
            tdata[i].end += 10;
        }
        pos = pos + chunkSize + 1;
    }
    //create child threads
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_create(&tids[i], NULL, findMaxSumSeq, &tdata[i]);
    }

    // wait for the child threads to terminate
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(tids[i], NULL);
    }

    int j;
    int max = -1;

    // gather data from the individual results
    for (int i = 0; i < NUM_THREADS; ++i) {
        j = 0;
        while (j < i) { 
            if(tdata[j].bestpos == tdata[i].bestpos) break;
            j ++;
        }
        // find max and filter duplicate values
        if (j == i) {
            result[0][count] = tdata[i].bestpos;
            result[1][count] = tdata[i].max;
            if (max < result[1][count]) max = result[1][count];
            count ++;

        }
    }

    // results print
    printf("Max Length is %d\n\n", max);
    for (int i = 0; i < NUM_THREADS; ++i) {
        if  (result [0][i] != -1 && result [1][i] == max) {
            printf("In position, %d, there is a sequence ",result [0][i]);
            for(j = 0; j < result [1][i]; j ++){
                printf("%d", buffer[result [0][i] - result [1][i] + j + 1]);
            }
            printf("\n");
        }
    }

    return 0;
}