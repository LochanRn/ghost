//#include "autobot.h"
#include "finalclient.h"
#include "../udp/udp.h"
// #include "../spi/spi.h"

Autobot::Autobot(){
}
int Autobot::updateDat(string data, Helper H)
{
  int dat;
  int n = strlen(data);
  float xVal[2];
  vector<string> token = H.split(point, ',');
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
  */
  
  
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


int Autobot::update(double heading, Helper H){
  H.get_latlon(latitude, longitude);
  bearing = H.get_bearing(latitude,longitude,destlat,destlon);
  distance = H.get_dist(latitude,longitude,destlat,destlon);
  difference = heading - bearing;
  //difference = (abs(difference)>180)?-((360-abs(difference))*difference/abs(difference)):difference;

  if(abs(difference) > 180)
    {
      if(difference >= 0)
        {
          difference = 360 - difference;
        }
      else
      {
         difference = -(difference + 360);
      }
    }


   difference = - difference;
//  printf("latitude %.6f longitude %.6f ",latitude,longitude);
  cout<<"heading: "<<heading<<" bearing: "<<bearing<<" difference: "<<difference<<" distance:"<<distance<<endl;

  if( distance < ACCURACY){
    dat = -1;
  }
  else {
    if(difference > werr) {
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
    else if(difference < -werr){
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
