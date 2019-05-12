// obstacleAvoidanceForRover 
//working perfectly Thoeritically...
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <Servo.h>
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 39);

unsigned int localPort = 23909;
EthernetUDP Udp;

double FACTOR = 0.01745329;
long duration;
float distance;

class sonicServo
{
public:
        
    int trigPin = 5;
    int echoPin = 6;
  
    long servoPin;
    long anPin;
    float range[2];
    int noOfReadings = 5;
    float offsetValue = (range[1] - range[0]) / noOfReadings; // value equvalent to 10 deg change
    float offSetTheta = (90 - 0) / noOfReadings;              // angle sweeped between two consecutive readings
    float reading[10];                                        // raw data from the sensor
    float bearing;                                            // angle wrt to front the rover should turn to avoid obstacle
    float clearance = 10;                                     // minimum side distance that rover should malongain
    float clearanceF = 30;                                    // minimum distance rover should keep in front before turning
    float isClear;                                            // sustains the value 1 is side is clear else 0
    float temp;                                               // a temporary veriable to check if side is clear
    float maximumRange = 100;                                 // maximum distance upto which sensor gives accurate reading.
    Servo servo;                                              // object of  Servo
    long checkEachReading(long i);                            //searches for obstacle with front clearance
    long checkRadially(long i);                               //searches for obstacle radially "in a semicircle"
    float ultraRead();                                        //reads the value from sensor.....
};
sonicServo left, right;

long sonicServo::checkRadially(long i)
{
 if (reading[i] < clearanceF)   
    return 0;   
 else      
    return 1;
}

long sonicServo::checkEachReading(long i) // initialisation of servo
{
    if (i == 0)
    { if (reading[i] < clearanceF)
            return 0;
        else
            return 1;
    }
    else
    {
        if (reading[i] * sin(i * offSetTheta * FACTOR) < clearance)
            return 0;
        else
            return 1;
    }
}


float sonicServo::ultraRead()
{
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH); // echo pin is output.
    distance = duration * 0.034 / 2;
    return (distance);
}

void updateHeading()
{
    right.isClear = 0;
    left.isClear = 0;
    for (int i = 0; i < left.noOfReadings; i++)
    {
        right.reading[i] = right.reading[i] < right.clearanceF ? right.reading[i] : ( 10000);
        left.reading[i]  =    left.reading[i] < left.clearanceF ? left.reading[i] : ( 10000);
        if (i == 0)
        {
          right.temp = right.reading[i] < right.clearanceF ? right.temp + 1 : right.temp;
          left.temp = left.reading[i] < left.clearanceF ? left.temp + 1 : left.temp;
        }
        else
        {
          right.temp = right.reading[i] *sin(i *right.offSetTheta*FACTOR) < right.clearance ? right.temp + 1 : right.temp;
          left.temp =   left.reading[i] * sin(i*left.offSetTheta*FACTOR) < left.clearance   ? left.temp + 1  : left.temp;
        }
    }

    right.isClear = right.temp != 0 ? 0 : 1;
    left.isClear  = left.temp  != 0 ? 0 : 1;
    long start = 0, End = right.noOfReadings;
    long startDis, EndDis;
    long flag = 1;
    double fi1, fi2;

    if (left.isClear == 1 && right.isClear == 1)
    {
        Serial.print("everything is clear.");
        left.bearing = 0;
        right.bearing = 0;
    }
    else if (left.isClear == 1 && right.isClear == 0)
    {
        Serial.print("obstacle was detected in right");
        start =right.noOfReadings;
        End=0;
        for (long i = 0, flag = 1; flag == 1 && i <= right.noOfReadings; i++)
        {
            if (right.checkRadially(i) == 0)//checkeachreading() returns 1 if that sensor is clear
            {
                start = i;
                startDis = right.reading[i];
                flag = 0;
                    }
                }

        for (long i = right.noOfReadings, flag = 1; i >= 0 && flag == 1; i--)
        {
            if (right.checkRadially(i) == 0)
            {
                End = i;
                EndDis = right.reading[i];
                flag = 0;
            }
        }
        
        fi1 = start * right.offSetTheta ;
        fi2 = End * right.offSetTheta;

        left.bearing = -1 * (((90 - fi1) - acos((right.clearance) / (startDis))/FACTOR ));
        right.bearing = (fi2 + asin((right.clearance) / (EndDis))/FACTOR) ;
    }
    
    else if (left.isClear == 0 && right.isClear == 1)
    {
     Serial.println("obstacle is detected in left");
     start=left.noOfReadings;
     End=0;
        for (long i = 0, flag = 1; i <= left.noOfReadings && flag == 1; i++)
        {
            if (left.checkRadially(i) == 0)
            {
                start = i;
                startDis = left.reading[i];
                flag = 0;
            }
        }

        for (long i = left.noOfReadings, flag = 1; i >= 0 && flag == 1; i--)
        { 
          if(left.checkRadially(i)==0)//error is here
        {
          End=i;
          EndDis=left.reading[i];
         flag=0;
        }        
        }
        fi1 = start * left.offSetTheta;
        fi2 = End * right.offSetTheta;
      
        right.bearing = (((90 - fi1) - acos((right.clearance) / (startDis))/FACTOR));
        left.bearing = (-1 * (fi2 + asin((right.clearance) / (EndDis))/FACTOR)) ;
    }
    else
    {
        start=left.noOfReadings;
        End=right.noOfReadings;
        Serial.println("obstacle was detected by both left and right sensor");
        for (long i = left.noOfReadings, flag = 1; i >= 0 && flag == 1; i--)
        {
            if (left.checkRadially(i) == 0)
            { 
                start = i;
                startDis = left.reading[i];
                flag = 0;
            }
            else
            start--;
        }

        for (long i = right.noOfReadings, flag = 1; i >= 0 && flag == 1; i--)
        {
            if (right.checkRadially(i) == 0)
            {
                End = i;
                EndDis = right.reading[i];
                flag = 0;

            }
            else
            End--;
            
        }
        fi1 = start * left.offSetTheta;
        fi2 = End * right.offSetTheta;
        
        left.bearing = (-1 * (fi2 + asin((right.clearance) / (EndDis))/FACTOR));
        right.bearing = (fi2 + asin((left.clearance) / (startDis))/FACTOR);
    }
    Serial.println();
    Serial.println();
    Serial.print("Bearing1 : ");
    Serial.print(left.bearing);
    Serial.print("      Bearing2 : ");
    Serial.println(right.bearing);
    char l = (int)left.bearing ; char r = (int)right.bearing;
    Udp.beginPacket("192.168.1.32",3304);
    Udp.write(l+"," + r);
    Udp.endPacket();
}

void loopingblock(long i)
{
    right.servo.write(right.range[1] - (i * right.offSetTheta));
    left.servo.write(left.range[0] + (i * left.offSetTheta));
    Serial.print("\nleft val :");
    Serial.print(left.range[0] + (i * left.offSetTheta));
    Serial.print("      right val :");
    Serial.print(right.range[1] - (i * right.offSetTheta));
    //Serial.println(i);
   
   //accept the readings from the sensor here. 
    delay(300);
    right.reading[i]=right.ultraRead();
    left.reading[i]= left.ultraRead();
}

void setup()
{
    right.range[0] = 45;
    right.range[1] = 135;

    left.range[1] = 45;
    left.range[0] = 135;
    
    left.servoPin = 13;
    right.servoPin = 11;

    
    left.trigPin = 9;
    left.echoPin = 7;
   
    right.trigPin = 5;
    right.echoPin = 3;
    
   pinMode(left.trigPin, OUTPUT);
   pinMode(left.echoPin, INPUT);

   pinMode(right.trigPin, OUTPUT);
   pinMode(right.echoPin, INPUT);

    left.servo.attach(left.servoPin);
    right.servo.attach(right.servoPin);
    
    right.servo.write(right.range[0]);
    left.servo.write(left.range[1]);
    
     Ethernet.begin(mac, ip);
     Udp.begin(localPort);
    Serial.begin(9600);
    delay(4000);
  /*
 IMPORTANT NOTE!!!!!!!!!
 all the readings should be greater that right.clearance=10
 and left.clearance=10 and dont set random values.....
 */
//Hard Coding the readings to test the algorithm......
 //   for (int i = 0; i <= left.noOfReadings; i++)
  //  {
   //     left.reading[i] = 1000;
    //    right.reading[i] = 1000;
   // }
  /*
    right.reading[0] = 16;
    right.reading[1] =290;
    right.reading[2] = 130;
    right.reading[3] = 110;
    right.reading[4] = 150;
    right.reading[5] = 150;
   
  
   left.reading[0] = 100;
   left.reading[1] = 150;
   left.reading[2] = 150;
   left.reading[3] = 25;
   left.reading[4] = 150;
   left.reading[5] = 150;
*/
}

void loop()
{
    long i;
    left.temp = 0;
    right.temp = 0;
    for (i = 1; i <= right.noOfReadings; i++)
    {
        loopingblock(i);

    }
    updateHeading();
  
    left.temp = 0;
    right.temp = 0;

    for (i = right.noOfReadings-1; i >= 0; i--)
    {
        loopingblock(i);
    }

    updateHeading();
}
