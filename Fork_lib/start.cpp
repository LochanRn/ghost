#include "Fork/Fork.h"
#include "udp/udp.h"
#include <iostream>
using namespace std;

const char * gps = "gps2udp -j -u 192.168.1.21:23907 -b";

string fileName[3] = {"/home/nvidia/Rudra_19/onboard/controller/index",
                      "/home/nvidia/test/test.py",
                      "/home/nvidia/live_stream/send.py"
                      };

Udp gs(3300);
Fork process(gps,fileName);

unsigned char  *data;
int dataRecv = 0;

int decode(unsigned char * dat){
  if(dat[0] == '~')
     return (dat[1]=='-' ?  ((int)dat[2]-48) * -1 : ((int) dat[1]-48));
}

int main(){

  while(1){
      data = gs.read(1,0);
      dataRecv = decode(data);

      if(dataRecv > 0)
          process.setFile(dataRecv);
      if(dataRecv>=1 && dataRecv<=4){
            if(process.pi[dataRecv-1]!=0)
              process.transmit("!",dataRecv,gs);
            else
              process.startProcess(process.args, dataRecv, gs);
      }
      else if (dataRecv<=-1 && dataRecv>=-4){
            if(process.pi[abs(dataRecv)-1]==0)
                process.transmit("^", dataRecv, gs);
            else
                process.killProcess(abs(dataRecv), gs);
      }
  }
  return 0;
}