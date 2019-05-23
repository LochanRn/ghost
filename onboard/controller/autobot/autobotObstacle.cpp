#include "autobotObstacle.h"
#include "../helper/helper.h"
// #include "../spi/spi.h"

Autobot::Autobot(){
}
int Autobot::update(double heading, Helper H){//, double offsetLeft, double offsetRight){
  H.get_latlon(latitude, longitude);
  bearing = H.get_bearing(latitude,longitude,destlat,destlon);
  distance = H.get_dist(latitude,longitude,destlat,destlon);
  //differenceLeft = heading - bearing + offsetLeft;
  //differenceRight = heading - bearing + offsetRight;
  //difference = (abs(difference)>180)?-((360-abs(difference))*difference/abs(difference)):difference;
  difference = calcDifference(heading - bearing);
  //differenceRight = calcDifference(differenceRight);
  /*if(abs(difference1) > 180)
    {
      if(difference1 >= 0)
        {
          difference1 = 360 - difference1;
        }
      else
      {
         difference1 = -(difference1 + 360);
      }
    }
   difference1 = - difference1;



   if(abs(difference2) > 180)
     {
       if(difference2 >= 0)
         {
           difference2 = 360 - difference2;
         }
       else
       {
          difference2 = -(difference2 + 360);
       }
     }
    difference2 = -difference2;
*/
  // if(abs(differenceLeft) > abs(differenceRight))
  // {
  //    difference = differenceRight;
  // }
  // else difference = differenceLeft;

//   if(abs(heading + offset1 - bearing) >  abs(heading + offset2 - bearing))

//  printf("latitude %.6f longitude %.6f ",latitude,longitude);
  cout<<"heading: "<<heading<<" bearing: "<<bearing<<" difference: "<<difference<<" distance:"<<distance<<endl;

  if( distance < ACCURACY){
    dat = -1;
  }
  else {
    if(difference > wideErr) {
      if(dat == stop || dat == Right)
        dat = Right;
      else dat = stop;
    }
    else if(difference > err){
     if(dat == Right)
       dat = Right;
     else  if(dat == stop || dat == Rightdrift || dat == fwd)
       dat = Rightdrift;
     else dat = stop;
    }
    else if(difference < -wideErr){
      if(dat == stop || dat == Left)
        dat = Left;
      else dat = stop;
    }
    else if(difference < -err){
      if (dat == Left)// && difference <-werr/2)
        dat = Left;
      else if(dat == stop || dat == Leftdrift || dat == fwd)
        dat = Leftdrift;
      else dat = stop;
    }
    else {
      dat = fwd;
    }
  }
  return dat;
}
