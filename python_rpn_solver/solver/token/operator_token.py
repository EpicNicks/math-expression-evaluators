from .token import Token
from enum import Enum, auto

class Associativity(Enum):
    LEFT = auto()
    RIGHT = auto()
    NONE = auto()

class Operator(Enum):
    # Binary Ops
    PLUS = auto()
    MINUS = auto()
    MULTIPLY = auto()
    DIVIDE = auto()
    EXPONENT = auto()
    # Unary Ops
    UNARY_PLUS = auto()
    UNARY_MINUS = auto()
    # Special Ops
    LPAREN = auto()
    RPAREN = auto()
    

class OperatorToken(Token):
    @classmethod
    def tokenPropertyName(cls, operator: Operator):
        return f'{operator.name}_TOKEN'
    
    @classmethod
    def get_token(cls, operator: Operator) -> 'OperatorToken':
        return getattr(cls, cls.tokenPropertyName(operator))

    def __new__(cls, operator: Operator, precedence: int, associativity: Associativity) -> 'OperatorToken':
        attribute_name = OperatorToken.tokenPropertyName(operator)
        if hasattr(cls, attribute_name):
            return getattr(cls, attribute_name)
        instance = super().__new__(cls)
        instance.__init__(operator, precedence, associativity)
        setattr(cls, attribute_name, instance)
        return instance

    def __init__(self, operator: Operator, numOperands: int, associativity: Associativity) -> None:
        self.operator = operator
        self.numOperands = numOperands
        self.associativity = associativity

    def to_simple_string(self):
        return f"{{{self.operator.name}}}"
    
    def precedence(self):
        match self.operator:
            case Operator.PLUS | Operator.MINUS:
                return 1
            case Operator.MULTIPLY | Operator.DIVIDE:
                return 2
            case Operator.EXPONENT:
                return 3
            case Operator.UNARY_MINUS | Operator.UNARY_PLUS:
                return 4
            case Operator.LPAREN | Operator.RPAREN:
                return 5
            
    @staticmethod
    def unary_from_char(opChar: str):
        match opChar:
            case '+':
                return OperatorToken.get_token(Operator.UNARY_PLUS)
            case '-':
                return OperatorToken.get_token(Operator.UNARY_MINUS)
            case _:
                return None
    @staticmethod
    def binary_from_char(opChar: str):
        match opChar:
            case '+':
                return OperatorToken.get_token(Operator.PLUS)
            case '-':
                return OperatorToken.get_token(Operator.MINUS)
            case '*' | 'x':
                return OperatorToken.get_token(Operator.MULTIPLY)
            case '/':
                return OperatorToken.get_token(Operator.DIVIDE)
            case '^':
                return OperatorToken.get_token(Operator.EXPONENT)
            case '(':
                return OperatorToken.get_token(Operator.LPAREN)
            case ')':
                return OperatorToken.get_token(Operator.RPAREN)
            case _:
                return None

# Operators registered on creation in OperatorToken.__new__

# Binary Operators
OperatorToken(Operator.PLUS, 2, Associativity.LEFT)
OperatorToken(Operator.MINUS, 2, Associativity.LEFT)
OperatorToken(Operator.MULTIPLY, 2, Associativity.LEFT)
OperatorToken(Operator.DIVIDE, 2, Associativity.LEFT)
OperatorToken(Operator.EXPONENT, 2, Associativity.RIGHT)

# Unary Operators
OperatorToken(Operator.UNARY_PLUS, 1, Associativity.RIGHT)
OperatorToken(Operator.UNARY_MINUS, 1, Associativity.RIGHT)

# Special Operators
OperatorToken(Operator.LPAREN, 0, Associativity.NONE)
OperatorToken(Operator.RPAREN, 0, Associativity.NONE)