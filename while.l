%{
int comment=0;
extern int nb_lines ;
%}

space [ \t]
bl {space}+
digit [0-9]
letter  [a-zA-Z]
integer  {digit}+
identifier {letter}({letter}|{digit})*

%%

\(\*([^"*"]|"*"[^")"])*\*\) ;
{bl} ;
"\n" {nb_lines++;}

declare {return(Declare);}
while {return(While);}
do {return(Do);}
od {return(Od);}
if {return(If);}
then {return(Then);}
else {return(Else);}
begin {return(Begin);}
end {return(End);}
skip {return(Skip);}
call {return(Call);}
proc {return(Proc);}
var {return(Var);}
is {return(Is);}
{identifier} {strcpy(yylval.u_char,yytext);return (Identifier);}
{integer} {yylval.u_int=atoi(yytext);return(Integer);}

";" {return(';');}
":" {return(':');}
"," {return(',');}
"=" {return('=');}
">" {return('>');}
"<" {return('<');}
"=<" {return(Leq);}
"=>" {return(Geq);}
"^" {return('^');}
"+" {return('+');}
"*" {return('*');}
"(" {return('(');}
")" { return(')');}
":=" { return(Assign);}
"/\\" { return(And);}

. {fprintf(stderr,"lexical error:%s\n",yytext);return(Lexical_error);}

%%
int yywrap()
{
        return(1);
}


