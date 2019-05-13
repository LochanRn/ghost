#include "autobot.h"
#include "../helper/helper.h"

Autobot::Autobot()
{

}

int Autobot::update(double heading, Helper H)
{ int L=Left;
  int R=Right;

  if(heading == 360)
   heading = 0;
  
  H.get_latlon(latitude, longitude);
  bearing = H.get_bearing(latitude,longitude,destlat,destlon);
  distance = H.get_dist(latitude,longitude,destlat,destlon);
  difference = bearing-heading

  cout<<"heading: "<<heading<<" bearing: "<<bearing<<" difference: "<<difference<<" distance:"<<distance<<endl;
 

  if(abs(difference)<  5*err)
  {
    L=Leftdrift;
    R=Rightdrift;
  }
  
  if( distance < ACCURACY)
  {
    dat = -1;
  }
  else
  {
    if(abs(difference) < err) 
      dat = fwd;

     else if((-180 < difference )&&(difference <0)||((180<difference)&&(difference<360)))
      dat = L;
    
     else
      dat = R;
  }
 
  return dat;
}


