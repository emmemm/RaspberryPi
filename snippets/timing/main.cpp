// -*- c++ -*-

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <bcm2835.h>
#include "CRecorder.h"

#define BCM2835_INPUT_PIN RPI_GPIO_P1_15

int main(int argc, char *argv[])
{
  struct SGlobalArgs {
    int nSamples;               /* -n option */
    int nRun1;                  /* -1 option */
    int nRun2;                  /* -2 option */
    int nRun10;                 /* -0 option */
    int nRunGPIO;               /* -g option */
  } oGlobalArgs;

  static const char *szOptString = "n:120g";

  oGlobalArgs.nSamples = 100000;
  oGlobalArgs.nRun1 = 0;
  oGlobalArgs.nRun2 = 0;
  oGlobalArgs.nRun10 = 0;
  oGlobalArgs.nRunGPIO = 0;

  int opt = getopt( argc, argv, szOptString );
  while( opt != -1 ) {
    switch( opt ) {
    case 'n':
      oGlobalArgs.nSamples = atoi(optarg);
      break;
    case '1':
      oGlobalArgs.nRun1 = 1;
      break;
    case '2':
      oGlobalArgs.nRun2 = 1;
      break;
    case '0':
      oGlobalArgs.nRun10 = 1;
      break;
    case 'g':
      oGlobalArgs.nRunGPIO = 1;
      break;
    }
    opt = getopt( argc, argv, szOptString );
  }

  int nSamples = oGlobalArgs.nSamples;
\
  // initialize bcm2835
  if (!bcm2835_init())
    return 1;
  // and set the input pin
  bcm2835_gpio_fsel(BCM2835_INPUT_PIN, BCM2835_GPIO_FSEL_INPT);

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


  if (oGlobalArgs.nRun1) {
    char szTitle[1024];
    snprintf(szTitle, 1000, "measure duration of one call to gettimeofday(...)");
    CRecorder r(nSamples);
    timeval* p = r.get_p0();
    for (int i = 0; i < nSamples; ++i) {
      gettimeofday(p++, NULL);
    }
    r.statistics(szTitle);
  }


  if (oGlobalArgs.nRun2) {
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


  if (oGlobalArgs.nRun10) {
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


  if (oGlobalArgs.nRunGPIO) {
    char szTitle[1024];
    snprintf(szTitle, 1000, "measure duration of a GPIO pin read");
    CRecorder r(nSamples);
    timeval* p = r.get_p0();
    for (int i = 0; i < nSamples; ++i) {
      gettimeofday(p++, NULL);
      bcm2835_gpio_lev(BCM2835_INPUT_PIN);
    }
    r.statistics(szTitle);
  }

  return 0;
}
