#include "autobot.h"
#include "../helper/helper.h"
// #include "../spi/spi.h"

Autobot::Autobot(){
}
int Autobot::update(double heading, Helper H){
  H.get_latlon(latitude, longitude);
  bearing = H.get_bearing(latitude,longitude,destlat,destlon);
  distance = H.get_dist(latitude,longitude,destlat,destlon);
  difference = heading - bearing;
  difference = (abs(difference)>180)?-((360-abs(difference))*difference/abs(difference)):difference;

//  printf("latitude %.6f longitude %.6f ",latitude,longitude);
  cout<<"heading: "<<heading<<" bearing: "<<bearing<<" difference: "<<difference<<" distance:"<<distance<<endl;

  if( distance < ACCURACY){
    dat = -1;
  }
  else {
    if(difference > werr) {
      if(dat == stop || dat == Left|| dat == Leftdrift) dat = Left;
      else dat = stop;
    }
    else if(difference > err){
      if(dat == stop || dat == Left|| dat == Leftdrift || dat == fwd) dat = Leftdrift;
      else dat = stop;
    }
    else if(difference < -werr){
      if(dat == stop || dat == Right|| dat == Rightdrift) dat = Right;
      else dat = stop;
    }
    else if(difference < -err){
      if(dat == stop || dat == Right|| dat == Rightdrift || dat == fwd) dat = Rightdrift;
      else dat = stop;
    }
    else {
      dat = fwd;
    }
  }
  return dat;
}
