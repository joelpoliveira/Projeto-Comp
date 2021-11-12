%{ 
    // Tomás Mendes - 2019232272
    // Joel Oliveira - 2019227468

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "functions.h"
    #include "y.tab.h"

    int yylex(void);
    void yyerror (char *s);

    extern int flag_1, flag_2;
    //int yydebug = 1;

    //is_program* myprogram;
    //error_node* error_list;

%}

%union{
    char *id;
    // is_program* ip;
    // is_vardec_list* ivl;
    // is_vardec* iv;
    // is_statement_list* isl;
    // is_statement* is;

    // is_function* if;
    // is_function_list* ifl;
}

%token SEMICOLON COMMA BLANKID ASSIGN STAR DIV MINUS PLUS EQ GE GT LBRACE   //linhas 28-39
%token LE LPAR LSQ LT MOD NE NOT AND OR RBRACE RPAR RSQ PACKAGE RETURN ELSE //linhas 40-54
%token FOR IF VAR INT FLOAT32 BOOL STRING PRINT PARSEINT FUNC CMDARGS       //linhas 55-65
%token UNARY

//%token<string> RESERVED INTLIT REALLIT ID STRLIT
%token<id>ID RESERVED INTLIT REALLIT STRLIT
// %type<ip>program
// %type<ivl>vardeclist
// %type<iv>vardec
// %type<isl>statementlist
// %type<is>statement

// %type<if>functions
// %type<ifl>func_dec

//%type<>

%left  COMMA
%right ASSIGN //'+=' '-='
%left  OR
%left  AND
%left  EQ NE
%left  PLUS MINUS
%left  STAR DIV
%nonassoc UNARY
%left  LPAR RSQ

%%

program: PACKAGE ID SEMICOLON declarations {;}
        ;

declarations: /*EMPTY*/                        {;}  
            |  declarations var_dec SEMICOLON  {;}
            |  declarations func_dec SEMICOLON {;}
            ;

var_dec:    VAR var_spec                         {;}
        |   VAR LPAR var_spec SEMICOLON RPAR     {;}
        ;

var_spec: ID comma_id_rec type   {;}
        ;
comma_id_rec:   /*EMPTY*/            {;}
            |   comma_id_rec COMMA ID   {;}
            ;

type:   INT             {;}
    |   FLOAT32         {;}
    |   BOOL            {;}
    |   STRING          {;}
    ;

func_dec:   FUNC ID LPAR parameters RPAR type func_body     {;}
        |   FUNC ID LPAR RPAR type func_body                {;}
        |   FUNC ID LPAR parameters RPAR func_body          {;}
        |   FUNC ID LPAR RPAR func_body                     {;}
        ;

parameters: ID type comma_id_type_rec;
comma_id_type_rec: 
                | comma_id_type_rec COMMA ID type
                ;

func_body: LBRACE vars_and_statements RBRACE
        ;
vars_and_statements: /*EMPTY*/      {;}
                    |   vars_and_statements SEMICOLON
                    |   vars_and_statements var_dec SEMICOLON
                    |   vars_and_statements statements SEMICOLON
                    ;

statements: error
        |   IF expr states_in_brace
        |   IF expr states_in_brace ELSE states_in_brace
        |   FOR expr states_in_brace
        |   FOR states_in_brace
        |   RETURN expr
        |   RETURN
        |   PRINT LPAR expr RPAR
        |   PRINT LPAR STRLIT RPAR 
        |   ID ASSIGN expr
        |   states_in_brace
        |   final_states
        ;

final_states:   func_invocation
            |   parse_args
            ;

states_in_brace: LBRACE state_semic_rec RBRACE;
state_semic_rec: /*EMPTY*/      {;}
                | state_semic_rec statements SEMICOLON
                ;
parse_args: ID COMMA BLANKID ASSIGN PARSEINT LPAR error RPAR
        |ID COMMA BLANKID ASSIGN PARSEINT LPAR CMDARGS LSQ expr RSQ RPAR  
        ;

func_invocation: ID LPAR error RPAR
            |   ID LPAR expr comma_expr_rec RPAR
            |   ID LPAR RPAR
            ;
comma_expr_rec: /*EMPTY*/     {;}
            |   comma_expr_rec COMMA expr
            ;

expr:   LPAR error RPAR
    |   expr operators expr2
    |   expr2
    ;

expr2:  self_oper expr2 %prec UNARY {;}
    |   final_expr {;}
    ;

final_expr:   INTLIT
          |   REALLIT
          |   ID
          |   func_invocation
          |   LPAR expr RPAR
          ;

        
operators:  OR                  {;}
        |   AND                 {;}
        |   LT                  {;}
        |   GT                  {;}
        |   EQ                  {;}
        |   NE                  {;}
        |   GE                  {;}
        |   PLUS                {;}
        |   MINUS               {;}
        |   STAR                {;}
        |   DIV                 {;}
        |   MOD                 {;}
        ;

self_oper:  PLUS
        |   MINUS
        |   NE
        ;

%%



