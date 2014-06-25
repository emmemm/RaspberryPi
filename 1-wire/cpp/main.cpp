#include <stdio.h>
#include <dirent.h>
#include <vector>
#include <string>
//#include <sys/time.h>
//#include "CLapWatch.h"
//#include <wiringPi.h>

int main (void)
{
  //printf("Hallo!\n");
  std::vector<std::string> vDirData;
  DIR* pDirDevices = opendir ("/sys/bus/w1/devices");
  if (pDirDevices) {
    struct dirent* pEntry = NULL;
    while (pEntry = readdir(pDirDevices)) {
      std::string sDirData = "/sys/bus/w1/devices/";
      sDirData += pEntry->d_name;
      sDirData += "/w1_slave";
      //printf("%s\n", sDirData.c_str());
      DIR* pDirData = opendir(sDirData.c_str());
      FILE* fData = fopen(sDirData.c_str(), "r");
      if (fData) {
        vDirData.push_back(sDirData);
        fclose(fData);
      }
    }
    closedir(pDirDevices);
  }
  while (true) {
    for (int i = 0; i < vDirData.size(); ++i) {
      //printf("%s\n", vDirData[i].c_str());
      FILE* f = fopen(vDirData[i].c_str(), "r");
      if (f) {
        char buffer[60];
        buffer[0] = '\0';
        while (feof(f) == 0)
          fgets(buffer, 60, f);
        if (i > 0)
          printf("     ");
        printf("%c%c.%c%c%c", buffer[29], buffer[30], buffer[31], buffer[32], buffer[33]);
        fclose(f);
      }
    }
    printf("\n");
    sleep(1);
  }
  return 0;
}
