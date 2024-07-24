"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.tokensToRpn = tokensToRpn;
const Token_1 = require("../token/Token");
const Token_2 = require("../token/Token");
function tokensToRpn(infix) {
    if (infix.some(token => (0, Token_1.isInvalidToken)(token))) {
        throw new Error("Invalid Tokens were in the infix tokens");
    }
    const rpn = [];
    const operatorStack = [];
    let step = 1;
    for (const token of infix) {
        if ((0, Token_1.isLiteralToken)(token)) {
            rpn.push(token);
            console.log(`Step ${step}: Push literal ${token.value} to output`);
            step++;
        }
        else if ((0, Token_1.isOperatorToken)(token)) {
            if (token.operator === Token_2.Operator.LPAREN) {
                operatorStack.push(token);
                console.log(`Step ${step}: Push ( to operator stack`);
                step++;
            }
            else if (token.operator === Token_2.Operator.RPAREN) {
                while (operatorStack.length > 0 && operatorStack[operatorStack.length - 1].operator !== Token_2.Operator.LPAREN) {
                    const op = operatorStack.pop();
                    rpn.push(op);
                    console.log(`Step ${step}: Pop ${op.operator} from stack to output`);
                    step++;
                }
                if (operatorStack.length > 0 && operatorStack[operatorStack.length - 1].operator === Token_2.Operator.LPAREN) {
                    operatorStack.pop(); // Remove left parenthesis
                    console.log(`Step ${step}: Pop ( from operator stack and discard`);
                    step++;
                }
                else {
                    throw new Error("Mismatched parentheses");
                }
            }
            else {
                while (operatorStack.length > 0 &&
                    operatorStack[operatorStack.length - 1].operator !== Token_2.Operator.LPAREN &&
                    (operatorStack[operatorStack.length - 1].precedence > token.precedence ||
                        (operatorStack[operatorStack.length - 1].precedence === token.precedence &&
                            token.associativity === Token_1.Associativity.LEFT))) {
                    const op = operatorStack.pop();
                    rpn.push(op);
                    console.log(`Step ${step}: Pop ${op.operator} from stack to output (higher precedence)`);
                    step++;
                }
                operatorStack.push(token);
                console.log(`Step ${step}: Push ${token.operator} to operator stack`);
                step++;
            }
        }
        else if ((0, Token_1.isInvalidToken)(token)) {
            throw new Error(`Invalid token: ${token.invalidShard} at position ${token.position}`);
        }
    }
    while (operatorStack.length > 0) {
        const op = operatorStack.pop();
        if (op.operator === Token_2.Operator.LPAREN || op.operator === Token_2.Operator.RPAREN) {
            throw new Error("Mismatched parentheses");
        }
        rpn.push(op);
        console.log(`Step ${step}: Pop remaining ${op.operator} from stack to output`);
        step++;
    }
    return rpn;
}
