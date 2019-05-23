#include "helper/helper.h"
#include "compass/compass.h"
#include "udp/udp.h"
#include "autobot/autobotObstacle.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#define portroverW 23907
#define portroverR 3301

#define portDriveW 23908
#define portDriveR 3302

#define portArmW 23909
#define portArmR 3303


//HMC5883L compass;

Udp gs(portroverR);
Udp drive(portDriveR);
Udp arm(portArmR);

Helper H;
Autobot A;
Compass c;

int dat,isAuto = -1,size;
int compass_fd;

unsigned char data[4],Arm[4];
unsigned char *coods;

double heading = 0;

struct waypoints{
  double destlat;
  double destlon;
};

struct waypoints cord[5];

string message = "";

unsigned char* value;

void kill() {
  //cout<<"Killed"<<endl;
  static unsigned char controlsArm[2] = { (unsigned char)0, (unsigned char)0 }; 
  static unsigned char controlsDrive[4] = {(unsigned char)0, (unsigned char)0 ,(unsigned char)0, (unsigned char)0 };
  drive.write(controlsDrive,portDriveW);
  arm.write(controlsArm, portArmW);
}

void setup() {
    drive.setclientIP("192.168.1.37",portDriveW);
    arm.setclientIP("192.168.1.38",portArmW);

    dat = 0,isAuto = -1,size = 0;

   if(!H.gpsdintialise()){
    //cout<<"ERROR: initialising gpsd! "<<endl;
}

   compass_fd = c.compass_setup();

   kill();
}


void removeCoods(int check){
  if(check){
    if(size == 1) cord[0].destlat = 0, cord[0].destlon = 0;
    for(int i = 1;i < size;i++){
      cord[i-1].destlat = cord[i].destlat;
      cord[i-1].destlon = cord[i].destlon;
      // printf("\nLoop: Lat: %.6f, Lon: %6f\n",cord[i-1].destlat,cord[i-1].destlon);
    }
   size--;
 }else{
   for(int i=0;i<size;i++){
     cord[i].destlat = 0;
     cord[i].destlon = 0;
   }
   size = 0;
 }
}


void Compass(){
    unsigned char buff[16];
    heading  = c.compass(buff,compass_fd);
    message = "$," + to_string(heading) + ",!";
    value = (unsigned char*)message.c_str();
    gs.write(value,portroverW);
	//cout<<"compass reached";
}

int interrupt(){
  coods = gs.read(0,200000);
  if(coods[0] == '$') {
    removeCoods(0);
    kill();
    //cout<<"Autonomous Stopped ";
    return 1;
  }
  return 0;
}

void autonomous() {
 //cout<<"autonomous started";
 A.destlat = cord[0].destlat,A.destlon = cord[0].destlon;
 while(1){
  if(interrupt()) break;
  Compass();
   A.destlat = cord[0].destlat,A.destlon = cord[0].destlon;
   printf("\nAUTO: Lat: %.6f, Lon: %6f\n",cord[0].destlat,cord[0].destlon);
  dat = A.update(heading,H);
  //cout<<"--------------------"<<dat<<"--------------------\n ";

  if(dat == -1){
    kill();
    break;
  }
  else {
    data[0] = (unsigned char) 0;
    data[1] = (unsigned char) dat;
    data[2] = (unsigned char) 0;
    data[3] = (unsigned char) 0;
    drive.write(data,portDriveW);
  }
 }
}

void getdestlatlon(string point,int i){
  vector<string> token = H.split(point, ',');
  cord[i].destlat = stod(token[0]);
  cord[i].destlon = stod(token[1]);
}

int parseCoods(unsigned char* coods) {
  string data = H.toString(coods);
  data = data.substr(1, data.size() - 2);
  vector<string> tokens = H.split(data, '!');
  int i = 0;
  for (vector<string>::iterator it = tokens.begin() ; it != tokens.end(); ++it,i++){
    getdestlatlon(*it,i);
  }
  return tokens.size();
}

void keyboard(unsigned char *coods) {
 static int i, mid,mid1,mid2;
 Compass();
 //static unsigned char  Arm[2];//Data[4]

 //drive.write(Drive,portDriveW);
 //arm.write(Arm, portArmW);
 string val = H.toString(coods);

// cout<<val.substr(1,val.size()-1)<<endl;

 vector<string> c = H.split(val.substr(1,val.size()-1),',');

 for(int i = 0; i < 6 ; i++){

   if(i < 4){
     data[i]= (unsigned char)atoi(c[i].c_str());
   }
   else{
     Arm[i-4]= (unsigned char)atoi(c[i].c_str());
   }
   //cout<<c[i]<<" ";
 }
 //cout<<endl;
 drive.write(data,portDriveW);
 arm.write(Arm, portArmW);
}


void autoforsingledestination(){
  autonomous();
  message = "$," + to_string(heading) + ",Final destination: " + to_string(cord[0].destlat) + " " + to_string(cord[0].destlon) + "reached," + "~,";
  value = (unsigned char*)message.c_str();
  gs.write(value,portroverW);
  removeCoods(1);
}

void autoformultipledestination(){
  for(int i=0;i<size;i++){
    autonomous();
    if(size == i+1)
      message = "$," + to_string(heading) + ",Final destination: " + to_string(cord[0].destlat) + " " + to_string(cord[0].destlon) + "reached,";
    else
      message = "$," + to_string(heading) + ",Destination: " + to_string(cord[0].destlat) + " " + to_string(cord[0].destlon) + "reached,";
  }
}
void decideautonomous(unsigned char* coods){
  //cout<<"Deciding";
  if(coods[0] == '#') size = parseCoods(coods);
  message = "$," + to_string(heading) + ",%,";
  value = (unsigned char*)message.c_str();
  gs.write(value,portroverW);
  if(coods[0] == '@' && size > 0){
    autoforsingledestination();
  }
  else if(coods[0] == '*' && size > 0){

    autoformultipledestination();
  }
}

void check(unsigned char* coods) {
  isAuto = (coods[0] == '$')? 2 : isAuto;
  isAuto = (coods[0] == '#')? 1 : isAuto;
  isAuto = (coods[0] == '<')? 0 : isAuto;
  if (isAuto == 2 && size > 0 ) {
    removeCoods(0);
    //cout << "Autonomous Stopped" << endl;
  }
  else if(isAuto == 1){
    //cout << "Autonomous running" << endl;
    decideautonomous(coods);
  }
  else if(isAuto == 0){
    //cout << "Keyboard running" <<coods<< endl;
    keyboard(coods);

  }
}

void loop() {
 coods = gs.read(1,0);
  if(coods[0] != '0') {
     check(coods);
  }
  else kill();
}

int main(){
    setup();
    while (1) loop();
    return 0;
}

