package main

import (
	"fmt"
	"os"
	"rpn-solver/solver"
	"strings"
)

func main() {
	argLen := len(os.Args)
	if argLen == 2 {
		CalculateAndPrintSteps(os.Args[1])
	} else if argLen == 1 {
		exampleExpression := "2 - (1/2)^-2 - 3^3"
		fmt.Println("Solve on example expression:")
		CalculateAndPrintSteps(exampleExpression)
	} else {
		fmt.Println("Too many args: Provide an expression as the first argument or no expression to use an example expression.")
	}
}

func CalculateAndPrintSteps(expression string) {
	fmt.Printf("Expression: %s\n", expression)

	tokens, err := solver.Tokenize(expression)
	if err != nil {
		fmt.Println(err)
		return
	}
	tokensString := make([]string, 0, len(tokens))
	for _, token := range tokens {
		tokensString = append(tokensString, token.ToSimpleString())
	}
	fmt.Printf("Tokens:     %s\n", strings.Join(tokensString, " "))

	rpnList, err := solver.BuildRpnFromTokens(tokens)
	if err != nil {
		fmt.Println(err)
		return
	}
	rpnString := make([]string, 0, len(rpnList))
	for _, token := range rpnList {
		rpnString = append(rpnString, token.ToSimpleString())
	}
	fmt.Printf("RPN:        %s\n", strings.Join(rpnString, " "))

	rpnListCopy := append([]solver.Token(nil), rpnList...)
	arithmeticResult, steps, err := solver.SolveRpnOfTokens(rpnListCopy)
	if err != nil {
		fmt.Println(err)
		return
	}
	stepsString := make([]string, 0, len(steps))
	for index, token := range steps {
		separator := ""
		if index%2 == 0 {
			separator = "\n"
		} else {
			separator = "\n\t"
		}
		stepsString = append(stepsString, " ", token, separator)
	}
	fmt.Printf("Steps:\n%s\n", strings.Join(stepsString, ""))
	fmt.Printf("Result:     %f\n", arithmeticResult)
}
