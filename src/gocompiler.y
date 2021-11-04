%{
    #include <stdio.h>
    #include <string.h>
    
    int yylex(void);
    void yyerror (const char *s);
%}

%union{
    int intval;
    char* string;
    float floatval;
}
%token SEMICOLON COMMA BLANKID ASSIGN STAR DIV MINUS PLUS EQ GE GT LBRACE   //linhas 28-39
%token LE LPAR LSQ LT MOD NE NOT AND OR RBRACE RPAR RSQ PACKAGE RETURN ELSE //linhas 40-54
%token FOR IF VAR INT FLOAT32 BOOL STRING PRINT PARSEINT FUNC CMDARGS       //linhas 55-65

%type <>program 
%%

Program:    {printf("%d\n", $$);};



%%

int main(int argc, char *argv[]){
    if (argv[1] != 0 && (argv[1][0] == '-' && argv[1][1] == 'l')){
        //checkig for argument -l like this so we dont have to import string.h
        flag_1 = 1;
    }   

    yyparse();
    return 0;
}

/*void yyerror ( char * s ) {
    printf ( " Line %d, column % d: % s: % s \n " , line, col ,s ,yytext );
}*/