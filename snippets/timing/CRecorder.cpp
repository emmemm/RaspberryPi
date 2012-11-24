// -*- c++ -*-

#include <stdio.h>
#include <map>
#include "CRecorder.h"


CRecorder::CRecorder(int nSamples)
{
  nSamples_ = nSamples;
  array_timeval_ = new timeval[nSamples];
}

CRecorder::~CRecorder()
{
  delete array_timeval_;
}

timeval* CRecorder::get_p0()
{
  return &array_timeval_[0];
}

int CRecorder::get_micro_seconds(int nSample)
{
  if (nSample <= 0)
    return 0;
  if (nSample >= nSamples_)
    return 0;
  return (array_timeval_[nSample].tv_usec-array_timeval_[nSample-1].tv_usec) + (array_timeval_[nSample].tv_sec-array_timeval_[nSample-1].tv_sec)*1000000;
}

int CRecorder::get_min_micro_seconds()
{
  int nMinMicroSeconds = get_micro_seconds(1);
  for (int i = 2; i < nSamples_; ++i) {
    int n = get_micro_seconds(i);
    if (nMinMicroSeconds > n)
      nMinMicroSeconds = n;
  }
  return nMinMicroSeconds;
}

int CRecorder::get_max_micro_seconds()
{
  int nMaxMicroSeconds = get_micro_seconds(1);
  for (int i = 2; i < nSamples_; ++i) {
    int n = get_micro_seconds(i);
    if (nMaxMicroSeconds < n)
      nMaxMicroSeconds = n;
  }
  return nMaxMicroSeconds;
}


int CRecorder::get_count_micro_seconds(int nMuSec)
{
  int nCount = 0;
  for (int i = 1; i < nSamples_; ++i) {
    if (get_micro_seconds(i) == nMuSec)
      ++nCount;
  }
  return nCount;
}


void CRecorder::statistics(const char* sz)
{
  int nMinMuSec = get_min_micro_seconds();
  int nMaxMuSec = get_max_micro_seconds();
  printf("%s   --   Min %d - Max %d micro seconds\n", sz, nMinMuSec, nMaxMuSec);
  std::map<int,int> mapMusecCount;
  for (int iSample = 1; iSample < nSamples_; ++iSample) {
    int nMuSec = get_micro_seconds(iSample);
    ++mapMusecCount[nMuSec];
  }
  for (std::map<int,int>::iterator it = mapMusecCount.begin(); it != mapMusecCount.end(); ++it) {
    int nMuSec = it->first;
    int nCount = it->second;
    printf("%5dx %5d usec\n", nCount, nMuSec);
  }
}

void CRecorder::print(const char* sz, int nSamples)
{
  int iStep = 1;
  int i = 1;
  int iLast;
  while (i <= nSamples) {
    printf("%5d. %d.%06d", i, array_timeval_[i-1].tv_sec, array_timeval_[i-1].tv_usec);
    if (i == 1)
      printf("    %s\n", sz);
    else
      printf(" (%d usec)\n", (array_timeval_[i-1].tv_usec-array_timeval_[iLast-1].tv_usec) + (array_timeval_[i-1].tv_sec-array_timeval_[iLast-1].tv_sec)*1000000);
    iLast = i;
    if ((i/iStep) % 10 == 0)
      iStep *= 10;
    i += iStep;
  }
}
