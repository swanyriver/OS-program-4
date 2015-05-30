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
    char* enc = "ENCRYPT\n";
    char* key = "CRPTKEY\n";
    char* crypt = "CXOZOLW\n";
    char buffer[PACKETSIZE];
    strncpy(buffer,crypt,9);
    strncpy(buffer+HALFPACKET,key,9);

    write(sock,buffer,PACKETSIZE);
    int numread = read(sock,buffer,7);
    printf("HANDSHAKE read(%d):%s\n",numread,buffer);

    //todo remove this
    exit(0);

    return 1;
}
