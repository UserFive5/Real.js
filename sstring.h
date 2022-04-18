#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
char *concat(char **table,char *con,int size) {
    char *res = malloc(200 * sizeof(char));
    for ( int _l = 0; _l <= size; _l++) {
        if ( _l != size ) sprintf(res,"%s%s%s",res,table[_l],con);
        else sprintf(res,"%s%s",res,table[_l]);
    }
    return res;
}

char** split(char *string,char *spl) {
    char **res = malloc(200 * sizeof(char**));
    res[0] = strtok(string,spl);
    int length = 1;
    char *temp = malloc(200 * sizeof(char));
    while ( true ) {
        temp = strtok(NULL,spl);
        if ( temp != NULL ) {
          res[length] = temp;
          length++;
        }
        else break;
    }
    return res;
}

bool check(char *item,char **argv) {
   char *_item ;
   while ( (_item = *argv++) ) {
       if ( strcmp(_item,item) ) {
           break;
           return true;
       }
   }
}
