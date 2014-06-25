#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/time.h>
#include "CLapWatch.h"
#include <wiringPi.h>

static volatile int nRising = 0;
static volatile int nFalling = 0;
static int nMin500 = 799;
static int nMax500 = 200;

CLapWatch oLapWatch(10000);
bool bRising[10000];

//struct timeval tv0;
//struct timeval tv1;
//int nStatus = 0;

void fPINFalling(void)
{
//  nStatus = 1;
  oLapWatch.RecordLap();
//  gettimeofday(&tv1, NULL);
//  unsigned long n = (tv1.tv_usec-tv0.tv_usec) + (tv1.tv_sec-tv0.tv_sec)*1000000;
//  if (n > 200 && n < 799 && n < nMin500)
//    nMin500 = n;
//  if (n > 200 && n < 799 && n > nMax500)
//    nMax500 = n;
//  printf("%ld ", n);
//  if (digitalRead(3))
//    ++nRising;
//  else
//    ++nFalling;
//  tv0 = tv1;
//  nStatus = 0;
}


int main (void)
{
  wiringPiSetup () ;
  unsigned int nSamples = 0;
  unsigned int nSamplesLast = 0;
  unsigned int nCountConstantDuringLoops = 0;
  wiringPiISR (3, INT_EDGE_FALLING, &fPINFalling) ;
  for (;;) {
    delay(100);
    unsigned int nSamples = oLapWatch.GetSamples();
    if (nSamples > 0) {
      if (nSamples == nSamplesLast) {
        if (++nCountConstantDuringLoops > 3) {
          oLapWatch.WriteSamples();
          printf("\n");
          fflush(stdout);
          nSamples = 0;
          nSamplesLast = 0;
          nCountConstantDuringLoops = 0;
          oLapWatch.Reset();
        }
      }
      else {
        nSamplesLast = nSamples;
        nCountConstantDuringLoops = 0;
      }
    }
  }
  return 0 ;
}
