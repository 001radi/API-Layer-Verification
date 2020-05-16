//
// Created by d_dab on 15/05/2020.
//

#ifndef PROJECTEMOT_STRING_H
#define PROJECTEMOT_STRING_H

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

int charlen (char* s);

char* charcpy (char* source);

int charcmp(char* s1, char* s2);

void freeChar(Element val);
Element copyChar(Element val);
int compareChar(Element val1,Element val2);
#endif //PROJECTEMOT_STRING_H
