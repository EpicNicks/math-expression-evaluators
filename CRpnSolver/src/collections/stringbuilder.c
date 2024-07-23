#include <stdlib.h>
#include <string.h>
#include "collections/stringbuilder.h"

stringbuilder *make_stringbuilder(size_t initialcapacity)
{
    stringbuilder *strbuilder = malloc(sizeof(strbuilder));
    if (initialcapacity < 0)
    {
        initialcapacity = 0;
    }
    strbuilder->buffer = malloc(sizeof(char) * initialcapacity);
    strbuilder->capacity = initialcapacity;
    strbuilder->count = 0;
}

void strbuilder_appendchar(stringbuilder *strbuilder, char toappend)
{
    if (strbuilder == NULL)
        return;

    if (strbuilder->count >= strbuilder->capacity)
    {
        size_t newcapacity = strbuilder->capacity >= 1 ? strbuilder->capacity * 2 : 1;
        char *new_buffer = realloc(strbuilder->buffer, sizeof(char) * (newcapacity + 1));
        if (new_buffer == NULL)
        {
            return;
        }
        strbuilder->buffer = new_buffer;
        strbuilder->capacity = newcapacity;
    }

    strbuilder->buffer[strbuilder->count++] = toappend;
    strbuilder->buffer[strbuilder->count] = '\0';
}

void strbuilder_appendstr(stringbuilder *strbuilder, char *toappend)
{
    if (strbuilder == NULL || toappend == NULL)
        return;

    size_t append_len = strlen(toappend);
    size_t new_count = strbuilder->count + append_len;

    if (new_count >= strbuilder->capacity)
    {
        size_t newcapacity = strbuilder->capacity;
        while (newcapacity <= new_count)
            newcapacity *= 2;

        char *new_buffer = realloc(strbuilder->buffer, sizeof(char) * (newcapacity + 1));
        if (new_buffer == NULL)
            return;

        strbuilder->buffer = new_buffer;
        strbuilder->capacity = newcapacity;
    }

    memcpy(strbuilder->buffer + strbuilder->count, toappend, append_len);
    strbuilder->count = new_count;
    strbuilder->buffer[new_count] = '\0';
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
    if (strbuilder == NULL || strbuilder->count == 0)
        return NULL;

    char *str = malloc(sizeof(char) * (strbuilder->count + 1));
    if (str == NULL)
        return NULL;

    errno_t err = strncpy_s(str, strbuilder->count + 1, strbuilder->buffer, strbuilder->count);
    if (err != 0)
    {
        free(str);
        return NULL;
    }
    str[strbuilder->count] = '\0';
    return str;
}