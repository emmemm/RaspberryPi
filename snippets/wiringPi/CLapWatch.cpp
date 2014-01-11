#include <stdio.h>
#include "CLapWatch.h"


CLapWatch::CLapWatch(int nMaxSamples)
{
  nMaxSamples_ = nMaxSamples;
  nSamples_ = 0;
  pa_timeval_ = new timeval[nMaxSamples];
}


CLapWatch::~CLapWatch()
{
  delete pa_timeval_;
}


void CLapWatch::Reset()
{
  nSamples_ = 0;
}


void CLapWatch::RecordLap()
{
  if (nSamples_ < nMaxSamples_) {
    gettimeofday(&pa_timeval_[nSamples_], 0);
    ++nSamples_;
  }
}


unsigned int CLapWatch::GetMicroseconds(unsigned int nSample)
{
  if (nSample == 0)
    return 0;
  else if (nSample >= nSamples_)
    return 0;
  else
    return (pa_timeval_[nSample].tv_usec-pa_timeval_[nSample-1].tv_usec) + (pa_timeval_[nSample].tv_sec-pa_timeval_[nSample-1].tv_sec)*1000000;
}


unsigned int CLapWatch::GetSamples()
{
  return nSamples_;
}


void CLapWatch::WriteSamples()
{
  for (unsigned int nSample = 1; nSample < nSamples_; ++nSample) {
    int nMicroSeconds = GetMicroseconds(nSample);
    printf("%lu ", nMicroSeconds);
  }
}

