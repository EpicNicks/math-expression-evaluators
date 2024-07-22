#pragma once

#include <vector>
#include <stack>
#include <memory>

#include "Token.hpp"

std::vector<std::unique_ptr<Token>> convertToRPN(const std::vector<std::unique_ptr<Token>>& infix);