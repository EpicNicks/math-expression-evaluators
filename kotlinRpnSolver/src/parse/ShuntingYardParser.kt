package parse

import java.util.Stack
import kotlin.math.pow

import exceptions.AggregateException
import token.*
import java.util.ArrayList

fun tokensToRpn(tokens: List<Token>): List<Token> {
    tokens.throwIfContainsInvalid()

    val operatorStack = Stack<OperatorToken>()
    val rpnSolveList = ArrayList<Token>()

    for (token in tokens) {
        when (token) {
            is LiteralToken -> rpnSolveList.add(token)
            is OperatorToken -> {
                when (token.operator) {
                    Operator.RPAREN -> {
                        while (true) {
                            if (operatorStack.isEmpty()) {
                                throw IllegalArgumentException("Mismatched Parentheses in token list\n\nSolve List:${rpnSolveList}\n\nOperator Stack:${operatorStack}")
                            }
                            if (!operatorStack.pop().run {
                                    when (operator) {
                                        Operator.LPAREN -> false
                                        else -> rpnSolveList.add(this)
                                    }
                                } || operatorStack.isEmpty()) {
                                break;
                            }
                        }
                    }

                    else -> {
                        while (operatorStack.isNotEmpty() && precedenceCompare(token, operatorStack.peek())) {
                            rpnSolveList.add(operatorStack.pop())
                        }
                        operatorStack.push(token)
                    }
                }
            }
        }
    }
    while (operatorStack.isNotEmpty()) {
        rpnSolveList.add(operatorStack.pop())
    }
    return rpnSolveList
}

private fun precedenceCompare(leftOpToken: OperatorToken, rightOpToken: OperatorToken): Boolean = when (rightOpToken.associativity) {
    Associativity.LEFT -> leftOpToken.precedence() <= rightOpToken.precedence()
    Associativity.RIGHT -> leftOpToken.precedence() < rightOpToken.precedence()
    Associativity.NONE -> false
}


fun shuntingYardSolve(rpnSolveList: List<Token>): Pair<Double, List<String>> {
    rpnSolveList.throwIfContainsInvalid()

    val solveStack = Stack<Token>()
    val steps = ArrayList<String>()
    steps.add("Showing steps for RPN solve:")
    var stepCount = 0
    var stepChanged = true

    for (token in rpnSolveList) {
        when (token) {
            is LiteralToken -> {
                solveStack.push(token)
            }

            is OperatorToken -> {
                when (token.numOperands) {
                    2 -> {
                        val lhs = solveStack.pop()
                        val rhs = solveStack.pop()
                        if (lhs !is LiteralToken || rhs !is LiteralToken) {
                            throw IllegalArgumentException("Operator arguments were not both literals. lhs: ${lhs.toSimpleString()} rhs: ${rhs.toSimpleString()}.")
                        }
                        val arithmeticResult = when (token.operator) {
                            Operator.PLUS -> lhs.value + rhs.value
                            Operator.MINUS -> lhs.value - rhs.value
                            Operator.MULTIPLY -> lhs.value * rhs.value
                            Operator.DIVIDE -> lhs.value / rhs.value
                            Operator.EXPONENT -> lhs.value.pow(rhs.value)
                            else -> throw IllegalArgumentException("Invalid operator provided: ${token.toSimpleString()}, number of operands: ${token.numOperands}. Binary operator expected.")
                        }
                        stepCount++;
                        stepChanged = true;
                        steps.add("BINARY OPERATION ${token.toSimpleString()} ON ${lhs.toSimpleString()} AND ${rhs.toSimpleString()} => INSERT {$arithmeticResult}");

                        solveStack.push(LiteralToken(arithmeticResult))
                    }

                    1 -> {
                        val (arithmeticResult, operand) = solveStack.pop().let { operand ->
                            when (operand) {
                                is LiteralToken -> when (token.operator) {
                                    Operator.UNARY_PLUS -> Pair(+operand.value, operand)
                                    Operator.UNARY_MINUS -> Pair(-operand.value, operand)
                                    else -> throw IllegalArgumentException("Invalid operator provided: ${token.toSimpleString()}, number of operands: ${token.numOperands}. Unary operator expected.")
                                }

                                else -> throw IllegalArgumentException("Operator argument was not a literal. lhs: ${operand.toSimpleString()}.")
                            }
                        }
                        stepCount++;
                        stepChanged = true;
                        steps.add("UNARY OPERATION ${token.toSimpleString()} ON ${operand.toSimpleString()} => INSERT {$arithmeticResult}");

                        solveStack.push(LiteralToken(arithmeticResult))
                    }

                    else -> throw IllegalArgumentException("Invalid number of operands ${token.numOperands} for operator ${token.operator}")
                }
            }
        }
    }
    return solveStack.singleOrNull()?.let {
        when (it) {
            is LiteralToken -> Pair(it.value, steps)
            else -> throw IllegalStateException("Final token is not a LiteralToken")
        }
    } ?: throw IllegalArgumentException("Final result was not a single value. Result stack count: ${solveStack.count()}, Result stack values: ${solveStack.joinToString(",") { it.toSimpleString() }}")
}

private fun List<Token>.throwIfContainsInvalid() {
    filterIsInstance<InvalidToken>().run {
        if (isNotEmpty())
            throw AggregateException("Parsing errors encountered:", map { IllegalArgumentException("Invalid token: ${it.invalidShard} at position ${it.position}") })
    }
}