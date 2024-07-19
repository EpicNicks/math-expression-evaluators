from ..token.token import Token
from ..token.literal_token import LiteralToken
from ..token.operator_token import OperatorToken, Operator, Associativity


def rpn_from_tokens(tokens: list[Token]) -> list[Token]:
    operator_stack: list[OperatorToken] = []
    rpn_solve_list: list[Token] = []

    for token in tokens:
        if isinstance(token, LiteralToken):
            rpn_solve_list.append(token)
        elif isinstance(token, OperatorToken):
            if token.operator == Operator.RPAREN:
                while True:
                    if len(operator_stack) == 0:
                        raise ValueError(f"Mismatched Parentheses in token list\n\nSolve List:{rpn_solve_list}\n\nOperator Stack:{operator_stack}")
                    if (last_operator := operator_stack.pop()).operator != Operator.LPAREN:
                        rpn_solve_list.append(last_operator)
                    else:
                        break
                    if len(operator_stack) == 0:
                        break
            else:
                while len(operator_stack) != 0 and precedence_compare(token, operator_stack[-1]):
                    rpn_solve_list.append(operator_stack.pop())
                operator_stack.append(token)
    while len(operator_stack) != 0:
        rpn_solve_list.append(operator_stack.pop())

    return rpn_solve_list


def solve_rpn(tokens: list[Token]) -> tuple[float, list[str]]:
    tokens = tokens.copy() # to not mess with the original parameters
    steps = ["Showing steps for RPN Solve:"]
    step = 0
    step_changed = True

    i = 0
    while i < len(tokens):
        token = tokens[i]
        if isinstance(token, OperatorToken):
            if i < token.numOperands:
                raise ValueError(f'rpn solve failed: Operator at index [{i}] too far left: missing arguments')
            else:
                if token.numOperands == 2:
                    lhs, rhs = tokens[i-2], tokens[i-1]
                    if not isinstance(lhs, LiteralToken) or not isinstance(rhs, LiteralToken):
                        raise ValueError('Operator arguments were not both literals')
                    arithmetic_result = 0
                    match token.operator:
                        case Operator.PLUS:
                            arithmetic_result = lhs.value + rhs.value
                        case Operator.MINUS:
                            arithmetic_result = lhs.value - rhs.value
                        case Operator.MULTIPLY:
                            arithmetic_result = lhs.value * rhs.value
                        case Operator.DIVIDE:
                            arithmetic_result = lhs.value / rhs.value
                        case Operator.EXPONENT:
                            arithmetic_result = pow(lhs.value, rhs.value)
                    step += 1
                    step_changed = True
                    steps.append(f"BINARY OPERATION {token.to_simple_string()} ON {lhs.to_simple_string()} AND {rhs.to_simple_string()} => INSERT {arithmetic_result}")
                    
                    tokens.remove(tokens[i])
                    tokens.remove(tokens[i-1])
                    tokens[i-2] = LiteralToken(arithmetic_result)
                    i -= 2
                elif token.numOperands == 1:
                    lhs = tokens[i-1]
                    if not isinstance(lhs, LiteralToken):
                        raise ValueError(f'Operator argument for op: {lhs.to_simple_string()} was not a literal')
                    arithmetic_result = 0
                    match token.operator:
                        case Operator.UNARY_PLUS:
                            arithmetic_result = +lhs.value
                        case Operator.UNARY_MINUS:
                            arithmetic_result = -lhs.value
                    step += 1
                    step_changed = True
                    steps.append(f'UNARY OPERATION {token.to_simple_string()} ON {lhs.to_simple_string()} => INSERT {arithmetic_result}')

                    tokens.remove(tokens[i])
                    tokens[i-1] = LiteralToken(arithmetic_result)
                    i -= 1
        if step_changed:
            steps.append(f"step: {step} list: {' '.join(token.to_simple_string() for token in tokens)}")
            step_changed = False
        i += 1
    if len(tokens) == 1 and isinstance(tokens[0], LiteralToken):
        return tokens[0].value, steps
    else:
        raise ValueError(f'rpn solve list did not evaluate to a single literal result, tokens left: {tokens}')


def precedence_compare(opToken: OperatorToken, otherOpToken: OperatorToken) -> bool:
    if otherOpToken.associativity == Associativity.LEFT:
        return opToken.precedence() <= otherOpToken.precedence()
    elif otherOpToken.associativity == Associativity.RIGHT:
        return opToken.precedence() < otherOpToken.precedence()
    else:
        return False # must be a paren