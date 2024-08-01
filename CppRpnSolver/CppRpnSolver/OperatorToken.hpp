#pragma once

#include <string>

#include "TokenVisitor.hpp"
#include "Token.hpp"
#include "Associativity.hpp"
#include "Operator.hpp"

class OperatorToken : public Token {
public:
	static const OperatorToken PLUS;
	static const OperatorToken MINUS;
	static const OperatorToken MULTIPLY;
	static const OperatorToken DIVIDE;
	static const OperatorToken EXPONENT;
	static const OperatorToken UNARY_PLUS;
	static const OperatorToken UNARY_MINUS;
	static const OperatorToken LPAREN;
	static const OperatorToken RPAREN;

	static OperatorToken unaryFromChar(char c);
	static OperatorToken binaryFromChar(char c);

	explicit OperatorToken(Operator op, int numOperands, Associativity associativity);
	void accept(TokenVisitor& visitor) const override;
	std::string toSimpleString() const override;

	int precedence();

	Operator op() const;
	int numOperands() const;
	Associativity associativity() const;

private:
	Operator m_operator;
	int m_numOperands;
	Associativity m_associativity;
};