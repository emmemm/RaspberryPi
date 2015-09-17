#include <chrono>
#include <random>
#include <thread>
#include <iostream>

void WaitSomeTimePrinting(int nTimes, char ch)
{
  // get seed from true random device
  std::random_device oRandomDevice;
  auto oSeed = oRandomDevice();
  // seed PRNG
  std::default_random_engine oPRNG(oSeed);
  std::uniform_int_distribution<int> oDistribution(10, 1000);
  for (int i=0; i < nTimes; ++i) {
    std::this_thread::sleep_for(std::chrono::milliseconds(oDistribution(oPRNG)));
    std::cout.put(ch);
    std::cout.flush();
  }
}

// Bis hier ist der Code fuer das was ich zeigen will uninteressant.
// Die Funktion WaitSomeTimePrinting(...) ist nur dazu da ein Zeichen auszugeben und dazwischen unterschiedlich lange
// Pausen einzulegen.


// 1. Beispiel
// Wir geben 10x eine '1' und dann 10x eine '2' aus.
// Da das, so wie es hier passiert, nichts mit threads zu tun hat, passert das schoen hinereinander...
  
void f1()
{
  WaitSomeTimePrinting(10, '1');
}

void f2()
{
  WaitSomeTimePrinting(10, '2');
}

int main()
{
  f1();
  f2();
  std::cout << std::endl;
}
