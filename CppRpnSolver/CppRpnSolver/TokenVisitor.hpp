#pragma once

class LiteralToken;
class OperatorToken;
class InvalidToken;

class TokenVisitor {
public:
	virtual void visit(const LiteralToken& literalToken) = 0;
	virtual void visit(const OperatorToken& operatorToken) = 0;
	virtual void visit(const InvalidToken& invalidToken) = 0;
	virtual ~TokenVisitor() = default;
};