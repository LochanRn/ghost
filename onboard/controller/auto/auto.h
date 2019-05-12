#ifndef __AUTO_H__
#define __AUTO_H__

#include <iostream>
#include <cmath>
#include "../helper/helper.h"
// #include "../spi/spi.h"

#define ACCURACY 1.5

struct direction{
  int val;
  string name;
  direction(int value,string Name){
    val = value;
    name = Name;
  }
};

class Autobot{
private:
    double latitude,longitude,difference,distance,bearing;
    direction Left(64,"left"),Right(16,"right"),Leftdrift(192,"leftdrift"),Rightdrift(144,"rightdrift"),Stop(0,"stop"),Forward(128,"fwd"),dat(0,"initialized");
    // int Right = 16,Left = 64,Leftdrift = 192,Rightdrift = 144,fwd = 128,stop = 0,dat = 0;
    int err = 5,werr = 15, isAuto = 0;
public:
    double destlat,destlon;
    Autobot();
    int update(double heading,Helper H);


};

#endif

