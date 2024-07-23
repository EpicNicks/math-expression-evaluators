#include <stdio.h>
#include <stdlib.h>

#include "collections/tokenlist.h"
#include "solver/tokenizer.h"
#include "solver/rpnconverter.h"
#include "solver/rpnsolver.h"

void solveandprintsteps(const char *input)
{
    token_list *tlist = tokenize(input);
    if (tlist == NULL)
    {
        return;
    }
    char *tokensstr = tokenlisttostr(tlist, " ");
    if (tokensstr == NULL)
    {
        return;
    }
    printf("token list: %s\n", tokensstr);
    free(tokensstr);

    token_list *rpnlist = tokenstorpn(tlist);
    char *rpnstr = tokenlisttostr(rpnlist, " ");
    if (rpnstr == NULL)
    {
        return;
    }
    printf("rpn of tokens: %s\n", rpnstr);
    free(rpnstr);

    double rpnsolveresult = rpnsolve(rpnlist, true);
    printf("result: %f", rpnsolveresult);

    // cleanup
    free(tlist);
    free(rpnlist);
}

int main(int argc, const char *argv[])
{
    if (argc == 1)
    {
        const char examplestr[] = "22 + -31 ^ 3";
        printf("Example solve on %s\n", examplestr);
        solveandprintsteps(examplestr);
    }
    else if (argc == 2)
    {
        // do solve on argv[1]
        if (argv[1] != NULL)
        {
            solveandprintsteps(argv[1]);
        }
    }
    else
    {
        printf("Invalid usage: too many parameters. Please enter one parameter to solve in quotes ex: \"1 + -2 * 3 ^ (4/5)\" or no parameters for an example solve.\n");
    }
}