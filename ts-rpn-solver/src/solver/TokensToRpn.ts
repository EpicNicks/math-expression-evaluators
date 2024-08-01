import { Token, OperatorToken, isOperatorToken, isLiteralToken, isInvalidToken, Associativity } from '../token/Token'
import { Operator } from '../token/Token'

export function tokensToRpn(infix: Token[]): Token[] {
    if (infix.some(token => isInvalidToken(token))) {
        throw new Error("Invalid Tokens were in the infix tokens")
    }

    const rpn: Token[] = []
    const operatorStack: OperatorToken[] = []
    let step = 1

    for (const token of infix) {
        if (isLiteralToken(token)) {
            rpn.push(token)
            console.log(`Step ${step}: Push literal ${token.value} to output`)
            step++
        }
        else if (isOperatorToken(token)) {
            if (token.operator === Operator.LPAREN) {
                operatorStack.push(token)
                console.log(`Step ${step}: Push ( to operator stack`)
                step++
            }
            else if (token.operator === Operator.RPAREN) {
                while (operatorStack.length > 0 && operatorStack[operatorStack.length - 1].operator !== Operator.LPAREN) {
                    const op = operatorStack.pop()!
                    rpn.push(op)
                    console.log(`Step ${step}: Pop ${op.operator} from stack to output`)
                    step++
                }
                if (operatorStack.length > 0 && operatorStack[operatorStack.length - 1].operator === Operator.LPAREN) {
                    operatorStack.pop() // Remove left parenthesis
                    console.log(`Step ${step}: Pop ( from operator stack and discard`)
                    step++
                } else {
                    throw new Error("Mismatched parentheses")
                }
            }
            else {
                while (operatorStack.length > 0 &&
                    operatorStack[operatorStack.length - 1].operator !== Operator.LPAREN &&
                    (operatorStack[operatorStack.length - 1].precedence > token.precedence ||
                        (operatorStack[operatorStack.length - 1].precedence === token.precedence &&
                            token.associativity === Associativity.LEFT))) {
                    const op = operatorStack.pop()!
                    rpn.push(op)
                    console.log(`Step ${step}: Pop ${op.operator} from stack to output (higher precedence)`)
                    step++
                }
                operatorStack.push(token)
                console.log(`Step ${step}: Push ${token.operator} to operator stack`)
                step++
            }
        }
        else if (isInvalidToken(token)) {
            throw new Error(`Invalid token: ${token.invalidShard} at position ${token.position}`)
        }
    }

    while (operatorStack.length > 0) {
        const op = operatorStack.pop()!
        if (op.operator === Operator.LPAREN || op.operator === Operator.RPAREN) {
            throw new Error("Mismatched parentheses")
        }
        rpn.push(op)
        console.log(`Step ${step}: Pop remaining ${op.operator} from stack to output`)
        step++
    }

    return rpn
}
