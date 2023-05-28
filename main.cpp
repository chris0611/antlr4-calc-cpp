#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <stack>
#include <memory>
#include <cmath>

#include "calcLexer.h"
#include "calcParser.h"
#include "calcBaseListener.h"

using namespace antlr4;

enum class Funcs {
    SIN, COS, TAN, SQRT
};

class EvalListener : public calcBaseListener {
public:
    EvalListener() {
        stack = std::stack<double>();
        results = std::vector<double>();
        functions = {
            {"sin", Funcs::SIN},
            {"cos", Funcs::COS},
            {"tan", Funcs::TAN},
            {"sqrt", Funcs::SQRT}
        };
    }

    void exitNumber(calcParser::NumberContext *ctx) override {
        double num = std::stod(ctx->NUMBER()->toString());
        stack.push(num);
    }

    void exitUnaryPlusOrNegation(calcParser::UnaryPlusOrNegationContext *ctx) override {
        if (ctx->op->getText() == "-") {
            double old = stack.top();
            stack.pop();
            stack.push(-1 * old);
        }
    }

    void exitPlusOrMinus(calcParser::PlusOrMinusContext *ctx) override {
        double rhs = stack.top();
        stack.pop();
        double lhs = stack.top();
        stack.pop();
        if (ctx->op->getText() == "+") {
            stack.push(lhs + rhs);
        } else {
            stack.push(lhs - rhs);
        }
    }

    void exitMultiplyOrDivide(calcParser::MultiplyOrDivideContext *ctx) override {
        double rhs = stack.top();
        stack.pop();
        double lhs = stack.top();
        stack.pop();
        if (ctx->op->getText() == "*") {
            stack.push(lhs * rhs);
        } else {
            stack.push(lhs / rhs);
        }
    }

    void exitExponentiation(calcParser::ExponentiationContext *ctx) override {
        double exp = stack.top();
        stack.pop();
        double base = stack.top();
        stack.pop();
        double result = std::pow(base, exp);
        stack.push(result); 
    }

    void exitImplicitMultiplication(calcParser::ImplicitMultiplicationContext *ctx) override {
        double rhs = stack.top();
        stack.pop();
        double lhs = stack.top();
        stack.pop();
        double res = rhs * lhs;
        stack.push(res);
    }

    void exitExprStmt(calcParser::ExprStmtContext *ctx) override {
        results.push_back(stack.top());
        stack.pop();
    }

    void exitFunction(calcParser::FunctionContext *ctx) override {
        // TODO: Check number of arguments
        double val = stack.top();
        stack.pop();

        if (functions.find(ctx->fn->getText()) == functions.end()) {
            std::cerr << "ERROR: Unknown Function '" << ctx->fn->getText() << "'\n";
            std::exit(1);
        }

        switch (functions.at(ctx->fn->getText())) {
        case Funcs::SIN: 
            stack.push(std::sin(val));
            break;
        case Funcs::COS:
            stack.push(std::cos(val));
            break;
        case Funcs::TAN:
            stack.push(std::tan(val));
            break;
        case Funcs::SQRT:
            stack.push(std::sqrt(val));
            break;
        }
    }

    const std::vector<double>& getResults() const {
        return results;
    }

private:
    std::stack<double> stack;
    std::vector<double> results;
    std::unordered_map<std::string_view, Funcs> functions;
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

    tree::ParseTree *tree = parser.stmts();
    EvalListener listener;
    tree::ParseTreeWalker::DEFAULT.walk(&listener, tree);

    const std::vector<double> &results = listener.getResults();

    for (auto res : results) {
        std::cout << res << '\n';
    }

    return 0;
}
