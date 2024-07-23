#pragma once

typedef struct
{
    char *buffer;
    size_t capacity;
    size_t count;
} stringbuilder;

void strbuilder_appendstr(stringbuilder *strbuilder, char *toappend);
void strbuilder_appendchar(stringbuilder *strbuilder, char toappend);
void freestrbuilder(stringbuilder *strbuilder);
char *tostr(stringbuilder *strbuilder);