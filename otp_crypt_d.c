/*
 * otp_crypt_d.c
 *
 *  Created on: May 28, 2015
 *      Author: brandon
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <errno.h>
#include <sys/prctl.h>

#include "packet.h"
#include "handshake.h"

void error_exit(char* message);
void communicate(int sock);
int __crypt(char* text, char* key);

int main(int argc, char **argv) {

    if( argc < 2) error_exit("Must provide port number");

    const int NONSENSE = -5;
    const int POOL_SIZE = 5;
    int socketFD, commSocket, port;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clength;

    //open and verify socket
    socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if(socketFD<0) error_exit("Socket Error: Opening");

    //set ports to be reused
    int yes=1;
    setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

    //set up sever addr struct
    port = strtol(argv[1],NULL,10);
    bzero((void *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    //register socket as passive, with cast to generic socket address
    if (bind(socketFD, (struct sockaddr *) &serv_addr,
                  sizeof(serv_addr)) < 0)
        error_exit("Socket Error: Opening");

    listen(socketFD,POOL_SIZE);

    int spawnpid;

    int i = 1;
    for(; ;){
        spawnpid = NONSENSE;

        //begin separate processes
        spawnpid = fork();

        //Unlikely to be reached
        if(spawnpid == NONSENSE){
            fprintf(stderr,"%s","FORK ERROR: fork returned nonsense value");
            exit(1);
        }
        if(spawnpid == -1){
            fprintf(stderr,"%s","FORK ERROR: forking process failed");
            exit(1);
        }

        if ( !spawnpid ) break;

        if(i<POOL_SIZE){
            i++;
        }else{
            //if one of their is a child to be reaped (unexpected but possible)
            //the loop will run again and a new process will be spawned
            int status;
            wait(&status);
        }
    }

    //child processes

    //ensure it dies with parent
    prctl(PR_SET_PDEATHSIG, SIGHUP);

    //become a listening daemon
    while(1){
        commSocket = accept(socketFD, (struct sockaddr *) &cli_addr,
                &clength);

        if(commSocket < 0){
            fprintf(stderr,"[%d] %s\n",getpid(),"error on accept");
            if(errno){
               const char* error = strerror(errno);
               fprintf(stderr, "(%d)%s\n",errno,error);
            }
        } else {
            communicate(commSocket);
        }

        close(commSocket);
    }

}

void communicate(int sock){
    char text[PACKETSIZE];
    int readT;

    do{
        readT = read(sock,text,PACKETSIZE);
        if(readT < 0)
            fprintf(stderr,"[%d] %s\n",getpid(),"error on socket read");
            if(errno){
               const char* error = strerror(errno);
               fprintf(stderr, "%s\n",error);
           }
        else if(readT){
            int out = __crypt(text,text+HALFPACKET);
            write(sock,text,out);
        }

    }while(readT);


}

char modAdd(char text, char key){
    return 'A' + ((text - 'A') + (key - 'A')) % 27;
}
char modSub(char text, char key){
    return 'A' + ((((text - 'A') + 27) - (key - 'A'))%27);
}

int __crypt(char* text, char* key){
    //returns either halfpacket or parital if encouter /n

    char (*modCrypt)(char,char);
#ifdef ENC
    modCrypt = modAdd;
#else
    modCrypt = modSub;
#endif

    int numRead = 0;
    char spaceEncode = 'A' + 26;
    while(numRead < HALFPACKET && *text != '\n'){
        if(*text == ' ') *text = spaceEncode;
        if(*key == ' ') *key = spaceEncode;
        *text = modCrypt(*text,*key);
        if(*text == spaceEncode) *text = ' ';
        ++text;
        ++key;
        ++numRead;
    }
    if(*text == '\n') ++numRead;
    return numRead;
}

void error_exit(char* message){
   fprintf(stderr,"%s\n",message);
   if(errno){
       const char* error = strerror(errno);
       fprintf(stderr, "%s\n",error);
   }
   exit(1);
}

