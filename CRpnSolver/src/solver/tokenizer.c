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
                strncpy(literaltoken.strValue, tokenstr + startindex, endindex - startindex);
                literaltoken.strValue[endindex - startindex] = '\0';
                tlistadd(tlist, literaltoken);
            }
            else
            {
                token invalid = {
                    .position = startindex,
                    .strValue = malloc(sizeof(char) * (endindex - startindex + 1)),
                    .type = INVALID};
                strncpy(invalid.strValue, tokenstr + startindex, endindex - startindex);
                invalid.strValue[endindex - startindex] = '\0';
                tlistadd(tlist, invalid);
            }
        }
        else if (strchr("+-*/x()^", tokenstr[i]) != NULL)
        {
            token operatortoken = {
                .position = i,
                .strValue = malloc(sizeof(char) * 2),
                .type = OPERATOR};
            operatortoken.strValue[0] = tokenstr[i];
            operatortoken.strValue[1] = '\0';
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