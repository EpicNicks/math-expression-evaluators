#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "collections/stringbuilder.h"
#include "collections/tokenlist.h"

char *tokentostr(token *t)
{
    if (t == NULL || t->strValue == NULL)
    {
        return NULL;
    }
    int len = strlen(t->strValue);
    char *ret = malloc(sizeof(char) * (3 + len));

    ret[0] = t->type == INVALID ? '<' : '{';
    strcpy(ret + 1, t->strValue);
    ret[len + 1] = t->type == INVALID ? '>' : '}';
    ret[len + 2] = '\0';

    return ret;
}

token_list *make_tlist(int capacity)
{
    token_list *t = malloc(sizeof(token_list));

    t->tokens = capacity > 0 ? malloc(sizeof(token) * capacity) : NULL;
    t->capacity = capacity;
    t->count = 0;

    return t;
}

void free_tlist(token_list *tlist)
{
    if (tlist)
    {
        free(tlist->tokens);
        free(tlist);
    }
}

char *tokenlisttostr(const token_list *tlist, const char *delim)
{
    if (tlist == NULL)
    {
        return NULL;
    }
    if (delim == NULL)
    {
        delim = "";
    }

    if (tlist->count == 0)
    {
        return _strdup("");
    }

    stringbuilder *sb = malloc(sizeof(stringbuilder));
    if (sb == NULL)
    {
        return NULL;
    }
    sb->buffer = NULL;
    sb->count = 0;
    sb->capacity = 0;

    for (int i = 0; i < tlist->count; i++)
    {
        char *token_str = tokentostr(&tlist->tokens[i]);
        if (token_str == NULL)
        {
            freestrbuilder(sb);
            return NULL;
        }

        for (int j = 0; token_str[j] != '\0'; j++)
        {
            strbuilder_append(sb, (char *)&token_str[j]);
        }
        free(token_str);

        if (i < tlist->count - 1)
        {
            for (int j = 0; delim[j] != '\0'; j++)
            {
                strbuilder_append(sb, (char *)&delim[j]);
            }
        }
    }

    strbuilder_append(sb, (char *)"\0"); // Null-terminate the string

    char *result = tostr(sb);
    freestrbuilder(sb);

    return result;
}

void tlistadd(token_list *tlist, token t)
{
    if (tlist->count == tlist->capacity)
    {
        int newcapacity = tlist->capacity >= 1 ? tlist->capacity * 2 : 1;
        tlist->tokens =
            tlist->tokens != NULL
                ? realloc(tlist->tokens, sizeof(token) * newcapacity)
                : malloc(sizeof(token) * newcapacity);
        tlist->capacity = newcapacity;
    }
    tlist->tokens[tlist->count++] = t;
}

token tlistpop(token_list *tlist)
{
    if (tlist->count == 0)
    {
        return (token){.type = INVALID, .strValue = NULL, .position = 0};
    }
    return tlist->tokens[--tlist->count];
}

void tlistpush(token_list *tlist, token t)
{
    tlistadd(tlist, t);
}

bool tlistisempty(token_list *tlist)
{
    return tlist->count == 0;
}
