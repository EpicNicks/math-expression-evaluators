#include <stdlib.h>
#include <string.h>
#include "collections/stringbuilder.h"

void strbuilder_append(stringbuilder *strbuilder, char *toappend)
{
    if (strbuilder->count == strbuilder->capacity)
    {
        int newcapacity = strbuilder->capacity >= 1 ? strbuilder->capacity * 2 : 1;
        strbuilder->buffer =
            strbuilder->buffer != NULL
                ? realloc(strbuilder->buffer, sizeof(char) * newcapacity)
                : malloc(sizeof(char) * newcapacity);
        strbuilder->capacity = newcapacity;
    }
    strbuilder->buffer[strbuilder->count++] = *toappend;
}

void freestrbuilder(stringbuilder *strbuilder)
{
    if (strbuilder != NULL)
    {
        free(strbuilder->buffer);
        free(strbuilder);
    }
}

char *tostr(stringbuilder *strbuilder)
{
    char *str = malloc(sizeof(char) * strbuilder->count);
    strncpy(str, strbuilder->buffer, strbuilder->count);
    return str;
}
