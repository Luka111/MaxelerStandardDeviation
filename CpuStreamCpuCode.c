#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "Maxfiles.h"
#include <MaxSLiCInterface.h>

void generateData(int size, float *dataIn)
{
	for (int i = 0; i < size; i++)
		dataIn[i]  = (random() % 100) / 100.0 * 100;
}

float avgN(float *data, int size){
    float ret=0;
    int i;
    for (i=0; i<size; i++){
        ret += data[i];
    }
    return ret/size;
}

float stDev(float *data, float avg, int size){
    float ret=0;
    int i=0;
    for (i=0; i<size; i++){
        ret += pow((data[i] - avg),2);
    }
    return sqrt(ret/size);
}

int main()
{
	const int size = 1000000;
	int sizeBytes = size * sizeof(float);
	float *dataIn   = malloc(sizeBytes);
	float *dataOut  = malloc(sizeBytes);
	float stdev_cpu, stdev_max;

	generateData(size, dataIn);

	float scalarIn  = avgN(dataIn,size);

	printf("Running on CPU\n");

	clock_t start = clock();

	stdev_cpu = stDev(dataIn,scalarIn,size);

	clock_t end = clock();
	float timeInMillis = (float)(end-start)/(CLOCKS_PER_SEC/1000);

	printf("CPU ST.DEV: %f\n",stdev_cpu);

	printf("Running on MAXELER.\n");

	start = clock();

	CpuStream(size, scalarIn, dataIn, dataOut);

	stdev_max = sqrt(avgN(dataOut,size));

	end = clock();

	timeInMillis = (float)(end-start)/(CLOCKS_PER_SEC/1000);

	printf("MAXELER ST.DEV: %f\n",stdev_max);

	if (abs(stdev_max - stdev_cpu) > 0.01)
		printf("Test failed.\n");
	else
		printf("Test passed OK!\n");

	return 0;
}

