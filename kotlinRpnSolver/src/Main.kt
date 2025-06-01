import parse.tokenizeFsm
import parse.tokensToRpn
import parse.shuntingYardSolve
import kotlin.math.exp

fun main(args: Array<String>) {
    when (args.size) {
        1 -> calculateExpressionAndPrintSteps(args[0]).also { println("result: $it") }
        0 -> {
            val exampleExpression = "3 + 4 * 2 / (1 - 5)^2^1"
            println("Using example expression $exampleExpression")
            calculateExpressionAndPrintSteps(exampleExpression).also { println("result: $it") }
        }
        else -> {
            println("Invalid expression provided. Provide an expression to parse (ex. \"1 + 2 * 3 ^ 4 / 5\" or no arguments for an example expression.")
        }
    }
}

fun calculateExpressionAndPrintSteps(expression: String): Double {
    println(expression)
    tokenizeFsm(expression).also { tokens ->
        println(tokens.joinToString(" ") { token -> token.toSimpleString() })
        tokensToRpn(tokens).also { rpn ->
            println(rpn.joinToString(" ") { token -> token.toSimpleString() })
            shuntingYardSolve(rpn).also { solved ->
                println(solved.second.joinToString("\n\t"))
                println("Final result: ${solved.first}")
                return solved.first
            }
        }
    }
}

fun calculateExpression(expression: String): Double =
    tokenizeFsm(expression).run {
        tokensToRpn(this).run {
            shuntingYardSolve(this).first
        }
    }