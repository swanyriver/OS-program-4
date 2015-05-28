/*
 * modAdd.c
 *
 *  Created on: May 28, 2015
 *      Author: brandon
 */


char modCrypt(char text, char key){
    return 'A' + ((text - 'A') + (key - 'A')) % 27;
}
