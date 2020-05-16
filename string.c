//
// Created by d_dab on 15/05/2020.
//

#include "string.h"
#include <assert.h>



int charcmp(char* s1, char* s2){
    assert(s1 != NULL);
    assert(s2 != NULL);
    while(*s1 != '\0' && *s2 != '\0'){
        if(*s1 < *s2){
            return 1;
        }
        else if(*s1 > *s2){
            return -1;
        }
        s1++;
        s2++;
    }
    return *s2 - *s1;
}

int charlen (char* s){
    assert(s!=NULL);
    int count=0;
    while(*s!='\0'){
        count++;
        s++;
    }
    return count;

}

char* charcpy (char* source) {
    assert(source != NULL);
    int len = charlen(source)+1;
    char* destination = malloc((size_t)len);
    if(!destination){
        return NULL;
    }
    while (*source != '\0')
    {
        *destination = *source;
        destination++;
        source++;
    }
    destination='\0';
    return destination;
}

void freeChar(Element val){
    assert(val != NULL);
    char* str = (char*) val;
    free(str);
}

Element copyChar(Element val){
    assert(val != NULL);
    char* str = (char*) val;
    return charcpy(str);
}

int compareChar(Element val1,Element val2){
    assert(val1 != NULL);
    assert(val2 != NULL);
    char* str1 = (char*) val1;
    char* str2 = (char*) val2;
    return charcmp(str1,str2);
}
