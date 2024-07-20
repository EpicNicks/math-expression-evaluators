namespace ExpressionParser.Tokens;

public class LiteralToken(decimal value) : Token
{
    public decimal Value => value;
    public override string ToString() => $"{{TokenType: LITERAL, Value: {Value}}}";
    public override string ToSimpleString() => $"{{{Value}}}";
}
