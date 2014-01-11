// -*- c++ -*-

#include <stdio.h>
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

unsigned int CRecorder::get_micro_seconds(int nSample)
{
  if (nSample <= 0)
    return 0;
  if (nSample >= nSamples_)
    return 0;
  return (array_timeval_[nSample].tv_usec-array_timeval_[nSample-1].tv_usec) + (array_timeval_[nSample].tv_sec-array_timeval_[nSample-1].tv_sec)*1000000;
}

void CRecorder::setup_statistics(SStatistics& oStatistics)
{
  oStatistics.m_nMinMicroSeconds = 0;
  oStatistics.m_nMaxMicroSeconds = 0;
  oStatistics.m_nAvgMicroSeconds = 0;
  oStatistics.m_nSumMicroSeconds = 0;
  oStatistics.m_nCountSamples = 0;
  oStatistics.m_mapMicroSeconds2Count.clear();
  if (nSamples_ > 1) {
    int nMicroSeconds = get_micro_seconds(1);
    oStatistics.m_nMinMicroSeconds = nMicroSeconds;
    oStatistics.m_nMaxMicroSeconds = nMicroSeconds;
    oStatistics.m_nSumMicroSeconds = nMicroSeconds;
    oStatistics.m_nCountSamples = 1;
    ++oStatistics.m_mapMicroSeconds2Count[nMicroSeconds];
    for (int iSample = 2; iSample < nSamples_; ++iSample) {
      int nMicroSeconds = get_micro_seconds(iSample);
      if (oStatistics.m_nMinMicroSeconds > nMicroSeconds)
        oStatistics.m_nMinMicroSeconds = nMicroSeconds;
      if (oStatistics.m_nMaxMicroSeconds < nMicroSeconds)
        oStatistics.m_nMaxMicroSeconds = nMicroSeconds;
      oStatistics.m_nSumMicroSeconds += nMicroSeconds;
      ++oStatistics.m_nCountSamples;
      ++oStatistics.m_mapMicroSeconds2Count[nMicroSeconds];
    }
    oStatistics.m_nAvgMicroSeconds = oStatistics.m_nSumMicroSeconds / oStatistics.m_nCountSamples;
  }
}

void CRecorder::statistics(const char* sz)
{
  SStatistics oStatistics; 
  setup_statistics(oStatistics);
  printf("%s   --   Min %d - Max %d - Avg %d  micro seconds\n", sz, oStatistics.m_nMinMicroSeconds, oStatistics.m_nMaxMicroSeconds, oStatistics.m_nAvgMicroSeconds);
  unsigned int nCountPrints = 0;
  for (SStatistics::TMapMicroSeconds2Count::iterator it = oStatistics.m_mapMicroSeconds2Count.begin(); it != oStatistics.m_mapMicroSeconds2Count.end(); ++it) {
    int nMicroSeconds = it->first;
    int nCount = it->second;
    printf("%5dx %5d", nCount, nMicroSeconds);
    ++nCountPrints;
    if (nCountPrints % 10)
      printf("\t");
    else
      printf("\n");
  }
  printf("\n");
}

//void CRecorder::print(const char* sz, int nSamples)
//{
//  int iStep = 1;
//  int i = 1;
//  int iLast;
//  while (i <= nSamples) {
//    printf("%5d. %d.%06d", i, array_timeval_[i-1].tv_sec, array_timeval_[i-1].tv_usec);
//    if (i == 1)
//      printf("    %s\n", sz);
//    else
//      printf(" (%d usec)\n", (array_timeval_[i-1].tv_usec-array_timeval_[iLast-1].tv_usec) + (array_timeval_[i-1].tv_sec-array_timeval_[iLast-1].tv_sec)*1000000);
//    iLast = i;
//    if ((i/iStep) % 10 == 0)
//      iStep *= 10;
//    i += iStep;
//  }
//}
