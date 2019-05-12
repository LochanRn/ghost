/*This code controls an arm which includes two Linear actuators,
  One power window for the turntable 3 servos for ypr and a dc motor
  for the gripper
  Controls:
         Direction1  Direction2
  LA1     UP         ,DOWN arrows
  LA2     Shift+UP   ,Shift+DOWN arrows
  TT      RIGHT      ,LEFT arrows

          D1      D2
  Gripper I      ,O
  Yaw     J+LEft ,J+Right
  PITCH   K+LEft ,K+Right
  ROLL    L+LEft ,L+Right
*/

#include <Servo.h>
#include <SPI.h>         
#include <Ethernet2.h>
#include <EthernetUdp2.h>


//SPI variables
char data[UDP_TX_PACKET_MAX_SIZE];
int dat=0;
int ss = 53;
bool flag = false;
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 38);
unsigned int localPort = 3303; 
int bits[] = {0, 0, 0, 0, 0, 0, 0, 0};

int LA2[] =  { 43, 45, 9};                      // A, B, PWM short 0-1 up
int LA1[] =  { 35, 36, 7};                      // A, B, PWM long 0-1 up

int GRP[] =  { 39, 41, 8};                      // A, B, PWM grp 0-1 up

float ttfilter = 0.6, a = 0.8;

int s[] = {1500, 1500, 1500};                    //servo controllers
Servo yaw, pitch, roll;                          //Servo variables

int speed = 0, pwm = 120 , grpwm = 230;

int limit  = 50;
int accelUp = 1, accelDown = 1;
int lfilter, gfilter, tt, dir = 0;                       //linear actuator filter, gripper filter

int yawstep = 1, prstep = 1;                     //Servo steps
EthernetUDP Udp;
void setup() {
 Ethernet.begin(mac, ip);
  Udp.begin(localPort);

  // LA outputs
  for (int i = 0; i < 3; i++) {
    pinMode(LA1[i], OUTPUT);
    pinMode(LA2[i], OUTPUT);
    pinMode(GRP[i], OUTPUT);
  }

  Serial.begin(9600);   //Debug
  Serial3.begin(9600);  // Turn Table

  //YPR pins
  yaw.attach(6);
  pitch.attach(5);
  roll.attach(4);
  initial();
  Serial3.write(64);
  //  Serial.println("Setup");
}

void loop() {
int packetSize = Udp.parsePacket();
if (packetSize) {
   memset(data,0,24);
   Udp.read(data, UDP_TX_PACKET_MAX_SIZE);
   dat = int(data[0]);
   process(dat);
  }
}

void process(int input) {
  if (input) {
    updateBits(input);
    if (bits[0] && (bits[6] - bits[7])) {
      speed = (speed >= limit) ? limit : speed + accelUp;
      turntable(speed * (bits[6] - bits[7]));
      dir = (bits[6] - bits[7]);
    }
    
    control(pwm, grpwm);
    ypr();
  }
  else {
    control(0, 0);
    while (speed) {
      speed = (speed < accelDown) ? 0 : speed - accelDown;
      turntable(speed * dir);
    }
  }
}

void control(int pwm, int grpwm) {
  lup();
  lfilter = !bits[0] * (lfilter * a + (1 - a) * pwm);

  act(LA1, (bits[5] && (!bits[2])), (bits[4] && (!bits[2])), lfilter);
  act(LA2, (bits[7] && (!bits[3]) && (!bits[1])), (bits[6] && (!bits[3]) && (!bits[1])), lfilter);

  act(GRP, (bits[1] && bits[2]), (bits[2] && bits[3]), grpwm);
}

void lup(){
  pwm = bits[4]||bits[5]||bits[6]||bits[7]?++pwm:50;
  pwm = pwm>150?150:pwm;
}
int safeservo(int x) {
  if (x > 2300) return 2400;
  else if (x < 800) return 800;
  else return x;
}

void ypr() {
  if (bits[1] && bits[2] && bits[3] && (!bits[0])) {
    initial();
  }
  else if (bits[3]) {
    s[0] += yawstep * (bits[7] - bits[6]);
    s[0] = safeservo(s[0]);
    yaw.writeMicroseconds(s[0]);
  }
  else if (bits[2]) {
    s[1] += prstep * (bits[4] - bits[5]);
    s[2] += prstep * (bits[5] - bits[4]);
    s[1] = safeservo(s[1]);
    s[2] = safeservo(s[2]);
    pitch.writeMicroseconds(s[1]);
    roll.writeMicroseconds(s[2]);
  }
  else if (bits[1]) {
    s[1] += prstep * (bits[7] - bits[6]);
    s[2] += prstep * (bits[7] - bits[6]);
    s[1] = safeservo(s[1]);
    s[2] = safeservo(s[2]);
    pitch.writeMicroseconds(s[1]);
    roll.writeMicroseconds(s[2]);
  }

  //  Serial.print(s[0]);
  //  Serial.print(" ");
  //  Serial.print(s[1]);
  //  Serial.print(" ");
  //  Serial.print(s[2]);
  //  Serial.print(" ");
  //  Serial.println();
}

void turntable(int x) {
  int y = map(x , -100, 100, 1, 127);
  tt = (x) ? tt * ttfilter + (1 - ttfilter) * y : 64;
  //  Serial.print(x);
  //  Serial.print(" ");
  //  Serial.println(tt);
  Serial3.write(tt);
  delay(5);
}

void act(int arr[], boolean A, boolean B, int pwm) {
  digitalWrite(arr[0], A);
  digitalWrite(arr[1], B);
  analogWrite(arr[2], pwm);
//    Serial.print("   ");
//    Serial.print(arr[0]);
//    Serial.print("   ");
//    Serial.print(arr[1]);
//    Serial.print("   ");
//    Serial.print(A);
//    Serial.print("   ");
//    Serial.print(B);
//    Serial.print("   ");
//    Serial.println(pwm);
}

void updateBits(int val) {
  for (int i = 0; i < 8; i++)
    bits[i] = getBit(val, i);
//      display();
}

bool getBit(int n, int pos) {
  return (n >> pos) & 1;
}

void display() {
  for (int i = 0; i < 8; i++) {
    Serial.print(bits[i]);
    Serial.print(" ");
  }
  Serial.println();
}

void initial()
{
  //  Initialize YPR
  //  Serial.println("Intializing YPR");
  s[0] = 1500, s[1] = 1200, s[2] = 1800;
  yaw.writeMicroseconds(s[0]);
  pitch.writeMicroseconds(s[1]);
  roll.writeMicroseconds(s[2]);
}


