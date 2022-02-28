#include <iostream>
#include <fstream>
#include <string.h>


#include "antlr4-runtime.h"
#include "ExprLexer.h"
#include "ExprParser.h"
#include "Pass1Visitor.h"
#include "Pass2Visitor.h"

using namespace std;
using namespace antlrcpp;
using namespace antlr4;

string pname;

int main(int argc, const char *args[])
{
    ifstream ins;
    pname = args[1];
    pname = pname.substr(0, pname.find("."));
    ins.open(args[1]);

    ANTLRInputStream input(ins);
    ExprLexer lexer(&input);
    CommonTokenStream tokens(&lexer);

    ExprParser parser(&tokens);
    tree::ParseTree *tree = parser.program();

    Pass1Visitor *pass1 = new Pass1Visitor();
    pass1->visit(tree);

    Pass2Visitor *pass2 = new Pass2Visitor();
    pass2->visit(tree);

    return 0;
}
