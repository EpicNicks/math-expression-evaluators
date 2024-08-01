#include <stdexcept>
#include <sstream>

#include "OperatorToken.hpp"

const OperatorToken OperatorToken::PLUS(Operator::PLUS, 2, Associativity::LEFT);
const OperatorToken OperatorToken::MINUS(Operator::MINUS, 2, Associativity::LEFT);
const OperatorToken OperatorToken::MULTIPLY(Operator::MULTIPLY, 2, Associativity::LEFT);
const OperatorToken OperatorToken::DIVIDE(Operator::DIVIDE, 2, Associativity::LEFT);
const OperatorToken OperatorToken::EXPONENT(Operator::EXPONENT, 2, Associativity::RIGHT);
const OperatorToken OperatorToken::UNARY_PLUS(Operator::UNARY_PLUS, 1, Associativity::RIGHT);
const OperatorToken OperatorToken::UNARY_MINUS(Operator::UNARY_MINUS, 1, Associativity::RIGHT);
const OperatorToken OperatorToken::LPAREN(Operator::LPAREN, 0, Associativity::NONE);
const OperatorToken OperatorToken::RPAREN(Operator::RPAREN, 0, Associativity::NONE);

OperatorToken::OperatorToken(Operator op, int numOperands, Associativity associativity): 
	m_operator(op), 
	m_numOperands(numOperands), 
	m_associativity(associativity) 
{}

void OperatorToken::accept(TokenVisitor& visitor) const { visitor.visit(*this); }

std::string OperatorToken::toSimpleString() const {
    switch (m_operator) {
    case Operator::PLUS: return "{PLUS}";
    case Operator::MINUS: return "{MINUS}";
    case Operator::MULTIPLY: return "{MULTIPLY}";
    case Operator::DIVIDE: return "{DIVIDE}";
    case Operator::EXPONENT: return "{EXPONENT}";
    case Operator::UNARY_PLUS: return "{UNARY_PLUS}";
    case Operator::UNARY_MINUS: return "{UNARY_MINUS}";
    case Operator::LPAREN: return "{LPAREN}";
    case Operator::RPAREN: return "{RPAREN}";
    default: return "{UNKNOWN}";
    }
}

int OperatorToken::precedence() {
    switch (m_operator)
    {
    case Operator::PLUS:
    case Operator::MINUS:
        return 1;
    case Operator::MULTIPLY:
    case Operator::DIVIDE:
        return 2;
    case Operator::EXPONENT:
        return 3;
    case Operator::UNARY_PLUS:
    case Operator::UNARY_MINUS:
        return 4;
    case Operator::LPAREN:
    case Operator::RPAREN:
        return 5;
    default:
        throw std::runtime_error("Invalid operator enum class value was stored in operator");
    }
}

OperatorToken OperatorToken::unaryFromChar(char c) {
    switch (c) {
    case '+':
        return OperatorToken::UNARY_PLUS;
    case '-':
        return OperatorToken::UNARY_MINUS;
    default:
        std::stringstream ss{};
        ss << "Invalid unary operation char provided to unaryFromChar( " << c << " )";
        throw std::runtime_error(ss.str());
    }
}

OperatorToken OperatorToken::binaryFromChar(char c) {
    switch (c) {
    case '+':
        return OperatorToken::PLUS;
    case '-':
        return OperatorToken::MINUS;
    case '*':
    case 'x':
        return OperatorToken::MULTIPLY;
    case '/':
        return OperatorToken::DIVIDE;
    case '^':
        return OperatorToken::EXPONENT;
    case '(':
        return OperatorToken::LPAREN;
    case ')':
        return OperatorToken::RPAREN;
    default:
        std::stringstream ss{};
        ss << "Invalid binary operation char provided to binaryFromChar( " << c << " )";
        throw std::runtime_error(ss.str());
    }
}

Operator OperatorToken::op() const {
	return m_operator;
}

int OperatorToken::numOperands() const {
	return m_numOperands;
}

Associativity OperatorToken::associativity() const {
	return m_associativity;
}
