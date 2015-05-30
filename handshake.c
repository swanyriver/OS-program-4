/*
 * encHandshake.c
 *
 *  Created on: May 29, 2015
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
#include "packet.h"

int handshake(int sock){
    
#ifdef ENC
    char* enc = "ENCRYPT\n";
    char* crypt = "CXOZOLW\n";
#else
    char* enc = "DECRYPT\n";
    char* crypt = "FVRJHTQ\n";
#endif
    
    char* key = "CRPTKEY\n";
    char buffer[PACKETSIZE];
    strncpy(buffer,crypt,9);
    strncpy(buffer+HALFPACKET,key,9);

    int numWrite = write(sock,buffer,PACKETSIZE);
    int numread = read(sock,buffer,7);
    
    if(strcmp(enc,buffer)==0) return 1;
    else return 0;
}
