#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "type.h"

NODE *mk_node(type,n1,n2)
int type;
NODE *n1;
NODE *n2;
{
  NODE *n;
  n = (NODE *) calloc (1,sizeof(NODE));
  n->type_node=type;
  n->fg=n1;
  n->fd=n2;
  return(n);
}

NODE *mk_leaf_int(val)
int val;
{
  NODE *n;
  n = (NODE *) calloc (1,sizeof(NODE));
  n->type_node=NUM;
  (n->val_node).u_int=val;
  return(n);
}

NODE *maj_leaf_str(val)
char * val;
{
  NODE *n;
  int i ;
  n = (NODE *) calloc (1,sizeof(NODE));
  n->type_node=IDF;
  strcpy((n->val_node).u_str, val) ;
  return(n);
}

NODE *mk_leaf_str(val)
char * val;
{
  NODE *n;
  int i ;
  n = (NODE *) calloc (1,sizeof(NODE));
  n->type_node=IDF;
  strcpy((n->val_node).u_str, val) ;
  return(n);
}

print_sep()
{
  printf(" ");
}

NODE * Gauche(NODE *noeud)
{
  return(noeud->fg) ;
}

NODE * Droit(NODE *noeud)
{
  return(noeud->fd) ;
}


print_node(n)
NODE *n;
{
  if (n) {
  switch(n->type_node)
  {
  	case SKIP:
	  print_sep();
   	  printf("Skip");
  	  print_sep();
  	  break;
    case COMMA:
	  print_sep();
  	  print_node(n->fg);
	  printf(", ");
	  print_sep();
  	  print_node(n->fd);
  	  break;
    case SEMI_COLON:
	  print_sep();
	  print_sep();
  	  print_node(n->fg);
	  printf(" ;\n");
	  print_sep();
	  print_sep();
  	  print_node(n->fd);
  	  break;
  	  
    case WHILE:
	  printf("\nWhile\n") ;
	  printf("(");
  	  print_node(n->fg);
	  printf(")");
	  printf("\nDo\n") ;
  	  print_node(n->fd);
	  printf("\nOd\n") ;
  	  break;
    case IF:
	  printf("\nIf ") ;
  	  print_node(n->fg);
  	  print_node(n->fd);
  	  break;
    case THENELSE:
	  printf(" Then ") ;
  	  print_node(n->fg);
	  printf(" Else ") ;
  	  print_node(n->fd);
  	  break;
    case BLOC:
  	  print_node(n->fg) ;
	  printf("\nBegin\n") ;
  	  print_node(n->fd) ;
	  printf("\nEnd\n") ;
  	  break;
    case ASSIGN:
  	  print_node(n->fg);
	  printf(":=") ;
  	  print_node(n->fd);
  	  break;
    case PLUS:
  	  print_node(n->fg);
	  print_sep();
	  printf(" + ");
	  print_sep();
  	  print_node(n->fd);
  	  break;
    case AND:
  	  print_node(n->fg);
	  print_sep();
	  printf(" and ");
	  print_sep();
  	  print_node(n->fd);
  	  break;
    case SUP:
  	  print_node(n->fg);
	  print_sep();
	  printf(" > ");
	  print_sep();
  	  print_node(n->fd);
  	  break;
  	case SUPEQ:
  	  print_node(n->fg);
	  print_sep();
	  printf(" => ");
	  print_sep();
  	  print_node(n->fd);
  	  break;
  	 case INF:
  	  print_node(n->fg);
	  print_sep();
	  printf(" < ");
	  print_sep();
  	  print_node(n->fd);
  	  break;
  	case INFEQ:
  	  print_node(n->fg);
	  print_sep();
	  printf(" =< ");
	  print_sep();
  	  print_node(n->fd);
  	  break;
  	
    case EGAL:
  	  print_node(n->fg);
	  print_sep();
	  printf(" = ");
	  print_sep();
  	  print_node(n->fd);
  	  break;
    case NOT:
	  printf("^");
	  printf("(");
  	  print_node(n->fg);
	  printf(")");
	  print_sep();
  	  break;
    case TIMES:
  	  print_node(n->fg);
	  print_sep();
	  printf(" * ");
	  print_sep();
  	  print_node(n->fd);
  	  break;
    case NUM:
  	  printf("%d",(n->val_node).u_int);
  	  break;
    case IDF:
  	  printf("%s", (n->val_node).u_str);
  	  break;
    case CALL:
	  printf("Call ");
  	  print_node(n->fg);
	  printf("(");
	  print_node(n->fd);
	  printf(")");
	  break ;
    case PROC_DECL:
	  printf("\nProc") ;
	  print_sep();
  	  print_node(n->fg);
  	  printf("(");
  	  print_node(n->fd->fg);
  	  printf(")\n");
  	  print_node(n->fd->fd);
	break;
  }
  }

}

void print_tree()
{
  print_node(root);
  printf("\n");
}
