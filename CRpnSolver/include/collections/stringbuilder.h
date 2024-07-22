#pragma once

typedef struct
{
    char *buffer;
    int capacity;
    int count;
} stringbuilder;

void strbuilder_append(stringbuilder *strbuilder, char *toappend);
void freestrbuilder(stringbuilder *strbuilder);
char *tostr(stringbuilder *strbuilder);