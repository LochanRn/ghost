/* This code controls the drive consisting of two motors powered by sabertooth
   two friglie (i.e), Small LA for AA box one DC Motor which provides yaw for
   the analog camera
   Controls
   Drive:
   Forward  :w
   Right    :a
   Backword :s
   Left     :d
            D1        D2
   SLA1     q        ,ctrl + q
   SLA2     e        ,ctrl + e
   Cam      t        ,ctrl + t
*/
#include <Ethernet.h>
#include<EthernetUdp.h>
EthernetUDP Udp;
byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEF};
//byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

IPAddress ip(192, 168, 1, 37);
unsigned int localPort = 23908; 
unsigned char data[UDP_TX_PACKET_MAX_SIZE];
int speed = 0;       //speed variable
int filter = 0.3;    //fileter variable
int limit = 80;
int f_map[] = {0, 0};
int mapper[] = {0, 0};
int pwm = 40;
//Decode and find the direction
int bits[] = {0, 0, 0, 0, 0, 0, 0, 0};

int CAM[] = {45, 44, 43};
int accelUp = 1, accelDown = 1;
bool check = 0;
int dat = 0;
void setup () {

  Ethernet.begin(mac,ip);
 
    for (int i = 0; i < 3; i++) {
    pinMode(CAM[i], OUTPUT);
    
  }

  Serial.begin(9600);
//  Serial2.begin(9600);
   Udp.begin(localPort);
}

void loop () {
  int size = Udp.parsePacket();
if(size)
{
  Udp.read(data,UDP_TX_PACKET_MAX_SIZE);
  Serial.println(data[0]);
//  process(data[0]);
  }
}

void process(int input) {
  if (input) 
  {
    updateBits(input);
    speed = (speed >= limit) ? limit : speed + accelUp;
  //  Serial.println(speed);
    drive(speed, pwm, accelUp);
  } 
  else 
  {
    while (speed > 0) {
      speed = (speed < accelDown) ? 0 : speed - accelDown;
      drive(speed, 0, accelDown);
    }
  }
}
void updateBits(int val) {
  for (int i = 0; i < 8; i++)
    bits[i] = getBit(val, i);
// display();
}

bool getBit(int n, int pos) {
  return (n >> pos) & 1;
}

void drive(int spd, int pwm, int accel) {
  algo(bits[7] - bits[5], bits[4] - bits[6], spd, accel);            //  0    1         1         1       //stop
  act(CAM, abs(bits[0] - bits[1]), bits[0], pwm);               //  1    0         abs(-1)   0
}                                                               //  1    1          0        1


void display() {
 
  for (int i = 0; i < 8; i++) {
    Serial.print(bits[i]);
    Serial.print(" ");
 }
  
  Serial.println();
}

void algo(int a, int b, int spd, int accel) {
  mapper[0] = spd * (a + b);
  mapper[1] = spd * (a - b);

  if (abs(a + b) == 2) {
    mapper[0] /= 2, mapper[1] = mapper[0] / 2;
    check = 1;
  }
  if (abs(a - b) == 2) {
    mapper[1] /= 2, mapper[0] = mapper[1] / 2;
  }

  if ((int)f_map[0] > mapper[0]) mapper[0] = f_map[0] - accel;
  else if ((int)f_map[0] < mapper[0]) mapper[0] = f_map[0] + accel;
  else if ((int)f_map[1] > mapper[1]) mapper[1] = f_map[1] - accel;
  else if ((int)f_map[1] < mapper[1]) mapper[1] = f_map[1] + accel;

  f_map[0] = f_map[0] * filter + (1 - filter) * mapper[0];
  f_map[1] = f_map[1] * filter + (1 - filter) * mapper[1];

  Left(f_map[0]);
  Right(f_map[1]);

}
void Left(int t) {
  int x = map(t, -100, 100, 1, 127); // forward
  command(x);
}

void Right(int t) {
  int x = map(t, -100, 100, 129, 256); // backward
  command(x);
}

void act(int arr[], boolean A, boolean B, int pwm) {
  digitalWrite(arr[0], A);
  digitalWrite(arr[1], B);
  analogWrite(arr[2], pwm);
}
void command(int x) {
  Serial2.write(x);
  delay(5);
  
  
}
