package solver

import (
	"fmt"
)

// Token interface
type Token interface {
	ToSimpleString() string
}

// LiteralToken struct
type LiteralToken struct {
	Value float64
}

func NewLiteralToken(value float64) LiteralToken {
	return LiteralToken{Value: value}
}

func NewLiteralTokenAsBase(value float64) Token {
	return LiteralToken{Value: value}
}

func (lt LiteralToken) String() string {
	return fmt.Sprintf("{TokenType: LITERAL, Value: %v}", lt.Value)
}

func (lt LiteralToken) ToSimpleString() string {
	return fmt.Sprintf("{%v}", lt.Value)
}

// Operator type
type Operator int

const (
	PLUS Operator = iota
	MINUS
	MULTIPLY
	DIVIDE
	LPAREN
	RPAREN
	UNARY_MINUS
	UNARY_PLUS
	EXPONENT
)

// Associativity type
type Associativity int

const (
	NONE Associativity = iota
	LEFT
	RIGHT
)

// OperatorToken struct
type OperatorToken struct {
	Value         Operator
	NumOperands   int
	Associativity Associativity
}

func NewOperatorToken(value Operator, numOperands int, associativity Associativity) OperatorToken {
	return OperatorToken{Value: value, NumOperands: numOperands, Associativity: associativity}
}

func (ot OperatorToken) String() string {
	return fmt.Sprintf("{TokenType: OPERATOR, Operator: %v}", ot.Value)
}

func (ot OperatorToken) ToSimpleString() string {
	switch ot.Value {
	case PLUS:
		return "+"
	case MINUS:
		return "-"
	case MULTIPLY:
		return "*"
	case DIVIDE:
		return "/"
	case LPAREN:
		return "("
	case RPAREN:
		return ")"
	case UNARY_MINUS:
		return "-u"
	case UNARY_PLUS:
		return "+u"
	case EXPONENT:
		return "^"
	default:
		return fmt.Sprintf("Unknown(%d)", int(ot.Value))
	}
}

// Predefined OperatorTokens
var (
	PLUS_TOKEN        = NewOperatorToken(PLUS, 2, LEFT)
	MINUS_TOKEN       = NewOperatorToken(MINUS, 2, LEFT)
	MULTIPLY_TOKEN    = NewOperatorToken(MULTIPLY, 2, LEFT)
	DIVIDE_TOKEN      = NewOperatorToken(DIVIDE, 2, LEFT)
	LPAREN_TOKEN      = NewOperatorToken(LPAREN, 0, NONE)
	RPAREN_TOKEN      = NewOperatorToken(RPAREN, 0, NONE)
	UNARY_MINUS_TOKEN = NewOperatorToken(UNARY_MINUS, 1, RIGHT)
	UNARY_PLUS_TOKEN  = NewOperatorToken(UNARY_PLUS, 1, RIGHT)
	EXPONENT_TOKEN    = NewOperatorToken(EXPONENT, 2, RIGHT)
)

func UnaryFromChar(opChar rune) (OperatorToken, error) {
	switch opChar {
	case '+':
		return UNARY_PLUS_TOKEN, nil
	case '-':
		return UNARY_MINUS_TOKEN, nil
	default:
		return OperatorToken{}, fmt.Errorf("OperatorToken.UnaryFromChar: Illegal char (%c) provided", opChar)
	}
}

func BinaryFromChar(opChar rune) (OperatorToken, error) {
	switch opChar {
	case '+':
		return PLUS_TOKEN, nil
	case '-':
		return MINUS_TOKEN, nil
	case '*', 'x':
		return MULTIPLY_TOKEN, nil
	case '/':
		return DIVIDE_TOKEN, nil
	case '(':
		return LPAREN_TOKEN, nil
	case ')':
		return RPAREN_TOKEN, nil
	case '^':
		return EXPONENT_TOKEN, nil
	default:
		return OperatorToken{}, fmt.Errorf("OperatorToken.BinaryFromChar: Illegal char (%c) provided", opChar)
	}
}

func (ot OperatorToken) Precedence() int {
	switch ot.Value {
	case LPAREN, RPAREN:
		return 5
	case UNARY_PLUS, UNARY_MINUS:
		return 4
	case EXPONENT:
		return 3
	case MULTIPLY, DIVIDE:
		return 2
	case PLUS, MINUS:
		return 1
	default:
		panic(fmt.Sprintf("Invalid operator: %v", ot.Value))
	}
}
