#pragma once
#include <stdbool.h>

enum token_type
{
    LITERAL,
    OPERATOR,
    INVALID
};

typedef struct
{
    enum token_type type;
    char *strValue;
    int position;
} token;

typedef struct
{
    token *tokens;
    int capacity;
    int count;
} token_list;

char *tokentostr(token *t);
char *tokenlisttostr(const token_list *tlist, const char *delim);

token_list *make_tlist(int capacity);
void free_tlist(token_list *tlist);
void tlistadd(token_list *tlist, token t);
token tlistpop(token_list *tlist);
void tlistpush(token_list *tlist, token t);
bool tlistisempty(token_list *tlist);