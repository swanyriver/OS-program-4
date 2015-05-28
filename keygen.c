/*
 * keygen.c
 *
 *  Created on: May 27, 2015
 *      Author: brandon
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char **argv) {

    /*initialize random*/
    srand( time( NULL ) );

    int random;

    int keylength = strtol(argv[1],NULL,10);
    for(int i=0; i<keylength; ++i){
        random = (rand() % 27);
        if(random==26){
            putchar(' ');
        }
        else{
            putchar('A'+random);
        }
    }

    putchar('\n');

    return 0;
}



