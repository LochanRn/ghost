#include "auto.h"
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
  if( distance < ACCURACY){
    dat.val = -1;
  }
  else {
    if(difference > werr) {
      if(dat.val == Stop.val || dat.val == Left.val || dat.val == Leftdrift.val || dat.val == Forward.val) dat = Left;
      else dat = Stop;
    }
    else if(difference > err){
      if(dat.val == Stop.val || dat.val == Left.val || dat.val == Leftdrift.val || dat.val == Forward.val) dat = Leftdrift;
      else dat = Stop;
    }
    else if(difference < -err){
      if(dat.val == Stop.val || dat.val == Right.val || dat.val == Rightdrift.val || dat.val == Forward.val) dat = Right;
      else dat = Stop;
    }
    else if(difference < -werr){
      if(dat.val == Stop || dat.val == Right|| dat.val == Rightdrift.val || dat.val == Forward.val) dat = Rightdrift;
      else dat = Stop;
    }
    else {
      dat = Forward;
    }
  }
  cout<<dat.name<<" Heading: "<<heading<<" bearing: "<<bearing<<" difference: "<<difference<<" distance:"<<distance<<endl;
  return dat.val;
}

