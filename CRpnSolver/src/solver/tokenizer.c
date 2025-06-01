#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "collections/tokenlist.h"
#include "solver/tokenizer.h"

token_list *tokenize(const char *tokenstr)
{
    token_list *tlist = make_tlist(10);
    int i = 0;
    while (tokenstr[i] != '\0')
    {
        if (isspace(tokenstr[i]))
        {
            i++;
            continue;
        }
        if (isdigit(tokenstr[i]) || tokenstr[i] == '.')
        {
            int startindex = i;
            int decimalpointscount = 0;
            while (tokenstr[i] != '\0' && isdigit(tokenstr[i]) || tokenstr[i] == '.')
            {
                if (tokenstr[i] == '.')
                {
                    decimalpointscount++;
                }
                i++;
            }
            int endindex = i;

            if (decimalpointscount <= 1)
            {
                token literaltoken = {
                    .position = startindex,
                    .strValue = malloc(sizeof(char) * (endindex - startindex + 1)),
                    .type = LITERAL};
                errno_t err = strncpy_s(literaltoken.strValue,
                                        endindex - startindex + 1,
                                        tokenstr + startindex,
                                        endindex - startindex);
                if (err != 0)
                {
                    // Handle error
                    free(literaltoken.strValue);
                    // You might want to return an error code or throw an exception here
                }
                else
                {
                    literaltoken.strValue[endindex - startindex] = '\0';
                    tlistadd(tlist, literaltoken);
                }
            }
            else
            {
                token invalid = {
                    .position = startindex,
                    .strValue = malloc(sizeof(char) * (endindex - startindex + 1)),
                    .type = INVALID};
                errno_t err = strncpy_s(invalid.strValue,
                                        endindex - startindex + 1,
                                        tokenstr + startindex,
                                        endindex - startindex);
                if (err != 0)
                {
                    // Handle error
                    free(invalid.strValue);
                    // You might want to return an error code or throw an exception here
                }
                else
                {
                    invalid.strValue[endindex - startindex] = '\0';
                    tlistadd(tlist, invalid);
                }
            }
        }
        else if (strchr("+-*/x()^", tokenstr[i]) != NULL)
        {
            token operatortoken;
            if ((tokenstr[i] == '+' || tokenstr[i] == '-') && (tlist->count == 0 || (isoptoken(tlist->tokens[tlist->count - 1]) && tlist->tokens[tlist->count - 1].strValue[0] != ')')))
            {
                operatortoken = (token){
                    .position = i,
                    .strValue = malloc(sizeof(char) * 3),
                    .type = OPERATOR};
                operatortoken.strValue[0] = 'u';
                operatortoken.strValue[1] = tokenstr[i];
                operatortoken.strValue[2] = '\0';
            }
            else
            {
                operatortoken = (token){
                    .position = i,
                    .strValue = malloc(sizeof(char) * 2),
                    .type = OPERATOR};
                operatortoken.strValue[0] = tokenstr[i];
                operatortoken.strValue[1] = '\0';
            }
            tlistadd(tlist, operatortoken);
            i++;
        }
        else
        {
            token invalid = {
                .position = i,
                .strValue = malloc(sizeof(char) * 2),
                .type = INVALID};
            invalid.strValue[0] = tokenstr[i];
            invalid.strValue[1] = '\0';
            tlistadd(tlist, invalid);
            i++;
        }
    }

    return tlist;
}