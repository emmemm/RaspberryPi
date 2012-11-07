// sudo ./blink
//
/*
// *******************************
// Raspberry | Broadcom
// GPIO 17   = PIN_11
// GPIO 22   = PIN_15
// GPIO 25   = PIN_22
// *******************************

Broadcom Pin Belegung in Klammern
In GPIO 22 (15) geht der Ausgang des IR Empfängers rein. Wird kein IR Signal empfangen ist der Ausgang HIGH.
Bei Empfang von einem IR Burst springt er auf LOW.
An GPIO 17 (11) ist eine rote Kontroll LED (weil man IR nicht sieht ;-)
GPIO 25 (22) steuert den 38 hKz Generator. HIGH = IR senden
*/


#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <bcm2835.h>

#define PIN_15 RPI_GPIO_P1_15

static unsigned long long epoch ;

unsigned int millis (void)
{
  struct timeval tv ;
  unsigned long long t1 ;
  gettimeofday (&tv, NULL) ;
  t1 = (tv.tv_sec * 1000000 + tv.tv_usec) / 1000 ;
  return (unsigned int)(t1 - epoch) ;
}


const int MAX_SAMPLES = 6000;

int main(int argc, char **argv)
{
  // If you call this, it will not actually access the GPIO
  // Use for testing
  // bcm2835_set_debug(1);

  int a[MAX_SAMPLES];
  int k;
  for (k=0;k<MAX_SAMPLES;++k)
    a[k] = 100000 + k;
 
  struct timeval tv ;
  gettimeofday (&tv, NULL) ;
  epoch = (tv.tv_sec * 1000000 + tv.tv_usec) / 1000 ;

  if (!bcm2835_init()) return 1;

 // Set the pin to be an input
  bcm2835_gpio_fsel(PIN_15, BCM2835_GPIO_FSEL_INPT);  // Ausgang IR Empfänger


  // Funktioniert als IR Repeater. IR Empfangsmodul ist an IN PIN_15
  // OUT PIN_22 steuert 38kHz Oszillator
  while (1) {
    int i=0;
    int msBegin = millis();
    int msEnd = 0;
    a[i] = bcm2835_gpio_lev(PIN_15);
    if (a[i] == 0) {
      ++i;
      while (i < MAX_SAMPLES) {
        a[i] = bcm2835_gpio_lev(PIN_15);
        ++i;
        int p,q;
        for (p=0; p < 1000; ++p)
          q=p;
      }
      msEnd = millis();
      
      printf("%d - %d, %d samples, %d ms per 100 samples\n", msBegin, msEnd, MAX_SAMPLES, (msEnd-msBegin)*100/MAX_SAMPLES);
      int j;
      int nLastPrint = 0;
      for (j=0;j<MAX_SAMPLES;++j) {
        if (j>0 && a[j] != a[j-1] && a[j] == 0) {
          printf(" (%d)\n", j-nLastPrint);
          nLastPrint=j;
        }
        printf("%d", (a[j]) ? 0 : 1);
      }
      printf("\n");
    }
  }
  return 0;
}
