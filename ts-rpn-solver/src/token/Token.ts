export type Token = LiteralToken | OperatorToken | InvalidToken

export type LiteralToken = {
    value: number
}

export type OperatorToken = {
    operator: Operator
    numOperands: number
    associativity: Associativity
    precedence: number
}

export type InvalidToken = {
    position: number
    invalidShard: string
}

export const Operator = {
    PLUS: "PLUS",
    MINUS: "MINUS",
    MULTIPLY: "MULTIPLY",
    DIVIDE: "DIVIDE",
    EXPONENT: "EXPONENT",
    UNARY_PLUS: "UNARY_PLUS",
    UNARY_MINUS: "UNARY_MINUS",
    LPAREN: "LPAREN",
    RPAREN: "RPAREN"
} as const
export type Operator = keyof typeof Operator

export const Associativity = {
    LEFT: "LEFT",
    RIGHT: "RIGHT",
    NONE: "NONE"
} as const
export type Associativity = keyof typeof Associativity

export const OperatorTokens: Record<Operator, OperatorToken> = {
    [Operator.PLUS]: { operator: Operator.PLUS, numOperands: 2, associativity: Associativity.LEFT, precedence: 1 },
    [Operator.MINUS]: { operator: Operator.MINUS, numOperands: 2, associativity: Associativity.LEFT, precedence: 1 },
    [Operator.MULTIPLY]: { operator: Operator.MULTIPLY, numOperands: 2, associativity: Associativity.LEFT, precedence: 2 },
    [Operator.DIVIDE]: { operator: Operator.DIVIDE, numOperands: 2, associativity: Associativity.LEFT, precedence: 2 },
    [Operator.EXPONENT]: { operator: Operator.EXPONENT, numOperands: 2, associativity: Associativity.RIGHT, precedence: 3 },
    [Operator.UNARY_PLUS]: { operator: Operator.UNARY_PLUS, numOperands: 1, associativity: Associativity.RIGHT, precedence: 4 },
    [Operator.UNARY_MINUS]: { operator: Operator.UNARY_MINUS, numOperands: 1, associativity: Associativity.RIGHT, precedence: 4 },
    [Operator.LPAREN]: { operator: Operator.LPAREN, numOperands: 0, associativity: Associativity.NONE, precedence: 5 },
    [Operator.RPAREN]: { operator: Operator.RPAREN, numOperands: 0, associativity: Associativity.NONE, precedence: 5 }
}


export const unaryOpFromChar = {
    "+": OperatorTokens.UNARY_PLUS,
    "-": OperatorTokens.UNARY_MINUS,
} as const

export const binaryOpFromChar = {
    "+": OperatorTokens.PLUS,
    "-": OperatorTokens.MINUS,
    "*": OperatorTokens.MULTIPLY,
    "/": OperatorTokens.DIVIDE,
    "^": OperatorTokens.EXPONENT
} as const


export function isLiteralToken(token: Token): token is LiteralToken {
    return 'value' in token
}

export function isOperatorToken(token: Token): token is OperatorToken {
    return 'operator' in token
}

export function isInvalidToken(token: Token): token is InvalidToken {
    return 'invalidShard' in token
}