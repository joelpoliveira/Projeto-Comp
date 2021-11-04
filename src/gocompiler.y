%{
    #include <stdio.h>
    #include <string.h>
    #include "functions.h"

    int yylex(void);
    void yyerror (char *s);

    is_program* myprogram;
    int flag_1 = 1;
    int yydebug = 1;
%}

%union{
    char* string;

}

%token SEMICOLON COMMA BLANKID ASSIGN STAR DIV MINUS PLUS EQ GE GT LBRACE   //linhas 28-39
%token LE LPAR LSQ LT MOD NE NOT AND OR RBRACE RPAR RSQ PACKAGE RETURN ELSE //linhas 40-54
%token FOR IF VAR INT FLOAT32 BOOL STRING PRINT PARSEINT FUNC CMDARGS       //linhas 55-65

%token <string> RESERVED INTLIT REALLIT ID STRLIT


%%

program: PACKAGE ID SEMICOLON vardeclist {printf("PROGRAM\n");}
        ;

vardeclist:                             {printf("\n");}  
        |  vardeclist vardec SEMICOLON  {printf("VAR\n"); }
        |  vardeclist funcdec SEMICOLON {printf("FUNC\n"); }
        ;

vardec: VAR varspec                         {printf("VarDec\n"); }
    |   VAR LPAR varspec SEMICOLON RPAR     {printf("VarDec\n"); }
    ;

varspec: ID comma_id_rec type   {printf("VarSpec\n"); }
        ;
comma_id_rec:
            |   comma_id_rec COMMA ID
            ;

type:   INT
    |   FLOAT32
    |   BOOL
    |   STRING
    ;

funcdec: FUNC ID LPAR parameters RPAR type funcbody
    |   FUNC ID LPAR RPAR type funcbody
    |   FUNC ID LPAR parameters RPAR funcbody
    |   FUNC ID LPAR RPAR funcbody
    ;

parameters: ID type comma_id_type_rec;
comma_id_type_rec: 
                | comma_id_type_rec COMMA ID type
                ;

funcbody: LBRACE vars_and_statements RBRACE;
vars_and_statements:
                |   vars_and_statements SEMICOLON
                |   vars_and_statements vardec SEMICOLON
                |   vars_and_statements statement SEMICOLON 
                ;

statement:  ID ASSIGN expr
        |   LBRACE state_semic_rec RBRACE
        |   IF expr LBRACE state_semic_rec RBRACE
        |   IF expr LBRACE state_semic_rec RBRACE ELSE LBRACE state_semic_rec RBRACE
        |   FOR expr LBRACE state_semic_rec RBRACE
        |   FOR LBRACE state_semic_rec RBRACE
        |   RETURN expr
        |   RETURN
        |   func_invocation
        |   parse_args
        |   PRINT LPAR expr RPAR
        |   PRINT LPAR STRLIT RPAR 
        ;
state_semic_rec:
                | state_semic_rec statement SEMICOLON
                ;

parse_args: ID COMMA BLANKID ASSIGN PARSEINT LPAR CMDARGS LSQ expr RSQ RPAR;

func_invocation: ID LPAR expr comma_expr_rec RPAR
            |   ID LPAR RPAR
            ;
comma_expr_rec:
            |   comma_expr_rec COMMA expr
            ;

expr:   expr AND expr
    |   expr OR expr
    |   expr LT expr
    |   expr GT expr
    |   expr EQ expr
    |   expr NE expr
    |   expr LE expr
    |   expr GE expr
    |   expr PLUS expr
    |   expr MINUS expr
    |   expr STAR expr
    |   expr DIV expr
    |   expr MOD expr
    |   NOT expr
    |   PLUS expr
    |   MINUS expr
    |   INTLIT
    |   REALLIT
    |   ID
    |   func_invocation
    |   LPAR expr RPAR
    ;
%%

int main(int argc, char *argv[]){
    if (argv[1] != 0 && (argv[1][0] == '-' && argv[1][1] == 'l')){
        //checkig for argument -l like this so we dont have to import string.h
        flag_1 = 1;
    }   

    yyparse();
    return 0;
}

void yyerror ( char * s ) {
    //printf ( " Line %d, column % d: % s: % s \n " , line, col ,s ,yytext );
    printf("Print no yyerror\n");
}