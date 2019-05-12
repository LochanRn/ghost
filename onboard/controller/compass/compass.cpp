#include "compass.h"



double Compass::maps(double x, double in_min, double in_max, double out_min, double out_max){

  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}



void Compass::selectDevice(int fd, int addr){
    if (ioctl(fd, I2C_SLAVE, addr) < 0)
    {
       cout<<"HMC5883L not present"<<endl;
    }
}


void Compass::writeToDevice(int fd, int reg, int value){

    char buf[2];
    buf[0]=reg;
    buf[1]=value;

    if (write(fd, buf, 2) != 2)
    {
       cout<<"Can't write to ADXL345\n";
    }
}

int Compass::compass_setup(){
    int fd;
    if ((fd = open("/dev/i2c-1", O_RDWR)) < 0)
    {
        //Open port for reading and writing
        cout<<"Failed to open i2c bus\n";
        return 1;
    }
    /* initialise ADXL345 */
    selectDevice(fd, HMC5883L_I2C_ADDR);
    writeToDevice(fd, 0x01, 32);
    writeToDevice(fd, 0x02, 0);
    return fd;
}

double Compass::compass(unsigned char buf[], int fd){
  buf[0] = 0x03;

        if ((write(fd, buf, 1)) != 1)
        {
            // Send the register to read from
           cout<<"Error writing to i2c slave\n";
        }

        if (read(fd, buf, 6) != 6) {
           cout<<"Unable to read from HMC5883L\n";
        } else {
            short x = (buf[0] << 8) | buf[1];
            short y = (buf[4] << 8) | buf[5];
            short z = (buf[2] << 8) | buf[3];

            double angle = atan2(y, x) * 180 / M_PI;
	          angle += 96;
            if(angle>360) angle-=360;
            if(angle<0) angle+=360;

            angle=maps(angle,0,360,360,0);

            //cout<<angle<<endl;
            return (int)angle;
        }
}





