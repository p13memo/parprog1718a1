#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

// compile like:  gcc -Wall -O2 -NROWS=10000 matrix1.c -o matrix1


#define NCOLS 100

void get_walltime(double *wct) {
  struct timeval tp;
  gettimeofday(&tp,NULL);
  *wct = (double)(tp.tv_sec+tp.tv_usec/1000000.0);
}


int main() {
  double *table;
  double *rand1;
  double *rand2;
  double ts, te;
  double mflops, maccesses;
  int i = 0;
  int j = 0;

  table = (double *)malloc(NROWS*NCOLS*sizeof(double)); 
  rand1 = (double *)malloc(NROWS*NCOLS*sizeof(double)); 
  rand2 = (double *)malloc(NROWS*NCOLS*sizeof(double)); 
  
  if ( (table == NULL) || (rand1 == NULL) || (rand2 == NULL )) {
    printf("alloc error!\n");
    exit(1);
  }
  
  // Set seed to get really random numbers
  srand((unsigned)time(NULL));

  // warmup
  for (i=0;i<NROWS;i++) {
      for (j=0;j<NCOLS;j++){
          table[i*NCOLS+j]=0.0;
          rand1[i*NCOLS+j]=rand()%5 + 1.0;
          rand2[i*NCOLS+j]=rand()%80;
      }
  }

  // get starting time (double, seconds) 
  get_walltime(&ts);
  
  // workload
  for (i=0;i<NROWS;i++) {
      for (j=0;j<NCOLS;j++){
          table[i*NCOLS+j] = rand1[i*NCOLS+j] + rand2[i*NCOLS+j]/rand1[i*NCOLS+j];
      }
  }

  // get ending time
  get_walltime(&te);

  // check results
  printf("Πλήθος γραμμών: %d\n",NROWS);
  printf("Τελευταίο στοιχείο του πίνακα rand1: %f\n", rand1[NROWS*NCOLS - 1]);
  printf("Τελευταίο στοιχείο του πίνακα rand2: %f\n", rand2[NROWS*NCOLS - 1]);
  printf("Τελευταίο στοιχείο του πίνακα table: %f\n", table[NROWS*NCOLS - 1]);

  // print time elapsed and/or Maccesses/sec
  mflops = (NROWS*NCOLS*2.0)/((te-ts)*1e6);
  maccesses = (NROWS*NCOLS*4.0)/((te-ts)*1e6);
  printf("Time elapsed: %f\n",te-ts);
  printf("Mflops/sec: %f\n",mflops);
  printf("Maccesses/sec: %f\n",maccesses);
  
  free(table);
  free(rand1);
  free(rand2);

  return 0;
}

