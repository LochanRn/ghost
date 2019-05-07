#include "helper.h"


Helper::Helper()
{
}


double Helper::maps(double x, double in_min, double in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


unsigned char Helper::parse(unsigned char *n, int start, int end)
{
  int a = 0;
  for (int i = start; i < end; i++)
  {
    a += (n[i] - '0');
    a *= 10;
  }
  return (unsigned char)(a / 10);
}

vector<string> Helper::split(const string &s, char delim)
{
  stringstream ss(s);
  string item;
  vector<string> tokens;
  while (getline(ss, item, delim))
    tokens.push_back(item);
  return tokens;
}

string Helper::toString(unsigned char *str)
{
  return reinterpret_cast<char *>(str);
}

double Helper::calcDifference(double diff){
  if (abs(diff) > 180){
    if (diff >= 0) diff = 360 - diff;
    else diff = -(diff + 360);
  }
  else
    diff = -diff;
  return diff;
}
