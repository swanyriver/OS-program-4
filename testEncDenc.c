/*
 * testEncDenc.c
 *
 *  Created on: May 27, 2015
 *      Author: brandon
 */


#include <stdio.h>
#include <stdlib.h>


void ENCODE(char* key, char* text, size_t len){
    int read = 0;
    while(read < len && *text != '\n'){
        if(*text == ' ') *text = '['; //'A' + 26;
        if(*key == ' ') *key = '[';  //'A' + 26;
        *text = 'A' + ((*text - 'A') + (*key - 'A')) % 27;
        if(*text == '[') *text = ' ';
        ++text;
        ++key;
        ++len;
    }
}


int main(void)
{
    FILE *keyFD,*cryptFD;
    char *key = NULL;
    char *text = NULL;
    size_t len = 0;
    ssize_t read;

    keyFD = fopen("key", "r");
    if (keyFD == NULL)
       exit(EXIT_FAILURE);

    cryptFD = fopen("plaintext1", "r");
    if (cryptFD == NULL)
       exit(EXIT_FAILURE);

    read = getline(&key, &len, keyFD);
    read = getline(&text, &len, cryptFD);

    printf("key:%s",key);
    printf("plaintext1:%s",text);

    ENCODE(key,text,len);

    printf("encoded text:%s",text);

    free(key);
    free(text);
    fclose(keyFD);
    exit(EXIT_SUCCESS);


    //boiler plate getline
    /*
     * FILE *stream;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    stream = fopen("/etc/motd", "r");
    if (stream == NULL)
       exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, stream)) != -1) {
       printf("Retrieved line of length %zu :\n", read);
       printf("%s", line);
    }

    free(line);
    fclose(stream);
    exit(EXIT_SUCCESS);*/
    //end of getline boiler
}
