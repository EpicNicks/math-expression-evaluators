#include <regex>
#include <string>
#include <vector>
#include <memory>

#include "TokenizerFsm.hpp"
#include "Token.hpp"
#include "LiteralToken.hpp"
#include "OperatorToken.hpp"
#include "Operator.hpp"
#include "InvalidToken.hpp"

std::vector<std::unique_ptr<Token>> tokenizeFsm(const std::string& input) {
    std::vector<std::unique_ptr<Token>> collectedTokens;
    std::regex literalRegex(R"(^\d+(\.\d+)?)");
    std::regex operatorRegex(R"(^[+\-*/^()])");
    std::regex whitespaceRegex(R"(^\s+)");

    size_t pos = 0;
    while (pos < input.length()) {
        std::smatch match;
        std::string remainingInput = input.substr(pos);

        if (std::regex_search(remainingInput, match, literalRegex)) {
            double value = std::stod(match.str());
            collectedTokens.push_back(std::make_unique<LiteralToken>(value));
            pos += match.length();
        }
        else if (std::regex_search(remainingInput, match, operatorRegex)) {
            char op = match.str()[0];
            if ((op == '+' || op == '-') && (pos == 0 || (dynamic_cast<const OperatorToken*>(collectedTokens.back().get()) && static_cast<const OperatorToken&>(*collectedTokens.back()).op() != Operator::RPAREN))) {
                collectedTokens.push_back(std::make_unique<OperatorToken>(OperatorToken::unaryFromChar(op)));
            }
            else {
                collectedTokens.push_back(std::make_unique<OperatorToken>(OperatorToken::binaryFromChar(op)));
            }
            pos += match.length();
        }
        else if (std::regex_search(remainingInput, match, whitespaceRegex)) {
            pos += match.length();
        }
        else {
            size_t endPos = remainingInput.find_first_of("+-*/^() \t\n");
            if (endPos == std::string::npos) endPos = remainingInput.length();
            std::string invalidShard = remainingInput.substr(0, endPos);
            collectedTokens.push_back(std::make_unique<InvalidToken>(pos, invalidShard));
            pos += invalidShard.length();
        }
    }

    return collectedTokens;
}