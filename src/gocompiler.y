%{
    #include <stdio.h>
    #include <string.h>
    #include "y.tab.h"
    //#include "header.h"

    int yylex(void);
    void yyerror (char *s);

    int flag_1 = 0, flag_2;
    int yydebug = 1;
%}

%union{
    char* string;

}

%token SEMICOLON COMMA BLANKID ASSIGN STAR DIV MINUS PLUS EQ GE GT LBRACE   //linhas 28-39
%token LE LPAR LSQ LT MOD NE NOT AND OR RBRACE RPAR RSQ PACKAGE RETURN ELSE //linhas 40-54
%token FOR IF VAR INT FLOAT32 BOOL STRING PRINT PARSEINT FUNC CMDARGS       //linhas 55-65

%token <string> RESERVED INTLIT REALLIT ID STRLIT

%left  COMMA
%right ASSIGN //'+=' '-='
%left  OR
%left  AND
%left  EQ NE
%left  PLUS MINUS
%left  STAR
%left  LPAR RSQ

%%

program: PACKAGE ID SEMICOLON var_declarations {printf("PROGRAM\n");}
        ;

var_declarations:                                      {printf("\n");}  
                |  var_declarations var_dec SEMICOLON  {printf("VAR\n"); }
                |  var_declarations func_dec SEMICOLON {printf("FUNC\n"); }
                ;

var_dec:    VAR var_spec                         {printf("VarDec\n"); }
        |   VAR LPAR var_spec SEMICOLON RPAR     {printf("VarDec\n"); }
        ;

var_spec: ID comma_id_rec type   {printf("VarSpec\n"); }
        ;
comma_id_rec:
            |   comma_id_rec comma_id
            ;

type:   INT
    |   FLOAT32
    |   BOOL
    |   STRING
    ;

func_dec:   FUNC ID LPAR parameters RPAR type func_body
        |   FUNC ID LPAR RPAR type func_body
        |   FUNC ID LPAR parameters RPAR func_body
        |   FUNC ID LPAR RPAR func_body
        ;

parameters: ID type comma_id_type_rec;
comma_id_type_rec: 
                | comma_id_type_rec comma_id type
                ;

func_body: LBRACE vars_and_statements RBRACE;
vars_and_statements:
                    |   vars_and_statements SEMICOLON
                    |   vars_and_statements var_dec SEMICOLON
                    |   vars_and_statements statements SEMICOLON 
                    ;

statements: if_state
        |   for_state
        |   return_state
        |   normal_state
        |   print_state
        ;

if_state: IF expr states_in_brace
        | IF expr states_in_brace ELSE states_in_brace
        ;

for_state:  FOR expr states_in_brace
        |   FOR states_in_brace
        ;

return_state:   RETURN expr
            |   RETURN
            ;

print_state:    PRINT LPAR expr RPAR
            |   PRINT LPAR STRLIT RPAR 
            ;

normal_state:   ID ASSIGN expr
            |   states_in_brace
            |   func_invocation
            |   parse_args
            ;
states_in_brace: LBRACE state_semic_rec RBRACE;
state_semic_rec:
                | state_semic_rec statements SEMICOLON
                ;
parse_args: ID COMMA BLANKID ASSIGN PARSEINT LPAR CMDARGS LSQ expr RSQ RPAR;

func_invocation: ID LPAR expr comma_expr_rec RPAR
            |   ID LPAR RPAR
            ;
comma_expr_rec:
            |   comma_expr_rec COMMA expr
            ;

expr:   expr OR expr2
    |   expr AND expr2
    |   expr2
    ;

// tentar comparações
expr2:  expr2 LT expr3
    |   expr2 GT expr3
    |   expr2 EQ expr3
    |   expr2 NE expr3
    |   expr2 LE expr3
    |   expr2 GE expr3
    |   expr3
    ;

// tentar operações
expr3:  expr3 PLUS expr4
    |   expr3 MINUS expr4
    |   expr3 STAR expr4
    |   expr3 DIV expr4
    |   expr3 MOD expr4
    |   expr4
    ;

//tentar self operations e finalizar
expr4: NOT final_expr
    |  PLUS final_expr
    |  MINUS final_expr
    |  final_expr
    ;

final_expr:   INTLIT
          |   REALLIT
          |   ID
          |   func_invocation
          |   LPAR expr RPAR
          ;

comma_id: COMMA ID;

%%

int main(int argc, char *argv[]){
    //checkig for argument -l like this so we dont have to import string.h
    if (argv[1] != 0 && (argv[1][0] == '-' && argv[1][1] == 'l')){
        flag_1 = 1;
    }

    if (argv[1] != 0 && (argv[1][0] == '-' && argv[1][1] == 't')){
        flag_2 = 1;
    } 

    yyparse();
    return 0;
}

void yyerror (char *s) {
    //printf ( " Line %d, column % d: % s: % s \n " , line, col ,s ,yytext );
    printf("Erro: %s\n\n", s);
}