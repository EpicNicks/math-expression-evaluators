package token

class InvalidToken(val position: Int, val invalidShard: String): Token {
    override fun toSimpleString(): String = "<INVALID>"
}