#include <stack>
#include <vector>
#include <string>

#include "TokenVisitor.hpp"
#include "OperatorToken.hpp"
#include "LiteralToken.hpp"
#include "InvalidToken.hpp"

class RpnEvaluatorVisitor : public TokenVisitor {
public:
	void visit(const InvalidToken& invalidToken) override;
	void visit(const LiteralToken& literalToken) override;
	void visit(const OperatorToken& literalToken) override;

	double evaluate(const std::vector<std::unique_ptr<Token>>& rpnTokens);
	std::vector<std::string> getSteps();

private:
	std::stack<double> m_operandStack;
	std::vector<std::string> m_steps;
	double getResult();
};