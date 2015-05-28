/*
 * keygen.c
 *
 *  Created on: May 27, 2015
 *      Author: brandon
 */


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

