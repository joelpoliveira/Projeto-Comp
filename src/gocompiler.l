%{
    // Tomás Mendes - 2019232272
    // Joel Oliveira - 2019227468

    /*
        REVER   :: ids_keywords             ( C ) 20/20
                :: literals                 ( G ) 25/30
                :: unterminated_comments    ( I ) 10/10
                :: str_bad                  ( J ) 0/20
                :: semicolons               ( K ) 25/25
    */
    int line = 1;
    int comment_lines = 1; 
    int col = 0;
    int comment_cols = 0;

    int flag_1, swaped_line;
%}

letter  [a-zA-Z_]
number  [0-9]
exp     (e|E)[-+]?{number}+
float   ({number}+"."{number}*)|({number}*"."{number}+)
hexa	(0(x|X)[0-9a-fA-F]+)
newline \n|\r\n|\r|\f
sp_chr  \\(\"|n|r|t|f|\\)
whispace (" "|\t)

%X SEMICOLON BCOMMENT LCOMMENT STRING DUMMY_QUOTE ILLEGAL_SCP OCTAL SEMIBCOMMENT SEMILCOMMENT

%%

";"				{if (flag_1) fprintf(yyout, "SEMICOLON\n"); col+=yyleng;}
","				{if (flag_1) fprintf(yyout, "COMMA\n"); col+=yyleng;}
"_"				{if (flag_1) fprintf(yyout, "BLANKID\n"); col+=yyleng;}
"="				{if (flag_1) fprintf(yyout, "ASSIGN\n"); col+=yyleng;}
"*"				{if (flag_1) fprintf(yyout, "STAR\n"); col+=yyleng;}
"/"				{if (flag_1) fprintf(yyout, "DIV\n"); col+=yyleng;}
"-"				{if (flag_1) fprintf(yyout, "MINUS\n"); col+=yyleng;}
"+"				{if (flag_1) fprintf(yyout, "PLUS\n"); col+=yyleng;}
"=="			{if (flag_1) fprintf(yyout, "EQ\n"); col+=yyleng;}
">="			{if (flag_1) fprintf(yyout, "GE\n"); col+=yyleng;}
">"				{if (flag_1) fprintf(yyout, "GT\n"); col+=yyleng;}
"{"				{if (flag_1) fprintf(yyout, "LBRACE\n"); col+=yyleng;}
"<="			{if (flag_1) fprintf(yyout, "LE\n"); col+=yyleng;}
"("				{if (flag_1) fprintf(yyout, "LPAR\n"); col+=yyleng;}
"["				{if (flag_1) fprintf(yyout, "LSQ\n"); col+=yyleng;}	
"<"				{if (flag_1) fprintf(yyout, "LT\n"); col+=yyleng;}
"%"				{if (flag_1) fprintf(yyout, "MOD\n"); col+=yyleng;}
"!="			{if (flag_1) fprintf(yyout, "NE\n"); col+=yyleng;}
"!"				{if (flag_1) fprintf(yyout, "NOT\n"); col+=yyleng;}
"&&"			{if (flag_1) fprintf(yyout, "AND\n"); col+=yyleng;}
"||"			{if (flag_1) fprintf(yyout, "OR\n"); col+=yyleng;}
"}"				{if (flag_1) fprintf(yyout, "RBRACE\n"); col+=yyleng; BEGIN SEMICOLON;}
")"				{if (flag_1) fprintf(yyout, "RPAR\n"); col+=yyleng; BEGIN SEMICOLON;}
"]"				{if (flag_1) fprintf(yyout, "RSQ\n"); col+=yyleng; BEGIN SEMICOLON;}
"package"		{if (flag_1) fprintf(yyout, "PACKAGE\n"); col+=yyleng;}
"return"		{if (flag_1) fprintf(yyout, "RETURN\n"); col+=yyleng; BEGIN SEMICOLON;}
"else"			{if (flag_1) fprintf(yyout, "ELSE\n"); col+=yyleng;}
"for"			{if (flag_1) fprintf(yyout, "FOR\n"); col+=yyleng;}
"if"			{if (flag_1) fprintf(yyout, "IF\n"); col+=yyleng;}
"var"			{if (flag_1) fprintf(yyout, "VAR\n"); col+=yyleng;}
"int"			{if (flag_1) fprintf(yyout, "INT\n"); col+=yyleng;}
"float32"		{if (flag_1) fprintf(yyout, "FLOAT32\n"); col+=yyleng;}
"bool"			{if (flag_1) fprintf(yyout, "BOOL\n"); col+=yyleng;}
"string"		{if (flag_1) fprintf(yyout, "STRING\n"); col+=yyleng;}
"fmt.Println"	{if (flag_1) fprintf(yyout, "PRINT\n"); col+=yyleng;}
"strconv.Atoi"	{if (flag_1) fprintf(yyout, "PARSEINT\n"); col+=yyleng;}
"func"			{if (flag_1) fprintf(yyout, "FUNC\n"); col+=yyleng;}
"os.Args"		{if (flag_1) fprintf(yyout, "CMDARGS\n"); col+=yyleng;}
"/*"			{col++; comment_lines = 0; comment_cols = 0; swaped_line = 0; BEGIN BCOMMENT;}
"//"			{col++; BEGIN LCOMMENT;}

"++"|"--"		{if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"break"         {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"case"          {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"chan"          {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"const"         {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"continue"      {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"default"       {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"defer"         {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"fallthrough"   {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"go"            {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"import"        {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"goto"          {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"interface"     {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"map"           {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"range"         {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"select"        {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"struct"        {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"switch"        {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"type"          {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}

0                               {yyless(0); BEGIN OCTAL;}
[1-9]{number}*|{hexa}          	{if (flag_1) fprintf(yyout, "INTLIT(%s)\n", yytext); col+=yyleng; BEGIN SEMICOLON;}
({float}{exp}?|{number}+{exp})  {if (flag_1) fprintf(yyout, "REALLIT(%s)\n", yytext); col+=yyleng; BEGIN SEMICOLON;}
{letter}[a-zA-Z0-9_]*			{if (flag_1) fprintf(yyout, "ID(%s)\n", yytext); col+=yyleng; BEGIN SEMICOLON;}
\"                              {yyless(0); BEGIN STRING;}

{newline}				    			  {line++; col = 0;}
{whispace}                                {col+=yyleng;}
.				        				  {col+=1; fprintf(yyout, "Line %d, column %d: illegal character (%s)\n", line, col, yytext);col+=yyleng-1;}

<BCOMMENT>"*/" 	        				  {line+=comment_lines; if (swaped_line) col = 0; else col++; col+=comment_cols+yyleng; BEGIN 0;}
<BCOMMENT>{newline}        	              {comment_lines++; comment_cols = 0; swaped_line = 1;}
<BCOMMENT><<EOF>>                         {fprintf(yyout, "Line %d, column %d: unterminated comment\n", line, col); yyterminate();}
<BCOMMENT>.     	        	          {comment_cols+=yyleng;}

<LCOMMENT>{newline} 	        		  {line++; col = 0; BEGIN 0;}
<LCOMMENT>. 	        				  {;}

<SEMICOLON>{newline}                      {if (flag_1) fprintf(yyout, "SEMICOLON\n"); line++; col = 0; BEGIN 0;}
<SEMICOLON>{whispace}	                  {col+=yyleng;}
<SEMICOLON>"/*"                           {col++; comment_lines = 0; comment_cols = 0; swaped_line = 0; BEGIN SEMIBCOMMENT;}
<SEMICOLON>"//"                           {BEGIN SEMILCOMMENT;}
<SEMICOLON>.                              {yyless(0); BEGIN 0;}
<SEMICOLON><<EOF>>                        {if (flag_1) fprintf(yyout, "SEMICOLON\n"); yyterminate();}

<SEMIBCOMMENT>"*/" 	        				  {line+=comment_lines;if (swaped_line) col = 0; else col++; col+=comment_cols+yyleng; BEGIN SEMICOLON;}
<SEMIBCOMMENT>{newline}        	              {comment_lines++; comment_cols = 0; swaped_line=1; }
<SEMIBCOMMENT><<EOF>>                         {fprintf(yyout, "SEMICOLON\n"); fprintf(yyout, "Line %d, column %d: unterminated comment\n", line, col); yyterminate();}
<SEMIBCOMMENT>.     	        	          {comment_cols+=yyleng;}

<SEMILCOMMENT>{newline} 	        		  {yyless(0); BEGIN SEMICOLON;}
<SEMILCOMMENT>. 	        				  {;}

<STRING>{newline}                                           {fprintf(yyout, "Line %d, column %d: unterminated string literal\n", line, col); line++; col = 0; BEGIN 0;}
<STRING>\"(\\(\"|n|r|t|f|\\)|[^\\\"\t\f\r\n])*\"            {if (flag_1) fprintf(yyout, "STRLIT(%s)\n", yytext); col+=yyleng; BEGIN SEMICOLON;}
<STRING>\"[^\"\r\n]*\\[^\\\"nrtf ][^\"\r\n]*(\"|{newline})? {yyless(0); BEGIN DUMMY_QUOTE;}
<STRING>\"[^\"\r\n]*\\                                      {yyless(0); BEGIN DUMMY_QUOTE;}
<STRING><<EOF>>                                             {fprintf(yyout, "Line %d, column %d: unterminated string literal\n", line, col); yyterminate();}
<STRING>\"                                                  {col+=yyleng;}
<STRING>.                                                   {;}

<DUMMY_QUOTE>\"                            {col++; comment_cols = 1; BEGIN ILLEGAL_SCP;}
<ILLEGAL_SCP>\"                            {col+=comment_cols; BEGIN 0;}
<ILLEGAL_SCP>{newline}                     {fprintf(yyout, "Line %d, column %d: unterminated string literal\n", line, col); line++; col = 0; BEGIN 0;}
<ILLEGAL_SCP>\\(\"|n|r|t|f|\\)             {comment_cols+=yyleng;}
<ILLEGAL_SCP>\\[^\\\"nrtf\r\n ]            {fprintf(yyout, "Line %d, column %d: invalid escape sequence (%s)\n", line, col+comment_cols, yytext); comment_cols+=yyleng;}
<ILLEGAL_SCP>\\{newline}                   {fprintf(yyout, "Line %d, column %d: invalid escape sequence (%c)\n"
                                                           "Line %d, column %d: unterminated string literal\n", line, col+comment_cols, yytext[0], line, col); line++; col=0; BEGIN 0;}
<ILLEGAL_SCP>\\                            {fprintf(yyout, "Line %d, column %d: invalid escape sequence (%s)\n", line, col+comment_cols, yytext); comment_cols+=yyleng;}
<ILLEGAL_SCP>\\" "                         {fprintf(yyout, "Line %d, column %d: invalid escape sequence (%s)\n", line, col+comment_cols, yytext); comment_cols+=yyleng;}
<ILLEGAL_SCP><<EOF>>                       {fprintf(yyout, "Line %d, column %d: unterminated string literal\n", line, col); yyterminate();}
<ILLEGAL_SCP>.                             {comment_cols+=yyleng;}

<OCTAL>0[0-7]*                             {if (flag_1) fprintf(yyout, "INTLIT(%s)\n", yytext); col+=yyleng; BEGIN SEMICOLON;}
<OCTAL>[0-9]+                              {col++; fprintf(yyout, "Line %d, column %d: invalid octal constant (%s)\n", line, col, yytext); col+=yyleng-1; BEGIN 0;}
<OCTAL>.                                   {yyless(0); BEGIN 0;}

%%

int main(int argc, char *argv[]){
    if (argv[1] != 0 && (argv[1][0] == '-' && argv[1][1] == 'l')){
        //checkig for argument -l like this so we dont have to import string.h
        flag_1 = 1;
    }   

    yylex();
    return 0;
}

int yywrap(){
    return 1;
}
