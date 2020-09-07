//
// Created by d_dab on 15/05/2020.
//

#include "string.h"
#include <assert.h>
#include "helpers.h"
#include <seahorn/seahorn.h>



int charcmp(char* s1, char* s2){
    assume(s1 != NULL);
    assume(s2 != NULL);
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
    assume(s!=NULL);
    int count=0;
    while(*s!='\0'){
        count++;
        s++;
    }
    return count;

}

char* charcpy (char* source) {
    assume(source != NULL);
    int len = charlen(source)+1;
    char* destination = xmalloc((size_t)len);
    sassert(destination > 0);
    char* ptr = destination;
    while (*source != '\0')
    {
        *ptr = *source;
        ptr++;
        source++;
    }
    *ptr='\0';
    sassert(destination != NULL);
    return destination;
}

void freeChar(Element val){
    assume(val != NULL);
    char* str = (char*) val;
    free(str);
}

Element copyChar(Element val){
    assume(val != NULL);
    char* str = (char*) val;
    char* res = charcpy(str);
    sassert(res != NULL);
    return res;
}

int compareChar(Element val1,Element val2){
    assume(val1 != NULL);
    assume(val2 != NULL);
    char* str1 = (char*) val1;
    char* str2 = (char*) val2;
    return charcmp(str1,str2);
}
