/*
 * client.c
 *
 *  Created on: May 28, 2015
 *      Author: brandon
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>

void error_exit(char* message);

int main(int argc, char **argv) {
    //usage
    //otp_dec ciphertext1 key70000 portnumber

    int socketFD, commSocket, port;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    //open and verify socket
    socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if(socketFD<0) error_exit("Socket Error: Opening");

    server = gethostbyname("localhost");
    if (server == NULL)
        error_exit("localhost not found");

    bzero((void *) &serv_addr, sizeof(serv_addr));
    port = strtol(argv[1],NULL,10);
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(port);

    //connect to server socket
    if (connect(socketFD,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error_exit("error connecting to server socket");

    int readfile = open(argv[2],O_RDONLY);
    char buffer[256];
    bzero(buffer,sizeof(buffer));
    int n;
    do{
        n = read(readfile,buffer,256);
        if(n>0){
            //buffer[n]=0;
            //printf("CLIENT READ[%d]:%s\n",n,buffer);
            write(socketFD,buffer,256);
            sleep(5);
        }
    }while(n);
    close(socketFD);

    return 0;
}

void error_exit(char* message){
   fprintf(stderr,"%s\n",message);
   if(errno){
       const char* error = strerror(errno);
       fprintf(stderr, "%s\n",error);
   }
   exit(1);
}
