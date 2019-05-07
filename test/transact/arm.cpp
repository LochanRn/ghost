#include "udp/udp.h"
#include "helper/helper.h"
#include "motor/motor.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#define portR 23909

Helper H;

Udp arm(portR);
unsigned char data;
unsigned char *coods;

int LATOP[] = {26, 28, 7, 5};
int LADOWN[] = {22, 24, 6, 3};

int direction[] = {0, 0};
int s[] = {0, 0, 0};

int bits[2][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}};

int speedTT = 0, speedGrp = 0, pwm = 200, limit = 50;
int yawStep = 1, rollStep = 1, pitchStep = 1; //Servo steps
int accel = 1;

float grpfilter, ttfilter = 0.6, a = 0.8;

motor top(LATOP);
motor down(LADOWN);

void saberTooth(int x, int mapperMin, int mapperMax)
{
    int y = H.maps(x, -100, 100, mapperMin, mapperMax);
    //   int tt = (x) ? tt * ttfilter + (1 - ttfilter) * y : (mapperMin + mapperMax)/2;

    cout << "x: " << x << endl;
    //   Serial.print("x:");
    //   Serial.print(x);
    //   Serial.print(" tt:");
    //   Serial.println(x);

    //   Serial3.write(tt);
    //   delay(5);
}

void display()
{
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            cout << bits[i][j] << " ";
            //   Serial.print(bits[i][j]);
            //   Serial.print(" ");
        }
        cout << endl;
        // Serial.println();
    }
    cout << endl;
    //   Serial.println();
}

bool getBit(int n, int pos)
{
    return (n >> pos) & 1;
}

void updateBits(int val, int row)
{
    for (int i = 0; i < 8; i++)
        bits[row][i] = getBit(val, i);
}

void processGrp(int input, int mapperMin, int mapperMax)
{
    if (input * bits[1][3] - bits[1][4])
    {
        direction[1] = bits[1][3] - bits[1][4];
        speedGrp = (speedGrp >= limit) ? limit : speedGrp + accel;
        saberTooth(speedGrp * direction[1], mapperMin, mapperMax);
    }
    while (speedGrp && !input)
    {
        speedGrp = (speedGrp < 0) ? 0 : speedGrp - accel;
        saberTooth(speedGrp * direction[1], mapperMin, mapperMax);
    }
}

void processTT(int input, int mapperMin, int mapperMax)
{

    if ((bits[0][0] * (bits[0][3] - bits[0][4])) && input)
    {
        direction[0] = bits[0][3] - bits[0][4];
        speedTT = (speedTT >= limit) ? limit : speedTT + accel;
        saberTooth(speedTT * direction[0], mapperMin, mapperMax);
    }
    while (speedTT && !input)
    {
        speedTT = (speedTT < 0) ? 0 : speedTT - accel;
        saberTooth(speedTT * direction[0], mapperMin, mapperMax);
    }
}

int safeservo(int x, int Min, int Max)
{
    if (x > Max)
        return Max;
    else if (x < Min)
        return Min;
    else
        return x;
}

void servo(int i, int step, int bit1, int bit2)
{
    s[i] += step * (bit1 - bit2);
    s[i] = safeservo(s[i], 800, 2400);
    cout << i << " " << s[i] << endl;
    // yaw.writeMicroseconds(s[0]);
}

void chotuLogic()
{
    //masala(&pwm1,&pwm2)
    top.act(bits[0][3], bits[0][4], pwm);
    down.act(bits[0][1], bits[0][2], pwm);
}

void actuators()
{
    if (bits[0][7])
    {
        chotuLogic();
    }
    else
    {
        top.act(bits[0][3], bits[0][4], pwm);
        down.act(bits[0][1], bits[0][2], pwm);
    }
}

void goTo(int topfb, int bottomfb)
{
}

void process(unsigned char input[])
{

    updateBits((int)input[0], 0);
    updateBits((int)input[1], 1);

    processTT((int)input[0], 1, 127);
    processGrp((int)input[1], 128, 255);

    if (bits[1][0])
        servo(0, yawStep, bits[0][3], bits[0][4]);
    if (bits[1][1])
        servo(1, rollStep, bits[0][1], bits[0][2]);
    if (bits[1][2])
        servo(2, pitchStep, bits[0][3], bits[0][4]);

    if (!bits[1][0] && !bits[1][1] && !bits[1][2] && !bits[0][0])
        actuators();
    if (bits[1][6])
        ;
}

void loop()
{
    coods = arm.read(1, 0);
    process(coods);
}

int main()
{
    while (1)
        loop();
    return 0;
}
