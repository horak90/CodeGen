%{
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "type.h"

extern int nb_lines ;

extern NODE *mk_node(int type, NODE *n1, NODE *n2);
extern NODE *mk_leaf_int ();
extern NODE *mk_leaf_str();
extern NODE *maj_leaf_str();
%}


%token While
%token Skip
%token If
%token Then
%token Else
%token Do
%token Od
%token Assign
%token And
%token Declare
%token Begin
%token End
%token Integer
%token Identifier
%token Call
%token Proc
%token Is
%token Var
%token Leq
%token Geq
%token Lexical_error


%union{
  PTR_NODE u_node; 
  char u_char[120];
  int u_int ;
} 

%type <u_node> program
%type <u_node> bloc
%type <u_node> declaration_list
%type <u_node> declaration
%type <u_node> instruction_list
%type <u_node> instruction
%type <u_node> formal_param_list
%type <u_node> effective_param_list
%type <u_node> e
%type <u_node> t
%type <u_node> f
%type <u_node> b
%type <u_node> bb

%type <u_int> Integer
%type <u_char> Identifier 

%start program
%%

program : bloc  
    {root=$1;YYACCEPT;}
  ;

bloc : 
	Declare declaration_list  Begin instruction_list End
       {$$= (PTR_NODE) mk_node(BLOC,$2,$4);}
	   ;

declaration_list : 
		declaration
		{$$=$1 ;}
	| 
		declaration_list ';' declaration
		{$$= (PTR_NODE) mk_node(SEMI_COLON,$1,$3);}
	;

declaration : 
		Var Identifier 
		{$$= (PTR_NODE) mk_leaf_str($2);} ;
	| 
		Proc Identifier '(' formal_param_list ')' Is bloc 
		{$$= (PTR_NODE) mk_node(PROC_DECL, mk_leaf_str($2), (PTR_NODE) mk_node(PROC ,$4, $7));}
	;

formal_param_list :  
		Identifier ',' formal_param_list 
		{$$= (PTR_NODE) mk_node(COMMA,mk_leaf_str($1),$3);}
	| 
		Identifier 
		{$$= mk_leaf_str($1);}
	| 
		{$$ = NULL;}
	;

instruction_list : 
		instruction
                {$$=$1 ;}
	| 
		instruction_list ';' instruction
		{$$= (PTR_NODE) mk_node(SEMI_COLON,$1,$3);}
				 
	;

instruction : 
		Identifier Assign e
		{$$= (PTR_NODE) mk_node(ASSIGN,maj_leaf_str($1),$3);}
	| 	
		Skip
		{$$= (PTR_NODE) mk_node(SKIP, NULL,NULL);}
	|
		While b Do instruction Od
		{$$= (PTR_NODE) mk_node(WHILE,$2,$4);}
	| 
		If b Then instruction Else instruction
		{$$= (PTR_NODE) mk_node(IF,$2, (PTR_NODE) mk_node(THENELSE,$4,$6));}
	| 
		Call Identifier '(' effective_param_list ')'
		{$$= (PTR_NODE) mk_node(CALL,mk_leaf_str($2), $4);}
	;

effective_param_list :  
		e ',' effective_param_list 
		{$$= (PTR_NODE) mk_node(COMMA,$1,$3);}
	| 
		e 
		{$$= $1;}
	| 
		{$$ = NULL;}
	;

e : 
		e '+' t
    		{$$= (PTR_NODE) mk_node(PLUS,$1,$3);}
  
	| 
		t
    		{$$=$1;}
  	;

t : 
		t '*' f
    		{$$= (PTR_NODE) mk_node(TIMES,$1,$3);}
  	| 
		f
    		{$$=$1;}
  	;

f : 
		'(' e ')'
    		{$$=$2;}
  	| 
		Identifier
    		{$$= (PTR_NODE) maj_leaf_str($1);}
  	| 
		Integer
    		{$$= (PTR_NODE) mk_leaf_int($1);}
  	;
 
b : 
		b And bb
    		{$$= (PTR_NODE) mk_node(AND,$1,$3);}
  	| 
		bb
  		{$$=$1;}
  	;

bb : 
		e Geq e
  		{$$= (PTR_NODE) mk_node(SUPEQ,$1,$3);}
  	| 
  		e Leq e
  		{$$= (PTR_NODE) mk_node(INFEQ,$1,$3);}
  	| 	
  		e '<' e
  		{$$= (PTR_NODE) mk_node(INF,$1,$3);}
  	| 	
  		e '>' e
  		{$$= (PTR_NODE) mk_node(SUP,$1,$3);}
  	| 
		e '=' e
  		{$$= (PTR_NODE) mk_node(EGAL,$1,$3);}
  
	| 
		'^' bb
  		{$$= (PTR_NODE) mk_node(NOT, $2,NULL);}
  	| 
		'('b')'
  		{$$=$2;}
  	;
%%

#include "lex.yy.c"

