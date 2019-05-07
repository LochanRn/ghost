#ifndef __HELPER_H__
#define __HELPER_H__

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

class Helper {
    private:
    public:
      Helper();
      double maps(double x, double in_min, double in_max, double out_min, double out_max);
      unsigned char parse(unsigned char *n, int start, int end) ;
      string toString(unsigned char* str);
      vector<string> split(const string &s, char delim);
      double calcDifference(double);
};

#endif

