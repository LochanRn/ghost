#ifndef __AUTOBOT_H__
#define __AUTOBOT_H__

#include <iostream>
#include <cmath>
#include "../helper/helper.h"
// #include "../spi/spi.h"

#define ACCURACY 1.5

class Autobot{
private:
    double latitude,longitude,difference,distance,bearing;
    int Right = 16,Left = 64,Leftdrift = 192,Rightdrift = 144,fwd = 128,stop = 0;
    int err = 5,werr = 15,dat = 0, isAuto = 0;
public:
    double destlat,destlon;
    Autobot();
    int update(double heading,Helper H);
    

};

#endif
