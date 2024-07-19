from .token import Token

class LiteralToken(Token):
    def __init__(self, value: float) -> None:
        self.value = value
    
    def to_simple_string(self):
        return f"{{{self.value}}}"