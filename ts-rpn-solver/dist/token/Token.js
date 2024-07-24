"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.binaryOpFromChar = exports.unaryOpFromChar = exports.OperatorTokens = exports.Associativity = exports.Operator = void 0;
exports.isLiteralToken = isLiteralToken;
exports.isOperatorToken = isOperatorToken;
exports.isInvalidToken = isInvalidToken;
exports.Operator = {
    PLUS: "PLUS",
    MINUS: "MINUS",
    MULTIPLY: "MULTIPLY",
    DIVIDE: "DIVIDE",
    EXPONENT: "EXPONENT",
    UNARY_PLUS: "UNARY_PLUS",
    UNARY_MINUS: "UNARY_MINUS",
    LPAREN: "LPAREN",
    RPAREN: "RPAREN"
};
exports.Associativity = {
    LEFT: "LEFT",
    RIGHT: "RIGHT",
    NONE: "NONE"
};
exports.OperatorTokens = {
    [exports.Operator.PLUS]: { operator: exports.Operator.PLUS, numOperands: 2, associativity: exports.Associativity.LEFT, precedence: 1 },
    [exports.Operator.MINUS]: { operator: exports.Operator.MINUS, numOperands: 2, associativity: exports.Associativity.LEFT, precedence: 1 },
    [exports.Operator.MULTIPLY]: { operator: exports.Operator.MULTIPLY, numOperands: 2, associativity: exports.Associativity.LEFT, precedence: 2 },
    [exports.Operator.DIVIDE]: { operator: exports.Operator.DIVIDE, numOperands: 2, associativity: exports.Associativity.LEFT, precedence: 2 },
    [exports.Operator.EXPONENT]: { operator: exports.Operator.EXPONENT, numOperands: 2, associativity: exports.Associativity.RIGHT, precedence: 3 },
    [exports.Operator.UNARY_PLUS]: { operator: exports.Operator.UNARY_PLUS, numOperands: 1, associativity: exports.Associativity.RIGHT, precedence: 4 },
    [exports.Operator.UNARY_MINUS]: { operator: exports.Operator.UNARY_MINUS, numOperands: 1, associativity: exports.Associativity.RIGHT, precedence: 4 },
    [exports.Operator.LPAREN]: { operator: exports.Operator.LPAREN, numOperands: 0, associativity: exports.Associativity.NONE, precedence: 5 },
    [exports.Operator.RPAREN]: { operator: exports.Operator.RPAREN, numOperands: 0, associativity: exports.Associativity.NONE, precedence: 5 }
};
exports.unaryOpFromChar = {
    "+": exports.OperatorTokens.UNARY_PLUS,
    "-": exports.OperatorTokens.UNARY_MINUS,
};
exports.binaryOpFromChar = {
    "+": exports.OperatorTokens.PLUS,
    "-": exports.OperatorTokens.MINUS,
    "*": exports.OperatorTokens.MULTIPLY,
    "/": exports.OperatorTokens.DIVIDE,
    "^": exports.OperatorTokens.EXPONENT
};
function isLiteralToken(token) {
    return 'value' in token;
}
function isOperatorToken(token) {
    return 'operator' in token;
}
function isInvalidToken(token) {
    return 'invalidShard' in token;
}
