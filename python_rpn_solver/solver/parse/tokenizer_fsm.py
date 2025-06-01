import re
from ..token.token import Token
from ..token.literal_token import LiteralToken
from ..token.operator_token import Operator, OperatorToken


def tokenize(input_string: str) -> list[Token]:
    token_specification = [
        ('NUMBER', r'\d+(\.\d*)?'),  # Integer or decimal number
        ('OPERATOR', r'[+x\-*/^()]'),  # Operator
        ('SKIP', r'\s+'),  # Skip over whitespace
        ('MISMATCH', r'.'),  # Any other character
    ]
    tok_regex = '|'.join('(?P<%s>%s)' % pair for pair in token_specification)
    
    collected_tokens = []
    for mo in re.finditer(tok_regex, input_string):
        kind = mo.lastgroup
        value = mo.group()
        
        match kind:
            case 'NUMBER':
                collected_tokens.append(LiteralToken(float(value)))
            case 'OPERATOR':
                if value in ['+', '-'] and (not collected_tokens or isinstance(collected_tokens[-1], OperatorToken) and collected_tokens[-1].operator != Operator.RPAREN):
                    collected_tokens.append(OperatorToken.unary_from_char(value))
                else:
                    collected_tokens.append(OperatorToken.binary_from_char(value))
            case 'SKIP':
                continue
            case 'MISMATCH':
                raise ValueError(f'Unexpected character {value!r}')
            case _:
                raise ValueError(f'Unexpected token type {kind}')

    return collected_tokens