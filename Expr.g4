grammar Expr;

@header {
#include "wci/intermediate/TypeSpec.h"
using namespace wci::intermediate;
}

program : header (stmtList)? block ;
header  : PROGRAM IDENTIFIER ;
block   : BEGIN (functionDefinition | stmtList)* END ;
declaration
	: typeId varId					   # variableDeclaration
	;

varList      : varId ( ',' varId )* ; //for multiple variables in one declaration
// used for declarations
varId locals [ TypeSpec *type = nullptr ] : IDENTIFIER ; //has type and identifier
typeId       : IDENTIFIER ;
functionId   : IDENTIFIER ;
function     : IDENTIFIER ;

functionDefinition  locals [ TypeSpec *type = nullptr ]
					: typeId functionId '(' parameters? ')' 
					   '{'
					   		stmtList?
					   		RETURN (expr)? ';'
					   '}'
				     ;
 
functionCall locals [ TypeSpec *type = nullptr ] : function '(' identifiers? ')' ;

identifiers  : expr  (',' expr)*  ;
parameters   : declaration (',' declaration)* ;


stmt : declareStmt
     | assignmentStmt
     | printStmt
     | ifStmt
     | ifStrStmt
     | forStmt
     |
     ;
     
stmtList       : (stmt ';')+ ; // one or more statements

declareStmt    : typeId varList ; //int a OR int a,b,c
assignmentStmt : variable '=' expr ;
printStmt      : PRINTF '(' formatString printArg* ')' ;
ifStrStmt	   : IF STRING relOp STRING ',' '{' stmtList '}' ; //compare string lengths
ifStmt		   : IF expr ',' '{' stmtList '}' ( ELSE '{' stmtList '}')? ;
forStmt		   : FOR '(' number 'x' ')' '{' stmtList '}' ;

formatString   : STRING ;
printArg       : ',' expr ;

variable : IDENTIFIER ; // used in statements

expr locals [ TypeSpec *type = nullptr ]
	: expr relOp expr	   # relExpr
    | expr mulDivOp expr   # mulDivExpr
    | expr addSubOp expr   # addSubExpr
    | number               # unsignedNumberExpr
    | signedNumber         # signedNumberExpr
    | boolType             # boolExpr
    | variable             # variableExpr
    | '(' expr ')'         # parenExpr
    | functionCall         # funcCallExpr
    ;
    
mulDivOp : MUL_OP | DIV_OP ;
addSubOp : ADD_OP | SUB_OP ;
relOp 	 : EQ_OP | NE_OP | LT_OP | LE_OP | GT_OP | GE_OP ;

signedNumber locals [ TypeSpec *type = nullptr ] 
    : sign number 
    ;
  
sign : ADD_OP | SUB_OP ;

number locals [ TypeSpec *type = nullptr ]
    : INTEGER    # integerConst
    | DECIMAL    # floatConst
    ;
    
boolType  locals [ TypeSpec *type = nullptr ]
	: BOOL		 # boolConst
	;
    
PROGRAM : 'start' ;
BEGIN   : '{.' ;
END     : '.}' ;
PRINTF  : 'print' ;
IF	    : 'if' ;
ELSE    : 'else' ;
FOR		: 'for' ;
RETURN	: 'return' ;

INTEGER    : [0-9]+ ;
DECIMAL    : [0-9]+ '.' [0-9]+ ;
BOOL       : 'true' | 'false' ;

MUL_OP :   '*' ;
DIV_OP :   '/' ;
ADD_OP :   '+' ;
SUB_OP :   '-' ;

EQ_OP  :	'==' | 'is' | 'are' ;
NE_OP  :	'=/=' | 'is not' | 'are not' ;
LT_OP  :	'<' ;
LE_OP  :	'<=' ;
GT_OP  :	'>' ;
GE_OP  :	'>=' ;

NEWLINE : '\r'? '\n' -> skip  ;
WS      : [ \t]+ -> skip ; 

QUOTE  : '"' ;
STRING : QUOTE STRING_CHAR* QUOTE ;

fragment STRING_CHAR : QUOTE QUOTE  // two consecutive quotes
                     | ~('"')      // any non-quote character
                     ;
                     
IDENTIFIER : [a-zA-Z][a-zA-Z0-9]* ;
                     