#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

#include "Token.hpp"
#include "TokenizerFsm.hpp"
#include "InfixToRpnConverter.hpp"
#include "RpnEvaluatorVisitor.hpp"

static std::string joinToString(std::vector<std::string>& items, const std::string& delimiter) {
    std::stringstream ss{};
    for (size_t i = 0; i < items.size(); i++) {
        ss << items[i];
        if (i < items.size() - 1) {
            ss << delimiter;
        }
    }
    return ss.str();
}

static std::vector<std::string> tokensToStrings(std::vector<std::unique_ptr<Token>>& tokens) {
    std::vector<std::string> output;
    output.reserve(tokens.size());
    std::transform(tokens.begin(), tokens.end(), std::back_inserter(output),
        [](const std::unique_ptr<Token>& item) { return item.get()->toSimpleString(); });
    return output;
}

void solveAndPrintSteps(const std::string& input) {
    std::cout << input << std::endl;

    std::vector<std::unique_ptr<Token>> tokens = tokenizeFsm(input);
    std::vector<std::string> tokensStrings = tokensToStrings(tokens);
    std::cout << joinToString(tokensStrings, " ") << std::endl;

    std::vector<std::unique_ptr<Token>> rpnTokens = convertToRPN(tokens);
    std::vector<std::string> rpnTokenStrings = tokensToStrings(rpnTokens);
    std::cout << joinToString(rpnTokenStrings, " ") << std::endl;

    RpnEvaluatorVisitor rpnEvaluatorVisitor{};
    double result = rpnEvaluatorVisitor.evaluate(rpnTokens);
    std::vector<std::string> steps = rpnEvaluatorVisitor.getSteps();
    std::cout << "Steps for RPN solve:\n\t" << joinToString(steps, "\n\t") << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc == 1) {
        std::string exampleExpression{ "2 + -2^(10 / 5) / 5" };
        std::cout << "RPN solve on example expression: " << exampleExpression << std::endl;
        solveAndPrintSteps(exampleExpression);
    }
    else if (argc == 2) {
        std::string input{ argv[1] };
        solveAndPrintSteps(input);
    }
    else {
        std::cout << "Invalid number of arguments passed. Pass one argument to parse that expression or none to show parsing of an example expression." << std::endl;
    }
}
