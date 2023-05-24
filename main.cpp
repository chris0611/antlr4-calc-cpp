#include <iostream>
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

    double getResult() {
        return stack->top();
    }

private:
    std::unique_ptr<std::stack<double>> stack;
};

int main(int argc, const char *argv[])
{
    std::ifstream stream;
    stream.open(argv[1]);   

    ANTLRInputStream input(stream);
    calcLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    calcParser parser(&tokens);

    tree::ParseTree *tree = parser.statement();
    TreeShapeListener listener;
    tree::ParseTreeWalker::DEFAULT.walk(&listener, tree);

    double result = listener.getResult();

    std::cout << "Result: " << result << '\n';

    return 0;
}