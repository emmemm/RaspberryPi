// -*- c++ -*-


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#if defined(WIRING_PI)
  #include <wiringPi.h>
#else
  #include <bcm2835.h>
#endif
#include "CRecorder.h"

#if defined(WIRING_PI)
  #define WIRINGPI_INPUT_PIN 3
#else
  #define BCM2835_INPUT_PIN RPI_GPIO_P1_15
#endif

int main(int argc, char *argv[])
{
  struct SGlobalArgs {
    unsigned int nSamples;               /* -s option */
    unsigned int nCalls;                 /* -n option */
    bool bRunEmpty;              /* -0 option */
    bool bRunGetTime;            /* -t option */
#if defined(WIRING_PI)
    bool bRunWiringPiRead;       /* -w option */
#else
    bool bRunGPIO;               /* -g option */
#endif
  } oGlobalArgs;

#if defined(WIRING_PI)
  static const char *szOptString = "s:n:0twh?";
#else
  static const char *szOptString = "s:n:0tgh?";
#endif

  oGlobalArgs.nSamples = 100000;
  oGlobalArgs.nCalls = 1;
  oGlobalArgs.bRunEmpty = false;
  oGlobalArgs.bRunGetTime = false;
#if defined(WIRING_PI)
  oGlobalArgs.bRunWiringPiRead = false;
#else
  oGlobalArgs.bRunGPIO = false;
#endif

  int opt = getopt( argc, argv, szOptString );
  while( opt != -1 ) {
    switch( opt ) {
    case 's':
      oGlobalArgs.nSamples = atoi(optarg);
      break;
    case 'n':
      oGlobalArgs.nCalls = atoi(optarg);
      break;
    case '0':
      oGlobalArgs.bRunEmpty = true;
      break;
    case 't':
      oGlobalArgs.bRunGetTime = true;
      break;
#if defined(WIRING_PI)
    case 'w':
      oGlobalArgs.bRunWiringPiRead = true;
      break;
#else
    case 'g':
      oGlobalArgs.bRunGPIO = true;
      break;
#endif
    }
    opt = getopt( argc, argv, szOptString );
  }

  int nSamples = oGlobalArgs.nSamples;

#if defined(WIRING_PI)
  if (oGlobalArgs.bRunWiringPiRead)
    wiringPiSetup();
#else    
  // initialize bcm2835
  if (!bcm2835_init())
    return 1;
  // and set the input pin
  bcm2835_gpio_fsel(BCM2835_INPUT_PIN, BCM2835_GPIO_FSEL_INPT);
#endif

  if (oGlobalArgs.bRunEmpty) {
    char szTitle[1024];
    snprintf(szTitle, 1000, "measure duration of a %u iterations of an empty loop)",  oGlobalArgs.nCalls);
    CRecorder r(nSamples);
    timeval* p = r.get_p0();
    for (unsigned int iSample = 0; iSample < nSamples; ++iSample) {
      gettimeofday(p++, NULL);
      for (unsigned int iCall = 0; iCall < oGlobalArgs.nCalls; ++iCall) {
      }
    }
    r.statistics(szTitle);
  }

  if (oGlobalArgs.bRunGetTime) {
    char szTitle[1024];
    snprintf(szTitle, 1000, "measure duration of a %u calls to gettimeofday(...)",  oGlobalArgs.nCalls);
    CRecorder r(nSamples);
    struct timeval tvDummy;
    timeval* p = r.get_p0();
    for (unsigned int iSample = 0; iSample < nSamples; ++iSample) {
      gettimeofday(p++, NULL);
      for (unsigned int iCall = 1; iCall < oGlobalArgs.nCalls; ++iCall)
        gettimeofday(&tvDummy, NULL);
    }
    r.statistics(szTitle);
  }

#if defined(WIRING_PI)
  if (oGlobalArgs.bRunWiringPiRead) {
    char szTitle[1024];
    snprintf(szTitle, 1000, "measure duration of a %u GPIO pin reads",  oGlobalArgs.nCalls);
    CRecorder r(nSamples);
    timeval* p = r.get_p0();
    for (unsigned int iSample = 0; iSample < nSamples; ++iSample) {
      gettimeofday(p++, NULL);
      for (unsigned int iCall = 0; iCall < oGlobalArgs.nCalls; ++iCall)
        digitalRead(3);
    }
    r.statistics(szTitle);
  }
#else
  if (oGlobalArgs.bRunGPIO) {
    char szTitle[1024];
    snprintf(szTitle, 1000, "measure duration of a %u GPIO pin reads",  oGlobalArgs.nCalls);
    CRecorder r(nSamples);
    timeval* p = r.get_p0();
    for (unsigned int iSample = 0; iSample < nSamples; ++iSample) {
      gettimeofday(p++, NULL);
      for (unsigned int iCall = 0; iCall < oGlobalArgs.nCalls; ++iCall)
        bcm2835_gpio_lev(BCM2835_INPUT_PIN);
    }
    r.statistics(szTitle);
  }
#endif


  if (0) {
    char szTitle[1024];
    snprintf(szTitle, 1000, "measure duration of an empty loop with %d iterations", nSamples);
    CRecorder r(2);
    timeval* p = r.get_p0();
    gettimeofday(p++, NULL);
    for (int i = 0; i < nSamples; ++i) {
    }
    gettimeofday(p++, NULL);
    r.statistics(szTitle);
  }

  if (0) {
    char szTitle[1024];
    snprintf(szTitle, 1000, "measure duration of one call to gettimeofday(...)");
    CRecorder r(nSamples);
    timeval* p = r.get_p0();
    for (int i = 0; i < nSamples; ++i) {
      gettimeofday(p++, NULL);
    }
    r.statistics(szTitle);
  }

  if (0) {
    char szTitle[1024];
    snprintf(szTitle, 1000, "measure duration of two calls to gettimeofday(...)");
    CRecorder r(nSamples);
    timeval* p = r.get_p0();
    struct timeval tvDummy;
    for (int i = 0; i < nSamples; ++i) {
      gettimeofday(p++, NULL);
      gettimeofday(&tvDummy, NULL);
    }
    r.statistics(szTitle);
  }


  if (0) {
    char szTitle[1024];
    snprintf(szTitle, 1000, "measure duration of ten calls to gettimeofday(...)");
    CRecorder r(nSamples);
    timeval* p = r.get_p0();
    struct timeval tvDummy;
    for (int i = 0; i < nSamples; ++i) {
      gettimeofday(p++, NULL);
      gettimeofday(&tvDummy, NULL);
      gettimeofday(&tvDummy, NULL);
      gettimeofday(&tvDummy, NULL);
      gettimeofday(&tvDummy, NULL);
      gettimeofday(&tvDummy, NULL);
      gettimeofday(&tvDummy, NULL);
      gettimeofday(&tvDummy, NULL);
      gettimeofday(&tvDummy, NULL);
      gettimeofday(&tvDummy, NULL);
    }
    r.statistics(szTitle);
  }

  return 0;
}
