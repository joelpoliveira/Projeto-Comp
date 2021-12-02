%{ 
    // Tomás Mendes - 2019232272
    // Joel Oliveira - 2019227468

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdbool.h>
    #include "functions.h"
    #include "free_tree.h"
    #include "print_ast.h"
    #include "y.tab.h"

    int yylex(void);
    void yyerror (char *s);

    extern bool flag_1, flag_2, flag_3, error_flag;

    int yydebug = 1;

    is_program * program = NULL;
    table_element* symtab = NULL;

    /*  
        Submissão no Mooshak:
        ====Meta 2====
        A -> 250/250    :: Global
        B -> 35/35      :: Syntax errors - Variable and function declarations/definitions
        C -> 35/35      :: Syntax errors - Expressions
        D -> 35/35      :: Syntax errors - Statements
        E -> 35/35      :: AST - Variable and function declarations/definitions
        F -> 35/35      :: AST - Expressions
        G -> 35/35      :: AST - Statements
        H -> 40/40      :: AST - Full Programs
        
    */
%}

%union{
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
}

%token SEMICOLON COMMA BLANKID ASSIGN LBRACE   //linhas 28-39
%token LPAR LSQ RBRACE RPAR RSQ PACKAGE RETURN ELSE //linhas 40-54
%token FOR IF VAR PRINT PARSEINT FUNC CMDARGS INT FLOAT32 STRING BOOL      //linhas 55-65
%token UNARY
%token<nop>STAR DIV MINUS PLUS EQ GE GT LE LT MOD NE NOT AND OR 
%token<id> ID STRLIT INTLIT REALLIT RESERVED
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
%type<ieol>  expr_or
%type<ifiel> comma_expr_rec
%type<ieal> expr_and
%type<iecl> expr_comp
%type<iesl> expr_sum_like
%type<iestl> expr_star_like
%type<nop> oper_comp oper_star_like oper_sum_like self_oper
%type<isel> self_expr
%type<ife> final_expr


%right ASSIGN
%left  OR
%left  AND
%right  EQ NE LE GE LT GT
%left PLUS MINUS
%left  STAR DIV 
%nonassoc UNARY
%nonassoc  LPAR LSQ

%%

program: PACKAGE ID SEMICOLON declarations {$$ = program = insert_program($4);}
        ;

declarations:    /*EMPTY*/                              {$$ = NULL;}  
                |  declarations var_dec SEMICOLON       {$$ = insert_declaration($1, $2); }
                |  declarations func_dec SEMICOLON      {$$ = insert_declaration($1, $2); }
                |  declarations error SEMICOLON         {error_flag = 1;}
                ;

var_dec:    VAR var_spec                         { $$ = insert_var_declaration($2);}
        |   VAR LPAR var_spec SEMICOLON RPAR     { $$ = insert_var_declaration($3);}
        ;

var_spec: ID comma_id_rec type   { $$ = insert_var_specifications($1, $2, $3);}
        ;
comma_id_rec:   /*EMPTY*/               { $$ = NULL;}
            |   comma_id_rec COMMA ID   { $$ = insert_var_id($1, $3);}
            ;

type:   INT             { $$ = insert_type("INT");}
    |   FLOAT32         { $$ = insert_type("FLOAT");}
    |   BOOL            { $$ = insert_type("BOOL");}
    |   STRING          { $$ = insert_type("STR");}
    ;

func_dec:   FUNC ID LPAR parameters RPAR type func_body     { $$=insert_func_declaration($2, $4, $6, $7);}
        |   FUNC ID LPAR RPAR type func_body                { $$=insert_func_declaration($2, NULL, $5, $6);}
        |   FUNC ID LPAR parameters RPAR func_body          { $$=insert_func_declaration($2, $4, d_none, $6);}
        |   FUNC ID LPAR RPAR func_body                     { $$=insert_func_declaration($2, NULL, d_none, $5);}
        ;

parameters: ID type comma_id_type_rec               {  $$ = insert_parameter($1, $2, $3);};
comma_id_type_rec:   /*EMPTY*/                     { $$ = NULL; }
                | comma_id_type_rec COMMA ID type   {   $$ = insert_id_type($1, $3, $4);}
                ;

func_body: LBRACE vars_and_statements RBRACE    { $$ = insert_func_body($2);}
        ;
vars_and_statements: /*EMPTY*/                                      { $$ = NULL; }
                    |   vars_and_statements SEMICOLON               { ;}
                    |   vars_and_statements var_dec SEMICOLON       { $$ = insert_var_dec($1, ($2)->dec.ivd);}
                    |   vars_and_statements statements SEMICOLON    { $$ = insert_statements($1, $2); }
                    ;

statements: IF expr_or states_in_brace                          { $$ = insert_if_statement($2, $3, NULL);  }
        |   IF expr_or states_in_brace ELSE states_in_brace     { $$ = insert_if_statement($2, $3, $5); }
        |   FOR states_in_brace                                 {$$ = insert_for_statement(NULL, $2); }
        |   FOR expr_or states_in_brace                         { $$ = insert_for_statement($2, $3); }
        |   RETURN expr_or                                      {$$ = insert_return_statement($2);    }
        |   RETURN                                              { $$ = insert_return_statement(NULL);  }
        |   PRINT LPAR expr_or RPAR                             {$$ = insert_print_expr_statement($3); }    
        |   PRINT LPAR STRLIT RPAR                              {$$ = insert_print_str_statement($3);  }
        |   ID ASSIGN expr_or                                   { $$ = insert_assign_statement($1, $3); }
        |   states_in_brace                                     { $$ = insert_statements_list($1); }
        |   final_states                                        { $$ = insert_final_statement($1); }
        |   error                                               {error_flag = 1; }
        ;

final_states:   func_invocation  { $$ = insert_final_state_func_inv($1);}
            |   parse_args       { $$ = insert_final_state_args($1);  }
            ;

states_in_brace: LBRACE state_semic_rec RBRACE          { $$ = $2; };
state_semic_rec: /*EMPTY*/                              { $$ = NULL ; }
                | state_semic_rec statements SEMICOLON  {   $$ = insert_statement_in_list($1, $2);}
                ;
parse_args: ID COMMA BLANKID ASSIGN PARSEINT LPAR CMDARGS LSQ expr_or RSQ RPAR {$$ = insert_parse_args($1, $9); }
        |   ID COMMA BLANKID ASSIGN PARSEINT LPAR error RPAR                {error_flag = 1; }
        ;

func_invocation: ID LPAR error RPAR                 { error_flag = 1;}
            |   ID LPAR expr_or comma_expr_rec RPAR    {$$ = insert_func_inv($1, $3, $4);}
            |   ID LPAR RPAR                        {  $$ = insert_func_inv($1, NULL, NULL);}
            ;
comma_expr_rec: /*EMPTY*/                   { $$ = NULL;}
            |   comma_expr_rec COMMA expr_or   { $$ = insert_expression($1, $3);}
            ;

expr_or: expr_or OR expr_and    {$$ = insert_or($1, $2, $3); }
        | expr_and              {$$ = insert_or(NULL, NULL, $1); }
        ;

expr_and: expr_and AND expr_comp        {$$ = insert_and($1, $2, $3);}
        | expr_comp                     {$$ = insert_and(NULL, NULL, $1);}
        ;

expr_comp: expr_comp oper_comp expr_sum_like    {$$ = insert_comp($1, $2, $3);}
        |  expr_sum_like                        {$$ = insert_comp(NULL, NULL, $1);}
        ;

expr_sum_like: expr_sum_like oper_sum_like expr_star_like {$$ = insert_sum_like($1,$2,$3);}
        |       expr_star_like                            {$$=insert_sum_like(NULL, NULL, $1);}
        ;

expr_star_like: expr_star_like oper_star_like self_expr {$$ = insert_star_like($1, $2, $3);}
        |       self_expr                               {$$ = insert_star_like(NULL, NULL, $1);}
        ;

self_expr: self_oper self_expr %prec UNARY              {$$ = insert_self($2, $1, NULL);}
        |  final_expr                                   {$$ = insert_self(NULL, NULL, $1); }
        ;

final_expr:   INTLIT               {$$ = insert_intlit($1); }           
          |   REALLIT               {$$ = insert_real($1); } 
          |   ID                    {$$ = insert_id($1); } 
          |   func_invocation       {$$ = insert_final_func_inv($1); } 
          |   LPAR expr_or RPAR        { $$ = insert_final_expr($2);} 
          |   LPAR error RPAR         {error_flag = 1;}
          ;

oper_comp:  LT                  {$$ = insert_comp_oper("LT", $1);}
        |   LE                  {$$ = insert_comp_oper("LE", $1); }
        |   GT                  {$$ = insert_comp_oper("GT", $1); }
        |   EQ                  {$$ = insert_comp_oper("EQ", $1); }
        |   NE                  {$$ = insert_comp_oper("NE", $1); }
        |   GE                  {$$ = insert_comp_oper("GE", $1);  }
        ;

oper_sum_like: PLUS             {  $$ = insert_sum_like_oper("PLUS", $1); }
        |   MINUS               {  $$ = insert_sum_like_oper("MINUS", $1); }
        ;

oper_star_like: STAR                    {  $$ = insert_star_like_oper("STAR", $1); }
        |   DIV                         {$$ = insert_star_like_oper("DIV", $1); }
        |   MOD                         {$$ = insert_star_like_oper("MOD", $1); }
        ;

self_oper:  PLUS                { $$ = insert_self_oper("PLUS", $1); }
        |   MINUS               { $$ = insert_self_oper("MINUS", $1); }
        |   NOT                 { $$ = insert_self_oper("NOT", $1); }
        ;



%%



