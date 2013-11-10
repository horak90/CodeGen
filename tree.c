#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "type.h"

block_t firstBLOCK;
block_t current_block;

void *run_node(NODE *n, void *arg);
void call_block(block_t pblock, param_t params);
block_t create_block(char *name);
block_t find_block(char *name);
var_t find_var(char *nvar);
var_t create_var(char *nvar);

void print_vars(block_t block);
void print_blocks(void);
char *types[] = 
{
  "PLUS", "", "TIMES", "", "IDF", "NUM", "SEMI_COLON", "BLOC", "VAR",
  "ASSIGN", "AND", "WHILE", "IF", "THENELSE", "EGAL", "NOT", "SUP", 
  "PROC_DECL", "PROC", "CALL", "COMMA", "SKIP", "INF", "INFEQ", "SUPEQ"
};

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


//Go to the right sub tree
NODE * Gauche(NODE *noeud)
{
  return(noeud->fg) ;
}


//Go to the left sub tree
NODE * Droit(NODE *noeud)
{
  return(noeud->fd) ;
}


print_node(n)
NODE *n;
{
  if (n) {
  //printf("Type of the node : %s\n", types[n->type_node]);
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
  firstBLOCK = current_block = create_block("main");
  run_node(root, NULL);
  printf("\n");
}

void *run_node(NODE *n, void *arg) {
  void *result = NULL;
  char *name = NULL;
  void *foo, *bar, *zoo;
  int i;
  var_t pvar;
  block_t pblock;

  if (!n)
    return (void *)-1;

  printf("Type of the node : %s\n", types[n->type_node]);
  switch (n->type_node) {
    case SKIP:
      break;
    case BLOC:
      printf("Left Block\n");
      current_block->state = DECLARE;
      run_node(n->fg, NULL);
      //print_vars(current_block);
      print_blocks();
      printf("Right Block\n");
      current_block->state = CODE;
      run_node(n->fd, NULL);
      print_vars(current_block);
      break;
    case ASSIGN:
      name = (n->fg->val_node).u_str;
      pvar = find_var(name);
      if (pvar) {
        pvar->value = (int)run_node(n->fd, NULL);
      } else {
        printf("Variable %s does not exist\n", name);
        abort();
      }
      break;
    case IDF:
      name = (n->val_node).u_str;
      if ((int)arg == VARIABLE || arg == NULL) {
        pvar = find_var(name);
        if (pvar && current_block->state == CODE) {
          result = (void *)pvar->value;
        } else if (pvar && current_block->state == DECLARE) {
          printf("Variable %s already exists in %s\n", name, current_block->id);
          abort();
        } else if (!pvar && current_block->state == CODE) {
          result = NULL;
        } else if (!pvar && current_block->state == DECLARE) {
          result = create_var(name);
        }
      } else if ((int)arg == PROCEDURE) {
        pblock = find_block(name);
        if (pblock && current_block->state == CODE) {
          result = pblock;
        } else if (pblock && current_block->state == DECLARE) {
          printf("Procedure %s already exists in %s\n", name, current_block->id);
          abort();
        } else if (!pblock && current_block->state == CODE) {
          result = NULL;
        } else if (!pblock && current_block->state == DECLARE) {
          result = create_block(name);
        }
      }
      break;
    case PLUS:
      result = (void *)((int)run_node(n->fg, NULL) + (int)run_node(n->fd, NULL));
      break;
    case TIMES:
      result = (void *)((int)run_node(n->fg, NULL) * (int)run_node(n->fd, NULL));
      break;
    case NUM:
      result = (void *)(n->val_node).u_int;
      break;
    case SEMI_COLON:
      printf("Left Semi colon\n");
      run_node(n->fg, NULL);
      print_vars(current_block);
      printf("Right Semi colon\n");
      run_node(n->fd, NULL);
      break;
    case NON_DEF:
      break;
    case VAR:
      break;
    case AND:
      result = (void *)(((int)(run_node(n->fg, NULL)) == TRUE && (int)(run_node(n->fd, NULL)) == TRUE) ? TRUE : FALSE);
      break;
    case WHILE:
      printf("Left While \n"); 
      foo = malloc(sizeof(int));     
      *((int *)foo) = (int)run_node(n->fg, NULL);
      print_vars(current_block);
      printf("Right While \n");

      if (*((int *)foo) == TRUE) {
        run_node(n->fd, NULL);  
        run_node(n, NULL);
      } else {
        break;
      }
    case IF:      
      printf("Left IF \n");
      foo = malloc(sizeof(int));
      *((int *)foo) = (int)run_node(n->fg, NULL);
      printf("Right IF \n");
      run_node(n->fd, foo);
      free(foo);
      break;
    case THENELSE:
      printf("BOOL %d\n", *((int *)arg));
      if (*((int *)arg) == TRUE) {
        printf("Left THENELSE \n");
        run_node(n->fg, NULL);
      } else {
        printf("Right THENELSE \n");
        run_node(n->fd, NULL);
      }      
      break;
    case EGAL:
      result = (void *)(((int)run_node(n->fg, NULL) == (int)run_node(n->fd, NULL)) ? TRUE : FALSE);
      break;
    case NOT:
      result = (void *)(((int)run_node(n->fg, NULL) == FALSE) ? TRUE : FALSE);
      break;
    case SUP:
      result = (void *)(((int)run_node(n->fg, NULL) > (int)run_node(n->fd, NULL)) ? TRUE : FALSE);
      break;
    case PROC_DECL:
      printf("Left PROC_DECL \n");      
      foo = run_node(n->fg, (void *)PROCEDURE);
      printf("Right PROC_DECL \n");
      run_node(n->fd, foo);
      break;
    case PROC:
      foo = (void *)current_block;
      current_block = (block_t)arg;
      current_block->state = DECLARE;
      printf("Left PROC \n");
      run_node(n->fg, NULL);
      printf("Right PROC \n");
      run_node(n->fd->fg, NULL);
      current_block->code = n->fd->fd;
      current_block = (block_t)foo;
      break;
    case CALL:
      pblock = (block_t)run_node(n->fg, (void *)PROCEDURE);
      if (pblock) {
        if (n->fd->type_node != COMMA) {
          foo = malloc(sizeof(param_s));
          ((param_t)foo)->value = (int)run_node(n->fd, NULL);
          ((param_t)foo)->next = NULL;
        } else {
          foo = run_node(n->fd, NULL);
        }
        call_block(pblock, foo);

        // This should be a free for all of the elements in the list!!
        free(foo);
      } else {
        printf("Procedure %s does not exist\n", name);
        abort();
      }
      break;
    case COMMA:
      if (current_block->state == DECLARE) {
        run_node(n->fg, NULL);
        run_node(n->fd, NULL);
      } else if (current_block->state == CODE) {
        foo = malloc(sizeof(param_s));
        ((param_t)foo)->value = (int)run_node(n->fg, NULL);
        ((param_t)foo)->next = NULL;
        
        // bar points to the first one
        // zoo points to the last one
        if (arg == NULL) {
          bar = foo;
          zoo = foo;
        } else {
          bar = arg;
          zoo = arg;
          while (((param_t)zoo)->next != NULL)
            zoo = ((param_t)zoo)->next; 
          ((param_t)zoo)->next = foo;
        }
   
        if (n->fd == NULL) {
          result = bar;
        } else if (n->fd->type_node != COMMA) {
          ((param_t)foo)->next = malloc(sizeof(param_s));
          ((param_t)foo)->next->value = (int)run_node(n->fd, NULL);
          ((param_t)foo)->next->next = NULL;
          result = bar;
        } else {
          result = run_node(n->fd, bar);
        }
      }
      break;
    case INF:
      result = (void *)(((int)run_node(n->fg, NULL) < (int)run_node(n->fd, NULL)) ? TRUE : FALSE);
      break;
    case INFEQ:
      result = (void *)(((int)run_node(n->fg, NULL) <= (int)run_node(n->fd, NULL)) ? TRUE : FALSE);
      break;
    case SUPEQ:
      result = (void *)(((int)run_node(n->fg, NULL) >= (int)run_node(n->fd, NULL)) ? TRUE : FALSE);
      break;
    default:
      break;
  }
  
  return result;
}

void print_var(var_t var) {
  printf("var %s = %d\n", var->id, var->value);
}

void print_vars(block_t block) {
  var_t current = block->firstVAR;
  while (current != NULL) {
    print_var(current);
    current = current->next;
  }
}

void print_block(block_t block) {
  printf("Block %s:\n", block->id);
  print_vars(block);
}

void print_blocks(void) {
  block_t current = firstBLOCK;
  while (current != NULL) {
    print_block(current);
    current = current->next;
  }
}

var_t find_var(char *nvar) {
  var_t current = current_block->firstVAR;
  var_t found = NULL;
  
  while (current != NULL) {
    if (strcmp(current->id, nvar) == 0) {
      found = current;
    }
      
    current = current->next;
  }

  return found;
}
 
void call_block(block_t pblock, param_t params) {
  block_t tmp = current_block;
  current_block = pblock;
  var_t var = current_block->firstVAR;
  while (var != NULL && params != NULL) {
    var->value = params->value;
    var = var->next;
    params = params->next;
  }

  run_node(pblock->code, NULL);
  current_block = tmp;
}

var_t create_var(char *nvar) { 
  var_t current = current_block->firstVAR;
  var_t next = (current != NULL) ? current->next : NULL;
  var_t newVariable;

  newVariable = (var_s*)malloc(sizeof(var_s));

  if (!current_block->firstVAR) {
    strcpy(newVariable->id, nvar);
    newVariable->next = NULL;
    newVariable->value = 0;
    current_block->firstVAR = newVariable;
  } else {  
    while (current->next != NULL) {
      current = current->next;          
    }

    if (current->next == NULL) {
      strcpy(newVariable->id, nvar);
      newVariable->next = NULL;
      newVariable->value = 0;
      current->next = newVariable;
    }     
  }
  return newVariable;
}


block_t find_block(char *nvar) {
  block_t current = firstBLOCK;
  block_t found = NULL;
  
  while (current != NULL) {
    if (strcmp(current->id, nvar) == 0) {
      found = current;
    }
      
    current = current->next;
  }
  
  return found;
}

block_t create_block(char *nvar)
{
  block_t current = firstBLOCK;
  block_t next = (firstBLOCK != NULL) ? firstBLOCK->next : NULL;
  block_t newBlock;

  newBlock = (block_s*)malloc(sizeof(block_s));

  if(!firstBLOCK)
  {
    strcpy(newBlock->id, nvar);
    newBlock->next = NULL;
    firstBLOCK = newBlock;
  }else
  {
    while(current->next != NULL)
    {
        current = current->next;          
    }

    if(current->next == NULL)
    {
      strcpy(newBlock->id, nvar);
      newBlock->next = NULL;
      current->next = newBlock;
    }
  }
  return newBlock;
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
