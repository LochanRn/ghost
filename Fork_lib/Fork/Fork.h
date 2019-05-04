#ifndef __FORK_H__
#define __FORK_H__
#include <iostream>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "../udp/udp.h"
using namespace std;

class Fork {
        string fileName[4];
    public:
        Fork(const char* s,string fileName[]);
        int pi[4];
        char * args[5];
        long getGPS2UDP();
        void transmit(string s, int data, Udp reader);
        void startProcess(char * args[], int index, Udp reader);
        void setFile(int index);
        void killProcess(int index, Udp reader);

};

#endif
