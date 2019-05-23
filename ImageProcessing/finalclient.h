#ifndef __AUTOBOT_H__
#define __AUTOBOT_H__

#include "../helper/helper.h"
#include <sys/socket.h>
#include <iostream>
#include <cmath>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#define ACCURACY 1.5

class Autobot{
private:
    double latitude,longitude,difference,distance,bearing;
    int Right = 16,Left = 64,Leftdrift = 192,Rightdrift = 144,fwd = 128,stop = 0;
    int err = 10,werr = 20,dat = 0, isAuto = 0;
    int CreateSocket = 0,n = 0, len;
    char dataReceived[1024], dataSending[1024];
    char hello[1024];
    int clintListn = 0, clintConnt = 0;
    struct sockaddr_in ipOfServer, cliaddr;
public:
    int flag=0;
    double destlat,destlon;
    Autobot();
    int update(double heading,Helper H);
    int updateDat(string data, Helper H);
};

#endif
