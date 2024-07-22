#pragma once

#include "Token.hpp"
#include <string>

class LiteralToken : public Token {
public:
	explicit LiteralToken(double value);
	std::string toSimpleString() const override;
	void accept(TokenVisitor& visitor) const override;
	double value() const;
private:
	double m_value;
};