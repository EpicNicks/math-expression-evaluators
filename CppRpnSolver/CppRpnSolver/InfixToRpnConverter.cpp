#include <vector>
#include <stack>
#include <memory>

#include "InfixToRpnConverter.hpp";
#include "Token.hpp"
#include "OperatorToken.hpp"
#include "LiteralToken.hpp"

std::vector<std::unique_ptr<Token>> convertToRPN(const std::vector<std::unique_ptr<Token>>& infix) {
    std::vector<std::unique_ptr<Token>> rpn;
    std::stack<std::unique_ptr<Token>> operatorStack;

    for (const auto& token : infix) {
        if (auto* literalToken = dynamic_cast<LiteralToken*>(token.get())) {
            rpn.push_back(std::make_unique<LiteralToken>(*literalToken));
        }
        else if (auto* opToken = dynamic_cast<OperatorToken*>(token.get())) {
            if (opToken->op() == Operator::LPAREN) {
                operatorStack.push(std::make_unique<OperatorToken>(*opToken));
            }
            else if (opToken->op() == Operator::RPAREN) {
                while (!operatorStack.empty() && dynamic_cast<OperatorToken*>(operatorStack.top().get())->op() != Operator::LPAREN) {
                    rpn.push_back(std::move(operatorStack.top()));
                    operatorStack.pop();
                }
                if (!operatorStack.empty()) operatorStack.pop(); // Remove left parenthesis
            }
            else {
                while (!operatorStack.empty() &&
                    dynamic_cast<OperatorToken*>(operatorStack.top().get())->op() != Operator::LPAREN &&
                    dynamic_cast<OperatorToken*>(operatorStack.top().get())->precedence() >= opToken->precedence()) {
                    rpn.push_back(std::move(operatorStack.top()));
                    operatorStack.pop();
                }
                operatorStack.push(std::make_unique<OperatorToken>(*opToken));
            }
        }
    }

    while (!operatorStack.empty()) {
        rpn.push_back(std::move(operatorStack.top()));
        operatorStack.pop();
    }

    return rpn;
}