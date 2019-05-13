#ifndef __COMPASS_H__
#define __COMPASS_H__

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <iostream>
#include <cmath>
using namespace std;

class Compass{ 

	private:
	        static const int HMC5883L_I2C_ADDR = 0x1E;
		int fd, addr, reg, value, x;
		double in_min, in_max, out_min, out_max;
		unsigned char buf[];	
		
	public:
		void selectDevice(int fd, int addr);
		void writeToDevice(int fd, int reg, int value);
		int compass_setup();
		double maps(double x, double in_min, double in_max, double out_min, double out_max);
		double compass(unsigned char buf[], int fd);	
			
};

#endif
		
