from ..token.token import Token
from ..token.literal_token import LiteralToken
from ..token.operator_token import Operator, OperatorToken


def tokenize(input_string: str) -> list[Token]:
    collected_tokens: list[Token] = []
    operators = ['+', '-', '*', 'x', '/', '(', ')', '^']

    skip_index = 0
    for index in range(len(input_string)):
        if skip_index > 0:
            skip_index -= 1
            continue
        if input_string[index].isspace():
            continue
        if input_string[index].isdecimal():
            literal = ''
            decimal_points_count = 0
            while index + skip_index < len(input_string):
                if input_string[index + skip_index] == '.':
                    literal += input_string[index + skip_index]
                    decimal_points_count += 1
                    if decimal_points_count > 1:
                        raise ValueError(f'Malformed input string. Invalid number ending at index {index+skip_index} found. Too many decimals.')
                elif input_string[index + skip_index].isdecimal():
                    literal += input_string[index + skip_index]
                    # collect if end of literal input
                    if index+skip_index+1 >= len(input_string) or input_string[index+skip_index+1].isspace() or input_string[index+skip_index+1] in operators:
                        try:
                            value = float(literal)
                            collected_tokens.append(LiteralToken(value))
                            break
                        except:
                            raise ValueError(f'Malformed input string. Invalid number ending at index {index+skip_index} found. Malformed decimal number.')
                skip_index += 1
        elif input_string[index] in operators:
            if input_string[index] in ['+', '-'] and (index == 0 or isinstance(collected_tokens[-1], OperatorToken) and collected_tokens[-1].operator != Operator.RPAREN):
                collected_tokens.append(OperatorToken.unary_from_char(input_string[index]))
            else:
                collected_tokens.append(OperatorToken.binary_from_char(input_string[index]))
        else:
            raise ValueError(f'Malformed input string. Unexpected character at index {index} received.')
    return collected_tokens