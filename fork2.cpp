#include <netinet/in.h>
#include <sys/socket.h>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <arpa/inet.h>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define R_BUFSIZE 128
#define W_BUFSIZE 256
#define TIMEOUT 0
#define U_TIMEOUT 0

int pi[4]={0,0,0,0};
char * my_args[5];


using namespace std;

class Udp {
    private:
        unsigned char buffer[R_BUFSIZE];
        int sock, bnd;
        struct sockaddr_in rover, ground;
        socklen_t addrlen;
        fd_set stReadFDS;
    public:
        Udp(int port);
        void setclientIP(const char* ip,int port);
        unsigned char* read(int sec,int usec);
        int write(unsigned char *data, int port);
};

Udp::Udp(int port) {
    addrlen = sizeof(ground);
    memset((char *)&rover, 0, sizeof(rover));
    rover.sin_family = AF_INET;
    rover.sin_addr.s_addr = htonl(INADDR_ANY);
    rover.sin_port = htons(port);
    if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        cout << "ERROR: setting socket in UDP" << endl;
    }
    if ((bnd = bind(sock, (struct sockaddr *)&rover, sizeof(rover))) < 0) {
        cout << "ERROR: binding server in UDP" << endl;
    }
}

unsigned char* Udp::read(int sec,int usec) {
    static struct timeval stTimeOut;
    stTimeOut.tv_sec = sec;
    stTimeOut.tv_usec = usec;
    fd_set stReadFDS;
    FD_ZERO(&stReadFDS);
    FD_SET(sock, &stReadFDS);
    if (select(sock + 1, &stReadFDS, NULL, NULL, &stTimeOut) < 0) {
        cout << "ERROR: setting timeout function in UDP" << endl;
    }
    if (FD_ISSET(sock, &stReadFDS) &&
        recvfrom(sock, buffer, R_BUFSIZE, 0, (struct sockaddr *)&ground, &addrlen) > 0) {
        return buffer;
    } else {
        return (unsigned char*)"0";
    }
}

void Udp::setclientIP(const char *ip,int port){
  memset((char *)&ground, 0, sizeof(ground));
  ground.sin_family = AF_INET;
  ground.sin_addr.s_addr = inet_addr(ip);
  ground.sin_port = htons(port);
  if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
      cout << "ERROR: setting socket in UDP" << endl;
  }
}

int Udp::write(unsigned char *data, int port) {
    ground.sin_port = htons(port);
    return sendto(sock, data, W_BUFSIZE, 0, (struct sockaddr*)&ground, sizeof(ground));
}


void transmit(string s , int data, Udp reader){
 	string message = s + to_string(data) + "$";
 	unsigned char * val = (unsigned char *)message.c_str();
	reader.write(val,23907);
}

void startProcess(char * args[], int a, Udp reader){
  pid_t pid;
  
  // string message = "@" + to_string(a+1) + "$";
  // string message1 = "?" + to_string(a+1) + "$";
  //
  // unsigned char * val = (unsigned char *)message.c_str();
  // unsigned char * val1 = (unsigned char *)message1.c_str();

  pid =fork();
   pi[a-1]=pid;
  if(pid<0){
    perror("Fork error");
  }

  else if(pid==0){

    transmit("@",a,reader);
  //  cout<<args[0]<<" "<<args[1]<<" "<<args[2]<<endl;
    execv(args[0],args);

    transmit("?",a,reader);

    exit(EXIT_FAILURE);
  }

    signal(SIGCHLD,SIG_IGN);
}


void setFile(int data){
    switch((int)abs(data)){
        case 1:
	             my_args[0] = (char*)"/home/nvidia/Rudra_19/onboard/controller/index";
	             my_args[1] = NULL;
	             break;
        case 2:
//		    cout<<"Case 2"<<"\n";
                     my_args[0] = (char*)"/usr/bin/python3";
		     my_args[1] = (char*)"/home/nvidia/test/test.py";
	             my_args[2] = NULL;
                     break;
        case 3:
	             my_args[0] = (char*)"/usr/bin/python3";
	             my_args[1] = (char*)"/home/nvidia/live_vidstream/send.py";
	             my_args[2] = NULL;
                     break;
        case 4:
	             my_args[0] = (char*)"./d";
	             my_args[1] = NULL;
                     break;
        default:     
		     return;
                     break;
    }
}

void killProcess(int data,Udp reader){
  kill(pi[data-1],SIGKILL);
  pi[data-1]=0;
  transmit("~",data,reader);
}

long getGPS2UDP()
{
  char line[10];
    char command[] = "pidof -s gps2udp";
  FILE *cmd = popen(command, "r");

	if(cmd == NULL) return 0;
    fgets(line, 10, cmd);
    string a = (string)line;

    pclose(cmd);
	return strtoul(line, NULL, 10);
}

void initGPS2UDP()
{
// kill(getGPS2UDP(), SIGKILL);
 while(1){
  if(getGPS2UDP())
    {kill(getGPS2UDP(), SIGKILL);
    }
  else
    {break;}
 }
 system("gps2udp -j -u 10.4.168.208:23907 -b");
}


int main(void){

  initGPS2UDP();
  Udp reader(3300);

  while(1){

       unsigned char * data = reader.read(1,0);
        int dataRecv=0;
      	if(data[0] == '~'){
          if(data[1]=='-')
            dataRecv = ((int)data[2]-48) * -1;
          else dataRecv = ((int) data[1]-48);
      	}

        if(dataRecv > 0)
          setFile(dataRecv);

        if(dataRecv>=1 && dataRecv<=4){

          if(pi[dataRecv-1]!=0)
		          transmit("!",dataRecv,reader);
          else
              startProcess(my_args,dataRecv,reader);
         }

        else if (dataRecv<=-1 && dataRecv>=-4){

            if(pi[abs(dataRecv)-1]==0)
		            transmit("^",dataRecv,reader);
             else
                killProcess(abs(dataRecv) ,reader);
        }
}
  return 0;
}
