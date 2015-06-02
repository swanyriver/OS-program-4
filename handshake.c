/*
 * handshake.c
 *
 *  Created on: May 29, 2015
 *      Author: Brandon Swanson
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

//determine if connected to appropriate server 
//making use of same encryption/decryption protocol
int handshake(int sock){
    
    //variable compilation by flag ENC
    //pre encrypted/decrypted sequences
#ifdef ENC
    char* enc = "ENCRYPT\n";
    char* crypt = "CXOZOLW\n";
#else
    char* enc = "DECRYPT\n";
    char* crypt = "FVRJHTQ\n";
#endif
    
    //prepare text/key packet
    char* key = "CRPTKEY\n";
    char buffer[PACKETSIZE];
    strncpy(buffer,crypt,9);
    strncpy(buffer+HALFPACKET,key,9);

    //get processed text from connected server
    int numWrite = write(sock,buffer,PACKETSIZE);
    int numread = read(sock,buffer,8);
    
    //encryption server will return ENCRYPT
    //decryption server will return DECRYPT
    if(strcmp(enc,buffer)==0) return 1;

    //wrong operation was performed return FALSE
    else return 0;
}
