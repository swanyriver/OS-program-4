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
#include "packet.h"

void error_exit(char* message);
void communicate(int sock, int textFD, int keyFD);

int main(int argc, char **argv) {
    //usage //todo check args
    //otp_dec ciphertext1 key70000 portnumber

    //todo compare length of key and text

    //todo check for bad chars in text

    int socketFD, port;
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
    if (connect(socketFD,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
        fprintf(stderr,"Error connecting to server socket on port:%d\n",port);
        if(errno){
           const char* error = strerror(errno);
           fprintf(stderr, "%s\n",error);
        }
        return 2;
    }

    int textFD = open(argv[2],O_RDONLY);
    if(textFD < 1 ) error_exit("Problem opening plaintext file");

    int keyFD = open(argv[3],O_RDONLY);
    if(keyFD < 1 ) error_exit("Problem opening key file");

    if( ! handshake(socketFD) ){
        fprintf(stderr,"Correct server unavailable on port:%d\n",port);
        return 2;
    }

    communicate(socketFD,textFD,keyFD);
    close(socketFD);

    return 0;
}

void communicate(int sock, int textFD, int keyFD){

    char buffer[PACKETSIZE];
    int numRead, keyRead, sockRead;
    int endOfText = 0;

    do{
        numRead=read(textFD,buffer,HALFPACKET);
        //todo error check this
        if(buffer[numRead-1]=='\n')endOfText=1;
        keyRead=read(keyFD,buffer+HALFPACKET,numRead);
        //todo error check this //break if read less than text

        //send text and key pair to server
        write(sock,buffer,PACKETSIZE);

        //recieve *crypted text
        sockRead=read(sock,buffer,numRead);
        //todo break if recireved less than read

        //FOR DEBUG ONLY
        //printf("\nfileRead:%d sockRead:%d\n",numRead,sockRead);

        //output *crypted text
        write(fileno(stdout),buffer,numRead);
        fflush(stdout);

    }while(!endOfText);

}

void error_exit(char* message){
   fprintf(stderr,"%s\n",message);
   if(errno){
       const char* error = strerror(errno);
       fprintf(stderr, "%s\n",error);
   }
   exit(1);
}
