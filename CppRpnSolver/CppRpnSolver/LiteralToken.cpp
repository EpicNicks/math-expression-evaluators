#include <string>
#include <sstream>

#include "LiteralToken.hpp"

LiteralToken::LiteralToken(double value)
	: m_value(value)
{}

std::string LiteralToken::toSimpleString() const {
	std::stringstream ss{};
	ss << "{" << m_value << "}";
	return ss.str();
}

void LiteralToken::accept(TokenVisitor& visitor) const { visitor.visit(*this); }

double LiteralToken::value() const {
	return m_value;
}
