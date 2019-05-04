#include "Fork.h"

Fork::Fork(const char* s,string fileName[])
{
 for(int i=0;i<4;i++){
   pi[i]=0;
   this->fileName[i]=fileName[i];
 }
 while(1){
  if(getGPS2UDP())
    kill(getGPS2UDP(), SIGKILL);
  else
     break;
   }
 system(s);
}

long Fork::getGPS2UDP()
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

void Fork ::transmit(string s , int data, Udp reader){
 	string message = s + to_string(data) + "$";
 	unsigned char * val = (unsigned char *)message.c_str();
	reader.write(val,23907);
}

void Fork::startProcess(char * args[], int a, Udp reader){
  pid_t pid;
  pid =fork();
  pi[a-1]=pid;
  if(pid<0){
    // perror("Fork error");
    transmit("%",a,reader);
   }
  else if(pid==0){
    transmit("@",a,reader);
    execv(args[0],args);
    transmit("?",a,reader);
    exit(EXIT_FAILURE);
  }
   signal(SIGCHLD,SIG_IGN);
}


void Fork::setFile(int data){
              if(data==1){
	             args[0] = (char*)fileName[data-1].c_str();
	             args[1] = NULL;
               }
             else
             { args[0] = (char*)"/usr/bin/python3";
               args[1] = (char*)fileName[data-1].c_str();
               args[2] = NULL;

             }
}

void Fork::killProcess(int data,Udp reader){

  kill(pi[data-1],SIGKILL);
  pi[data-1]=0;
  transmit("~",data,reader);

}
