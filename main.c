#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

extern FILE *yyin;
extern print_tree() ;

int nb_lines = 1 ;

yyerror(char *s)
{
  printf ("%s at line %d\n",s,nb_lines) ;
}

usage () 
{
  	printf ("usage: while [file]\n") ;
	exit (1) ;
}

check (char *s)
{
   yyin = fopen (s, "r") ;
  if (yyin==NULL) usage() ;
}

main (int argc, char *argv[])
{
  int result ;

  switch (argc)
  {
    case 1 :
    	yyin = stdin ;
    	break ;
    case 2 :
    	check (argv[1]) ;
    	break ;
    default:
  	usage ();
  }

  result = yyparse() ; // call to the parser

  if (result==0){
   	print_tree() ;

	/* insert here the calls to the code generation main function */
  }
} 
