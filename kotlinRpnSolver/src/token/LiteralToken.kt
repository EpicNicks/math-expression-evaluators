package token

class LiteralToken(val value: Double) : Token {
    override fun toSimpleString(): String = "{${value}}"
}