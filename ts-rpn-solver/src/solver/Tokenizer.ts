import { Token, OperatorToken, Operator, OperatorTokens, unaryOpFromChar, binaryOpFromChar, isOperatorToken } from '../token/Token'

export function tokenize(input: string): Token[] {
    const tokens: Token[] = []
    let position = 0

    const numberRegex = /^\d+(\.\d+)?/
    const operatorRegex = /^[+\-*/^()]/
    const whitespaceRegex = /^\s+/

    while (position < input.length) {
        const remainingInput = input.slice(position)

        // Check for whitespace
        const whitespaceMatch = remainingInput.match(whitespaceRegex)
        if (whitespaceMatch) {
            position += whitespaceMatch[0].length
            continue
        }

        // Check for numbers
        const numberMatch = remainingInput.match(numberRegex)
        if (numberMatch) {
            tokens.push({
                value: parseFloat(numberMatch[0])
            })
            position += numberMatch[0].length
            continue
        }

        // Check for operators
        const operatorMatch = remainingInput.match(operatorRegex)
        if (operatorMatch) {
            const char = operatorMatch[0]
            let operatorToken: OperatorToken

            if (char === '(') {
                operatorToken = OperatorTokens.LPAREN
            } else if (char === ')') {
                operatorToken = OperatorTokens.RPAREN
            } else if (tokens.length === 0 || (() => {
                const lastToken = tokens[tokens.length - 1]
                return isOperatorToken(lastToken) && lastToken.operator === Operator.LPAREN
            })()) {
                operatorToken = unaryOpFromChar[char as keyof typeof unaryOpFromChar]
            } else {
                operatorToken = binaryOpFromChar[char as keyof typeof binaryOpFromChar]
            }

            tokens.push(operatorToken)
            position += 1
            continue
        }

        // Invalid token
        tokens.push({
            position,
            invalidShard: remainingInput[0]
        })
        position += 1
    }

    return tokens
}