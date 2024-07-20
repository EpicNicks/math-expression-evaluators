namespace ExpressionParser.Tokens;
public class InvalidToken : Token
{
    public int Index { get; private set; }
    public string InvalidShard { get; private set; }
    public InvalidToken(string invalidShard, int index)
    {
        InvalidShard = invalidShard;
        Index = index;
    }

    public override string ToSimpleString()
    {
        return "<INVALID>";
    }
}
