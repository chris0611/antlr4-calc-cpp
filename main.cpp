#include <iostream>
#include <string>
#include <stack>
#include <memory>
#include <cmath>

#include "calcLexer.h"
#include "calcParser.h"
#include "calcBaseListener.h"

using namespace antlr4;

class TreeShapeListener : public calcBaseListener {
public:
    TreeShapeListener() {
        stack = std::make_unique<std::stack<double>>();
    }

    void exitNumber(calcParser::NumberContext *ctx) override {
        double num = std::stod(ctx->NUMBER()->toString());
        stack->push(num);
    }

    void exitNegationOrUnaryPlus(calcParser::NegationOrUnaryPlusContext *ctx) override {
        if (ctx->op->getText() == "-") {
            double old = stack->top();
            stack->pop();
            stack->push(-1 * old);
        }
    }

    void exitPlusOrMinus(calcParser::PlusOrMinusContext *ctx) override {
        double rhs = stack->top();
        stack->pop();
        double lhs = stack->top();
        stack->pop();
        if (ctx->op->getText() == "+") {
            stack->push(lhs + rhs);
        } else {
            stack->push(lhs - rhs);
        }
    }

    void exitMultiplyOrDivide(calcParser::MultiplyOrDivideContext *ctx) override {
        double rhs = stack->top();
        stack->pop();
        double lhs = stack->top();
        stack->pop();
        if (ctx->op->getText() == "*") {
            stack->push(lhs * rhs);
        } else {
            stack->push(lhs / rhs);
        }
    }

    void exitEmptyStmt(calcParser::EmptyStmtContext *ctx) override {
        stack->push(0);
    }

    void exitExponentiation(calcParser::ExponentiationContext *ctx) override {
        double exp = stack->top();
        stack->pop();
        double base = stack->top();
        stack->pop();
        double result = std::pow(base, exp);
        stack->push(result); 
    }

    void exitImplicitMultiplication(calcParser::ImplicitMultiplicationContext *ctx) override {
        double rhs = stack->top();
        stack->pop();
        double lhs = stack->top();
        stack->pop();
        double res = rhs * lhs;
        stack->push(res);
    }

    double getResult() {
        return stack->top();
    }

private:
    std::unique_ptr<std::stack<double>> stack;
};


int main(int argc, const char *argv[])
{
    ANTLRInputStream input;

    if (argc != 2) {
        ANTLRInputStream stdin_input(std::cin);
        input = stdin_input;
    } else {
        std::ifstream stream;
        stream.open(argv[1]);
        ANTLRInputStream file_input(stream);
        input = file_input;
    }

    calcLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    calcParser parser(&tokens);

    tree::ParseTree *tree = parser.stmt();
    TreeShapeListener listener;
    tree::ParseTreeWalker::DEFAULT.walk(&listener, tree);

    double result = listener.getResult();

    std::cout << result << '\n';

    return 0;
}