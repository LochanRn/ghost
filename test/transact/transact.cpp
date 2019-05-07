#include "udp/udp.h"
#include "helper/helper.h"
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

Udp gs(portroverR);
Udp drive(portDriveR);
Udp arm(portArmR);

Helper H;

int isAuto = -1;

unsigned char data;
unsigned char *coods;

double heading = 0;

string message = "";

unsigned char *value;
unsigned char controlsARM[2] = {(int)0, (int)0}, controlsDRIVE[2] = {(int)0, (int)0};

void kill()
{
    cout << "Killed" << endl;
    controlsDRIVE[0] = (int)0, controlsDRIVE[1] = (int)0;
    controlsARM[0] = (int)0, controlsARM[1] = (int)0;

    // drive.write(controlsDRIVE, portDriveW);
    arm.write(controlsARM, portArmW);
}

void setup()
{
    drive.setclientIP("192.168.1.34", portDriveW);
    arm.setclientIP("192.168.1.34", portArmW);

    isAuto = -1;

    kill();
}

void Compass()
{
    // unsigned char buff[16];
    message = "$," + to_string(heading) + ",!";
    value = (unsigned char *)message.c_str();
    gs.write(value, portroverW);
}

void keyboard(unsigned char *coods)
{
    static int i, ARM[2] = {0, 0}, DRIVE[2] = {0, 0};
    Compass();
    controlsARM[2], controlsDRIVE[2];
    for (i = 0; coods[i] != '>'; i++)
    {
        DRIVE[0] = (coods[i] == ',') ? i : DRIVE[0];
        DRIVE[1] = (coods[i] == '!') ? i : DRIVE[1];
        ARM[0] = (coods[i] == ';') ? i : ARM[0];
    }
    controlsDRIVE[0] = H.parse(coods, 1, DRIVE[0]);
    controlsDRIVE[1] = H.parse(coods, DRIVE[0] + 1, DRIVE[1]);
    controlsARM[0] = H.parse(coods, DRIVE[1] + 1, ARM[0]);
    controlsARM[1] = H.parse(coods, ARM[0] + 1, i);

    cout << " " << (int)controlsDRIVE[0] << " " << (int)controlsDRIVE[1] << " " << (int)controlsARM[0] << " " << (int)controlsARM[1] << endl;

    //   drive.write(controlsDRIVE,portDriveW);
    arm.write(controlsARM, portArmW);
}

void check(unsigned char *coods)
{
    // cout<<coods<<endl;
    isAuto = (coods[0] == '$') ? 2 : isAuto;
    isAuto = (coods[0] == '#') ? 1 : isAuto;
    isAuto = (coods[0] == '<') ? 0 : isAuto;
    if (isAuto == 0)
    {
        // cout << "Keyboard running" <<coods<< endl;
        keyboard(coods);
    }
}

void loop()
{
    coods = gs.read(1, 0);
    if (coods[0] != '0')
    {
        check(coods);
    }
    else
        kill();
}

int main()
{
    setup();
    while (1)
        loop();
    return 0;
}
