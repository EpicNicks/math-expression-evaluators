package solver

import (
	"fmt"
	"strconv"
	"unicode"
)

func Tokenize(input string) ([]Token, error) {
	collectedTokens := make([]Token, 0, 10)
	operators := map[rune]bool{
		'+': true,
		'-': true,
		'*': true,
		'x': true,
		'/': true,
		'(': true,
		')': true,
		'^': true,
	}

	inputRunes := []rune(input)
	inputLength := len(inputRunes)

	for i := 0; i < inputLength; i++ {
		if unicode.IsSpace(inputRunes[i]) {
			continue
		}
		if unicode.IsDigit(inputRunes[i]) {
			literal := ""
			decimalPointsCount := 0
			for ; i < inputLength; i++ {
				if inputRunes[i] == '.' {
					literal += string(inputRunes[i])
					decimalPointsCount++
					if decimalPointsCount > 1 {
						return nil, fmt.Errorf("{Tokenizer Error}: second decimal point found at index: [%d]", i)
					}
				} else if unicode.IsDigit(inputRunes[i]) {
					literal += string(inputRunes[i])
					if i+1 >= inputLength || unicode.IsSpace(inputRunes[i+1]) || operators[inputRunes[i+1]] {
						f, err := strconv.ParseFloat(literal, 64)
						if err != nil {
							return nil, fmt.Errorf("{Tokenizer Error}: number ending at index: [%d] was a malformed decimal number, error: %s", i, err.Error())
						}
						collectedTokens = append(collectedTokens, NewLiteralToken(f))
						break
					}
				}
			}
		} else if operators[inputRunes[i]] {
			lastCollectedIsOpToken := false
			var lastCollectedToken OperatorToken
			if len(collectedTokens) > 0 {
				lastCollectedToken, lastCollectedIsOpToken = collectedTokens[len(collectedTokens)-1].(OperatorToken)
			}
			if (inputRunes[i] == '-' || inputRunes[i] == '+') && (i == 0 || lastCollectedIsOpToken && lastCollectedToken.Value != RPAREN) {
				opToken, _ := UnaryFromChar(inputRunes[i])
				collectedTokens = append(collectedTokens, opToken)
			} else {
				opToken, _ := BinaryFromChar(inputRunes[i])
				collectedTokens = append(collectedTokens, opToken)
			}
		} else {
			return nil, fmt.Errorf("{TokenizerError}: unexpected char {input[%d]} received at index [{%d}]", i, i)
		}
	}

	return collectedTokens, nil
}
