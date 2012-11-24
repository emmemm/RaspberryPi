// -*- c++ -*-

#include <stdio.h>
#include <bcm2835.h>
#include "CRecorder.h"

#define BCM2835_INPUT_PIN RPI_GPIO_P1_15

int main(int argc, char *argv[])
{
  int nSamples = 10000;

  // initialize bcm2835
  if (!bcm2835_init())
    return 1;
  // and set the input pin
  bcm2835_gpio_fsel(BCM2835_INPUT_PIN, BCM2835_GPIO_FSEL_INPT);

  if (1) {
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


  if (1) {
    char szTitle[1024];
    snprintf(szTitle, 1000, "measure duration of one call to gettimeofday(...)");
    CRecorder r(nSamples);
    timeval* p = r.get_p0();
    for (int i = 0; i < nSamples; ++i) {
      gettimeofday(p++, NULL);
    }
    r.statistics(szTitle);
  }


  if (1) {
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


  if (1) {
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


  if (1) {
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
