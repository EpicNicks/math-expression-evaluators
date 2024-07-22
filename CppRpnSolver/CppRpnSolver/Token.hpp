#pragma once

#include <string>

#include "TokenVisitor.hpp"

class Token {
public:
	virtual ~Token() = default;    
	virtual void accept(TokenVisitor& visitor) const = 0;
	virtual std::string toSimpleString() const = 0;
};