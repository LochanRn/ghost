#include "Fork/Fork.h"
#include "udp/udp.h"
#include <iostream>
using namespace std;

const char * gps = "gps2udp -j -u 10.4.168.208:3300 -b";
string fileName[3] = {"/home/nvidia/Rudra_19/onboard/controller/index",
                      "/home/nvidia/test/test.py",
                      "/home/nvidia/live_stream/send.py"
                      };
Udp reader(3300);
Fork process(gps,fileName);

unsigned char * data;
int dataRecv = 0;

int decode(unsigned char * data){
  if(data[0] == '~')
     return (data[1]=='-' ?  ((int)data[2]-48) * -1 : ((int) data[1]-48));
}

int main()
{
while(1){
    data = reader.read(1,0);
    dataRecv = decode(data);

    if(dataRecv > 0)
         process.setFile(dataRecv);
    if(dataRecv>=1 && dataRecv<=4){
          if(process.pi[dataRecv-1]!=0)
             process.transmit("!",dataRecv,reader);
          else
             process.startProcess(process.args, dataRecv, reader);
      }
    else if (dataRecv<=-1 && dataRecv>=-4){
          if(process.pi[abs(dataRecv)-1]==0)
              process.transmit("^", dataRecv, reader);
          else
              process.killProcess(abs(dataRecv), reader);
      }
 }
return 0;
}
