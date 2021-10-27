%{
    #include <stdio.h>
    int yylex(void);
    void yyerror (const char *s);
%}

%token PROGRAM

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