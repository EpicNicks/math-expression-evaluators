#pragma once

#include <vector>
#include <string>
#include <memory>

#include "Token.hpp"


std::vector<std::unique_ptr<Token>> tokenizeFsm(const std::string& input);