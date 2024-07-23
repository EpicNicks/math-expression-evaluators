#pragma once
#include <stdbool.h>

enum token_type
{
    LITERAL,
    OPERATOR,
    INVALID
};

typedef enum associativity
{
    LEFT_ASSOCIATIVE,
    RIGHT_ASSOCIATIVE,
    NOT_ASSOCIATIVE
} associativity;

typedef struct token
{
    enum token_type type;
    char *strValue;
    int position;
} token;

typedef struct token_list
{
    token *tokens;
    int capacity;
    int count;
} token_list;

bool isoptoken(token t);
int operatorprecedence(const char *op);
associativity tokenassociativity(token t);
bool precedencecompare(token t1, token t2);

char *tokentostr(const token *t);
char *tokenlisttostr(const token_list *tlist, const char *delim);

token_list *make_tlist(int capacity);
void free_tlist(token_list *tlist);
void tlistadd(token_list *tlist, token t);
token tlistpop(token_list *tlist);
token tlisttop(token_list *tlist);
void tlistpush(token_list *tlist, token t);
bool tlistisempty(token_list *tlist);