/*
 * keygen.c
 *
 *  Created on: May 27, 2015
 *      Author: brandon
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int GetRandomInRange ( int min , int max );

int int main(int argc, char **argv) {
    int keylength = strtol(argv[1],NULL,10);
    for(int i=0; i<)
}

/******************************************************************************
 *    purpose:produce a random number
 *
 *    entry: max >= min
 *
 *    exit: random int in range [min,max]
 *
 *    Written by Brandon Swanson during Summer Term 2014
 *     @ Oregon State University
 ******************************************************************************/
int GetRandomInRange ( int min , int max ) {

    int random;
    /*calculate inclusive range size*/
    int range = max - min + 1;
    if ( range == 1 )
        return min;
    /*map return 0-range onto min through max*/
    random = (rand() % range) + min;
    return random;
}


