namespace ExpressionParser.Tokens;

public class OperatorToken : Token
{
    private readonly Operator value;
    private readonly int numOperands;
    private readonly Associativity associativity;

    private OperatorToken(Operator value, int numOperands, Associativity associativity)
    {
        this.value = value;
        this.numOperands = numOperands;
        this.associativity = associativity;
    }

    public Operator Value => value;
    public Associativity Associativity => associativity;
    public int NumOperands => numOperands;

    public static readonly OperatorToken PLUS = new OperatorToken(Operator.PLUS, 2, Associativity.LEFT);
    public static readonly OperatorToken MINUS = new OperatorToken(Operator.MINUS, 2, Associativity.LEFT);
    public static readonly OperatorToken MULTIPLY = new OperatorToken(Operator.MULTIPLY, 2, Associativity.LEFT);
    public static readonly OperatorToken DIVIDE = new OperatorToken(Operator.DIVIDE, 2, Associativity.LEFT);
    public static readonly OperatorToken LPAREN = new OperatorToken(Operator.LPAREN, 0, Associativity.NONE);
    public static readonly OperatorToken RPAREN = new OperatorToken(Operator.RPAREN, 0, Associativity.NONE);
    public static readonly OperatorToken UNARY_MINUS = new OperatorToken(Operator.UNARY_MINUS, 1, Associativity.RIGHT);
    public static readonly OperatorToken UNARY_PLUS = new OperatorToken(Operator.UNARY_PLUS, 1, Associativity.RIGHT);
    public static readonly OperatorToken EXPONENT = new OperatorToken(Operator.EXPONENT, 2, Associativity.RIGHT);

    public static OperatorToken UnaryFromChar(char opChar) => opChar switch
    {
        '+' => UNARY_PLUS,
        '-' => UNARY_MINUS,
        _ => throw new InvalidOperationException($"{{OperatorToken.UnaryFromChar}}: Illegal char ({opChar}) provided")
    };

    public static OperatorToken BinaryFromChar(char opChar) => opChar switch
    {
        '+' => PLUS,
        '-' => MINUS,
        '*' or 'x' => MULTIPLY,
        '/' => DIVIDE,
        '(' => LPAREN,
        ')' => RPAREN,
        '^' => EXPONENT,
        _ => throw new InvalidOperationException($"{{OperatorToken.BinaryFromChar}}: Illegal char ({opChar}) provided")
    };

    public int Precedence => value switch
    {
        // parens are a special case anyhow in shunting yard but they get top precedence anyway
        Operator.LPAREN or Operator.RPAREN => 5,
        Operator.UNARY_PLUS or Operator.UNARY_MINUS => 4,
        Operator.EXPONENT => 3,
        Operator.MULTIPLY or Operator.DIVIDE => 2,
        Operator.PLUS or Operator.MINUS => 1,
        _ => throw new InvalidOperationException($"Invalid token with value: {value}")
    };

    public override string ToString() => $"{{TokenType: OPERATOR, Operator: {Value}}}";
    public override string ToSimpleString() => $"{{{Value}}}";
}