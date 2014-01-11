#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/time.h>
#include <..\timing\CRecorder.h>
#include <wiringPi.h>


// globalCounter:
//	Global variable to count interrupts
//	Should be declared volatile to make sure the compiler doesn't cache it.

static volatile int nRising = 0;
static volatile int nFalling = 0;
static int nMin500 = 799;
static int nMax500 = 200;

// *********************************************************************************

struct timeval tv0;
struct timeval tv1;
int nStatus = 0;

// *********************************************************************************
class CLapWatch
{
  unsigned int nMaxSamples_;
  unsigned int nSamples_;
  struct timeval* pa_timeval_;
public:
  CLapWatch(int nSamples);
  ~CLapWatch();
  void Lap();
};

CLapWatch::CLapWatch(int nSamples)
{
  nMaxSamples_ = nMaxSamples;
  nSamples_ = 0;
  pa_timeval_ = new timeval[nMaxSamples];
}

CLapWatch::~CLapWatch()
{
  delete pa_timeval_;
}

void CLapWatch::Lap()
{
  if (nSample_ < nMaxSamples_)
    gettimeofday(pa_timeval_[nSample_++], NULL);
}
// *********************************************************************************

void fPINToggle(void)
{
  nStatus = 1;
  gettimeofday(&tv1, NULL);
  unsigned long n = (tv1.tv_usec-tv0.tv_usec) + (tv1.tv_sec-tv0.tv_sec)*1000000;
  if (n > 200 && n < 799 && n < nMin500)
    nMin500 = n;
  if (n > 200 && n < 799 && n > nMax500)
    nMax500 = n;
  printf("%ld ", n);
  if (digitalRead(3))
    ++nRising;
  else
    ++nFalling;
  tv0 = tv1;
  nStatus = 0;
}

CLapWatch oLapWatchFalling(10000);
CLapWatch oLapWatchRising(10000);

void fRising(void);

void fFalling(void)
{
  if (nStatus)
    printf("\\!!! ");
  nStatus = 1;
  gettimeofday(&tv1, NULL);
  unsigned long n = (tv1.tv_usec-tv0.tv_usec) + (tv1.tv_sec-tv0.tv_sec)*1000000;
  tv0 = tv1;
  printf("\\%lu ", n);
  nStatus = 0;
  wiringPiISR (3, INT_EDGE_RISING, &fRising) ;
}

void fRising(void)
{
  if (nStatus)
    printf("\\!!! ");
  nStatus = 1;
  gettimeofday(&tv1, NULL);
  unsigned long n = (tv1.tv_usec-tv0.tv_usec) + (tv1.tv_sec-tv0.tv_sec)*1000000;
  tv0 = tv1;
  printf("/%lu ", n);
  nStatus = 0;
  wiringPiISR (3, INT_EDGE_FALLING, &fFalling) ;
}


/*
 *********************************************************************************
 * main
 *********************************************************************************
 */

int main (void)
{
  wiringPiSetup () ;
//  wiringPiISR (3, INT_EDGE_BOTH, &fPINToggle) ;
  wiringPiISR (3, INT_EDGE_FALLING, &fFalling) ;
  for (;;) {
    delay(1000);
//    printf("__/ %d \\__ %d (%d...%d)    ", nRising, nFalling, nMin500, nMax500);
    fflush(stdout);
  }
  return 0 ;
}
