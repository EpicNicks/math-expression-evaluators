package solver

import (
	"errors"
	"fmt"
	"math"
	"rpn-solver/collections"
	"rpn-solver/sliceutils"
	"strings"
)

func BuildRpnFromTokens(tokens []Token) ([]Token, error) {
	operatorStack := collections.NewStack[OperatorToken](10)
	rpnSolveList := make([]Token, 0, 10)

	for _, token := range tokens {
		if opToken, ok := token.(LiteralToken); ok {
			rpnSolveList = append(rpnSolveList, opToken)
		} else if opToken, ok := token.(OperatorToken); ok {
			if opToken.Value == RPAREN {
				once := true
				for op, ok := operatorStack.Pop(); once || ok; op, ok = operatorStack.Pop() {
					once = false
					if operatorStack.IsEmpty() {
						return nil, errors.New("mismatched parentheses")
					}
					if op.Value != LPAREN {
						rpnSolveList = append(rpnSolveList, op)
					} else {
						break
					}
				}
			} else {
				for !operatorStack.IsEmpty() && precedenceCompareRaw(opToken, operatorStack.PeekDangerous()) {
					token, _ := operatorStack.Pop()
					rpnSolveList = append(rpnSolveList, token)
				}
				operatorStack.Push(opToken)
			}
		}
	}
	for token, ok := operatorStack.Pop(); ok; token, ok = operatorStack.Pop() {
		rpnSolveList = append(rpnSolveList, token)
	}

	return rpnSolveList, nil
}

func SolveRpnOfTokens(rpnSolveList []Token) (float64, []string, error) {
	steps := make([]string, 0, 20)
	steps = append(steps, "Showing steps for RPN solve:")

	step := 0
	stepChanged := true

	for i := 0; i < len(rpnSolveList); i++ {
		if opToken, ok := rpnSolveList[i].(OperatorToken); ok {
			if i < opToken.NumOperands {
				return 0, steps, errors.New("{ShuntingYardParser:Solve}: Operator too far left: missing arguments")
			} else {
				if opToken.NumOperands == 2 {
					lhs, lhsOk := rpnSolveList[i-2].(LiteralToken)
					rhs, rhsOk := rpnSolveList[i-1].(LiteralToken)
					if !lhsOk || !rhsOk {
						return 0, steps, errors.New("{ShuntingYardParser:Solve}: Operator arguments were not both literals")
					}
					arithmeticResult := 0.0
					switch opToken.Value {
					case MULTIPLY:
						arithmeticResult = lhs.Value * rhs.Value
					case DIVIDE:
						arithmeticResult = lhs.Value / rhs.Value
					case PLUS:
						arithmeticResult = lhs.Value + rhs.Value
					case MINUS:
						arithmeticResult = lhs.Value - rhs.Value
					case EXPONENT:
						arithmeticResult = math.Pow(lhs.Value, rhs.Value)
					}
					step++
					stepChanged = true
					steps = append(steps, fmt.Sprintf("BINARY OPERATION %s ON %s AND %s => INSERT {%f}", opToken.ToSimpleString(), lhs.ToSimpleString(), rhs.ToSimpleString(), arithmeticResult))

					sliceutils.RemoveAt(&rpnSolveList, i)
					sliceutils.RemoveAt(&rpnSolveList, i-1)
					var genericToken Token = NewLiteralToken(arithmeticResult)
					sliceutils.ReplaceAt(&rpnSolveList, i-2, genericToken)
					i -= 2
				} else if opToken.NumOperands == 1 {
					lhs, lhsOk := rpnSolveList[i-1].(LiteralToken)
					if !lhsOk {
						return 0, steps, fmt.Errorf("{ShuntingYardParser:Parse}: Operator argument for op: {%s} was not a literal", opToken.ToSimpleString())
					}
					arithmeticResult := 0.0
					switch opToken.Value {
					case UNARY_PLUS:
						arithmeticResult = +lhs.Value // yes I know semantically this is a noop
					case UNARY_MINUS:
						arithmeticResult = -lhs.Value
					}
					step++
					stepChanged = true
					steps = append(steps, fmt.Sprintf("UNARY OPERATION %s ON %s => INSERT {%f}", opToken.ToSimpleString(), lhs.ToSimpleString(), arithmeticResult))

					sliceutils.RemoveAt(&rpnSolveList, i)
					var genericToken Token = NewLiteralToken(arithmeticResult)
					sliceutils.ReplaceAt(&rpnSolveList, i-1, genericToken)
					i--
				}
			}
		}

		if stepChanged {
			stepsString := make([]string, 0, len(rpnSolveList))
			for _, token := range rpnSolveList {
				stepsString = append(stepsString, token.ToSimpleString())
			}
			steps = append(steps, fmt.Sprintf("step: %d list: %s", step, strings.Join(stepsString, " ")))
			stepChanged = false
		}
	}

	if litToken, ok := rpnSolveList[0].(LiteralToken); ok {
		return litToken.Value, steps, nil
	}
	return 0, steps, errors.New("rpn solve list did not evaluate to a single literal result")
}

func precedenceCompareRaw(token Token, otherToken Token) bool {
	if opToken, ok := token.(OperatorToken); ok {
		if otherOpToken, _ := otherToken.(OperatorToken); ok {
			return precedenceCompare(opToken, otherOpToken)
		}
	}
	return false
}

func precedenceCompare(opToken OperatorToken, otherToken OperatorToken) bool {
	if otherToken.Associativity == LEFT {
		return opToken.Precedence() <= otherToken.Precedence()
	} else if otherToken.Associativity == RIGHT {
		return opToken.Precedence() < otherToken.Precedence()
	} else {
		return false // must be a paren
	}
}
