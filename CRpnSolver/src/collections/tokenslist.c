#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "collections/stringbuilder.h"
#include "collections/tokenlist.h"

bool isoptoken(token t)
{
    if (t.strValue == NULL)
        return false;
    if (
        strcmp(t.strValue, "+") == 0 ||
        strcmp(t.strValue, "-") == 0 ||
        strcmp(t.strValue, "*") == 0 ||
        strcmp(t.strValue, "x") == 0 ||
        strcmp(t.strValue, "/") == 0 ||
        strcmp(t.strValue, "^") == 0 ||
        strcmp(t.strValue, "(") == 0 ||
        strcmp(t.strValue, ")") == 0 ||
        strcmp(t.strValue, "u+") == 0 ||
        strcmp(t.strValue, "u-") == 0)
    {
        return true;
    }
    return false;
}

int operatorprecedence(const char *op)
{
    if (op == NULL)
        return -1; // Invalid input

    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0)
        return 1;
    if (strcmp(op, "*") == 0 || strcmp(op, "/") == 0 || strcmp(op, "x") == 0)
        return 2;
    if (strcmp(op, "^") == 0)
        return 3;
    if (strcmp(op, "u+") == 0 || strcmp(op, "u-") == 0)
        return 4;
    if (strcmp(op, "(") == 0 || strcmp(op, ")") == 0)
        return 5;

    return 0; // Unknown operator
}

associativity tokenassociativity(token t)
{
    if (t.strValue == NULL)
        return NOT_ASSOCIATIVE;
    if (
        strcmp(t.strValue, "+") == 0 ||
        strcmp(t.strValue, "-") == 0 ||
        strcmp(t.strValue, "*") == 0 ||
        strcmp(t.strValue, "x") == 0 ||
        strcmp(t.strValue, "/") == 0 ||
        strcmp(t.strValue, "u+") == 0 ||
        strcmp(t.strValue, "u-") == 0)
    {
        return LEFT_ASSOCIATIVE;
    }
    else if (strcmp(t.strValue, "^") == 0)
    {
        return RIGHT_ASSOCIATIVE;
    }
    return NOT_ASSOCIATIVE;
}

bool precedencecompare(token t1, token t2)
{
    int p1 = operatorprecedence(t1.strValue);
    int p2 = operatorprecedence(t2.strValue);
    associativity a1 = tokenassociativity(t1);

    if (a1 == LEFT_ASSOCIATIVE)
    {
        return p1 <= p2;
    }
    else if (a1 == RIGHT_ASSOCIATIVE)
    {
        return p1 < p2;
    }
    else // NOT_ASSOCIATIVE (parentheses)
    {
        return false;
    }
}

char *tokentostr(const token *t)
{
    if (t == NULL || t->strValue == NULL)
    {
        return NULL;
    }

    size_t len = strlen(t->strValue);
    char *ret = malloc(sizeof(char) * (3 + len + 1));

    if (ret == NULL)
    {
        return NULL;
    }

    char open_char = t->type == INVALID ? '<' : '{';
    char close_char = t->type == INVALID ? '>' : '}';

    int written = snprintf(ret, len + 3 + 1, "%c%s%c", open_char, t->strValue, close_char);

    if (written < 0 || (size_t)written >= len + 3 + 1)
    {
        free(ret);
        return NULL;
    }

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
            strbuilder_appendchar(sb, token_str[j]);
        }
        free(token_str);

        if (i < tlist->count - 1)
        {
            for (int j = 0; delim[j] != '\0'; j++)
            {
                strbuilder_appendchar(sb, delim[j]);
            }
        }
    }

    strbuilder_appendchar(sb, '\0'); // Null-terminate the string

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
    if (tlist == NULL || tlist->count == 0 || tlist->tokens == NULL)
    {
        return (token){.type = INVALID, .strValue = NULL, .position = 0};
    }

    tlist->count--;
    token popped = tlist->tokens[tlist->count];

    tlist->tokens[tlist->count] = (token){.type = INVALID, .strValue = NULL, .position = 0};

    return popped;
}

token tlisttop(token_list *tlist)
{
    if (tlist == NULL || tlist->count == 0)
    {
        return (token){.type = INVALID, .strValue = NULL, .position = 0};
    }
    return tlist->tokens[tlist->count - 1];
}

void tlistpush(token_list *tlist, token t)
{
    tlistadd(tlist, t);
}

bool tlistisempty(token_list *tlist)
{
    return tlist == NULL || tlist->count == 0;
}
