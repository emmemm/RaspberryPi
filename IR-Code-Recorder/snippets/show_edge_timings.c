//#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <bcm2835.h>

#define PIN_15 RPI_GPIO_P1_15

const int MICROSECS_NEW_COMMAND = 100000;

#define SAMPLES 10000
struct timeval tv[SAMPLES];

int main(int argc, char **argv)
{
  struct timeval tvCurrent;
  int nWaitingForEdge = 0;  // 1: waiting for 01, 0: waiting for 10
  int nEdgesDetected = 0;

  if (!bcm2835_init())
    return 1;

  printf("init Level of Pin 15: %d\n", bcm2835_gpio_lev(PIN_15)); 
  printf("all timings in microseconds\n"); 
  while (1) {
    if (nEdgesDetected) {
      gettimeofday(&tvCurrent, NULL);
      int nMicroSecs = (tvCurrent.tv_usec-tv[nEdgesDetected-1].tv_usec) + (tvCurrent.tv_sec-tv[nEdgesDetected-1].tv_sec)*1000000;
      if (nMicroSecs >= MICROSECS_NEW_COMMAND) {
        // Da kommt nix mehr...
        // Ausgabe der letzten Aufzeichnung
        int i;
        for (i = 0; i < nEdgesDetected; ++i) {
          if (i == 0)
            nMicroSecs = 0;
          else
            nMicroSecs = (tv[i].tv_usec-tv[i-1].tv_usec) + (tv[i].tv_sec-tv[i-1].tv_sec)*1000000;
          if (i % 2 == 0)
            printf("___/ %2d. %5d   ~~~~"   , i+1, nMicroSecs);
          else
            printf("\\___ %2d. %5d\n", i+1, nMicroSecs);
        }
        nEdgesDetected = 0;
      }
    }
    if (bcm2835_gpio_lev(PIN_15) == nWaitingForEdge) {
      gettimeofday(&tv[nEdgesDetected], NULL);
      ++nEdgesDetected;
      nWaitingForEdge = (nWaitingForEdge == 1) ? 0 : 1;
    }
  }

  return 0;
}
