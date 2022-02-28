#include <iostream>
#include <string>
#include <vector>

#include "Pass1Visitor.h"
#include "wci/intermediate/SymTabFactory.h"
#include "wci/intermediate/symtabimpl/Predefined.h"
#include "wci/util/CrossReferencer.h"

using namespace std;
using namespace wci;
using namespace wci::intermediate;
using namespace wci::intermediate::symtabimpl;
using namespace wci::util;

const bool DEBUG_1 = true;
static string function_name = "";
extern string pname;

static SymTabEntry *variable_id;

#include <stdio.h>

static string EXCEPTION(string message)
{
	{
		string filename = pname + ".j";
		char file[filename.length() + 1];
		strcpy(file, filename.c_str());

		std::remove(file);
	}
	exit(1);
}

bool Pass1Visitor::determineType(ExprParser::TypeIdContext *ctx, TypeSpec ** type, string * type_indicator)
{
	if(ctx == NULL)
	{
		EXCEPTION("No type found");
	}

	string type_name = ctx->IDENTIFIER()->toString();

    bool success = true;
    if(type_name == "int")
    {
        *type = Predefined::integer_type;
        *type_indicator = "I";
    }
    else if (type_name == "bool")
    {
        *type = Predefined::boolean_type;
        *type_indicator = "Z";
    }
    else if (type_name == "string")
    {
        *type = Predefined::char_type;
        *type_indicator = "Ljava/lang/String;";
    }
    else if (type_name == "void")
    {
        *type = Predefined::void_type;
        *type_indicator = "V";
    }
    else
    {
        *type = nullptr;
        *type_indicator = "?";
        success = false;
    }
    return success;
}

Pass1Visitor::Pass1Visitor()
{
    // Create and initialize the symbol table stack.
    symtab_stack = SymTabFactory::create_symtab_stack();
    Predefined::initialize(symtab_stack);

    if (DEBUG_1) cout << "=== Pass 1: Pass1Visitor(): symtab stack initialized." << endl;
}

Pass1Visitor::~Pass1Visitor() {}

antlrcpp::Any Pass1Visitor::visitProgram(ExprParser::ProgramContext *ctx)
{
    auto value = visitChildren(ctx);

    if (DEBUG_1) cout << "=== Pass 1: visitProgram: Printing xref table." << endl;

    // Print the cross-reference table.
    CrossReferencer cross_referencer;
    cross_referencer.print(symtab_stack);

    if (DEBUG_1) cout << "=== Pass 1: visitProgram: Done printing xref table." << endl;

    return value;
}

antlrcpp::Any Pass1Visitor::visitHeader(ExprParser::HeaderContext *ctx)
{
    if (DEBUG_1) cout << "=== Pass 1: visitHeader: " + ctx->getText() << endl;

    string program_name = ctx->IDENTIFIER()->toString();

    SymTabEntry *program_id = symtab_stack->enter_local(program_name);
    program_id->set_definition((Definition)DF_PROGRAM);
    program_id->set_attribute((SymTabKey) ROUTINE_SYMTAB,
                              symtab_stack->push());
    symtab_stack->set_program_id(program_id);
    if (DEBUG_1) cout << "=== Pass 1: visitHeader: pushed " + program_name + " onto stack" << endl;

    return visitChildren(ctx);
}

antlrcpp::Any Pass1Visitor::visitDeclareStmt(ExprParser::DeclareStmtContext *ctx)
{
	if (DEBUG_1) cout << "=== Pass 1: visitDeclareStmt: " + ctx->getText() << endl;

	//varList
	variable_id_list.resize(0);
	variable_ctx_list.resize(0);

	string variable_list = ctx->varList()->getText();

	vector<string> var_array;
	stringstream var_stream(variable_list);

	while(var_stream.good())
	{
		string substr;
		getline(var_stream, substr, ',');
		var_array.push_back(substr);
	}

	if (DEBUG_1) cout << "=== Pass 1: visitDeclareStmt: var_array = " << endl;

	if (DEBUG_1)
	{
		for(int i = 0; i < var_array.size(); i++)
		{
			cout << " [ " + var_array.at(i) + " ] " << endl;
		}
	}

	//varId
	for(int i = 0; i < var_array.size(); i++)
	{
		SymTabEntry *variable_id = symtab_stack->enter_local(var_array.at(i));
		variable_id->set_definition((Definition) DF_VARIABLE);
		variable_id_list.push_back(variable_id);
	}

	//TypeID
	TypeSpec *type;

	string type_name = ctx->typeId()->getText();

	if (DEBUG_1) cout << "=== Pass 1: visitDeclareStmt: type_name = " + type_name << endl;

	type = (type_name == "int") ? Predefined::integer_type
		 : (type_name == "dec")     ? Predefined::real_type
		 : (type_name == "bool")    ? Predefined::boolean_type
		 :                            nullptr;

	// Set the type of the symbol table entries.
	for (SymTabEntry *id : variable_id_list)
	{
		id->set_typespec(type);
		if (DEBUG_1) cout << "=== Pass 1: visitDeclareStmt: ID = " + id->get_name() + " TYPE = " + type_name << endl;
	}

	// Set the type of the variable parse tree nodes.
	for (ExprParser::VarIdContext *var_ctx : variable_ctx_list)
	{
		var_ctx->type = type;
	}

	return visitChildren(ctx);
}

antlrcpp::Any Pass1Visitor::visitFunctionDefinition(ExprParser::FunctionDefinitionContext *ctx)
{
    cout << "===  Pass 1: visitFunctionDefinition: " + ctx->getText() << endl;

    function_name = ctx->functionId()->getText() + "_";

    variable_id = nullptr;
    auto value = visit(ctx->functionId());
    visit(ctx->typeId());

    TypeSpec *type;
    string type_indicator;

    if(determineType(ctx->typeId(), &type, &type_indicator) == false)
    {
        EXCEPTION("Invalid type!");
    }

    variable_id->set_typespec(type);

    visit(ctx->parameters());
    visitChildren(ctx->stmtList());
    visit(ctx->expr());
    function_name = "";

    return value;
}

antlrcpp::Any Pass1Visitor::visitFunctionId(ExprParser::FunctionIdContext *ctx)
{
    cout << "===  Pass 1: visitFunctionID: " + ctx->getText() << endl;

    string func_name = ctx->IDENTIFIER()->toString();

    if(symtab_stack->lookup(func_name) != NULL)
    {
        EXCEPTION("Redeclaration of function");
    }

    variable_id = symtab_stack->enter_local(func_name);
    variable_id->set_definition((Definition) DF_FUNCTION);

    return visitChildren(ctx);
}

antlrcpp::Any Pass1Visitor::visitAddSubExpr(ExprParser::AddSubExprContext *ctx)
{
    if (DEBUG_1) cout << "=== Pass 1: visitAddSubExpr: " + ctx->getText() << endl;

    auto value = visitChildren(ctx);

    TypeSpec *type1 = ctx->expr(0)->type;
    TypeSpec *type2 = ctx->expr(1)->type;

    bool integer_mode =    (type1 == Predefined::integer_type)
                        && (type2 == Predefined::integer_type);
    bool real_mode    =    (type1 == Predefined::real_type)
                        && (type2 == Predefined::real_type);

    TypeSpec *type = integer_mode ? Predefined::integer_type
                   : real_mode    ? Predefined::real_type
                   :                nullptr;
    ctx->type = type;

    return value;
}

antlrcpp::Any Pass1Visitor::visitMulDivExpr(ExprParser::MulDivExprContext *ctx)
{
    if (DEBUG_1) cout << "=== Pass 1: visitMulDivExpr: " + ctx->getText() << endl;

    auto value = visitChildren(ctx);

    TypeSpec *type1 = ctx->expr(0)->type;
    TypeSpec *type2 = ctx->expr(1)->type;

    bool integer_mode =    (type1 == Predefined::integer_type)
                        && (type2 == Predefined::integer_type);
    bool real_mode    =    (type1 == Predefined::real_type)
                        && (type2 == Predefined::real_type);

    TypeSpec *type = integer_mode ? Predefined::integer_type
                   : real_mode    ? Predefined::real_type
                   :                nullptr;
    ctx->type = type;

    return value;
}

antlrcpp::Any Pass1Visitor::visitRelExpr(ExprParser::RelExprContext *ctx)
{
	if (DEBUG_1) cout << "=== Pass 1: visitRelExpr: " + ctx->getText() << endl;

	    auto value = visitChildren(ctx);
	    ctx->type = Predefined::boolean_type;

	    return value;
}

antlrcpp::Any Pass1Visitor::visitFuncCallExpr(ExprParser::FuncCallExprContext *ctx)
{
    // cout << "=== visitFuncCallExpr: " + ctx->getText() << endl;

    string func_name = ctx->functionCall()->function()->IDENTIFIER()->toString();
    SymTabEntry *function_id = symtab_stack->lookup(func_name);

    if(function_id == NULL)
    {
    	EXCEPTION("Function is not defined");
    }
    ctx->type = function_id->get_typespec();

    return visitChildren(ctx);
}

antlrcpp::Any Pass1Visitor::visitVariableExpr(ExprParser::VariableExprContext *ctx)
{
    if (DEBUG_1) cout << "=== Pass 1: visitVariableExpr: " + ctx->getText() << endl;

    string variable_name = ctx->variable()->IDENTIFIER()->toString();
    SymTabEntry *variable_id = symtab_stack->lookup(variable_name);

    ctx->type = variable_id->get_typespec();
    return visitChildren(ctx);
}

antlrcpp::Any Pass1Visitor::visitSignedNumberExpr(ExprParser::SignedNumberExprContext *ctx)
{
    if (DEBUG_1) cout << "=== Pass 1: visitSignedNumberExpr: " + ctx->getText() << endl;

    auto value = visitChildren(ctx);
    ctx->type = ctx->signedNumber()->type;
    return value;
}

antlrcpp::Any Pass1Visitor::visitSignedNumber(ExprParser::SignedNumberContext *ctx)
{
    if (DEBUG_1) cout << "=== Pass 1: visitSignedNumber: " + ctx->getText() << endl;

    auto value = visit(ctx->number());
    ctx->type = ctx->number()->type;
    return value;
}

antlrcpp::Any Pass1Visitor::visitUnsignedNumberExpr(ExprParser::UnsignedNumberExprContext *ctx)
{
    if (DEBUG_1) cout << "=== Pass 1: visitUnsignedNumberExpr: " + ctx->getText() << endl;

    auto value = visit(ctx->number());
    ctx->type = ctx->number()->type;
    return value;
}

antlrcpp::Any Pass1Visitor::visitBoolExpr(ExprParser::BoolExprContext *ctx)
{
    if (DEBUG_1) cout << "=== Pass 1: visitBooleanExpr: " + ctx->getText() << endl;

    auto value = visit(ctx->boolType());
    ctx->type = ctx->boolType()->type;
    return value;
}

antlrcpp::Any Pass1Visitor::visitIntegerConst(ExprParser::IntegerConstContext *ctx)
{
    if (DEBUG_1) cout << "=== Pass 1: visitIntegerConst: " + ctx->getText() << endl;

    ctx->type = Predefined::integer_type;
    return visitChildren(ctx);
}

antlrcpp::Any Pass1Visitor::visitFloatConst(ExprParser::FloatConstContext *ctx)
{
    if (DEBUG_1) cout << "=== Pass 1: visitFloatConst: " + ctx->getText() << endl;

    ctx->type = Predefined::real_type;
    return visitChildren(ctx);
}

antlrcpp::Any Pass1Visitor::visitBoolConst(ExprParser::BoolConstContext *ctx)
{
    if (DEBUG_1) cout << "=== Pass 1: visitBooleanConst: " + ctx->getText() << endl;

    ctx->type = Predefined::boolean_type;
    return visitChildren(ctx);
}

antlrcpp::Any Pass1Visitor::visitParenExpr(ExprParser::ParenExprContext *ctx)
{
    if (DEBUG_1) cout << "=== Pass 1: visitParenExpr: " + ctx->getText() << endl;

    auto value = visitChildren(ctx);
    ctx->type = ctx->expr()->type;
    return value;
}
