// -*- c++ -*-

#include <sys/time.h>
#include <map>

struct SStatistics
{
  unsigned int m_nMinMicroSeconds;
  unsigned int m_nMaxMicroSeconds;
  unsigned int m_nAvgMicroSeconds;
  unsigned int m_nSumMicroSeconds;
  unsigned int m_nCountSamples;
  typedef std::map<unsigned int,unsigned int> TMapMicroSeconds2Count;
  TMapMicroSeconds2Count m_mapMicroSeconds2Count;
};


class CRecorder
{
private:
  int nSamples_;
  struct timeval* array_timeval_;

  void setup_statistics(SStatistics& oStatistics);
  unsigned int get_micro_seconds(int nSample);
  
public:

  CRecorder(int nSamples = 10000);
  ~CRecorder();

  timeval* get_p0();
//  void print(const char* sz, int nSamples);

  void statistics(const char* sz);
};


