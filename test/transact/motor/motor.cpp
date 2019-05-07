#include "motor.h"

motor::motor(int pins[]){
    for(int i=0;i<4;i++)
        this->pins[i] = pins[i];
    for(int i=0;i<3;i++);
        // pinMode(pins[i],OUTPUT);
        // pinMode(pins[3],INPUT);
}

void motor::act(int A,int B,int pwm){                                  //1,0 -Forward 0,1-Backward   0,0-Pause
  cout<<pins[0]<<" "<<pins[1]<<" "<<A<<" "<<B<<" "<<pwm<<endl;
  // digitalWrite(pins[0],A);
  // digitalWrite(pins[1],B);  
  // analogWrite(pins[2],pwm);
}

int motor::getPotVal(){ 
  return 0;
  // return analogRead(pins[3]);
}

void motor::run(int dALPHAdt,int pwm){
 if     (dALPHAdt>0)  act(1,0,pwm);
 else if(dALPHAdt<0)  act(0,1,pwm);
 else                 act(0,0,pwm);
}