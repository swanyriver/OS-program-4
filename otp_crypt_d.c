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
#include "ModuloOperation.h"

void error_exit(char* message);
void communicate(int sock);

int main(int argc, char **argv) {

    if( argc < 2) error_exit("Must provide port number");

    //to prevent the creation of zombies
    //signal(SIGCHLD,SIG_IGN); todo remove if we stick with the process pool methd

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

    listen(socketFD,5);

    int spawnpid;

    int i = 0;
    for(; i<POOL_SIZE; i++){
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
    }

    if(spawnpid){
        close(socketFD);
        while(1){
            //nothing, search for better
            sleep(10000);
        }
    }

    //child processes

    //ensure it dies with parent
    prctl(PR_SET_PDEATHSIG, SIGHUP);

    //become a listening daeomon
    while(1){
        commSocket = accept(socketFD, (struct sockaddr *) &cli_addr,
                &clength);

        if(commSocket < 0){
            fprintf(stderr,"[%d] %s\n",getpid(),"error on accept");
            if(errno){
               const char* error = strerror(errno);
               fprintf(stderr, "%s\n",error);
            }
        } else {
            communicate(commSocket);
        }
        //todo client should close socket, how does server now when its done

        printf("SERVER[%d]:%s\n",getpid(),"socket closed on client side");

        close(commSocket);
    }

}

void communicate(int sock){
    char text[PACKETSIZE+1];
    char key[PACKETSIZE+1];
    bzero(text,sizeof(text));
    bzero(key, sizeof(key));
    int readN;

    do{
        readN = read(sock,text,PACKETSIZE);
        if(readN < 0)
            fprintf(stderr,"[%d] %s\n",getpid(),"error on socket read");
        else if(readN){
            if (text[readN-1]) text[readN]=0;
            printf("SERVEr[%d]:%s\n",getpid(),text);
        }
    }while(readN);


}

void error_exit(char* message){
   fprintf(stderr,"%s\n",message);
   if(errno){
       const char* error = strerror(errno);
       fprintf(stderr, "%s\n",error);
   }
   exit(1);
}

