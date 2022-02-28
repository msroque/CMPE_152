#ifndef PASS1VISITOR_H_
#define PASS1VISITOR_H_

#include <iostream>
#include <string>

#include "wci/intermediate/SymTabStack.h"
#include "wci/intermediate/SymTabEntry.h"
#include "wci/intermediate/TypeSpec.h"

#include "ExprBaseVisitor.h"
#include "antlr4-runtime.h"
#include "ExprVisitor.h"

using namespace wci;
using namespace wci::intermediate;

class Pass1Visitor : public ExprBaseVisitor
{
private:
    SymTabStack *symtab_stack;
    vector<SymTabEntry *> variable_id_list;
    vector<ExprParser::VarIdContext *> variable_ctx_list;
    ofstream j_file;

public:
    Pass1Visitor();
    virtual ~Pass1Visitor();

    ostream& get_assembly_file();

    bool determineType(ExprParser::TypeIdContext *ctx, TypeSpec ** type, string * type_indicator);

    antlrcpp::Any visitProgram(ExprParser::ProgramContext *ctx) override;
    antlrcpp::Any visitFunctionDefinition(ExprParser::FunctionDefinitionContext *ctx) override;
    antlrcpp::Any visitFunctionId(ExprParser::FunctionIdContext *ctx) override;
    antlrcpp::Any visitHeader(ExprParser::HeaderContext *ctx) override;
    antlrcpp::Any visitDeclareStmt(ExprParser::DeclareStmtContext *ctx) override;
    antlrcpp::Any visitAddSubExpr(ExprParser::AddSubExprContext *ctx) override;
    antlrcpp::Any visitMulDivExpr(ExprParser::MulDivExprContext *ctx) override;
    antlrcpp::Any visitRelExpr(ExprParser::RelExprContext *ctx) override;
    antlrcpp::Any visitVariableExpr(ExprParser::VariableExprContext *ctx) override;
    antlrcpp::Any visitSignedNumberExpr(ExprParser::SignedNumberExprContext *ctx) override;
    antlrcpp::Any visitFuncCallExpr(ExprParser::FuncCallExprContext *ctx) override;
    antlrcpp::Any visitSignedNumber(ExprParser::SignedNumberContext *ctx) override;
    antlrcpp::Any visitUnsignedNumberExpr(ExprParser::UnsignedNumberExprContext *ctx) override;
    antlrcpp::Any visitBoolExpr(ExprParser::BoolExprContext *ctx) override;
    antlrcpp::Any visitIntegerConst(ExprParser::IntegerConstContext *ctx) override;
    antlrcpp::Any visitFloatConst(ExprParser::FloatConstContext *ctx) override;
    antlrcpp::Any visitBoolConst(ExprParser::BoolConstContext *ctx) override;
    antlrcpp::Any visitParenExpr(ExprParser::ParenExprContext *ctx) override;
};

#endif /* PASS1VISITOR_H_ */
