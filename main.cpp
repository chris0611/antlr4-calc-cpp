#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include <string>
#include <stack>
#include <memory>
#include <cmath>

#include "antlr4-runtime.h"
#include "calcLexer.h"
#include "calcParser.h"
#include "calcBaseListener.h"

using namespace antlr4;

enum class Funcs {
    SIN, COS, TAN, SQRT, HYPOT
};

class EvalListener : public calcBaseListener {
public:
    EvalListener() {
        stack = {};
        results = {};
        memory = {};
        functions = {
            {"sin", Funcs::SIN},
            {"cos", Funcs::COS},
            {"tan", Funcs::TAN},
            {"sqrt", Funcs::SQRT},
            {"hypot", Funcs::HYPOT}
        };
    }

    void exitNumber(calcParser::NumberContext *ctx) override {
        double num = std::stod(ctx->NUMBER()->getText());
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
        std::vector<double> args = {};
        const size_t argCount = ctx->arg->expr().size(); 

        for (size_t i = 0; i < argCount; i++)
        {
            args.push_back(stack.top());
            stack.pop();
        }

        // We want args reversed
        std::reverse(args.begin(), args.end());

        if (functions.find(ctx->fn->getText()) == functions.end()) {
            std::cerr << "ERROR: Unknown Function '" << ctx->fn->getText() << "'\n";
            std::exit(1);
        }

        switch (functions.at(ctx->fn->getText())) {
        case Funcs::SIN: 
            stack.push(std::sin(args[0]));
            break;
        case Funcs::COS:
            stack.push(std::cos(args[0]));
            break;
        case Funcs::TAN:
            stack.push(std::tan(args[0]));
            break;
        case Funcs::SQRT:
            stack.push(std::sqrt(args[0]));
            break;
        case Funcs::HYPOT:
            if (argCount == 2) {
                double res = std::hypot(args[0], args[1]);
                stack.push(res);
            } else if (argCount == 3) {
                double res = std::hypot(args[0], args[1], args[2]);
                stack.push(res);
            }
            break;
        }
    }

    void exitVariable(calcParser::VariableContext *ctx) override {
        if (memory.find(ctx->var->getText()) == memory.end()) {
            // TODO: Handle this better
            stack.push(0);
            return;
        }

        double val = memory[ctx->var->getText()];
        stack.push(val);
    }

    void exitVariableAssignment(calcParser::VariableAssignmentContext *ctx) override {
        double val = stack.top();
        memory[ctx->var->getText()] = val;
    }

    void exitClear(calcParser::ClearContext *ctx) override {
        memory.clear();
        stack.push(0);
    }

    const std::vector<double>& getResults() const {
        return results;
    }

private:
    std::stack<double> stack;
    std::vector<double> results;
    std::unordered_map<std::string, double> memory;
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
