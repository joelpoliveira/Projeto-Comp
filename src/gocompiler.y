%{ 
    // Tomás Mendes - 2019232272
    // Joel Oliveira - 2019227468

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdbool.h>
    #include "functions.h"
    #include "print_ast.h"
    #include "y.tab.h"

    int yylex(void);
    void yyerror (char *s);

    extern bool flag_1, flag_2, error_flag;

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
    operation_type io;
    self_operation_type iso;
    is_func_dec * ifd;
}

%token SEMICOLON COMMA BLANKID ASSIGN STAR DIV MINUS PLUS EQ GE GT LBRACE   //linhas 28-39
%token LE LPAR LSQ LT MOD NE NOT AND OR RBRACE RPAR RSQ PACKAGE RETURN ELSE //linhas 40-54
%token FOR IF VAR PRINT PARSEINT FUNC CMDARGS INT FLOAT32 STRING BOOL      //linhas 55-65
%token UNARY

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

program: PACKAGE ID SEMICOLON declarations { $$ = program = insert_program($4); if (!error_flag && flag_2) print_ast(program);}
        ;

declarations:    /*EMPTY*/                         {$$ = NULL;}  
                |  declarations var_dec SEMICOLON  {/*printf("Decs1\n");*/$$ = insert_declaration($1, $2); }
                |  declarations func_dec SEMICOLON {/* printf("Decs2\n"); */$$ = insert_declaration($1, $2); }
                ;

var_dec:    VAR var_spec                         {/* printf("vdec1\n"); */$$ = insert_var_declaration($2);}
        |   VAR LPAR var_spec SEMICOLON RPAR     {/* printf("vdec2\n"); */$$ = insert_var_declaration($3);}
        ;

var_spec: ID comma_id_rec type   {/* printf("vspec\n"); */$$ = insert_var_specifications($1, $2, $3);}
        ;
comma_id_rec:   /*EMPTY*/               {$$ = NULL;}
            |   comma_id_rec COMMA ID   {/* printf(", id_list\n"); */$$ = insert_var_id($1, $3);}
            ;

type:   INT             {/* printf("int\n"); */$$ = insert_type("INT");}
    |   FLOAT32         {/* printf("float\n"); */$$ = insert_type("FLOAT");}
    |   BOOL            {/* printf("bool\n"); */$$ = insert_type("BOOL");}
    |   STRING          {/* printf("string\n"); */$$ = insert_type("STR");}
    ;

func_dec:   FUNC ID LPAR parameters RPAR type func_body     {/* printf("f_dec1\n"); */$$=insert_func_declaration($2, $4, $6, $7);}
        |   FUNC ID LPAR RPAR type func_body                {/* printf("f_dec2\n"); */$$=insert_func_declaration($2, NULL, $5, $6);}
        |   FUNC ID LPAR parameters RPAR func_body          {/* printf("f_dec3\n"); */$$=insert_func_declaration($2, $4, d_dummy, $6);}
        |   FUNC ID LPAR RPAR func_body                     {/* printf("f_dec4\n"); */$$=insert_func_declaration($2, NULL, d_dummy, $5);}
        ;

parameters: ID type comma_id_type_rec               {  /* printf("parameters\n"); */$$ = insert_parameter($1, $2, $3);};
comma_id_type_rec:   /*EMPTY*/                     { $$ = NULL; }
                | comma_id_type_rec COMMA ID type   { /* printf(", id_type_list\n"); */ $$ = insert_id_type($1, $3, $4);}
                ;

func_body: LBRACE vars_and_statements RBRACE    {/* printf("func_body\n"); */$$ = insert_func_body($2);}
        ;
vars_and_statements: /*EMPTY*/                                      { $$ = NULL; }
                    |   vars_and_statements SEMICOLON               { $$ = NULL;}
                    |   vars_and_statements var_dec SEMICOLON       { /* printf("vars_states1\n"); */$$ = insert_var_dec($1, ($2)->dec.ivd);}
                    |   vars_and_statements statements SEMICOLON    {/* printf("vars_states2\n"); */ $$ = insert_statements($1, $2); }
                    ;

statements: IF expr states_in_brace                     { /* printf("state1\n") */;$$ = insert_if_statement($2, $3, NULL);  }
        |   IF expr states_in_brace ELSE states_in_brace{ /* printf("state2\n"); */$$ = insert_if_statement($2, $3, $5); }
        |   FOR expr states_in_brace                    { /* printf("state3\n"); */$$ = insert_for_statement($2, $3); }
        |   FOR states_in_brace                         { /* printf("state4\n"); */$$ = insert_for_statement(NULL, $2); }
        |   RETURN expr                                 { /* printf("state5\n"); */$$ = insert_return_statement($2);    }
        |   RETURN                                      { /* printf("state6\n"); */ $$ = insert_return_statement(NULL);  }
        |   PRINT LPAR expr RPAR                        { /* printf("state7\n"); */$$ = insert_print_expr_statement($3); }    
        |   PRINT LPAR STRLIT RPAR                      { /* printf("state8\n"); */$$ = insert_print_str_statement($3);  }
        |   ID ASSIGN expr                              { /* printf("state9\n") */;$$ = insert_assign_statement($1, $3); }
        |   states_in_brace                             { /* printf("state10\n"); */$$ = insert_statements_list($1); }
        |   final_states                                { /* printf("state11\n"); */$$ = insert_final_statement($1); }
        ;

final_states:   func_invocation  {  /* printf("final_s1\n"); */ $$ = insert_final_state_func_inv($1);}
            |   parse_args       { /* printf("final_s2\n"); */$$ = insert_final_state_args($1);  }
            |   error                                       { $$ = NULL; error_flag = 1;}
            ;

states_in_brace: LBRACE state_semic_rec RBRACE          {/* printf("in_brace\n"); */ $$ = $2; };
state_semic_rec: /*EMPTY*/                              { $$ = NULL ; }
                | state_semic_rec statements SEMICOLON  {  /* printf("s_;_list\n"); */ $$ = insert_statement_in_list($1, $2);}
                ;
parse_args: ID COMMA BLANKID ASSIGN PARSEINT LPAR CMDARGS LSQ expr RSQ RPAR {  /* printf("parse\n"); */ $$ = insert_parse_args($1, $9); }
        |   ID COMMA BLANKID ASSIGN PARSEINT LPAR error RPAR                { $$ = NULL; error_flag = 1; }
        ;

func_invocation: ID LPAR error RPAR                 { error_flag = 1; }
            |   ID LPAR expr comma_expr_rec RPAR    { /* printf("f_iv1\n"); */ $$ = insert_func_inv($1, $3, $4);}
            |   ID LPAR RPAR                        { /* printf("f_inv2\n"); */ $$ = insert_func_inv($1, NULL, NULL);}
            ;
comma_expr_rec: /*EMPTY*/                   { $$ = NULL;}
            |   comma_expr_rec COMMA expr   { /* printf("expr_list\n"); */ $$ = insert_expression($1, $3);}
            ;

expr:   expr operators expr2    {  /* printf("expr1\n"); */ $$ = insert_first_oper($1, $2, $3); }
    |   expr2                   {  /* printf("expr1_2\n"); */ $$ = insert_first_expr($1); }
    ;

expr2:  self_oper expr2 %prec UNARY { /* printf("expr2\n"); */$$ = insert_second_oper($2, $1); }
    |   final_expr                  {/*  printf("expr2_2\n"); */$$ = insert_second_expr($1);  }
    ;

final_expr:   INTLIT               { /* printf("intlit\n"); */$$ = insert_intlit($1); }           
          |   REALLIT               { /* printf("reallit\n"); */$$ = insert_real($1); } 
          |   ID                    { /* printf("id\n"); */$$ = insert_id($1); } 
          |   func_invocation       { /* printf("fun_inv\n"); */$$ = insert_final_func_inv($1); } 
          |   LPAR expr RPAR        {   /* printf("final_expr_5\n"); */$$ = insert_final_expr($2);} 
          |   LPAR error RPAR         {error_flag = 1;}
          ;

operators:  OR                  {/* printf("or\n"); */ $$ = insert_oper("OR"); }
        |   AND                 {/* printf("and\n"); */ $$ = insert_oper("AND"); }
        |   LT                  { /* printf("lt\n"); */$$ = insert_oper("LT");}
        |   LE                  {$$ = insert_oper("LE"); }
        |   GT                  {/* printf("gt\n"); */ $$ = insert_oper("GT"); }
        |   EQ                  {/* printf("eq\n"); */ $$ = insert_oper("EQ"); }
        |   NE                  { /* printf("ne\n"); */ $$ = insert_oper("NE"); }
        |   GE                  {/* printf("ge\n"); */ $$ = insert_oper("GE");  }
        |   PLUS                { /* printf("plus\n"); */ $$ = insert_oper("PLUS"); }
        |   MINUS               {  /* printf("minus\n"); */ $$ = insert_oper("MINUS"); }
        |   STAR                { /* printf("star\n"); */ $$ = insert_oper("STAR"); }
        |   DIV                 {/*  printf("div\n"); */$$ = insert_oper("DIV"); }
        |   MOD                 {/* printf("mod\n"); */ $$ = insert_oper("MOD"); }
        ;
self_oper:  PLUS                { /* printf("self_plus\n"); */$$ = insert_self_oper("PLUS"); }
        |   MINUS               {/* printf("self_minus\n"); */ $$ = insert_self_oper("MINUS"); }
        |   NOT                 {/* printf("self_not\n"); */ $$ = insert_self_oper("NOT"); }
        ;

%%



