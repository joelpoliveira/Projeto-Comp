%{
    // Tomás Mendes - 2019232272
    // Joel Oliveira - 2019227468
    #include "structures.h"
    #include "functions.h"
    #include "semantics.h"
    #include "y.tab.h"
    #include <string.h>
    #include <stdbool.h>

    void yyerror (char *s);

    bool flag_1 = 0, 
        flag_2 = 0,
        flag_3 = 0,
        swaped_line = 0,
        error_line = 0,
        error_flag = 0;


    int line = 1,
        comment_lines = 1,
        col = 0,
        comment_cols = 0;

    extern is_program * program;
    //extern table_element* symtab;

    /*
        Submissão no Mooshak:
        ====Meta 1====
        A -> 190/190    :: Global
        B ->       :: 
        C ->       :: 
        D ->       :: 
        E ->       :: 
        F ->       :: 
        G ->       :: 
        H ->       ::
        I ->
        J ->
        K ->
        L ->
    */
%}

letter  [a-zA-Z_]
number  [0-9]
exp     (e|E)[-+]?{number}+
float   ({number}+"."{number}*)|("."{number}+)
hexa	(0(x|X)[0-9a-fA-F]+)
newline \r?\n
sp_chr  \\(\"|n|r|t|f|\\)
whispace (" "|\t)

%X _SEMICOLON BCOMMENT LCOMMENT _STRING DUMMY_QUOTE OCTAL SEMIBCOMMENT SEMILCOMMENT INVALID_STRING CHECK_EOF

%%

";"				{if (flag_1) fprintf(yyout, "SEMICOLON\n");  col+=yyleng;error_line=0; return SEMICOLON;  }
","				{if (flag_1) fprintf(yyout, "COMMA\n");  col+=yyleng; error_line=0; return COMMA;}
"_"				{if (flag_1) fprintf(yyout, "BLANKID\n"); col+=yyleng;error_line=0; return BLANKID;}
"="				{if (flag_1) fprintf(yyout, "ASSIGN\n");  col+=yyleng;error_line=0; return ASSIGN;}
"*"				{if (flag_1) fprintf(yyout, "STAR\n");  col+=yyleng;error_line=0; return STAR;}
"/"				{if (flag_1) fprintf(yyout, "DIV\n");  col+=yyleng;error_line=0; return DIV; }
"-"				{if (flag_1) fprintf(yyout, "MINUS\n");  col+=yyleng;error_line=0; return MINUS; }
"+"				{if (flag_1) fprintf(yyout, "PLUS\n");  col+=yyleng; error_line=0;return PLUS; }
"=="			{if (flag_1) fprintf(yyout, "EQ\n");  col+=yyleng; error_line=0;return EQ;}
">="			{if (flag_1) fprintf(yyout, "GE\n");  col+=yyleng;error_line=0; return GE;}
">"				{if (flag_1) fprintf(yyout, "GT\n");  col+=yyleng; error_line=0;return GT;}
"{"				{if (flag_1) fprintf(yyout, "LBRACE\n");  col+=yyleng;error_line=0; return LBRACE;}
"<="			{if (flag_1) fprintf(yyout, "LE\n");  col+=yyleng;error_line=0; return LE; }
"("				{if (flag_1) fprintf(yyout, "LPAR\n");  col+=yyleng;error_line=0; return LPAR; }
"["				{if (flag_1) fprintf(yyout, "LSQ\n");  col+=yyleng;error_line=0; return LSQ;}	
"<"				{if (flag_1) fprintf(yyout, "LT\n");  col+=yyleng; error_line=0;return LT;}
"%"				{if (flag_1) fprintf(yyout, "MOD\n");  col+=yyleng; error_line=0;return MOD;}
"!="			{if (flag_1) fprintf(yyout, "NE\n");  col+=yyleng; error_line=0;return NE;}
"!"				{if (flag_1) fprintf(yyout, "NOT\n");  col+=yyleng; error_line=0;return NOT;}
"&&"			{if (flag_1) fprintf(yyout, "AND\n");  col+=yyleng; error_line=0;return AND;}
"||"			{if (flag_1) fprintf(yyout, "OR\n");  col+=yyleng; error_line=0;return OR;}
"}"				{if (flag_1) fprintf(yyout, "RBRACE\n");  col+=yyleng;error_line=0;  BEGIN _SEMICOLON; return RBRACE;}
")"				{if (flag_1) fprintf(yyout, "RPAR\n");  col+=yyleng; error_line=0; BEGIN _SEMICOLON; return RPAR;}
"]"				{if (flag_1) fprintf(yyout, "RSQ\n");  col+=yyleng;error_line=0; BEGIN _SEMICOLON; return RSQ;}
"package"		{if (flag_1) fprintf(yyout, "PACKAGE\n");  col+=yyleng; error_line=0; return PACKAGE;}
"return"		{if (flag_1) fprintf(yyout, "RETURN\n");  col+=yyleng;error_line=0; BEGIN _SEMICOLON; return RETURN;}
"else"			{if (flag_1) fprintf(yyout, "ELSE\n");  col+=yyleng; error_line=0;return ELSE; }
"for"			{if (flag_1) fprintf(yyout, "FOR\n");  col+=yyleng; error_line=0;return FOR; }
"if"			{if (flag_1) fprintf(yyout, "IF\n");  col+=yyleng;error_line=0; return IF; }
"var"			{if (flag_1) fprintf(yyout, "VAR\n");  col+=yyleng;error_line=0; return VAR; }
"int"			{if (flag_1) fprintf(yyout, "INT\n");  col+=yyleng;error_line=0; return INT;}
"float32"		{if (flag_1) fprintf(yyout, "FLOAT32\n");  col+=yyleng; error_line=0;return FLOAT32;}
"bool"			{if (flag_1) fprintf(yyout, "BOOL\n");  col+=yyleng; error_line=0;return BOOL; }
"string"		{if (flag_1) fprintf(yyout, "STRING\n");  col+=yyleng;error_line=0; return STRING; }
"fmt.Println"	{if (flag_1) fprintf(yyout, "PRINT\n");  col+=yyleng;error_line=0; return PRINT; }
"strconv.Atoi"	{if (flag_1) fprintf(yyout, "PARSEINT\n");  col+=yyleng;error_line=0; return PARSEINT; }
"func"			{if (flag_1) fprintf(yyout, "FUNC\n");  col+=yyleng; error_line=0;return FUNC; }
"os.Args"		{if (flag_1) fprintf(yyout, "CMDARGS\n");  col+=yyleng; error_line=0;return CMDARGS;}
"/*"			{; comment_lines = 0; comment_cols = 0; swaped_line = 0;error_line=0; BEGIN BCOMMENT;}
"//"			{; error_line=0;BEGIN LCOMMENT;}

"++"|"--"		{if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext);  yylval.id = create_token(strdup(yytext), line, col); col+=yyleng; error_line=0; return RESERVED; }
"break"         {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext);  yylval.id = create_token(strdup(yytext), line, col); col+=yyleng;error_line=0; return RESERVED; }
"case"          {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext);  yylval.id = create_token(strdup(yytext), line, col); col+=yyleng; error_line=0;return RESERVED; }
"chan"          {if  (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext);  yylval.id = create_token(strdup(yytext), line, col); col+=yyleng; error_line=0;return RESERVED; }
"const"         {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); yylval.id = create_token(strdup(yytext), line, col); col+=yyleng;error_line=0; return RESERVED; }
"continue"      {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext);  yylval.id = create_token(strdup(yytext), line, col); col+=yyleng; error_line=0;return RESERVED; }
"default"       {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); yylval.id = create_token(strdup(yytext), line, col); col+=yyleng; error_line=0;return RESERVED; }
"defer"         {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); yylval.id = create_token(strdup(yytext), line, col); col+=yyleng; error_line=0;return RESERVED; }
"fallthrough"   {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); yylval.id = create_token(strdup(yytext), line, col); col+=yyleng; error_line=0;return RESERVED; }
"go"            {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); yylval.id = create_token(strdup(yytext), line, col); col+=yyleng;error_line=0; return RESERVED;}
"import"        {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); yylval.id = create_token(strdup(yytext), line, col); col+=yyleng; error_line=0;return RESERVED;}
"goto"          {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); yylval.id = create_token(strdup(yytext), line, col); col+=yyleng; error_line=0;return RESERVED;}
"interface"     {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); yylval.id = create_token(strdup(yytext), line, col); col+=yyleng; error_line=0;return RESERVED;}
"map"           {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); yylval.id = create_token(strdup(yytext), line, col); col+=yyleng;error_line=0; return RESERVED;}
"range"         {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); yylval.id = create_token(strdup(yytext), line, col); col+=yyleng;error_line=0; return RESERVED;}
"select"        {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); yylval.id = create_token(strdup(yytext), line, col); col+=yyleng; error_line=0;return RESERVED;}
"struct"        {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); yylval.id = create_token(strdup(yytext), line, col); col+=yyleng; error_line=0;return RESERVED;}
"switch"        {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); yylval.id = create_token(strdup(yytext), line, col); col+=yyleng;error_line=0; return RESERVED;}
"type"          {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); yylval.id = create_token(strdup(yytext), line, col); col+=yyleng; error_line=0;return RESERVED;}

0                               {error_line=0;yyless(0); BEGIN OCTAL;}
[1-9]{number}*|{hexa}          	{if (flag_1) fprintf(yyout, "INTLIT(%s)\n", yytext);  yylval.id = create_token(strdup(yytext), line, col); col+=yyleng; error_line=0;  BEGIN _SEMICOLON; return INTLIT; }
({float}{exp}?|{number}+{exp})  {if (flag_1) fprintf(yyout, "REALLIT(%s)\n", yytext); yylval.id = create_token(strdup(yytext), line, col); col+=yyleng; error_line=0; BEGIN _SEMICOLON; return REALLIT;}
{letter}[a-zA-Z0-9_]*			{if (flag_1) fprintf(yyout, "ID(%s)\n", yytext); yylval.id = create_token(strdup(yytext), line, col);  col+=yyleng; error_line=0; BEGIN _SEMICOLON; return ID;}
\"                              {error_line=0;yyless(0); BEGIN _STRING;}

{newline}				    			  {line++; col = 0; error_line=1;}
{whispace}                                {col+=yyleng;error_line=0;}
.				        				  {col+=1; fprintf(yyout, "Line %d, column %d: illegal character (%s)\n", line, col, yytext);col+=yyleng-1;error_line=0;}

<BCOMMENT>"*/" 	        				  {line+=comment_lines; if (swaped_line) { col = 0; error_line=1;} else ;  col+=comment_cols+yyleng; BEGIN 0;}
<BCOMMENT>{newline}        	              {comment_lines++; comment_cols = 0; swaped_line = 1;error_line=0;}
<BCOMMENT><<EOF>>                         {fprintf(yyout, "Line %d, column %d: unterminated comment\n", line, col);error_line=0; BEGIN CHECK_EOF;}
<BCOMMENT>.     	        	          {comment_cols+=yyleng;error_line=0;}

<LCOMMENT>{newline} 	        		  { line++; col = 0; error_line=0; BEGIN 0;}
<LCOMMENT>. 	        				  {;}

<_SEMICOLON>{newline}                      {if (flag_1) fprintf(yyout, "SEMICOLON\n"); line++; col = 0; error_line=1; BEGIN 0;  return SEMICOLON;    }
<_SEMICOLON>{whispace}	                   {col+=yyleng;error_line=0;}
<_SEMICOLON>"/*"                           {; comment_lines = 0; comment_cols = 0; swaped_line = 0; error_line=0; BEGIN SEMIBCOMMENT;}
<_SEMICOLON>"//"                           {error_line=0; BEGIN SEMILCOMMENT;}
<_SEMICOLON>.                              {error_line=0; yyless(0); BEGIN 0;}
<_SEMICOLON><<EOF>>                        {if (flag_1) fprintf(yyout, "SEMICOLON\n");  error_line=0; BEGIN CHECK_EOF; return SEMICOLON;}

<SEMIBCOMMENT>"*/" 	        			  {line+=comment_lines; if (swaped_line) {col = 0; error_line=1;} else ; col+=comment_cols+yyleng; if (swaped_line) {fprintf(yyout, "SEMICOLON\n"); BEGIN 0; return SEMICOLON; } else BEGIN _SEMICOLON;}
<SEMIBCOMMENT>{newline}        	          {comment_lines++; comment_cols = 0; swaped_line=1; error_line=0;}
<SEMIBCOMMENT><<EOF>>                     {if (flag_1) fprintf(yyout, "SEMICOLON\n"); fprintf(yyout, "Line %d, column %d: unterminated comment\n", line, col); error_line=0; BEGIN CHECK_EOF; return SEMICOLON;}
<SEMIBCOMMENT>.     	        	      {comment_cols+=yyleng;error_line=0;}

<SEMILCOMMENT>{newline} 	              {yyless(0); BEGIN _SEMICOLON;}
<SEMILCOMMENT><<EOF>>                     {if (flag_1) fprintf(yyout, "SEMICOLON\n"); error_line=0; BEGIN CHECK_EOF; return SEMICOLON;}
<SEMILCOMMENT>. 	        		      {error_line=0;}

<_STRING>{newline}                                   {fprintf(yyout, "Line %d, column %d: unterminated string literal\n", line, col);  error_line=1;  col = 0; BEGIN 0;}
<_STRING>\"(\\(\"|n|r|t|f|\\)|[^\\\"\r\n])*\"        {if (flag_1) fprintf(yyout, "STRLIT(%s)\n", yytext); yylval.id = create_token(strdup(yytext), line, col);  col+=yyleng; error_line=0;  BEGIN _SEMICOLON; return STRLIT;}
<_STRING>\"[^\"\r\n]*\\[^\\\"nrtf][^\"\r\n]*\"       {yyless(0); BEGIN DUMMY_QUOTE;}
<_STRING>\"[^\"\r\n]*{newline}?                      {yyless(0); BEGIN DUMMY_QUOTE;}

<DUMMY_QUOTE>\"                            {comment_cols = 1; BEGIN INVALID_STRING;}

<INVALID_STRING>\"                         { col+=comment_cols; BEGIN 0;}
<INVALID_STRING>{newline}                  {fprintf(yyout, "Line %d, column %d: unterminated string literal\n", line, col);  line++; error_line=1;  col = 0; BEGIN 0;}
<INVALID_STRING>\\(\"|n|r|t|f|\\)          {comment_cols+=yyleng; error_line=0;}
<INVALID_STRING>\\[^\\\"nrtf\r\n]          {fprintf(yyout, "Line %d, column %d: invalid escape sequence (%s)\n", line, col+comment_cols, yytext); comment_cols+=yyleng; error_line=0;}
<INVALID_STRING>\\                         {fprintf(yyout, "Line %d, column %d: invalid escape sequence (%s)\n", line, col+comment_cols, yytext); comment_cols+=yyleng; error_line=0;}
<INVALID_STRING><<EOF>>                    {fprintf(yyout, "Line %d, column %d: unterminated string literal\n", line, col); error_line=0; BEGIN CHECK_EOF;}
<INVALID_STRING>.                          {comment_cols+=yyleng; error_line=0;}

<OCTAL>0[0-7]*                             {if (flag_1) fprintf(yyout, "INTLIT(%s)\n", yytext);  yylval.id = create_token(strdup(yytext), line, col);  col+=yyleng; error_line=0; BEGIN _SEMICOLON; return INTLIT;}
<OCTAL>[0-9]+                              {; fprintf(yyout, "Line %d, column %d: invalid octal constant (%s)\n", line, col, yytext);  col+=yyleng-1; error_line=0; BEGIN _SEMICOLON; BEGIN 0;}
<OCTAL>.                                   {yyless(0); error_line=0; BEGIN 0;}

<CHECK_EOF><<EOF>>                         {yyterminate();}
<CHECK_EOF>.                               {BEGIN 0;}

%%

int main(int argc, char *argv[]){
    if(argv[1] != 0 && !strcmp(argv[1], "-l"))
        flag_1 = 1;
    
    if(argv[1] != 0 && !strcmp(argv[1], "-t"))
        flag_2 = 1;

    if(argv[1] != 0 && !strcmp(argv[1], "-s"))
        flag_3 = 1;
    

    if (!flag_1){
        yyparse();
        check_program(program);
        print_symbol_tables(program);
    }
    else
        while(yylex());

    return 0;
}

void yyerror (char *s) {
    //TODO alterar para usar a linha e coluna do token
    //fprintf(yyout, "Line %d, column %d: %s: %s\n", , s, yytext );
    fprintf(yyout, "dsajkndasdaoisdoiasd");
}

int yywrap(){
    return 1;
}

