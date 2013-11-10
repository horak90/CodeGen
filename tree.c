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

void print_var(var_t var);
void print_vars(block_t block);
void print_block(block_t block);
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

void print_tree() {
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
      // To the left of the block we find the declaration of variables 
      // and procedures
      current_block->state = DECLARING;
      run_node(n->fg, NULL);
      printf("Right Block\n");
      // To the right of the block we find the code to be executed
      current_block->state = RUNNING;
      run_node(n->fd, NULL);
      // Finished executing the block
      current_block->state = NOT_EXECUTING;
      break;
    case ASSIGN:
      // Find the variable to be modified
      name = (n->fg->val_node).u_str;
      pvar = find_var(name);
      if (pvar) {
        // Assign to pvar the value to the right of the equal sign
        pvar->value = (int)run_node(n->fd, NULL);
        print_block(current_block);
      } else {
        printf("Variable %s does not exist\n", name);
        abort();
      }
      break;
    case IDF:
      name = (n->val_node).u_str;
      if ((int)arg == VARIABLE || arg == NULL) {
        pvar = find_var(name);
        if (pvar && current_block->state == RUNNING) {
          result = (void *)pvar->value;
        } else if (pvar && current_block->state == DECLARING) {
          #ifndef DYNAMIC_LINKING_FOR_VARIABLES
          printf("Variable %s already exists in %s\n", name, current_block->id);
          abort();
          #else
          result = pvar;
          #endif
        } else if (!pvar && current_block->state == RUNNING) {
          printf("Variable %s does not exist\n", name);
          abort();
        } else if (!pvar && current_block->state == DECLARING) {
          result = create_var(name);
        }
      } else if ((int)arg == PROCEDURE) {
        pblock = find_block(name);
        if (pblock && current_block->state == RUNNING) {
          result = pblock;
        } else if (pblock && current_block->state == DECLARING) {
          printf(
            "Procedure %s already exists in %s\n", name, current_block->id
          );
          abort();
        } else if (!pblock && current_block->state == RUNNING) {
          printf("Procedure %s does not exist\n", name);
          abort();
        } else if (!pblock && current_block->state == DECLARING) {
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
      // If condition is TRUE then execute the node on the right and then
      // execute again this node
      if ((int)run_node(n->fg, NULL) == TRUE) {
        printf("Right While \n");
        run_node(n->fd, NULL);  
        run_node(n, NULL);
      } else {
        break;
      }
    case IF:      
      printf("Left IF \n");
      // The condition is evaluated and passed as a parameter to
      // THENELSE
      foo = malloc(sizeof(int));
      *((int *)foo) = (int)run_node(n->fg, NULL);
      printf("Right IF \n");
      run_node(n->fd, foo);
      free(foo);
      break;
    case THENELSE:
      // The condition is passed in arg from IF. If TRUE execute the node on
      // the left. If FALSE execute the node on the right
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
      // To the left is the name of the procedure as an IDF
      foo = run_node(n->fg, (void *)PROCEDURE);
      printf("Right PROC_DECL \n");
      // To the right is PROC
      run_node(n->fd, foo);
      break;
    case PROC:
      // Save current_block and change environment to the new one
      foo = current_block;
      current_block = (block_t)arg;

      // Declare internal variables 
      current_block->state = DECLARING;
      printf("Left PROC \n");

      // These are the parameters (arguments)
      if (n->fg != NULL) {
        // If it's only one parameter we create the list 
        // of params manually with that parameter alone
        if (n->fg->type_node == IDF) {
          bar = malloc(sizeof(param_s));
          strcpy(
            ((param_t)bar)->content.id, ((var_t)run_node(n->fg, NULL))->id
          );
          ((param_t)bar)->next = NULL;
        } else if (n->fg->type_node == COMMA) {
          // ... if it's a set of parameters we let the appropiate
          // function create the list.
          bar = run_node(n->fg, NULL);
        }
      }
      
      current_block->params = (param_t)bar;

      // These are the variables declared inside the procedure
      printf("Right PROC \n");
      run_node(n->fd->fg, NULL);
      current_block->state = NOT_EXECUTING;

      #ifndef DYNAMIC_LINKING_FOR_VARIABLES
      // Put a link to the first of the external ones at the end of the list
      // of the internal variables
      bar = current_block->firstVAR;
      if (bar == NULL) {
        current_block->firstVAR = ((block_t)foo)->firstVAR;
      } else {
        while (((var_t)bar)->next != NULL)
          bar = ((var_t)bar)->next;
        ((var_t)bar)->next = ((block_t)foo)->firstVAR;
      }
      #endif 

      // Set back current_block
      current_block->code = n->fd->fd;
      current_block = (block_t)foo;
      break;
    case CALL:
      foo = NULL;
      
      // Find the procedure to be called
      pblock = (block_t)run_node(n->fg, (void *)PROCEDURE);
      if (pblock) {
        if (n->fd != NULL) {
          // If it's only one parameter we create the list 
          // of params manually with that parameter alone
          if (n->fd->type_node != COMMA) {
            foo = malloc(sizeof(param_s));
            ((param_t)foo)->content.value = (int)run_node(n->fd, NULL);
            ((param_t)foo)->next = NULL;
          } else {
            // ... if it's a set of parameters we let the appropiate
            // function create the list.
            foo = run_node(n->fd, NULL);
          }
        }

        call_block(pblock, foo);

        // Erase the list of params
        while (foo != NULL) {
          bar = ((param_t)foo)->next;
          free(foo);
          foo = bar;
        }
      } else {
        printf("Procedure %s does not exist\n", name);
        abort();
      }
      break;
    case COMMA:
      // We create the parameters' list in a top-down approach
      foo = malloc(sizeof(param_s));
      ((param_t)foo)->next = NULL;
 
      if (current_block->state == RUNNING) {
        ((param_t)foo)->content.value = (int)run_node(n->fg, NULL);
      } else if (current_block->state == DECLARING) {
        strcpy(
          ((param_t)foo)->content.id, ((var_t)run_node(n->fg, NULL))->id
        );
      }
        
      // After this piece of code, bar points to the first one param
      // and zoo points to the last one. Foo is put in the last position
      // always. Arg is always the first param.
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
   
      // We figure out what to do with the next param depending on what 
      // is to the right
      if (n->fd == NULL) {
        result = bar;
      } else if (n->fd->type_node != COMMA) {
        ((param_t)foo)->next = malloc(sizeof(param_s));
        ((param_t)foo)->next->next = NULL;

        if (current_block->state == RUNNING) {
          ((param_t)foo)->next->content.value = (int)run_node(n->fd, NULL);
        } else if (current_block->state == DECLARING) {
          strcpy(
            ((param_t)foo)->next->content.id, 
            ((var_t)run_node(n->fd, NULL))->id
          );
        }
        result = bar;
      } else {
        result = run_node(n->fd, bar);
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
  // Save current_block state
  current_block->state = WAITING_CHILD;
  block_t tmp = current_block;
  
  // Initialize arguments if any
  current_block = pblock;
  var_t var;
  param_t p; 

  for (p = current_block->params; 
       p != NULL && params != NULL; 
       p = p->next, params = params->next) {
    var = find_var((p->content).id);
    if (var)
      var->value = (params->content).value;
  }

  // Run block
  current_block->state = RUNNING;
  run_node(pblock->code, NULL);
  current_block->state = NOT_EXECUTING;

  // Restore parent block state
  current_block = tmp;
  current_block->state = RUNNING;
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
    while (current->next != NULL)
      current = current->next;          

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
    if (strcmp(current->id, nvar) == 0) 
      found = current;
      
    current = current->next;
  }
  
  return found;
}

block_t create_block(char *nvar) {
  block_t current = firstBLOCK;
  block_t next = (firstBLOCK != NULL) ? firstBLOCK->next : NULL;
  block_t newBlock;

  // Initialize block
  newBlock = (block_s*)malloc(sizeof(block_s));
  strcpy(newBlock->id, nvar);
  newBlock->next = NULL;
  newBlock->state = NOT_EXECUTING;

  #ifdef DYNAMIC_LINKING_FOR_VARIABLES
  if (firstBLOCK != NULL)
    newBlock->firstVAR = firstBLOCK->firstVAR;
  #endif
  
  // We are the first block so we go to the head of the list
  if (!firstBLOCK) {
    firstBLOCK = newBlock;
  } else {
    // We put the block in the back of the list
    while (current->next != NULL)
      current = current->next;
    current->next = newBlock;
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
