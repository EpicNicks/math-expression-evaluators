#pragma once

typedef struct
{
    char *buffer;
    size_t capacity;
    size_t count;
} stringbuilder;

stringbuilder *make_stringbuilder(size_t initialcapacity);
void strbuilder_appendstr(stringbuilder *strbuilder, char *toappend);
void strbuilder_appendchar(stringbuilder *strbuilder, char toappend);
void freestrbuilder(stringbuilder *strbuilder);
char *tostr(stringbuilder *strbuilder);