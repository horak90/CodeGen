/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     While = 258,
     Skip = 259,
     If = 260,
     Then = 261,
     Else = 262,
     Do = 263,
     Od = 264,
     Assign = 265,
     And = 266,
     Declare = 267,
     Begin = 268,
     End = 269,
     Integer = 270,
     Identifier = 271,
     Call = 272,
     Proc = 273,
     Is = 274,
     Var = 275,
     Leq = 276,
     Geq = 277,
     Lexical_error = 278
   };
#endif
/* Tokens.  */
#define While 258
#define Skip 259
#define If 260
#define Then 261
#define Else 262
#define Do 263
#define Od 264
#define Assign 265
#define And 266
#define Declare 267
#define Begin 268
#define End 269
#define Integer 270
#define Identifier 271
#define Call 272
#define Proc 273
#define Is 274
#define Var 275
#define Leq 276
#define Geq 277
#define Lexical_error 278




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 2068 of yacc.c  */
#line 39 "while.y"

  PTR_NODE u_node; 
  char u_char[120];
  int u_int ;



/* Line 2068 of yacc.c  */
#line 104 "y.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


