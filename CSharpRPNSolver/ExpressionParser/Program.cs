using ExpressionParser.Parse;
using ExpressionParser.Tokens;

decimal Calculate(string expression)
{
    var tokens = AlgebraTokenizer.Tokenize(expression);
    return ShuntingYardParser.Parse(tokens);
}

#pragma warning disable CS8321 // Local function is declared but never used
void TestExpressions()
{
    Console.WriteLine($"{Calculate("-3 + 4")} = 1");
    Console.WriteLine($"{Calculate("10 * (2 - 5)")} = -30");
    Console.WriteLine($"{Calculate("10 * -(2 - 5)")} = 30");
    Console.WriteLine($"{Calculate("-(2 - 1)/-(1 - 2)")} = -1");
    Console.WriteLine($"{Calculate("-(2 - 1)/-(1 - 2)")} = -1");
    Console.WriteLine($"{Calculate("2 + -3 * (4 - 2)")} = -4");
    Console.WriteLine($"{Calculate("2 + --3 * (4 - 2) * 3")} = 20");
}
#pragma warning restore CS8321 // Local function is declared but never used

void CalculateAndPrintSteps(string expression)
{
    var tokens = AlgebraTokenizer.Tokenize(expression);
    var rpnList = ShuntingYardParser.BuildRPNFrom(tokens);
    var (result, steps) = ShuntingYardParser.SolveRPNOf(new List<Token>(rpnList));

    Console.WriteLine($"Expression: {expression}");
    Console.WriteLine($"Tokens:     {string.Join(' ', tokens.Select(token => $"{token.ToSimpleString()}"))}");
    Console.WriteLine($"RPN:        {string.Join(' ', rpnList.Select(token => $"{token.ToSimpleString()}"))}");
    Console.WriteLine($"Steps:      {string.Join("\n\t\t", steps)}");
    Console.WriteLine($"Result:     {result}");
}

CalculateAndPrintSteps("2 - --1^2 - 3^3");