#include <iostream>
#include <string>

#include <unordered_map>
#include <vector>

#include "Pass2Visitor.h"
#include "wci/intermediate/SymTabStack.h"
#include "wci/intermediate/SymTabEntry.h"
#include "wci/intermediate/TypeSpec.h"
#include "wci/intermediate/symtabimpl/Predefined.h"
#include <stdio.h>

using namespace wci;
using namespace wci::intermediate;
using namespace wci::intermediate::symtabimpl;

static unordered_map<string, vector<vector<string>>> function_param_map;
const bool DEBUG_2 = true;
int label_cnt = 0;

extern string pname;
static string func_name = "";

static string EXCEPTION(string message)
{
	cout << "Error: " << message << endl;
	{
		string filename = pname + ".j";
		char file[filename.length() + 1];
		strcpy(file, filename.c_str());

		std::remove(file);
	}
	exit(1);
}

Pass2Visitor::Pass2Visitor()
    : program_name(""), j_file(nullptr)
{
}

Pass2Visitor::~Pass2Visitor() {}

ostream& Pass2Visitor::get_assembly_file() { return j_file; }

antlrcpp::Any Pass2Visitor::visitProgram(ExprParser::ProgramContext *ctx)
{
    auto value = visitChildren(ctx);

    j_file.close();

    if (DEBUG_2) cout << "=== Pass 2: visitProgram: closed " + j_file_name << endl;
    return value;
}

antlrcpp::Any Pass2Visitor::visitHeader(ExprParser::HeaderContext *ctx)
{
    if (DEBUG_2) cout << "=== Pass 2: visitHeader: " + ctx->getText() << endl;

    program_name = ctx->IDENTIFIER()->toString();

    // Create the assembly output file.
    j_file_name = program_name + ".j";
    j_file.open(j_file_name);
    if (j_file.fail())
    {
        cout << "***** Cannot open assembly file." << endl;
        exit(-99);
    }

    if (DEBUG_2) cout << "=== Pass 2: visitHeader: created " + j_file_name << endl;

    // Emit the program header.
    j_file << ".class public " << program_name << endl;
    j_file << ".super java/lang/Object" << endl;

    return visitChildren(ctx);
}

antlrcpp::Any Pass2Visitor::visitFunctionDefinition(ExprParser::FunctionDefinitionContext *ctx)
 {
	if (DEBUG_2) cout << "=== Pass 2: visitFunctionDefinition: " + ctx->getText() << endl;

	j_file << endl;

	//function label
	func_name = ctx->functionId()->getText();
	j_file << "goto " << "End_" << func_name << endl;
	if (DEBUG_2) cout << "=== Pass 2: visitFunctionDefinition: wrote goto" << endl;
	j_file << "L_" << func_name << ": " << endl;
	if (DEBUG_2) cout << "=== Pass 2: visitFunctionDefinition: made Label_" + func_name << endl;
	j_file << "\tastore_1" << endl;

	vector<vector<string>> paras;

	 if(ctx->parameters() != NULL)
	 {
		 for(int i = 0; i < ctx->parameters()->declaration().size(); i++)
		 {
			 string type_name     = ctx->parameters()->declaration(i)->children[0]->getText();
			 string variable_name = ctx->parameters()->declaration(i)->children[1]->getText();

			 paras.push_back({variable_name, type_name});
		 }
	 }

	function_param_map.emplace(ctx->functionId()->IDENTIFIER()->getText(), paras);

	auto value = visitChildren(ctx->stmtList());

	if(ctx->expr() != NULL) {
		visit(ctx->expr());
	}

	j_file << endl;
	j_file << "\tret 1" << endl;
	j_file << "End_" << func_name << ":" << endl << endl;

	func_name = "";
	return value;
 }

 antlrcpp::Any Pass2Visitor::visitFunctionCall(ExprParser::FunctionCallContext *ctx)
 {
	 if (DEBUG_2) cout << "=== Pass 2: visitFunctionCall: " + ctx->getText() << endl;
	 if (DEBUG_2) cout << "=== Pass 2: visitFunctionCall: Identifiers" << endl;
	 if (DEBUG_2) cout << ctx->identifiers()->getText()<< endl << endl;

	 if(ctx->identifiers() != NULL)
	 {
		 if (DEBUG_2) cout << "=== Pass 2: visitFunctionCall: identifiers not null" << endl;
		 int input_count = ctx->identifiers()->expr().size();
		 if (DEBUG_2) cout << "=== Pass 2: visitFunctionCall: input count = " << input_count << endl;

		 if (DEBUG_2) cout << "=== Pass 2: visitFunctionCall: finding " + ctx->function()->IDENTIFIER()->getText() << endl;
		 vector<vector<string>> paras = function_param_map.find(ctx->function()->IDENTIFIER()->getText())->second;
		 if (DEBUG_2) cout << "=== Pass 2: visitFunctionCall: found" + ctx->function()->IDENTIFIER()->getText() << endl;

		 int paras_count = paras.size();

		 int max = (paras_count > input_count) ? input_count: paras_count;
		 for(int i = 0; i < max; i++)
		 {
			 string variable_name = paras[i][0];
			 string type_name     = paras[i][1];
			 visit(ctx->identifiers()->expr(i));

			string type_indicator =
						  (type_name == "int")     ? "I"
						: (type_name == "dec")	   ? "F"
						: (type_name == "bool")    ? "Z"
						//: (type_name == "string")  ? "Ljava/lang/String;"
						:                         EXCEPTION("Invalid Type");

			// Emit a field put instruction.
			j_file << "\tputstatic\t" << program_name
				   << "/" << func_name <<  variable_name
				   << " " << type_indicator << endl;
		 }
	 }

	 j_file << "\tjsr L_" << ctx->function()->IDENTIFIER()->getText() << endl;
	 return nullptr;
 }

antlrcpp::Any Pass2Visitor::visitBlock(ExprParser::BlockContext *ctx)
{
    if (DEBUG_2) cout << "=== Pass 2: visitBlock" << endl;

    // Emit the RunTimer and PascalTextIn fields.
    j_file << endl;
    j_file << ".field private static _runTimer LRunTimer;" << endl;
    j_file << ".field private static _standardIn LPascalTextIn;" << endl;

    // Emit the class constructor.
    j_file << ".method public <init>()V" << endl;
    j_file << endl;
    j_file << "\taload_0" << endl;
    j_file << "\tinvokenonvirtual java/lang/Object/<init>()V" << endl;
    j_file << "\treturn" << endl;
    j_file << endl;
    j_file << ".limit locals 1" << endl;
    j_file << ".limit stack 1" << endl;
    j_file << ".end method" << endl;
    j_file << endl;

    // Emit the main program header and prologue.
    j_file << ".method public static main([Ljava/lang/String;)V" << endl;
    j_file << endl;
    j_file << "\tnew RunTimer" << endl;
    j_file << "\tdup" << endl;
    j_file << "\tinvokenonvirtual RunTimer/<init>()V" << endl;
    j_file << "\tputstatic\t" << program_name
           << "/_runTimer LRunTimer;" << endl;
    j_file << "\tnew PascalTextIn" << endl;
    j_file << "\tdup" << endl;
    j_file << "\tinvokenonvirtual PascalTextIn/<init>()V" << endl;
    j_file << "\tputstatic\t" + program_name
           << "/_standardIn LPascalTextIn;" << endl;

    // Emit code for the main program's block statement list.
    visitChildren(ctx);

    // Emit the main program epilogue.
    j_file << endl;
    j_file << "\tgetstatic     " << program_name
               << "/_runTimer LRunTimer;" << endl;
    j_file << "\tinvokevirtual RunTimer.printElapsedTime()V" << endl;
    j_file << endl;
    j_file << "\treturn" << endl;
    j_file << endl;
    j_file << ".limit locals 16" << endl;
    j_file << ".limit stack 16" << endl;
    j_file << ".end method" << endl;

    return nullptr;
}

antlrcpp::Any Pass2Visitor::visitDeclareStmt(ExprParser::DeclareStmtContext *ctx)
{
    if (DEBUG_2) cout << "=== Pass 2: visitDeclareStmt: " + ctx->getText() << endl;

    string variable_list = ctx->varList()->getText();

    vector<string> var_array;
	stringstream var_stream(variable_list);

	while(var_stream.good())
	{
		string substr;
		getline(var_stream, substr, ',');
		var_array.push_back(substr);
	}

	//TypeID
	TypeSpec *type;

	string type_name = ctx->typeId()->getText();

	type = (type_name == "int")     ? Predefined::integer_type
		 : (type_name == "dec")     ? Predefined::real_type
		 : (type_name == "bool")    ? Predefined::boolean_type
		 :                            nullptr;

    // Emit a field declaration.
    string type_indicator = (type == Predefined::integer_type) ? "I"
						  : (type == Predefined::real_type)    ? "F"
						  : (type == Predefined::boolean_type) ? "Z"
						  :                                      "?";

    for(int i = 0; i < var_array.size(); i++)
    {
    	j_file << ".field private static "
    		   << var_array.at(i) << " " << type_indicator << endl;
    }

    return visitChildren(ctx);
}

antlrcpp::Any Pass2Visitor::visitIfStmt(ExprParser::IfStmtContext *ctx)
{
    if (DEBUG_2) cout << "=== Pass 2: visitIfStmt" << endl;

    bool has_else		= ctx->getText().find("else");
    int  current_label	= label_cnt;
    int  stmt_size		= ctx->stmtList().size();
    int  next_label		= current_label + 1;
    int  final_label	= current_label + 2;

    //evaluate relExpr, if true, branches to current label
    visit(ctx->expr());
    //if false, go to else label
    j_file << "\tgoto Label_" << next_label << endl << endl;
    j_file << "Label_" << current_label << ":" << endl;			//true label
    visitChildren(ctx->stmtList(0));							//visit first stmtList
    j_file << "\tgoto Label_" << final_label << endl << endl;
    j_file << "Label_" << next_label << ":" << endl;			//false, else label
    if(has_else) {
    	visitChildren(ctx->stmtList(stmt_size - 1));			//visit else stmtList
    }
    j_file << "Label_" << final_label << ":" << endl;
    label_cnt++;
    label_cnt++;

    return nullptr;
}

antlrcpp::Any Pass2Visitor::visitIfStrStmt(ExprParser::IfStrStmtContext *ctx)
{
    if (DEBUG_2) cout << "=== Pass 2: visitIfStrStmt" << endl;

    int str1_size		= ctx->STRING(0)->getText().size();
    int str2_size		= ctx->STRING(1)->getText().size();
    string op 			= ctx->relOp()->getText();
    string opcode;

    //load string lengths
    j_file << "\tldc\t" << str1_size << endl;
    j_file << "\tldc\t" << str2_size << endl;

    if (op == "==" || op ==  "is" || op == "are") {					//EQ_OP
		opcode = "if_icmpeq";
	}

	else if ( op == "=/=" || op == "is not" || op == "are not" ) {	//NE_OP
		opcode = "if_icmpne";
	}

	else {
		opcode = "????";
		if (DEBUG_2) cout << "=== Pass 2: visitIfStrStmt: REL_OP is invalid" << endl;
	}

    j_file << "\t" << opcode << " Label_" << label_cnt << endl;

    int true_label	= label_cnt;
    int false_label	= true_label + 1;

    j_file << "\tgoto Label_" << false_label << endl << endl;
    j_file << "Label_" << true_label << ":" << endl;			//true label
    visitChildren(ctx->stmtList());
    j_file << "Label_" << false_label << ":" << endl;			//false label
    label_cnt++;
    label_cnt++;

    return nullptr;
}
antlrcpp::Any Pass2Visitor::visitForStmt(ExprParser::ForStmtContext *ctx)
{
	if (DEBUG_2) cout << "=== Pass 2: visitForStmt" << endl;

	int start_label = label_cnt;
	int end_label = start_label + 1;

	TypeSpec *type = ctx->number()->type;
	bool integer_mode = (type == Predefined::integer_type);
	if(integer_mode) {
		//set up
		j_file << "\t.var 0 is i I from Label_" << start_label << " to Label_" << end_label << endl;
		j_file << "\tbipush\t" << ctx->number()->getText() << endl;
		j_file << "\tistore_0" << endl;

		//for loop label
		j_file << "Label_" << start_label << ":" << endl;

		//boolean code : if i <= 0 , go to end label
		j_file << "\tiload_0" << endl;
		j_file << "\tbipush 0" << endl;
		j_file << "\tif_icmple Label_" << end_label << endl;

		//statement code
		visitChildren(ctx->stmtList());
		j_file << "\tiinc 0 -1" << endl;
		j_file << "\tgoto Label_" << start_label << endl << endl;

		//end for loop label
		j_file << "Label_" << end_label << ":" << endl;

		label_cnt++;
		label_cnt++;
	}

	return nullptr;
}
antlrcpp::Any Pass2Visitor::visitStmt(ExprParser::StmtContext *ctx)
{
    if (DEBUG_2) cout << "=== Pass 2: visitStmt" << endl;

    j_file << endl << "; " + ctx->getText() << endl << endl;

    return visitChildren(ctx);
}

antlrcpp::Any Pass2Visitor::visitAssignmentStmt(ExprParser::AssignmentStmtContext *ctx)
{
    if (DEBUG_2) cout << "=== Pass 2: visitAssignmentStmt" << endl;

    auto value = visit(ctx->expr());

    string type_indicator =
                  (ctx->expr()->type == Predefined::integer_type) ? "I"
                : (ctx->expr()->type == Predefined::real_type)    ? "F"
                : (ctx->expr()->type == Predefined::boolean_type) ? "Z"
                :                                                   "?";

    // Emit a field put instruction.
    j_file << "\tputstatic\t" << program_name
           << "/" << ctx->variable()->IDENTIFIER()->toString()
           << " " << type_indicator << endl;

    return value;
}

antlrcpp::Any Pass2Visitor::visitPrintStmt(ExprParser::PrintStmtContext *ctx)
{
    // Get the format string without the surrounding the single quotes.
    string str = ctx->formatString()->getText();
    string format_string = str.substr(1, str.length() - 2);

    // Emit code to push the java.lang.System.out object.
    j_file << "\tgetstatic\tjava/lang/System/out Ljava/io/PrintStream;" << endl;

    // Emit code to push the format string.
    j_file << "\tldc\t\"" << format_string << "\"" << endl;

    // Array size is the number of expressions to evaluate and print.
    int array_size = ctx->printArg().size();

    // Emit code to create the array of the correct size.
    j_file << "\tldc\t" << array_size << endl;
    j_file << "\tanewarray\tjava/lang/Object" << endl;

    // Loop to generate code for each expression.
    for (int i = 0; i < array_size; i++)
    {
        j_file << "\tdup" << endl;         // duplicate the array address
        j_file << "\tldc\t" << i << endl;  // array element index

        // Emit code to evaluate the expression.
        visit(ctx->printArg(i)->expr());
        TypeSpec *type = ctx->printArg(i)->expr()->type;

        // Emit code to convert a scalar integer or float value
        // to an Integer or Float object, respectively.
        if (type == Predefined::integer_type)
        {
            j_file << "\tinvokestatic\tjava/lang/Integer.valueOf(I)"
                   << "Ljava/lang/Integer;"
                   << endl;
        }
        else if (type == Predefined::real_type)
        {
            j_file << "\tinvokestatic\tjava/lang/Float.valueOf(F)"
                   << "Ljava/lang/Float;"
                   << endl;
        }
        else
        {
        	j_file << "\tinvokestatic\tjava/lang/Boolean.valueOf(Z)"
				   << "Ljava/lang/Boolean;"
				   << endl;
        }

        j_file << "\taastore" << endl;  // store the value into the array
    }

    // Emit code to call java.io.PrintStream.printf.
    j_file << "\tinvokevirtual java/io/PrintStream.printf"
           << "(Ljava/lang/String;[Ljava/lang/Object;)"
           << "Ljava/io/PrintStream;"
           << endl;
    j_file << "\tpop" << endl;

    return nullptr;
}

antlrcpp::Any Pass2Visitor::visitAddSubExpr(ExprParser::AddSubExprContext *ctx)
{
    if (DEBUG_2) cout << "=== Pass 2: visitAddSubExpr" << endl;

    auto value = visitChildren(ctx);

    TypeSpec *type1 = ctx->expr(0)->type;
    TypeSpec *type2 = ctx->expr(1)->type;

    bool integer_mode =    (type1 == Predefined::integer_type)
                        && (type2 == Predefined::integer_type);
    bool real_mode    =    (type1 == Predefined::real_type)
                        && (type2 == Predefined::real_type);

    string op = ctx->addSubOp()->getText();
    string opcode;

    if (op == "+")
    {
        opcode = integer_mode ? "iadd"
               : real_mode    ? "fadd"
               :                "????";
    }
    else
    {
        opcode = integer_mode ? "isub"
               : real_mode    ? "fsub"
               :                "????";
    }

    // Emit an add or subtract instruction.
    j_file << "\t" << opcode << endl;

    return value;
}

antlrcpp::Any Pass2Visitor::visitMulDivExpr(ExprParser::MulDivExprContext *ctx)
{
    if (DEBUG_2) cout << "=== Pass 2: visitMulDivExpr" << endl;

    auto value = visitChildren(ctx);

    TypeSpec *type1 = ctx->expr(0)->type;
    TypeSpec *type2 = ctx->expr(1)->type;

    bool integer_mode =    (type1 == Predefined::integer_type)
                        && (type2 == Predefined::integer_type);
    bool real_mode    =    (type1 == Predefined::real_type)
                        && (type2 == Predefined::real_type);

    string op = ctx->mulDivOp()->getText();
    string opcode;

    if (op == "*")
    {
        opcode = integer_mode ? "imul"
               : real_mode    ? "fmul"
               :                "????";
    }
    else
    {
        opcode = integer_mode ? "idpv"
               : real_mode    ? "fdiv"
               :                "????";
    }

    // Emit a multiply or divide instruction.
    j_file << "\t" << opcode << endl;

    return value;
}

antlrcpp::Any Pass2Visitor::visitRelExpr(ExprParser::RelExprContext *ctx)
{
    if (DEBUG_2) cout << "=== Pass 2: visitRelExpr" << endl;

    auto value = visitChildren(ctx);

    TypeSpec *type1 = ctx->expr(0)->type;
    TypeSpec *type2 = ctx->expr(1)->type;

    bool integer_mode =    (type1 == Predefined::integer_type)
                        && (type2 == Predefined::integer_type);
    bool boolean_mode    = (type1 == Predefined::boolean_type)
                        && (type2 == Predefined::boolean_type);

    if (integer_mode) cout << "=== Pass 2: visitRelExpr: INTEGER MODE" << endl << endl;
    else if (boolean_mode) cout << "=== Pass 2: visitRelExpr: BOOLEAN MODE" << endl << endl;
    else cout << "=== Pass 2: visitRelExpr: NO MODE" << endl << endl;

    string op = ctx->relOp()->getText();
    string opcode;

    if (op == "==" || op ==  "is" || op == "are") {					//EQ_OP
    	opcode = integer_mode ? "if_icmpeq"
			   : boolean_mode ? "if_icmpeq"
			   : 				"????" ;
    	opcode = "if_icmpeq";
    }

    else if ( op == "=/=" || op == "is not" || op == "are not" ) {	//NE_OP
    	opcode = integer_mode ? "if_icmpne"
			   : boolean_mode ? "if_icmpne"
			   : 				"????" ;
    }

    else if ( op == "<" ) {						//LT_OP
		opcode = integer_mode ? "if_icmplt"
			   : boolean_mode ? "if_icmplt"
			   : 				"????" ;
    }

	else if ( op == "<=" ) {					//LE_OP
		opcode = integer_mode ? "if_icmple"
			   : boolean_mode ? "if_icmple"
			   : 				"????" ;
	}

	else if ( op == ">" ) {						//GT_OP
		opcode = integer_mode ? "if_icmpgt"
			   : boolean_mode ? "if_icmpgt"
			   : 				"????" ;
	}

	else if ( op == ">=" ) {					//GE_OP
		opcode = integer_mode ? "if_icmpge"
			   : boolean_mode ? "if_icmpge"
			   : 				"????" ;
	}

	else {
		opcode = "????";
		if (DEBUG_2) cout << "=== Pass 2: visitRelExpr: REL_OP is invalid" << endl;
	}

    // Emit a branch instruction to true condition.
    j_file << "\t" << opcode << " Label_" << label_cnt++ << endl;

    return value;
}


antlrcpp::Any Pass2Visitor::visitVariableExpr(ExprParser::VariableExprContext *ctx)
{
    if (DEBUG_2) cout << "=== Pass 2: visitVariableExpr" << endl;

    string variable_name = ctx->variable()->IDENTIFIER()->toString();
    TypeSpec *type = ctx->type;

    string type_indicator = (type == Predefined::integer_type) ? "I"
                          : (type == Predefined::real_type)    ? "F"
                          : (type == Predefined::boolean_type) ? "Z"
                          :                                      "?";

    // Emit a field get instruction.
    j_file << "\tgetstatic\t" << program_name
           << "/" << variable_name << " " << type_indicator << endl;

    return visitChildren(ctx);
}

//antlrcpp::Any visitFuncCallExpr(ExprParser::FuncCallExprContext *ctx)
//{
//	if (DEBUG_2) cout << "=== Pass 2: visitFuncCallExpr" << endl;
//
//	ctx->type = ctx->functionCall()->type;
//	auto value = ctx->type;
//
//	return value;
//}

antlrcpp::Any Pass2Visitor::visitSignedNumber(ExprParser::SignedNumberContext *ctx)
{
    if (DEBUG_2) cout << "=== Pass 2: visitSignedNumber" << endl;

    auto value = visitChildren(ctx);
    TypeSpec *type = ctx->number()->type;

    if (ctx->sign()->children[0] == ctx->sign()->SUB_OP())
    {
        string opcode = (type == Predefined::integer_type) ? "ineg"
                      : (type == Predefined::real_type)    ? "fneg"
                      :                                      "?neg";

        // Emit a negate instruction.
        j_file << "\t" << opcode << endl;
    }

    return value;
}

antlrcpp::Any Pass2Visitor::visitIntegerConst(ExprParser::IntegerConstContext *ctx)
{
    if (DEBUG_2) cout << "=== Pass 2: visitIntegerConst" << endl;

    // Emit a load constant instruction.
    j_file << "\tldc\t" << ctx->getText() << endl;

    return visitChildren(ctx);
}

antlrcpp::Any Pass2Visitor::visitFloatConst(ExprParser::FloatConstContext *ctx)
{
    if (DEBUG_2) cout << "=== Pass 2: visitFloatConst" << endl;

    // Emit a load constant instruction.
    j_file << "\tldc\t" << ctx->getText() << endl;

    return visitChildren(ctx);
}

antlrcpp::Any Pass2Visitor::visitBoolConst(ExprParser::BoolConstContext *ctx)
{
    if (DEBUG_2) cout << "=== Pass 2: visitBooleanConst" << endl;

    // Emit a load constant instruction.
    j_file << "\tldc\t\"" << ctx->getText() << "\"" << endl;
    j_file << "\tinvokestatic\tjava/lang/Boolean.parseBoolean(Ljava/lang/String;)Z" << endl;

    return visitChildren(ctx);
}
