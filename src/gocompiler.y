%{ 
    // Tomás Mendes - 2019232272
    // Joel Oliveira - 2019227468

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "y.tab.h"
    #include "functions.h"

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
    is_expression_list * iel;
    is_expression2_list * ie2l;
    is_final_expression * ife;
    is_operation * io;
    is_self_operation * iso;
    is_func_dec * ifd;
}

%token SEMICOLON COMMA BLANKID ASSIGN STAR DIV MINUS PLUS EQ GE GT LBRACE   //linhas 28-39
%token LE LPAR LSQ LT MOD NE NOT AND OR RBRACE RPAR RSQ PACKAGE RETURN ELSE //linhas 40-54
%token FOR IF VAR PRINT PARSEINT FUNC CMDARGS INT FLOAT32 STRING BOOL      //linhas 55-65
%token UNARY

//%token <string> RESERVED INTLIT REALLIT ID STRLIT
%token<id> ID STRLIT INTLIT REALLIT RESERVED
//%token<pt> 

%type<ip> program
%type<idl> declarations
%type<idec> var_dec func_dec
%type<ivs> var_spec
%type<iil> comma_id_rec
%type<pt> type
%type<iparam> parameters
%type<iitl> comma_id_type_rec
%type<ifb> func_body
%type<ivsl> vars_and_statements
%type<is> statements
%type<ifs> final_states
%type<isl> states_in_brace state_semic_rec
%type<ipa> parse_args
%type<ifi> func_invocation
%type<iel> comma_expr_rec expr
%type<ie2l> expr2
%type<ife> final_expr
%type<io> operators
%type<iso> self_oper

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

var_spec: ID comma_id_rec type   {$$ = insert_var_specifications($1, $2, $3);}
        ;
comma_id_rec:   /*EMPTY*/               {;}
            |   comma_id_rec COMMA ID   {$$ = insert_var_id($1, $3);}
            ;

type:   INT             {$$ = insert_type("INT");}
    |   FLOAT32         {$$ = insert_type("FLOAT");}
    |   BOOL            {$$ = insert_type("BOOL");}
    |   STRING          {$$ = insert_type("STR");}
    ;

func_dec:   FUNC ID LPAR parameters RPAR type func_body     {insert_func_declaration($2, $4, $6, $7);}
        |   FUNC ID LPAR RPAR type func_body                {insert_func_declaration($2, NULL, $5, $6);}
        |   FUNC ID LPAR parameters RPAR func_body          {insert_func_declaration($2, $4, void, $6);}
        |   FUNC ID LPAR RPAR func_body                     {insert_func_declaration($2, NULL, NULL $5);}
        ;

parameters: ID type comma_id_type_rec               { $$ = insert_parameter($1, $2, $3); };
comma_id_type_rec:                                  { ; }
                | comma_id_type_rec COMMA ID type   { $$ = insert_id_type($1, $3, $4); }
                ;

func_body: LBRACE vars_and_statements RBRACE    {$$ = insert_func_body($2);}
        ;
vars_and_statements: /*EMPTY*/                                      { $$ = NULL; }
                    |   vars_and_statements SEMICOLON               {;}
                    |   vars_and_statements var_dec SEMICOLON       { $$ = insert_var_dec($1, ($2)->dec.ivd); }
                    |   vars_and_statements statements SEMICOLON    { $$ = insert_statements($1, $2); }
                    ;

statements: IF expr states_in_brace                     { $$ = insert_if_statement($2, $3, NULL); }
        |   IF expr states_in_brace ELSE states_in_brace{ $$ = insert_if_statement($2, $3, $5); }
        |   FOR expr states_in_brace                    { $$ = insert_for_statement($2, $3);}
        |   FOR states_in_brace                         { $$ = insert_for_statement(NULL, $2);  }
        |   RETURN expr                                 { $$ = insert_return_statement($2);     }
        |   RETURN                                      { $$ = insert_return_statement(NULL);   }
        |   PRINT LPAR expr RPAR                        { $$ = insert_print_expr_statement($3); }    
        |   PRINT LPAR STRLIT RPAR                      { $$ = insert_print_str_statement($3);  }
        |   ID ASSIGN expr                              { $$ = insert_assign_statement($1, $3); }
        |   states_in_brace                             { $$ = insert_statements_list($1); }
        |   final_states                                { $$ = insert_final_statement($1); }
        |   error                                       { $$ = NULL;}
        ;

final_states:   func_invocation  { $$ = insert_final_state_func_inv($1); }
            |   parse_args       { $$ = insert_final_state_args($1); }
            ;

states_in_brace: LBRACE state_semic_rec RBRACE          { $$ = $2; };
state_semic_rec: /*EMPTY*/                              { ;}
                | state_semic_rec statements SEMICOLON  { $$ = insert_statement_in_list($1, $2); }
                ;
parse_args: ID COMMA BLANKID ASSIGN PARSEINT LPAR CMDARGS LSQ expr RSQ RPAR { $$ = insert_parse_args($1, $9); }
        |   ID COMMA BLANKID ASSIGN PARSEINT LPAR error RPAR                { $$ = NULL; }
        ;

func_invocation: ID LPAR error RPAR                 { $$ = NULL; }
            |   ID LPAR expr comma_expr_rec RPAR    {$$ = insert_func_inv($1, $3, $4);}
            |   ID LPAR RPAR                        {$$ = insert_func_inv($1, NULL, NULL);}
            ;
comma_expr_rec: /*EMPTY*/                   {;}
            |   comma_expr_rec COMMA expr   {$$ = insert_expression($1, $3);}
            ;

expr:   LPAR error RPAR         {$$ = NULL;}
    |   expr operators expr2    { $$ = insert_first_expr($1, $2, $3); }
    |   expr2                   { $$ = insert_first_expr(NULL, NULL, $1); }
    ;

expr2:  self_oper expr2 %prec UNARY {$$ = insert_second_oper($2, $1);}
    |   final_expr                  { $$ = insert_second_expr($1);}
    ;

final_expr:   INTLIT               { $$ = insert_intlit($1);}           
          |   REALLIT               { $$ = insert_real($1);} 
          |   ID                    { $$ = insert_id($1);} 
          |   func_invocation       { $$ = insert_final_func_inv($1);} 
          |   LPAR expr RPAR        { $$ = insert_final_expr($2);} 
          ;

operators:  OR                  {$$ = insert_oper("OR");}
        |   AND                 {$$ = insert_oper("AND");}
        |   LT                  {$$ = insert_oper("LT");}
        |   GT                  {$$ = insert_oper("GT");}
        |   EQ                  {$$ = insert_oper("EQ");}
        |   NE                  {$$ = insert_oper("NE");}
        |   GE                  {$$ = insert_oper("GE");}
        |   PLUS                {$$ = insert_oper("PLUS");}
        |   MINUS               {$$ = insert_oper("MINUS");}
        |   STAR                {$$ = insert_oper("STAR");}
        |   DIV                 {$$ = insert_oper("DIV");}
        |   MOD                 {$$ = insert_oper("MOD");}
        ;
self_oper:  PLUS                {$$ = insert_self_oper("PLUS");}
        |   MINUS               {$$ = insert_self_oper("MINUS");}
        |   NOT                 {$$ = insert_self_oper("NOT");}
        ;

%%



