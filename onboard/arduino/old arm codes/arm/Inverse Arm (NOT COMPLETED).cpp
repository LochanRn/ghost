class motor
{
  public:
    int pin[4];//pin1,pin2,pwm,potPin
    int potVal;
    int velocity;
    void updatePot();//Update Pot Values
  //public:
    void setPins(int[]);
    void boot();// start the pins
    void runFor();//Move the motor forward continuesly
    void runBack();//move the motor backward continuesly
    void pause();//Stop the motor
    void setVelocity(int);//Set velocity of the motors
    int getPotVal();//Returns potVal
};
double calcFI(double,double);//returns angle FI in radian arg-(Xcoordnate,Ycoordinate)
double calcTHETA(double,double);//returns angle THETA in radian arg-(Xcoordnate,Ycoordinate)
double calcALPHA(double,double);//returns bottom actuator length arg-THETA and FI
double calcBETA(double,double);//returns top actuator length arg-THETA and FI
int calcPot1(double);//returns pot value for bottom actuator given length
int calcPot2(double);//return pot value  for top actuator given length
int calcPot1(double,double);//returns pot value for bottom actuator given angle arg-alpha
int calcPot2(double,double);//return pot value for top actuator given angle arg-beta
int calcPot3(double);//returns pot value for turn table
double calcX();//returns present X coordinate...
double calcY();//returns present Y coordinate...
double calcZ();//returns present Z coordinate...
void calcXYZ();//calculates present 3D coordinates...

class armMasala
{
public:
  motor LA1;
  motor LA2;
  motor TT;
//public:
  void boot(int[],int[],int[]);
  void line(double,double,double,int,double);
  void gotoXY(double,double,double,double);//goto given x and y,z coordinate -
  /*This function needs to be changed alot which includes change of normal 3D system into  */
  void gotoAngle(double,double,double,double);//goto specified angles including zeta.
};
armMasala arm;

void motor::setPins(int temp[4])
{
  for(int i=0;i<4;i++)
    pin[i]=temp[i];
  boot();
}

void motor::boot()
{
  for(int i=0;i<3;i++)
    pinMode(pin[i],OUTPUT);
    pinMode(pin[3],INPUT);
    
}

void motor::updatePot()
{
  potVal=analogRead(pin[3]);
}

void motor::setVelocity(int newSpeed)
{
  velocity=newSpeed;
}

void motor::pause()
{
  analogWrite(pin[2],0);
  digitalWrite(pin[0],0);
  digitalWrite(pin[1],0);
}

void motor::runFor()
{
  analogWrite(pin[2], velocity);
  digitalWrite(pin[0],1);
  digitalWrite(pin[1],0);
}

void motor::runBack()
{
  analogWrite(pin[2], velocity);
  digitalWrite(pin[0],0);
  digitalWrite(pin[1],1);
  
}

int motor::getPotVal()
{
  updatePot();
  return potVal;
}


#define FACTOR 0.017453  //degree to radian conversion factor
#define PI 3.14
double L[]={0,515,330,342,'\0',150,327.112,80.316,344.33,20};  //constant link lengths
double TP[]={50,70.7,50,43.53*FACTOR,90*FACTOR,46.44*FACTOR};  //constant triangle  Parameters
double AL[]={242,342,342,452};  //actuator length Limits
double FA[]={atan(L[9]/L[6]),acos(L[3]/L[8]),21.66*FACTOR,45*FACTOR,43.53*FACTOR,90*FACTOR,46.44*FACTOR};
double TL[2];//={,};//min and max angle limits for turn table. 
double TTP[2];//={,};//min and max pot value for turn table
double AP[4]={0,1023,0,1023};//pot limits min1 max1 min 2 max2..
double calcX()
{
  double BETA=map(arm.LA2.getPotVal(),AP[2],AP[3],AL[2],AL[3]);
  double ALPHA=map(arm.LA1.getPotVal(),AP[0],AP[1],AL[0],AL[1]);
  
  double THETA=acos( (pow(L[6],2)+pow(L[8],2)-pow(ALPHA,2)  )/ (2*L[6]*L[7])  )+FA[0]-FA[2];
  double lengths=pow(  (pow(TP[1],2)+pow(L[8],2)-2*TP[1]*L[8]*cos(PI+TP[5]-THETA-FA[3])       )  ,0.5);
  double H=asin( (L[8]*sin(PI+TP[5]-THETA-FA[3]))/(lengths) );
  double Q=acos((pow(L[5],2)+pow(lengths,2)-pow(BETA,2))/(2*L[5]*lengths))-H;
  double FI=Q-PI+TP[3]+THETA+TP[4]+FA[3];

  double X=(L[1]*cos(THETA))+(L[2]*cos(THETA-FI));//in cylindrical system...
  double Y=(L[1]*sin(THETA))+(L[2]*sin(THETA-FI));//in cylindrical system...
  double ZETA=map(arm.TT.getPotVal(),TTP[0],TTP[1],TL[0],TL[1]);//angle of turn table...
  
  double z=X*sin(ZETA);//3D coordinate...
  double y=Y;//3D coordinate...
  double x=X*cos(ZETA);//3D coordinate...
  return x;
}
double calcY()
{
  double BETA=map(arm.LA2.getPotVal(),AP[2],AP[3],AL[2],AL[3]);
  double ALPHA=map(arm.LA1.getPotVal(),AP[0],AP[1],AL[0],AL[1]);
  
  double THETA=acos( (pow(L[6],2)+pow(L[8],2)-pow(ALPHA,2)  )/ (2*L[6]*L[7])  )+FA[0]-FA[2];
  double lengths=pow(  (pow(TP[1],2)+pow(L[8],2)-2*TP[1]*L[8]*cos(PI+TP[5]-THETA-FA[3])       )  ,0.5);
  double H=asin( (L[8]*sin(PI+TP[5]-THETA-FA[3]))/(lengths) );
  double Q=acos((pow(L[5],2)+pow(lengths,2)-pow(BETA,2))/(2*L[5]*lengths))-H;
  double FI=Q-PI+TP[3]+THETA+TP[4]+FA[3];

  double X=(L[1]*cos(THETA))+(L[2]*cos(THETA-FI));//in cylindrical system...
  double Y=(L[1]*sin(THETA))+(L[2]*sin(THETA-FI));//in cylindrical system...
  double ZETA=map(arm.TT.getPotVal(),TTP[0],TTP[1],TL[0],TL[1]);//angle of turn table...
  
  double z=X*sin(ZETA);//3D coordinate...
  double y=Y;//3D coordinate...
  double x=X*cos(ZETA);//3D coordinate...
  return y;
}
double calcZ()
{
  double BETA=map(arm.LA2.getPotVal(),AP[2],AP[3],AL[2],AL[3]);
  double ALPHA=map(arm.LA1.getPotVal(),AP[0],AP[1],AL[0],AL[1]);
  
  double THETA=acos( (pow(L[6],2)+pow(L[8],2)-pow(ALPHA,2)  )/ (2*L[6]*L[7])  )+FA[0]-FA[2];
  double lengths=pow(  (pow(TP[1],2)+pow(L[8],2)-2*TP[1]*L[8]*cos(PI+TP[5]-THETA-FA[3])       )  ,0.5);
  double H=asin( (L[8]*sin(PI+TP[5]-THETA-FA[3]))/(lengths) );
  double Q=acos((pow(L[5],2)+pow(lengths,2)-pow(BETA,2))/(2*L[5]*lengths))-H;
  double FI=Q-PI+TP[3]+THETA+TP[4]+FA[3];

  double X=(L[1]*cos(THETA))+(L[2]*cos(THETA-FI));//in cylindrical system...
  double Y=(L[1]*sin(THETA))+(L[2]*sin(THETA-FI));//in cylindrical system...
  double ZETA=map(arm.TT.getPotVal(),TTP[0],TTP[1],TL[0],TL[1]);//angle of turn table...
  
  double z=X*sin(ZETA);//3D coordinate...
  double y=Y;//3D coordinate...
  double x=X*cos(ZETA);//3D coordinate...
  return z;
}
void calcXYZ()
{
 
   
  double BETA=map(arm.LA2.getPotVal(),AP[2],AP[3],AL[2],AL[3]);
  double ALPHA=map(arm.LA1.getPotVal(),AP[0],AP[1],AL[0],AL[1]);
  
  double THETA=acos( (pow(L[6],2)+pow(L[8],2)-pow(ALPHA,2)  )/ (2*L[6]*L[7])  )+FA[0]-FA[2];
  double lengths=pow(  (pow(TP[1],2)+pow(L[8],2)-2*TP[1]*L[8]*cos(PI+TP[5]-THETA-FA[3])       )  ,0.5);
  double H=asin( (L[8]*sin(PI+TP[5]-THETA-FA[3]))/(lengths) );
  double Q=acos((pow(L[5],2)+pow(lengths,2)-pow(BETA,2))/(2*L[5]*lengths))-H;
  double FI=Q-PI+TP[3]+THETA+TP[4]+FA[3];

  double X=(L[1]*cos(THETA))+(L[2]*cos(THETA-FI));//in cylindrical system...
  double Y=(L[1]*sin(THETA))+(L[2]*sin(THETA-FI));//in cylindrical system...
  double ZETA=map(arm.TT.getPotVal(),TTP[0],TTP[1],TL[0],TL[1]);//angle of turn table...
  
  double z=X*sin(ZETA);//3D coordinate...
  double y=Y;//3D coordinate...
  double x=X*cos(ZETA);//3D coordinate...
  
  Serial.println("=====================================");
  
  Serial.print("X :");
  Serial.println(X);
  Serial.print("Y :");
  Serial.println(Y);
  
  Serial.println("=====================================");

}




double calcFI(double x,double y)
{
   return (acos((pow(x,2)+pow(y,2)-pow(L[1],2)-pow(L[2],2))/(2*L[1]*L[2])));
}

double calcTHETA(double x,double y)
{
  return (atan(((y)*(L[1]+(L[2]*cos(calcFI(x,y))))+(x*(L[2])*(sin(calcFI(x,y)))) )/((x)*(L[1]+((L[2])*(cos(calcFI(x,y)))))-(y*(L[2])*(sin(calcFI(x,y)))))));
}

double calcALPHA(double theta,double fi)
{
  return (pow((pow(L[6],2) + pow(L[7],2)-2*(L[6])*(L[7])*cos(FA[2]+theta-FA[0])),0.5));
}

double calcBETA(double theta,double fi)
{
  double Q=(PI-TP[3]-theta-TP[4]-FA[3]+fi);
  double lambda=pow((pow(TP[1],2)+pow(L[5],2)-2*(TP[1])*(L[5])*cos(Q)),0.5);
  double delta=acos((pow(TP[1],2)+pow(lambda,2)-pow(L[5],2))/(2*(TP[1])*(lambda)));
  double gama=(PI-delta-FA[1]+theta+FA[3]-FA[6]);

  return (pow((pow(lambda,2)+pow(L[8],2))-2*(lambda)*(L[8])*cos(gama),0.5));

}

int calcPot1(double alpha)
{
  return (map(alpha,AL[0],AL[1],AP[0],AP[1]));
}

int calcPot2(double beta)
{
  return (map(beta,AL[2],AL[3],AP[2],AP[3]));
}

int calcPot1(double theta,double fi)//p
{
  return (map(calcALPHA(theta,fi),AL[0],AL[1],AP[0],AP[1]));
}

int calcPot2(double theta,double fi)//p
{
  return (map(calcBETA(theta,fi),AL[2],AL[3],AP[2],AP[3]));
}
int calcPot3(double zeta)
{
  return (map(zeta,TL[0],TL[1],TTP[0],TTP[1]));
}
void armMasala::boot(int m1Pin[4],int m2Pin[4],int m3Pin[4])
{
  LA1.setPins(m1Pin);
  LA2.setPins(m2Pin);
  TT.setPins(m3Pin);
}
void armMasala::line(double x,double y double z,int NOP,double vel)//3D coordinates...NOP are no of point in the line through which it will move
{
 double x1=calcX();//current coordinates
 double y1=calcY();//current coordinates
 double z1=calcZ();//current coordinates

  double X=x1,Y=y1,Z=z1;
  double lambda=0; 
  for(int i=0;i<NOP;i++)
  {
   X=x1+lambda*(x-x1);
   Y=y1+lambda*(y-y1);
   Z=z1+lambda*(z-z1);
   gotoXY(X,Y,Z,vel);
  lambda=lambda+(1/NOP);
  }
}
void armMasala::gotoXY(double X,double Y,double Z,double vel)// here X,Y,Z are 3D coordinates...
{
  //values of the arguments must be interpreted
 double x,y,z;      //z is angle
 z=atan(Z/X);
 x=X/cos(z);
 y=Y;
 gotoAngle(calcTHETA(x,y),calcFI(x,y),z,vel);//z is angle....
}
double tempvel1,tempvel2;
void armMasala::gotoAngle(double t,double f,double z,double vel) //theta,fi,maxVelocity
{
  double endPot1=calcPot1(t,f),endPot2=calcPot2(t,f),endPot3=calcPot3(z),del1=0,del2=0,del3=0;
  
  bool flag=0;//flag is 1 if given angle is achievable else it is 1
  
  flag= (endPot1>0 && endPot2>0)  ? 1 : 0;
  
  del1=fabs(endPot1-LA1.getPotVal());
  
  while(flag==1 && (fabs(endPot1-LA1.getPotVal())>5 || fabs(endPot2-LA2.getPotVal())>5  ||fabs(endPot3-TT.getPotVal())>5))
  {
    del1=fabs(endPot1-LA1.getPotVal());
    Serial.println("______________________________");
    Serial.print("Del1: ");
    Serial.println(del1);
    
    del2=fabs(endPot2-LA2.getPotVal());
    Serial.print("Del2: ");
    Serial.println(del2);
    
    del3=fabs(endPot3-TT.getPotVal());
    Serial.print("Del3: ");
    Serial.println(del3);
    
    Serial.print("Vel: ");
    Serial.println(vel);
    Serial.println("____________________________");
    //this comparison of the deltas should be refiened afterwards....
    if(del1>del2)
    {
      LA1.setVelocity(vel);
      tempvel1= vel*(del2/(del1));
      LA2.setVelocity(tempvel1);
      tempvel2=vel*(del3/del1);    
      TT.setVelocity(tempvel2);
    }
      
    else
    {
      LA2.setVelocity(vel);
      tempvel1=vel*(del1/(del2));
      LA1.setVelocity(tempvel1);
      tempvel2=vel*(del3/del2);
      TT.setVelocity(tempvel2);
    }
    /* In this case we are comparing only the deltas of actuators but not of the turn table so there is a case that the velocity 
    of the turntable may exceed the limit value */

    Serial.print("End Pot1: ");
    Serial.println(endPot1);
    Serial.print("Current Pot1:");
    Serial.println(LA1.getPotVal());
    Serial.print("vel1:");
    Serial.println(LA1.velocity);
    Serial.println("________________");
    
    Serial.print("End Pot2: ");
    Serial.println(endPot2);
    Serial.print("Current Pot2:");
    Serial.println(LA2.getPotVal());
    Serial.print("vel2:");
    Serial.println(LA2.velocity); 
    Serial.println("_____________");

    Serial.print("End Pot3: ");
    Serial.println(endPot3);
    Serial.print("Current Pot3:");
    Serial.println(TT.getPotVal());
    Serial.print("vel3:");
    Serial.println(TT.velocity); 
    Serial.println("_____________");

    endPot1>LA1.getPotVal()?LA1.runFor():LA1.runBack();
    endPot2>LA2.getPotVal()?LA2.runFor():LA2.runBack();
    endPot3>TT.getPotVal()?TT.runFor():TT.runBack();
  }
   
   TT.pause();
   LA1.pause();
   LA2.pause();

}


int linearActuator1[4]={37,39,9,A1};                 //pin1,pin2,pwm,potPin
int linearActuator2[4]={41,43,10,A0};
int turntable[4]={33,35,11,A2};                                                                                                                                                                                                                                                                                                                                                                          
void setup()
{
 Serial.begin(9600);
 arm.boot(linearActuator1,linearActuator2,turntable);
}
void loop()
{
 Serial.println("Starting");
 arm.gotoAngle(45*0.017453,89*0.017453,0*0.017453,40);
 delay(2000);
 arm.gotoAngle(70*0.017453,90*0.017453,0*0.017453,40); 
 Serial.println("End...");
}