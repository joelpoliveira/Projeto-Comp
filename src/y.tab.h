/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    SEMICOLON = 258,
    COMMA = 259,
    BLANKID = 260,
    LBRACE = 261,
    LPAR = 262,
    LSQ = 263,
    RBRACE = 264,
    RPAR = 265,
    RSQ = 266,
    PACKAGE = 267,
    RETURN = 268,
    ELSE = 269,
    FOR = 270,
    IF = 271,
    VAR = 272,
    PRINT = 273,
    FUNC = 274,
    CMDARGS = 275,
    INT = 276,
    FLOAT32 = 277,
    STRING = 278,
    BOOL = 279,
    UNARY = 280,
    STAR = 281,
    DIV = 282,
    MINUS = 283,
    PLUS = 284,
    EQ = 285,
    GE = 286,
    GT = 287,
    LE = 288,
    LT = 289,
    MOD = 290,
    NE = 291,
    NOT = 292,
    AND = 293,
    OR = 294,
    ID = 295,
    STRLIT = 296,
    INTLIT = 297,
    REALLIT = 298,
    RESERVED = 299,
    PARSEINT = 300,
    ASSIGN = 301
  };
#endif
/* Tokens.  */
#define SEMICOLON 258
#define COMMA 259
#define BLANKID 260
#define LBRACE 261
#define LPAR 262
#define LSQ 263
#define RBRACE 264
#define RPAR 265
#define RSQ 266
#define PACKAGE 267
#define RETURN 268
#define ELSE 269
#define FOR 270
#define IF 271
#define VAR 272
#define PRINT 273
#define FUNC 274
#define CMDARGS 275
#define INT 276
#define FLOAT32 277
#define STRING 278
#define BOOL 279
#define UNARY 280
#define STAR 281
#define DIV 282
#define MINUS 283
#define PLUS 284
#define EQ 285
#define GE 286
#define GT 287
#define LE 288
#define LT 289
#define MOD 290
#define NE 291
#define NOT 292
#define AND 293
#define OR 294
#define ID 295
#define STRLIT 296
#define INTLIT 297
#define REALLIT 298
#define RESERVED 299
#define PARSEINT 300
#define ASSIGN 301

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 39 "gocompiler.y"

    id_token* id;
    parameter_type pt;
    is_program* ip;
    is_declarations_list* idl;
    is_declaration * idec;
    is_var_spec * ivs;    
    is_id_list * iil;
    is_parameter * iparam;
    is_id_type_list * iitl;
    is_func_body * ifb;
    is_vars_and_statements_list * ivsl;
    is_statement * is;
    is_final_statement * ifs;
    is_statements_list * isl;
    is_parse_arguments * ipa;
    is_function_invocation * ifi;
    is_expression_or_list * ieol;
    is_expression_and_list * ieal;
    is_expression_comp_list * iecl;
    is_expression_sum_like_list * iesl;
    is_expression_star_like_list * iestl;
    is_self_expression_list * isel;
    is_final_expression * ife;
    is_func_dec * ifd;
    is_func_inv_expr_list * ifiel;
    next_oper * nop;
    location * loc;

#line 179 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
