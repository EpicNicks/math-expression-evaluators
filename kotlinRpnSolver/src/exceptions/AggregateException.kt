package exceptions

class AggregateException(message: String, val exceptions: List<Exception>) : Exception(message)
