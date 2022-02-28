#ifndef PASS2VISITOR_H_
#define PASS2VISITOR_H_

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

class Pass2Visitor : public ExprBaseVisitor
{
private:
	string program_name;
	string j_file_name;
	ofstream j_file;

public:
	Pass2Visitor();
    virtual ~Pass2Visitor();

    ostream& get_assembly_file();

    antlrcpp::Any visitProgram(ExprParser::ProgramContext *ctx) override;
    antlrcpp::Any visitHeader(ExprParser::HeaderContext *ctx) override;
    antlrcpp::Any visitFunctionDefinition(ExprParser::FunctionDefinitionContext *ctx) override;
    antlrcpp::Any visitFunctionCall(ExprParser::FunctionCallContext *ctx) override;
    antlrcpp::Any visitBlock(ExprParser::BlockContext *ctx) override;
    antlrcpp::Any visitDeclareStmt(ExprParser::DeclareStmtContext *ctx) override;
    antlrcpp::Any visitStmt(ExprParser::StmtContext *ctx) override;
    antlrcpp::Any visitIfStmt(ExprParser::IfStmtContext *ctx) override;
    antlrcpp::Any visitIfStrStmt(ExprParser::IfStrStmtContext *ctx) override;
    antlrcpp::Any visitForStmt(ExprParser::ForStmtContext *ctx) override;
    antlrcpp::Any visitAssignmentStmt(ExprParser::AssignmentStmtContext *ctx) override;
    antlrcpp::Any visitPrintStmt(ExprParser::PrintStmtContext *ctx) override;
    antlrcpp::Any visitAddSubExpr(ExprParser::AddSubExprContext *ctx) override;
    antlrcpp::Any visitMulDivExpr(ExprParser::MulDivExprContext *ctx) override;
    antlrcpp::Any visitRelExpr(ExprParser::RelExprContext *ctx) override;
    antlrcpp::Any visitVariableExpr(ExprParser::VariableExprContext *ctx) override;
    //antlrcpp::Any visitFuncCallExpr(ExprParser::FuncCallExprContext *ctx) override;
    antlrcpp::Any visitSignedNumber(ExprParser::SignedNumberContext *ctx) override;
    antlrcpp::Any visitIntegerConst(ExprParser::IntegerConstContext *ctx) override;
    antlrcpp::Any visitFloatConst(ExprParser::FloatConstContext *ctx) override;
    antlrcpp::Any visitBoolConst(ExprParser::BoolConstContext *ctx) override;
};

#endif /* PASS2VISITOR_H_ */
