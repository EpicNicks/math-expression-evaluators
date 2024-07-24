"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.tokenize = tokenize;
const Token_1 = require("../token/Token");
function tokenize(input) {
    const tokens = [];
    let position = 0;
    const numberRegex = /^\d+(\.\d+)?/;
    const operatorRegex = /^[+\-*/^()]/;
    const whitespaceRegex = /^\s+/;
    while (position < input.length) {
        const remainingInput = input.slice(position);
        // Check for whitespace
        const whitespaceMatch = remainingInput.match(whitespaceRegex);
        if (whitespaceMatch) {
            position += whitespaceMatch[0].length;
            continue;
        }
        // Check for numbers
        const numberMatch = remainingInput.match(numberRegex);
        if (numberMatch) {
            tokens.push({
                value: parseFloat(numberMatch[0])
            });
            position += numberMatch[0].length;
            continue;
        }
        // Check for operators
        const operatorMatch = remainingInput.match(operatorRegex);
        if (operatorMatch) {
            const char = operatorMatch[0];
            let operatorToken;
            if (char === '(') {
                operatorToken = Token_1.OperatorTokens.LPAREN;
            }
            else if (char === ')') {
                operatorToken = Token_1.OperatorTokens.RPAREN;
            }
            else if (tokens.length === 0 || (() => {
                const lastToken = tokens[tokens.length - 1];
                return (0, Token_1.isOperatorToken)(lastToken) && lastToken.operator === Token_1.Operator.LPAREN;
            })()) {
                operatorToken = Token_1.unaryOpFromChar[char];
            }
            else {
                operatorToken = Token_1.binaryOpFromChar[char];
            }
            tokens.push(operatorToken);
            position += 1;
            continue;
        }
        // Invalid token
        tokens.push({
            position,
            invalidShard: remainingInput[0]
        });
        position += 1;
    }
    return tokens;
}
