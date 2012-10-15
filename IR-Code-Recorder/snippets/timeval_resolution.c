// -*- c -*-

#include <sys/time.h>
#include <stdio.h>

const int SAMPLES = 60;
int main()
{
  int i,j,k;
  struct timeval tv[SAMPLES];
  struct timeval tvDummy;
  for (i = 0; i < SAMPLES; ++i) {
    gettimeofday(&tv[i], NULL);
    for (j = 0; j < 1000; ++j)
      gettimeofday(&tvDummy, NULL);
  }
  for (i = 0; i < SAMPLES; ++i) {
    printf("%2d. %d.%06d", i, tv[i].tv_sec, tv[i].tv_usec);
    if (i == 0)
      printf("\n");
    else
      printf("(%d usec)\n", (tv[i].tv_usec-tv[i-1].tv_usec) + (tv[i].tv_sec-tv[i-1].tv_sec)*1000000);
  }
  return 0;
}
