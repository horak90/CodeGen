#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "type.h"


var_t firstVAR;
var_t find_var(char *nvar);
var_t create_var(char *nvar);
void print_vars(void);

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
      print_node(n->fg);
      printf("\nBegin\n") ;
      print_node(n->fd);
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

void run_tree() {
  run_node(root);
  printf("\n");
}

int run_node(NODE *n) {
  int result = 0;
  char *nvar = NULL;
  var_t pvar;

  if (!n)
    return -1;

  switch (n->type_node) {
    case SKIP:
      break;
    case BLOC:
      run_node(n->fg);
      print_vars();
      run_node(n->fd);
      print_vars();
      break;
    case ASSIGN:
      nvar = (n->fg->val_node).u_str;
      var_t pvar = find_var(nvar);
      if (pvar) {
        pvar->value = run_node(n->fd);
      } else {
        printf("Variable %s does not exist", nvar);
        abort();
      }
      break;
    case IDF:
      nvar = (n->val_node).u_str;
      pvar = find_var(nvar);
      if (pvar)
        result = pvar->value;
      else
        result = (create_var(nvar))->value;
      break;
    case PLUS:
      result = run_node(n->fg) + run_node(n->fd);
      break;
    case TIMES:
      result = run_node(n->fg) * run_node(n->fd);
      break;
    case NUM:
      result = (n->val_node).u_int;
      break;
    case SEMI_COLON:
      break;
    case NON_DEF:
      break;
    case VAR:
      break;
    case AND:
      result = ((run_node(n->fg) == 1) && (run_node(n->fd) == 1)) ? 1 : 0;
      break;
    case WHILE:
      break;
    case IF:
      break;
    case THENELSE:
      break;
    case EGAL:
      result = (run_node(n->fg) == run_node(n->fd)) ? 1 : 0;
      break;
    case NOT:
      result = (run_node(n->fg) == 0) ? 1 : 0;
      break;
    case SUP:
      result = (run_node(n->fg) > run_node(n->fd)) ? 1 : 0;
      break;
    case PROC_DECL:
      break;
    case PROC:
      break;
    case CALL:
      break;
    case COMMA:
      break;
    case INF:
      result = (run_node(n->fg) < run_node(n->fd)) ? 1 : 0;
      break;
    case INFEQ:
      result = (run_node(n->fg) <= run_node(n->fd)) ? 1 : 0;
      break;
    case SUPEQ:
      result = (run_node(n->fg) >= run_node(n->fd)) ? 1 : 0;
      break;
    default:
      break;
  }
  
  return result;
}

void print_vars(void) {
	var_t current = firstVAR;
  while (current != NULL) {
    printf("var %s = %d\n", current->id, current->value);
    current = current->next;
  }
}

var_t find_var(char *nvar)
{
	var_t current = firstVAR;
	var_t found = NULL;

	if(!firstVAR)
	{
		return NULL;
	}else if(firstVAR->next == NULL)
	{
		if(strcmp(firstVAR->id, nvar) == 0)
		{
			return firstVAR;
		}else
		{
			return NULL;
		}
	}else
	{
		while(current->next != NULL)
		{
			if(strcmp(current->id, nvar) == 0)
			{
				return current;

			}else
			{
				current = current->next;
			}

			
		}
	}
}
	
var_t create_var(char *nvar)
{	
	var_t current = firstVAR;
  var_t next = (firstVAR != NULL) ? firstVAR->next : NULL;
	var_t newVariable;

	newVariable = (var_s*)malloc(sizeof(var_s));

	if(!firstVAR)
	{
	  strcpy(newVariable->id, nvar);
		newVariable->next = NULL;
		newVariable->value = 0;
		firstVAR = newVariable;
	}else
	{
		do
		{
			if(current->next == NULL)
			{
				strcpy(newVariable->id, nvar);
				newVariable->next = NULL;
				newVariable->value = 0;
				current->next = newVariable;
			}else
			{
				current = current->next;
			}
		}
		while(current->next != NULL);
	}
	return newVariable;
}

/*
void print_tree_formatted(int depth) {
    int i;
    for (i = 1; i <= depth; i++) {
        printf("Level " + (i-1) + ": ");
        char *levelNodes = printLevel(root, i);
        printf(levelNodes + "\n");
    }
}

char *printLevel(NODE *t, int level) {
    if (t == NULL) {
        return "";

    if (level == 1) {
        return t.element + " ";
    } else if (level > 1) {
        char *leftStr = printLevel(t.left, level - 1);
        char *rightStr = printLevel(t.right, level - 1);
        return leftStr + rightStr;
    }
    else // you need this to get it to compile
        return "";
}*/
