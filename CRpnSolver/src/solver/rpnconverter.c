#include <stdio.h>
#include <stdlib.h>

#include "collections/tokenlist.h"

token_list *tokenstorpn(token_list *tlist)
{
    if (tlist == NULL)
        return NULL;

    for (int i = 0; i < tlist->count; i++)
    {
        if (tlist->tokens[i].type == INVALID)
        {
            printf("A token in the input list was of type INVALID\n");
            return NULL;
        }
    }

    token_list *rpnsolvelist = make_tlist(tlist->count);
    if (rpnsolvelist == NULL)
        return NULL;

    token_list *operatorstack = make_tlist(tlist->count);
    if (operatorstack == NULL)
    {
        free_tlist(rpnsolvelist);
        return NULL;
    }

    for (int i = 0; i < tlist->count; i++)
    {
        if (tlist->tokens[i].type == LITERAL)
        {
            tlistadd(rpnsolvelist, tlist->tokens[i]);
        }
        else if (tlist->tokens[i].type == OPERATOR)
        {
            if (tlist->tokens[i].strValue[0] == ')')
            {
                while (operatorstack->count > 0)
                {
                    token optoken = tlistpop(operatorstack);
                    if (optoken.strValue[0] == '(')
                    {
                        break;
                    }
                    tlistadd(rpnsolvelist, optoken);
                }
                if (operatorstack->count == 0)
                {
                    printf("Mismatched parentheses\n");
                    goto cleanup_and_exit;
                }
            }
            else
            {
                while (operatorstack->count > 0 && precedencecompare(tlist->tokens[i], tlisttop(operatorstack)))
                {
                    tlistadd(rpnsolvelist, tlistpop(operatorstack));
                }
                tlistpush(operatorstack, tlist->tokens[i]);
            }
        }
    }
    while (operatorstack->count > 0)
    {
        token optoken = tlistpop(operatorstack);
        if (optoken.strValue[0] == '(')
        {
            printf("Mismatched parentheses\n");
            goto cleanup_and_exit;
        }
        tlistadd(rpnsolvelist, optoken);
    }

    free_tlist(operatorstack);
    return rpnsolvelist;

cleanup_and_exit:
    free_tlist(rpnsolvelist);
    free_tlist(operatorstack);
    return NULL;
}