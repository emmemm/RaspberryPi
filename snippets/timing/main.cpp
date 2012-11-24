// -*- c++ -*-

#include <stdio.h>
#include "CRecorder.h"

int main(int argc, char *argv[])
{
  int nSamples = 10000;

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

  return 0;
}
