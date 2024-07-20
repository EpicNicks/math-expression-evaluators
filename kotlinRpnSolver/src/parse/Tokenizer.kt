package parse

import token.*

fun tokenize(inputString: String) : List<Token> {
    val collectedTokens = ArrayList<Token>()
    val operators = charArrayOf('+', '-', '*', 'x', '/', '^', '(', ')')
    var skipIndex = 0
    for (i in inputString.indices) {
        if (skipIndex > 0) {
            skipIndex--
            continue
        }
        if (inputString[i].isWhitespace()) {
            continue
        }
        if (inputString[i].isDigit()) {
            var literal = ""
            var decimalPointsCount = 0
            while (i + skipIndex < inputString.length) {
                if (inputString[i + skipIndex] == '.'){
                    literal += inputString[i + skipIndex]
                    decimalPointsCount++
                    if (decimalPointsCount > 1) {
                        throw IllegalArgumentException("Malformed input string. Invalid number ending at index ${i+skipIndex} found. Too many decimals.")
                    }
                } else if (inputString[i + skipIndex].isDigit()) {
                    literal += inputString[i + skipIndex]
                    if (i+skipIndex+1 >= inputString.length || inputString[i+skipIndex].isWhitespace() || inputString[i+skipIndex] in operators) {
                        try {
                            val value = literal.toDouble()
                            collectedTokens.add(LiteralToken(value))
                        } catch (ex: NumberFormatException) {
                            throw IllegalArgumentException("Malformed input string. Invalid number ending at index ${i+skipIndex} found. Malformed decimal number.")
                        }
                    }
                }
                skipIndex++
            }
        } else if (inputString[i] in operators) {
            collectedTokens.add(
                if (inputString[i] in setOf('-', '+') && (i == 0 || collectedTokens.last().let { last -> last is OperatorToken && last.operator != Operator.RPAREN }))
                    OperatorToken.unaryFromChar(inputString[i])
                else
                    OperatorToken.binaryFromChar(inputString[i])
            )
        } else {
            throw IllegalArgumentException("Malformed input string. Unexpected character at index $i received.")
        }
    }

    return collectedTokens
}

fun tokenizeFsm(inputString: String) : List<Token> {
    val collectedTokens = ArrayList<Token>();

    val skipCapture = "SKIP"
    val numberCapture = "NUMBER"
    val operatorCapture = "OPERATOR"
    val mismatchCapture = "MISMATCH"

    val tokenRegex = Regex(
        "(?<$numberCapture>\\d+(\\.\\d*)?)|" +
                "(?<$operatorCapture>[+\\-*x/^()])|" +
                "(?<$skipCapture>\\s+)|" +
                "(?<$mismatchCapture>.)"
    )

    tokenRegex.findAll(inputString).forEach { matchResult ->
        when {
            matchResult.groups[skipCapture] != null -> {
                // Skip whitespace
            }
            matchResult.groups[numberCapture] != null -> {
                val value = matchResult.groups[numberCapture]!!.value
                collectedTokens.add(LiteralToken(value.toDouble()))
            }
            matchResult.groups[operatorCapture] != null -> {
                val operatorChar = matchResult.groups[operatorCapture]!!.value[0]
                if (operatorChar in setOf('-', '+') && (collectedTokens.isEmpty() || (collectedTokens.last() is OperatorToken && (collectedTokens.last() as OperatorToken).operator != Operator.RPAREN))) {
                    collectedTokens.add(OperatorToken.unaryFromChar(operatorChar))
                } else {
                    collectedTokens.add(OperatorToken.binaryFromChar(operatorChar))
                }
            }
            matchResult.groups[mismatchCapture] != null -> {
                val value = matchResult.groups[mismatchCapture]!!.value
                collectedTokens.add(InvalidToken(matchResult.range.first, value))
            }
        }
    }
    return collectedTokens
}