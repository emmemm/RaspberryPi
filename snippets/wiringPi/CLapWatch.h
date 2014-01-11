//#pragma once

#include <sys/time.h>

class CLapWatch
{
private:
  unsigned int nMaxSamples_;
  unsigned int nSamples_;
  struct timeval* pa_timeval_;

public:
  CLapWatch(int nMaxSamples);
  ~CLapWatch();
  void Reset();

  void RecordLap();
  unsigned int GetMicroseconds(unsigned int nSample);
  unsigned int GetSamples();

  void WriteSamples();
};



