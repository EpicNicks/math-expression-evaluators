#pragma once

#include <string>
#include "Token.hpp"


class InvalidToken : public Token {
public:
	explicit InvalidToken(int position, std::string invalidShard);
	void accept(TokenVisitor& visitor) const override;
	std::string toSimpleString() const override;

	const std::string& invalidShard() const;
	int position() const;
private:
	std::string m_invalidShard;
	int m_position;
};