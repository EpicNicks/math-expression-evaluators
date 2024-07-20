using ExpressionParser.Tokens;

namespace ExpressionParser.Parse.V1;

internal static class AlgebraTokenizer
{
    public static List<Token> Tokenize(string input)
    {
        var collected = new List<Token>();
        var operators = new char[] { '+', '-', '*', 'x', '/', '(', ')', '^' };

        for (int i = 0; i < input.Length; i++)
        {
            if (char.IsWhiteSpace(input[i]))
            {
                continue;
            }
            if (char.IsDigit(input[i]))
            {
                var literal = "";
                var decimalPointsCount = 0;
                for (; i < input.Length; i++)
                {
                    if (input[i] == '.')
                    {
                        literal += input[i];
                        decimalPointsCount++;
                        if (decimalPointsCount > 1)
                        {
                            throw new InvalidOperationException($"{{TokenizerError}}: second decimal point found in numeric literal at index [{i}]");
                        }
                    }
                    else if (char.IsDigit(input[i]))
                    {
                        literal += input[i];
                        if (i + 1 >= input.Length || char.IsWhiteSpace(input[i + 1]) || operators.Contains(input[i + 1]))
                        {
                            collected.Add(new LiteralToken(decimal.Parse(literal)));
                            break;
                        }
                    }
                    else
                    {
                        throw new InvalidOperationException($"{{TokenizerError}}: unexpected char {input[i]} received at index [{i}]");
                    }
                }
            }
            else if (operators.Contains(input[i]))
            {
                if ((input[i] == '-' || input[i] == '+') && (i == 0 || collected.Last() is OperatorToken lastOp && lastOp.Value != Operator.RPAREN))
                {
                    collected.Add(OperatorToken.UnaryFromChar(input[i]));
                }
                else
                {
                    collected.Add(OperatorToken.BinaryFromChar(input[i]));
                }
            }
            else
            {
                throw new InvalidOperationException($"{{TokenizerError}}: unexpected char {input[i]} received at index [{i}]");
            }
        }

        return collected;
    }
}
