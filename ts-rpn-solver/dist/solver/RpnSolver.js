"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.solveRpn = solveRpn;
const Token_1 = require("../token/Token");
function solveRpn(tokens) {
    const stack = [];
    let step = 1;
    for (const token of tokens) {
        if ((0, Token_1.isLiteralToken)(token)) {
            stack.push(token.value);
            console.log(`Step ${step}: Push ${token.value} onto the stack`);
            step++;
        }
        else if ((0, Token_1.isOperatorToken)(token)) {
            if (token.numOperands === 1) {
                if (stack.length < 1) {
                    throw new Error(`Not enough operands for ${token.operator}`);
                }
                const operand = stack.pop();
                const result = applyUnaryOperator(token.operator, operand);
                stack.push(result);
                console.log(`Step ${step}: Apply ${token.operator} to ${operand} = ${result}`);
                step++;
            }
            else if (token.numOperands === 2) {
                if (stack.length < 2) {
                    throw new Error(`Not enough operands for ${token.operator}`);
                }
                const rightOperand = stack.pop();
                const leftOperand = stack.pop();
                const result = applyBinaryOperator(token.operator, leftOperand, rightOperand);
                stack.push(result);
                console.log(`Step ${step}: Apply ${token.operator} to ${leftOperand} and ${rightOperand} = ${result}`);
                step++;
            }
            else {
                throw new Error(`Unsupported operator: ${token.operator}`);
            }
        }
        else if ((0, Token_1.isInvalidToken)(token)) {
            throw new Error(`Invalid token: ${token.invalidShard} at position ${token.position}`);
        }
    }
    if (stack.length !== 1) {
        throw new Error("Invalid expression: too many operands");
    }
    console.log(`Final result: ${stack[0]}`);
    return stack[0];
}
function applyUnaryOperator(operator, operand) {
    switch (operator) {
        case Token_1.Operator.UNARY_PLUS:
            return +operand;
        case Token_1.Operator.UNARY_MINUS:
            return -operand;
        default:
            throw new Error(`Unsupported unary operator: ${operator}`);
    }
}
function applyBinaryOperator(operator, left, right) {
    switch (operator) {
        case Token_1.Operator.PLUS:
            return left + right;
        case Token_1.Operator.MINUS:
            return left - right;
        case Token_1.Operator.MULTIPLY:
            return left * right;
        case Token_1.Operator.DIVIDE:
            if (right === 0) {
                throw new Error("Division by zero");
            }
            return left / right;
        case Token_1.Operator.EXPONENT:
            return Math.pow(left, right);
        default:
            throw new Error(`Unsupported binary operator: ${operator}`);
    }
}
