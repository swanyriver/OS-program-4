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

void error_exit(char* message){
   fprintf(stderr,"%s\n",message);
   if(errno){
       const char* error = strerror(errno);
       fprintf(stderr, "%s\n",error);
   }
   exit(1);
}



int main(int argc, char **argv) {

    if( argc < 2) error_exit("Must provide port number");

    //to prevent the creation of zombies
    //signal(SIGCHLD,SIG_IGN); todo remove if we stick with the process pool methd

    const int NONSENSE = -5;
    const int POOL_SIZE = 5;
    int socketFD, newsockfd, port;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clength;

    //open and verify socket
    socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if(socketFD<0) error_exit("Socket Error: Opening");

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

    int childcount = 0;

    for(int i=0; i<POOL_SIZE; i++){
        int spawnpid = NONSENSE;

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

        if (spawnpid == 0){
            ///////////////////////////
            ///////child process//////
            //////////////////////////

            //ensure it dies with parent
            prctl(PR_SET_PDEATHSIG, SIGHUP);

        } else {
            ///////////////////////
            ////parent process/////
            ///////////////////////
        }
    }

    while(1){
        //nothing, search for better
        sleep(10000);
    }

}
