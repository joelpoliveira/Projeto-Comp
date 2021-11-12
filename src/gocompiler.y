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

    is_program* program;
    //error_node* error_list;

    /*  
        Submissão no Mooshak:
        A -> 66/250     :: Global
        B -> 33/35      :: Syntax errors - Variable and function declarations/definitions
        C -> 7/35       :: Syntax errors -Expressions
        D -> 26/35      :: Syntax errors - Statements
        E -> 0/35       :: AST - Variable and function declarations/definitions
        F -> 0/35       :: AST - Expressions
        G -> 0/35       :: AST - Statements
        H -> 0/40       :: AST - Full Programs 
    */

%}

%union{
    char *id;
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
}

%token SEMICOLON COMMA BLANKID ASSIGN STAR DIV MINUS PLUS EQ GE GT LBRACE   //linhas 28-39
%token LE LPAR LSQ LT MOD NE NOT AND OR RBRACE RPAR RSQ PACKAGE RETURN ELSE //linhas 40-54
%token FOR IF VAR INT FLOAT32 BOOL STRING PRINT PARSEINT FUNC CMDARGS       //linhas 55-65
%token UNARY

//%token <string> RESERVED INTLIT REALLIT ID STRLIT
%token<id>ID STRLIT INTLIT REALLIT
%type<ip>program
%type<idl>declarations
%type<idec>var_dec func_dec
%type<ivs>var_spec
%type<iil>comma_id_rec
%type<id>type
%type<iparam>parameters
%type<iitl>comma_id_type_rec
%type<ifb>func_body
%type<ivsl> vars_and_statements
%type<is>statements
%type<ifs>final_states

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

program: PACKAGE ID SEMICOLON declarations {  $$ = program = insert_program($4);}
        ;

declarations:                                      {;}  
                |  declarations var_dec SEMICOLON  {$$ = insert_declaration($1, $2); }
                |  declarations func_dec SEMICOLON {$$ = insert_declaration($1, $2); }
                ;

var_dec:    VAR var_spec                         {$$ = insert_var_declaration($2);}
        |   VAR LPAR var_spec SEMICOLON RPAR     {$$ = insert_var_declaration($3);}
        ;

var_spec: ID comma_id_rec type   {$$ = insert_var_specifications($2, $3)}
        ;
comma_id_rec:   /*EMPTY*/            {;}
            |   comma_id_rec COMMA ID   {$$ = insert_var_id($1, $3);}
            ;

type:   INT             {$$ = $1;}
    |   FLOAT32         {$$ = $1;}
    |   BOOL            {$$ = $1;}
    |   STRING          {$$ = $1;}
    ;

func_dec:   FUNC ID LPAR parameters RPAR type func_body     {insert_func_declaration($4, $6, $7);}
        |   FUNC ID LPAR RPAR type func_body                {insert_func_declaration(NULL, $5, $7);}
        |   FUNC ID LPAR parameters RPAR func_body          {insert_func_declaration($4, NULL, $7);}
        |   FUNC ID LPAR RPAR func_body                     {insert_func_declaration(NULL, NULL $7);}
        ;

parameters: ID type comma_id_type_rec  { $$ = insert_parameter($1, $2, $3); };
comma_id_type_rec:  
                | comma_id_type_rec COMMA ID type   {$$ = insert_id_type($1, $3, $4)}
                ;

func_body: LBRACE vars_and_statements RBRACE    {$$ = insert_func_body($1);}
        ;
vars_and_statements: /*EMPTY*/                                      { $$ = NULL; }
                    |   vars_and_statements SEMICOLON               {;}
                    |   vars_and_statements var_dec SEMICOLON       { $$ = insert_var_dec($1, $2); }
                    |   vars_and_statements statements SEMICOLON    { $$ = insert_statements($1, $2); }
                    ;

statements: IF expr states_in_brace                     { $$ = insert_if_statement($2, $3, NULL); }
        |   IF expr states_in_brace ELSE states_in_brace{ $$ = insert_if_statement($2, $3, $5); }
        |   FOR expr states_in_brace                    { $$ = insert_for_statement($1, $2);}
        |   FOR states_in_brace                         { $$ = insert_for_statement(NULL, $1);  }
        |   RETURN expr                                 { $$ = insert_return_statement($1);     }
        |   RETURN                                      { $$ = insert_return_statement(NULL);   }
        |   PRINT LPAR expr RPAR                        { $$ = insert_print_expr_statement($3); }    
        |   PRINT LPAR STRLIT RPAR                      { $$ = insert_print_str_statement($3);  }
        |   ID ASSIGN expr                              { $$ = insert_assign_statement($1, $3); }
        |   states_in_brace                             { $$ = insert_statements_list($1); }
        |   final_states                                { $$ = insert_final_statement($1); }
        |   error
        ;

final_states:   func_invocation  { $$ = insert_final_state_func_inv($1) }
            |   parse_args       { $$ = insert_final_state_args($1) }
            ;

states_in_brace: LBRACE state_semic_rec RBRACE;
state_semic_rec: /*EMPTY*/      {$$ = NULL;}
                | state_semic_rec statements SEMICOLON  { $$ = insert_statement_in_list($1, $2); }
                ;
parse_args: ID COMMA BLANKID ASSIGN PARSEINT LPAR CMDARGS LSQ expr RSQ RPAR { $$ = insert_parse_args($1, $9); }
        |   ID COMMA BLANKID ASSIGN PARSEINT LPAR error RPAR
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
        |   NOT
        ;

%%



