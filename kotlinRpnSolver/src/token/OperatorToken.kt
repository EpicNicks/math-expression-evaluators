package token

class OperatorToken private constructor(val operator: Operator, val associativity: Associativity, val numOperands: Int) : Token {
    companion object {
        val PLUS = OperatorToken(Operator.PLUS, Associativity.LEFT, 2)
        val MINUS = OperatorToken(Operator.MINUS, Associativity.LEFT, 2)
        val MULTIPLY = OperatorToken(Operator.MULTIPLY, Associativity.LEFT, 2)
        val DIVIDE = OperatorToken(Operator.DIVIDE, Associativity.LEFT, 2)
        val EXPONENT = OperatorToken(Operator.EXPONENT, Associativity.LEFT, 2)

        val UNARY_PLUS = OperatorToken(Operator.UNARY_PLUS, Associativity.RIGHT, 1)
        val UNARY_MINUS = OperatorToken(Operator.UNARY_MINUS, Associativity.RIGHT, 1)

        val LPAREN = OperatorToken(Operator.LPAREN, Associativity.NONE, 0)
        val RPAREN = OperatorToken(Operator.RPAREN, Associativity.NONE, 0)

        fun unaryFromChar(opChar: Char): OperatorToken = when (opChar) {
            '+' -> UNARY_PLUS
            '-' -> UNARY_MINUS
            else -> throw IllegalArgumentException("opChar $opChar is an invalid unary char")
        }

        fun binaryFromChar(opChar: Char): OperatorToken = when (opChar) {
            '+' -> PLUS
            '-' -> MINUS
            '*', 'x' -> MULTIPLY
            '/' -> DIVIDE
            '(' -> LPAREN
            ')' -> RPAREN
            '^' -> EXPONENT
            else -> throw IllegalArgumentException("opChar $opChar does not map to a valid binary operation")
        }
    }

    fun precedence() : Int = when (operator) {
        Operator.LPAREN, Operator.RPAREN -> 5
        Operator.UNARY_MINUS, Operator.UNARY_PLUS -> 4
        Operator.EXPONENT -> 3
        Operator.MULTIPLY, Operator.DIVIDE -> 2
        Operator.PLUS, Operator.MINUS -> 1
    }

    override fun toSimpleString(): String = "{${operator.name}}"
}