package exceptions

class AggregateException : Exception {
    val innerExceptions: List<Throwable>

    constructor(message: String, exceptions: List<Throwable>) : super(message) {
        innerExceptions = exceptions.toList() // Create a defensive copy
    }

    constructor(message: String, cause: Throwable, exceptions: List<Throwable>) : super(message, cause) {
        innerExceptions = exceptions.toList() // Create a defensive copy
    }

    override val message: String
        get() = "${super.message}\nInner Exceptions:\n" +
                innerExceptions.mapIndexed { index, ex ->
                    "  ${index + 1}. ${ex.javaClass.simpleName}: ${ex.message}"
                }.joinToString("\n")

    override fun toString(): String = "${javaClass.simpleName}: $message"

    override fun printStackTrace(s: java.io.PrintStream) {
        s.println(this)
        innerExceptions.forEachIndexed { index, ex ->
            s.println("Inner Exception #${index + 1}:")
            ex.printStackTrace(s)
        }
    }
}
