#include "autobot.h"
#include "../helper/helper.h"
#include <string.h>
Autobot::Autobot()
{

}

int Autobot::updateDat(string data, Helper H)
{
  int dat;
//  int n = (data.length());
  float xVal[2];
  vector<string> token = H.split(data, ',');
  //v[0].destlat = stod(token[0]);
  //v[1].destlon = stod(token[1]);
  xVal[0]= stod(token[0]);
  xVal[1] = stod(token[1]);
  /*
  int framePos =  xVal[0], type = xVal[1];
  if(type==3)
    dat = Right;
  else {                          // Only for 3 split
    if( type == 0 )
      {if(framePos = 0)
        dat = Left;
      if(framePos = 1)
        dat = fwd;
      if(framePos = 2)  
        dat = Right;
      }
    else if ( type ==1 )
      dat = fwd;
    else if ( type ==2 )
        dat = -1;
  }
*/  //end comment ehere
  float xmin = xVal[0], xmax = xVal[1], size=xVal[1]-xVal[0];
  if(size==0)
    { dat=Right;  }
  else
  {
    if(size<50)
    {
      float mid=xmin+((xmax-xmin)/2);
      if(mid>=100 && mid<=500)
        dat=fwd;
      else if(mid<100)
        dat=Left;
      else if(mid>500)
        dat=Right;
    }
    
    else
    {  dat=-1;  }
  }
  return dat;
}



int Autobot::update(double heading, Helper H)
{ int L=Left;
  int R=Right;

  if(heading == 360)
   heading = 0;
  
  H.get_latlon(latitude, longitude);
  bearing = H.get_bearing(latitude,longitude,destlat,destlon);
  distance = H.get_dist(latitude,longitude,destlat,destlon);
  difference = bearing-heading;

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


