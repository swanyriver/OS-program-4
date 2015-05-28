/*
 * modSub.c
 *
 *  Created on: May 28, 2015
 *      Author: brandon
 */


char modCrypt(char text, char key){
    return 'A' + ((((text - 'A') + 27) - (key - 'A'))%27);
}
