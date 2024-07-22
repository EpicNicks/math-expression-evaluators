#include <string>

#include "InvalidToken.hpp"
#include "TokenVisitor.hpp"

InvalidToken::InvalidToken(int position, std::string invalidShard):
	m_position(position),
	m_invalidShard(invalidShard)
{}

void InvalidToken::accept(TokenVisitor& visitor) const { visitor.visit(*this); }

std::string InvalidToken::toSimpleString() const {
	return "{INVALID}";
}

const std::string& InvalidToken::invalidShard() const {
	return m_invalidShard;
}

int InvalidToken::position() const {
	return m_position;
}