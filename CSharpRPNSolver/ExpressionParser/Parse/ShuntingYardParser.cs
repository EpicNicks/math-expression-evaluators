using ExpressionParser.Tokens;
using System.Text.RegularExpressions.Generated;

namespace ExpressionParser.Parse;

internal static class ShuntingYardParser
{
    public static decimal Parse(List<Token> tokens)
    {
        var rpnList = BuildRPNFrom(tokens);
        return SolveRPNOf(rpnList).result;
    }

    public static List<Token> BuildRPNFrom(List<Token> tokens)
    {
        IEnumerable<InvalidToken> invalidTokens = tokens.OfType<InvalidToken>();
        if (invalidTokens.Any())
        {
            IEnumerable<ArgumentException> invalidTokenExceptions = invalidTokens.Select(t => new ArgumentException($"Invalid token: {t.InvalidShard} at position {t.Index}"));
            throw new AggregateException("Parsing errors encountered:", invalidTokenExceptions);
        }

        var operatorStack = new Stack<OperatorToken>();
        var rpnSolveList = new List<Token>();

        // build the solve list
        foreach (var token in tokens)
        {
            if (token is LiteralToken)
            {
                rpnSolveList.Add(token);
            }
            else if (token is OperatorToken opToken)
            {
                if (opToken.Value == Operator.RPAREN)
                {
                    do
                    {
                        if (operatorStack.Count == 0)
                        {
                            throw new InvalidOperationException($"{{ShuntingYardParser:Parse}}: Right Paren was missing a corresponding Left Paren");
                        }
                        var op = operatorStack.Pop();
                        if (op.Value != Operator.LPAREN)
                        {
                            rpnSolveList.Add(op);
                        }
                        else
                        {
                            break;
                        }
                    } while (operatorStack.Count > 0);
                }
                else
                {
                    // check precedence
                    bool PrecedenceCompare(OperatorToken otherToken)
                    {
                        if (otherToken.Associativity == Associativity.LEFT)
                        {
                            return opToken.Precedence <= otherToken.Precedence;
                        }
                        else if (otherToken.Associativity == Associativity.RIGHT)
                        {
                            return opToken.Precedence < otherToken.Precedence;
                        }
                        else //Associativity.NONE (paren)
                        {
                            return false;
                        }
                    }
                    while (operatorStack.Count > 0 && PrecedenceCompare(operatorStack.Peek()))
                    {
                        rpnSolveList.Add(operatorStack.Pop());
                    }
                    operatorStack.Push(opToken);
                }
            }
        }

        // dump remaining operators from operator stack
        while (operatorStack.Count > 0)
        {
            rpnSolveList.Add(operatorStack.Pop());
        }

        return rpnSolveList;
    }

    public static (decimal result, List<string> steps) SolveRPNOf(List<Token> rpnSolveList)
    {
        IEnumerable<InvalidToken> invalidTokens = rpnSolveList.OfType<InvalidToken>();
        if (invalidTokens.Any())
        {
            IEnumerable<ArgumentException> invalidTokenExceptions = invalidTokens.Select(t => new ArgumentException($"Invalid token: {t.InvalidShard} at position {t.Index}"));
            throw new AggregateException("Parsing errors encountered:", invalidTokenExceptions);
        }
        // resolve the solve list
        List<string> steps = ["Showing steps for RPN solve:"];

        var step = 0;
        var stepChanged = true;
        for (int i = 0; i < rpnSolveList.Count && rpnSolveList.Count > 1; i++)
        {
            if (rpnSolveList[i] is OperatorToken opToken)
            {
                if (i < opToken.NumOperands)
                {
                    throw new InvalidOperationException($"{{ShuntingYardParser:Parse}}: Operator too far left: missing arguments");
                }
                else
                {
                    if (opToken.NumOperands == 2)
                    {
                        if (rpnSolveList[i - 2] is LiteralToken lhs && rpnSolveList[i - 1] is LiteralToken rhs)
                        {
                            var arithmeticResult = opToken.Value switch
                            {
                                Operator.MULTIPLY => lhs.Value * rhs.Value,
                                Operator.DIVIDE => lhs.Value / rhs.Value,
                                Operator.PLUS => lhs.Value + rhs.Value,
                                Operator.MINUS => lhs.Value - rhs.Value,
                                Operator.EXPONENT => (decimal)Math.Pow((double)lhs.Value, (double)rhs.Value)
                            };

                            step++;
                            stepChanged = true;
                            steps.Add($"BINARY OPERATION {opToken.ToSimpleString()} ON {lhs.ToSimpleString()} AND {rhs.ToSimpleString()} => INSERT {{{arithmeticResult}}}");

                            rpnSolveList.RemoveAt(i);
                            rpnSolveList.RemoveAt(i - 1);
                            rpnSolveList[i - 2] = new LiteralToken(arithmeticResult);
                            i -= 2;
                        }
                        else
                        {
                            throw new InvalidOperationException($"{{ShuntingYardParser:Parse}}: Operator arguments were not both literals");
                        }
                    }
                    else if (opToken.NumOperands == 1)
                    {
                        if (rpnSolveList[i - 1] is LiteralToken operand)
                        {
                            var arithmeticResult = opToken.Value switch
                            {
                                Operator.UNARY_PLUS => +operand.Value,
                                Operator.UNARY_MINUS => -operand.Value,
                            };

                            step++;
                            stepChanged = true;
                            steps.Add($"UNARY OPERATION {opToken.ToSimpleString()} ON {operand.ToSimpleString()} => INSERT {{{arithmeticResult}}}");

                            rpnSolveList.RemoveAt(i);
                            rpnSolveList[i - 1] = new LiteralToken(arithmeticResult);
                            i--;
                        }
                        else
                        {
                            throw new InvalidOperationException($"{{ShuntingYardParser:Parse}}: Operator argument for op: {opToken.Value} was not a literal");
                        }
                    }
                    else
                    {
                        throw new InvalidOperationException($"{{ShuntingYardParser:Parse}}: Invalid number of operands {opToken.NumOperands} for operator {opToken.Value}");
                    }
                }
            }

            if (stepChanged)
            {
                steps.Add($"step {step} list: {string.Join(' ', rpnSolveList.Select(token => token.ToSimpleString()))}");
                stepChanged = false;
            }
        }

        if (rpnSolveList.Count == 1 && rpnSolveList[0] is LiteralToken litResult)
        {
            return (litResult.Value, steps);
        }
        throw new InvalidOperationException($"{rpnSolveList}");
    }
}
