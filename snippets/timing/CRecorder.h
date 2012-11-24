// -*- c++ -*-

#include <sys/time.h>

class CRecorder
{
private:
  int nSamples_;
  struct timeval* array_timeval_;
  
public:

  CRecorder(int nSamples = 10000);
  ~CRecorder();

  timeval* get_p0();
  void print(const char* sz, int nSamples);

  int get_micro_seconds(int nSample);
  int get_min_micro_seconds();
  int get_max_micro_seconds();
  int get_count_micro_seconds(int nMuSec);
  void statistics(const char* sz);
};


