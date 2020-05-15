//
// Created by d_dab on 15/05/2020.
//

#include "string.h"
#include <assert.h>

struct string_t{
    char* data;
    int length;
};

int charlen (const char* s);
void charcpy (const char* source, char* destination);

String  createString(const char *source){
    String s2;
    s2 = malloc(sizeof(String));
    if(!(s2)){
        return NULL;
    }
    s2->length = charlen(source);
    s2->data = malloc((size_t)s2->length);
    if(!(s2->data)){
        free(s2);
        return NULL;
    }
    charcpy(source,s2->data);
    // verf
    assert(s2 != NULL);
    assert(s2->data != NULL);
    return s2;
}

void deleteString(String s){
    //verf
    assert(s != NULL);
    free(s->data);
    free(s);
}

int strlen(String s){
    //verf
    assert(s != NULL);
    return s->length;
}

int strcpy(String source, String dest){
    //verf
    assert(source != NULL);
    assert(dest != NULL);
    assert(source->length == dest->length);

    charcpy(source->data, dest->data);
}

int strcmp(String s1, String s2){
    assert(s1 != NULL);
    assert(s2 != NULL);
    char* s1_ptr = s1->data ;
    char* s2_ptr = s2->data;
    while(*s1_ptr != '\0' && *s2_ptr != '\0'){
        if(*s1_ptr < *s2_ptr){
            return 1;
        }
        else if(*s1_ptr > *s2_ptr){
            return -1;
        }
        s1_ptr++;
        s2_ptr++;
    }
    return *s2_ptr - *s1_ptr;
}


//helpers:
int charlen (const char* s){
    //verf
    assert(s!=NULL);
    int count=0;
    while(*s!='\0'){
        count++;
        s++;
    }
    return count;

}

void charcpy (const char* source, char* destination) {

    while (*source != '\0')
    {
        *destination = *source;
        destination++;
        source++;
    }
    destination='\0';
}