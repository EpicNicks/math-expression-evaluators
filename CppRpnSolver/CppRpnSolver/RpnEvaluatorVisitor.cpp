#include <stack>
#include <stdexcept>
#include <cmath>
#include <sstream>
#include <vector>

#include "RpnEvaluatorVisitor.hpp"

#include "LiteralToken.hpp"
#include "OperatorToken.hpp"
#include "InvalidToken.hpp"

void RpnEvaluatorVisitor::visit(const InvalidToken& invalidToken) {
    std::stringstream ss{};
    ss << "Invalid Token found at " << invalidToken.position() << " with data " << invalidToken.invalidShard();
    throw std::runtime_error(ss.str());
}

void RpnEvaluatorVisitor::visit(const LiteralToken& literalToken) {
    m_operandStack.push(literalToken.value());
}

void RpnEvaluatorVisitor::visit(const OperatorToken& operatorToken) {
    if (operatorToken.numOperands() == 1) {
        double operand = m_operandStack.top();
        m_operandStack.pop();

        switch (operatorToken.op()) {
        case Operator::UNARY_PLUS:
            m_operandStack.push(+operand);
            break;
        case Operator::UNARY_MINUS:
            m_operandStack.push(-operand);
            break;
        }
        std::stringstream ss{};
        ss << "UNARY OPERATION " << operatorToken.toSimpleString() << " on {" << operand << "}, RESULT: " << m_operandStack.top();
        m_steps.push_back(ss.str());
    }

    if (operatorToken.numOperands() == 2) {
        double rhs = m_operandStack.top();
        m_operandStack.pop();
        double lhs = m_operandStack.top();
        m_operandStack.pop();

        switch (operatorToken.op()) {
        case Operator::PLUS:
            m_operandStack.push(lhs + rhs);
            break;
        case Operator::MINUS:
            m_operandStack.push(lhs - rhs);
            break;
        case Operator::MULTIPLY:
            m_operandStack.push(lhs * rhs);
            break;
        case Operator::DIVIDE:
            if (rhs == 0)
                throw std::runtime_error("Division by zero");
            m_operandStack.push(lhs / rhs);
            break;
        case Operator::EXPONENT:
            m_operandStack.push(std::pow(lhs, rhs));
            break;
        }

        std::stringstream ss{};
        ss << "BINARY OPERATION " << operatorToken.toSimpleString() << " on {" << lhs << "} and {" << rhs << "}, RESULT: " << m_operandStack.top();
        m_steps.push_back(ss.str());
    }
}

double RpnEvaluatorVisitor::getResult() {
    if (m_operandStack.size() != 1) {
        throw std::runtime_error("Invalid expression");
    }
    return m_operandStack.top();
}

double RpnEvaluatorVisitor::evaluate(const std::vector<std::unique_ptr<Token>>& rpnTokens) {
    for (const auto& token : rpnTokens) {
        token->accept(*this);
    }
    return getResult();
}

std::vector<std::string> RpnEvaluatorVisitor::getSteps() {
    return m_steps;
}
