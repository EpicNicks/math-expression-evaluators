using System.Text.RegularExpressions;

using ExpressionParser.Tokens;

namespace ExpressionParser.Parse.V2;
public partial class Tokenizer
{
    private const string
        SKIP_CAPTURE = "SKIP",
        NUMBER_CAPTURE = "NUMBER",
        OPERATOR_CAPTURE = "OPERATOR",
        MISMATCH_CAPTURE = "MISMATCH";

    private static readonly List<char> unaryOperators = ['+', '-'];

    public static List<Token> Tokenize(string inputString)
    {
        List<Token> collectedTokens = [];

        foreach (Match match in Tokens().Matches(inputString).Cast<Match>())
        {
            if (match.Groups[SKIP_CAPTURE].Success)
                continue;

            if (match.Groups[NUMBER_CAPTURE].Success)
            {
                collectedTokens.Add(new LiteralToken(decimal.Parse(match.Groups[NUMBER_CAPTURE].Value)));
            }
            else if (match.Groups[OPERATOR_CAPTURE].Success)
            {
                char operatorChar = match.Groups[OPERATOR_CAPTURE].Value[0];
                if (unaryOperators.Contains(operatorChar) && (collectedTokens.Count == 0 || (collectedTokens[^1] is OperatorToken lastOp && lastOp.Value != Operator.RPAREN)))
                {
                    collectedTokens.Add(OperatorToken.UnaryFromChar(operatorChar));
                }
                else
                {
                    collectedTokens.Add(OperatorToken.BinaryFromChar(operatorChar));
                }
            }
            else if (match.Groups[MISMATCH_CAPTURE].Success)
            {
                collectedTokens.Add(new InvalidToken(match.Groups[MISMATCH_CAPTURE].Value, match.Index));
            }
        }

        return collectedTokens;
    }


    [GeneratedRegex(@$"(?<{NUMBER_CAPTURE}>\d+(\.\d*)?)|(?<{OPERATOR_CAPTURE}>[+\-*x\/^()])|(?<{SKIP_CAPTURE}>\s+)|(?<{MISMATCH_CAPTURE}>.)")]
    private static partial Regex Tokens();
}
