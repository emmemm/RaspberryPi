// -*- c++ -*-

#include <sys/time.h>
#include <stdio.h>

#define SAMPLES 10000
struct timeval tv[SAMPLES];


void print_samples(const char* sz, int nSamples)
{
  int iStep = 1;
  int i = 1;
  int iLast;
  while (i <= nSamples) {
    printf("%5d. %d.%06d", i, tv[i-1].tv_sec, tv[i-1].tv_usec);
    if (i == 1)
      printf("    %s\n", sz);
    else
      printf(" (%d usec)\n", (tv[i-1].tv_usec-tv[iLast-1].tv_usec) + (tv[i-1].tv_sec-tv[iLast-1].tv_sec)*1000000);
    iLast = i;
    if ((i/iStep) % 10 == 0)
      iStep *= 10;
    i += iStep;
  }
}


int main(int argc, char *argv[])
{
  int i,j,k;
  struct timeval tvDummy;

  gettimeofday(&tv[0], NULL);
  for (i = 0; i < SAMPLES; ++i) {
  }
  gettimeofday(&tv[1], NULL);
  print_samples("nothing", 2);

  for (i = 0; i < SAMPLES; ++i) {
    gettimeofday(&tv[i], NULL);
  }
  print_samples("gettimeofday", SAMPLES);

  for (i = 0; i < SAMPLES; ++i) {
    gettimeofday(&tv[i], NULL);
    //    for (j = 0; j < 1000; ++j)
    gettimeofday(&tvDummy, NULL);
  }
  print_samples("gettimeofday extra", SAMPLES);

  return 0;
}
