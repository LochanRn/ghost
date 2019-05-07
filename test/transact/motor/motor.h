#ifndef __MOTOR_H__
#define __MOTOR_H__

#include<iostream>
using namespace std;
// #include "Arduino.h"

class motor{
  private:  
    int pins[4];                                           //pin1,pin2,pwm,potPin
  public:
    motor(int[]);
    void act(int ,int ,int);                              //runs motor forward(1,0) or backward(0,1) of pause(0,0) 
    int getPotVal();                                      //Returns potVal
    void run(int,int);
};

#endif