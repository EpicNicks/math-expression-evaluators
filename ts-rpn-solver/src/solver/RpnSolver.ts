import { Token, Operator, isInvalidToken, isLiteralToken, isOperatorToken } from '../token/Token'

export function solveRpn(tokens: Token[]): number {
    const stack: number[] = []
    let step = 1

    for (const token of tokens) {
        if (isLiteralToken(token)) {
            stack.push(token.value)
            console.log(`Step ${step}: Push ${token.value} onto the stack`)
            step++
        } else if (isOperatorToken(token)) {
            if (token.numOperands === 1) {
                if (stack.length < 1) {
                    throw new Error(`Not enough operands for ${token.operator}`)
                }
                const operand = stack.pop()!
                const result = applyUnaryOperator(token.operator, operand)
                stack.push(result)
                console.log(`Step ${step}: Apply ${token.operator} to ${operand} = ${result}`)
                step++
            } else if (token.numOperands === 2) {
                if (stack.length < 2) {
                    throw new Error(`Not enough operands for ${token.operator}`)
                }
                const rightOperand = stack.pop()!
                const leftOperand = stack.pop()!
                const result = applyBinaryOperator(token.operator, leftOperand, rightOperand)
                stack.push(result)
                console.log(`Step ${step}: Apply ${token.operator} to ${leftOperand} and ${rightOperand} = ${result}`)
                step++
            } else {
                throw new Error(`Unsupported operator: ${token.operator}`)
            }
        } else if (isInvalidToken(token)) {
            throw new Error(`Invalid token: ${token.invalidShard} at position ${token.position}`)
        }
    }

    if (stack.length !== 1) {
        throw new Error("Invalid expression: too many operands")
    }

    console.log(`Final result: ${stack[0]}`)
    return stack[0]
}

function applyUnaryOperator(operator: Operator, operand: number): number {
    switch (operator) {
        case Operator.UNARY_PLUS:
            return +operand
        case Operator.UNARY_MINUS:
            return -operand
        default:
            throw new Error(`Unsupported unary operator: ${operator}`)
    }
}

function applyBinaryOperator(operator: Operator, left: number, right: number): number {
    switch (operator) {
        case Operator.PLUS:
            return left + right
        case Operator.MINUS:
            return left - right
        case Operator.MULTIPLY:
            return left * right
        case Operator.DIVIDE:
            if (right === 0) {
                throw new Error("Division by zero")
            }
            return left / right
        case Operator.EXPONENT:
            return Math.pow(left, right)
        default:
            throw new Error(`Unsupported binary operator: ${operator}`)
    }
}