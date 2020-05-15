//
// Created by d_dab on 15/05/2020.
//

#ifndef PROJECTEMOT_STRING_H
#define PROJECTEMOT_STRING_H

#include <stdio.h>
#include <stdlib.h>

typedef enum{
    STRING_SUCCESS,
    STRING_MEMORY_ERROR,
}StringResult;

typedef struct string_t* String;

String createString(const char *s);

void deleteString(String s);

int strlen(String s);

int strcpy(String from, String dest);

int strcmp(String s1, String s2);



#endif //PROJECTEMOT_STRING_H
