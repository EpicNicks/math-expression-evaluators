#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "solver/rpnsolver.h"
#include "collections/tokenlist.h"

double rpnsolve(token_list *rpntokens, bool printsteps)
{
    if (rpntokens == NULL)
        return 0;

    for (int i = 0; i < rpntokens->count; i++)
    {
        if (rpntokens->tokens[i].type == INVALID)
        {
            printf("A token in the input list was of type INVALID\n");
            return 0;
        }
    }

    double result = 0;
    token_list *solvestack = make_tlist(rpntokens->count);

    for (int i = 0; i < rpntokens->count; i++)
    {
        token curtoken = rpntokens->tokens[i];
        if (curtoken.type == LITERAL)
        {
            tlistpush(solvestack, curtoken);
        }
        else if (curtoken.type == OPERATOR)
        {
            if (numoperands(curtoken) == 2)
            {
                token rhs = tlistpop(solvestack);
                token lhs = tlistpop(solvestack);
                if (lhs.type != LITERAL || rhs.type != LITERAL)
                {
                    printf("Operator arguments were not both literals. lhs: %s rhs: %s.\n", lhs.strValue, rhs.strValue);
                    goto cleanup_and_exit;
                }

                double arithmeticresult;
                if (strcmp(curtoken.strValue, "+") == 0)
                    arithmeticresult = atof(lhs.strValue) + atof(rhs.strValue);
                else if (strcmp(curtoken.strValue, "-") == 0)
                    arithmeticresult = atof(lhs.strValue) - atof(rhs.strValue);
                else if (strcmp(curtoken.strValue, "x") == 0 || strcmp(curtoken.strValue, "*") == 0)
                    arithmeticresult = atof(lhs.strValue) * atof(rhs.strValue);
                else if (strcmp(curtoken.strValue, "/") == 0)
                {
                    double rhsdvalue = atof(rhs.strValue);
                    if (rhsdvalue == 0)
                    {
                        printf("Attempted divide by zero: lhs: %s, rhs: %s.\n", lhs.strValue, rhs.strValue);
                        goto cleanup_and_exit;
                    }
                    arithmeticresult = atof(lhs.strValue) / atof(rhs.strValue);
                }
                else if (strcmp(curtoken.strValue, "^") == 0)
                    arithmeticresult = pow(atof(lhs.strValue), atof(rhs.strValue));
                else
                {
                    printf("Operator with invalid strValue for 2 operands provided: %s.\n", curtoken.strValue);
                    goto cleanup_and_exit;
                }
                int resultstrsize = snprintf(NULL, 0, "%f", arithmeticresult) + 1;
                char *resultstr = malloc(resultstrsize);
                snprintf(resultstr, resultstrsize, "%f", arithmeticresult);
                tlistpush(solvestack, (token){.type = LITERAL, .strValue = resultstr});
                if (printsteps)
                {
                    printf("\tBINARY OPERATION %s ON %s AND %s => INSERT %s\n", curtoken.strValue, lhs.strValue, rhs.strValue, resultstr);
                }
            }
            else if (numoperands(curtoken) == 1)
            {
                token operand = tlistpop(solvestack);
                if (operand.type != LITERAL)
                {
                    printf("Operand was not a literal. operand: %s.\n", operand.strValue);
                    goto cleanup_and_exit;
                }
                double arithmeticresult;
                if (strcmp(curtoken.strValue, "u+") == 0)
                {
                    arithmeticresult = +atof(operand.strValue);
                }
                else if (strcmp(curtoken.strValue, "u-") == 0)
                {
                    arithmeticresult = -atof(operand.strValue);
                }
                else
                {
                    printf("Operator with invalid strValue for 1 operand provided: %s.\n", curtoken.strValue);
                    goto cleanup_and_exit;
                }
                int resultstrsize = snprintf(NULL, 0, "%f", arithmeticresult) + 1;
                char *resultstr = malloc(resultstrsize);
                snprintf(resultstr, resultstrsize, "%f", arithmeticresult);
                tlistpush(solvestack, (token){.type = LITERAL, .strValue = resultstr});
                if (printsteps)
                {
                    printf("\tUNARY OPERATION %s ON %s => INSERT %s\n", curtoken.strValue, operand.strValue, resultstr);
                }
            }
            else
            {
                printf("invalid operator: %s at: %d.\n", curtoken.strValue, curtoken.position);
            }
        }
    }

    if (solvestack->count == 1)
    {
        char *endptr;
        result = strtod(tlisttop(solvestack).strValue, &endptr);
    }
cleanup_and_exit:
    free_tlist(solvestack);
    return result;
}