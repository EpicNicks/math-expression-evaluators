import sys

from solver.parse.tokenizer_fsm import tokenize
from solver.parse.parser import rpn_from_tokens, solve_rpn

def calculate_and_print_steps(expression: str):
    print(expression)
    tokens = tokenize(expression)
    print(tokens)
    rpn = rpn_from_tokens(tokens)
    print(rpn)
    solvedValue, steps = solve_rpn(rpn)
    print("result:", solvedValue)
    print('\n\t'.join(('\t' if index % 2 == 0 else '') + step for index, step in enumerate(steps)))


def main():
    if len(sys.argv) > 2:
        print("Too many args. Provide an expression as the first argument or no arguments to use an example expression")
        return
    expression = '22.5 x (1/2)^-2 - 3^3'
    if len(sys.argv) == 2:
        expression = sys.argv[1]
    else:
        print("Solve on example expression:")
    calculate_and_print_steps(expression)
    


if __name__ == "__main__":
    main()