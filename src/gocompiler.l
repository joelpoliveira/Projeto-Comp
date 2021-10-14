%{
    // Tomás Mendes - 2019232272
    // Joel Oliveira - 2019227468
    int line = 1;
    int comment_lines = 1; 
    int col = 0;
    int comment_cols = 0;

    int flag;
%}

letter  [a-zA-Z_]
number  [0-9]
exp     (e|E)[-+]?{number}+
float   ({number}+"."{number}*)|({number}*"."{number}+)
hexa	(0(x|X)[0-9a-fA-F]+)
newline \n|\r\n|\r|\f
sp_chr  \\(\"|n|r|t|f|\\)
whispace (" "|\t)

%X SEMICOLON BCOMMENT LCOMMENT STRING DUMMY_QUOTE ILLEGAL_SCP OCTAL

%%

";"				{if (flag) fprintf(yyout, "SEMICOLON\n"); col+=yyleng;}
","				{if (flag) fprintf(yyout, "COMMA\n"); col+=yyleng;}
"_"				{if (flag) fprintf(yyout, "BLANKID\n"); col+=yyleng;}
"="				{if (flag) fprintf(yyout, "ASSIGN\n"); col+=yyleng;}
"*"				{if (flag) fprintf(yyout, "STAR\n"); col+=yyleng;}
"/"				{if (flag) fprintf(yyout, "DIV\n"); col+=yyleng;}
"-"				{if (flag) fprintf(yyout, "MINUS\n"); col+=yyleng;}
"+"				{if (flag) fprintf(yyout, "PLUS\n"); col+=yyleng;}
"=="			{if (flag) fprintf(yyout, "EQ\n"); col+=yyleng;}
">="			{if (flag) fprintf(yyout, "GE\n"); col+=yyleng;}
">"				{if (flag) fprintf(yyout, "GT\n"); col+=yyleng;}
"{"				{if (flag) fprintf(yyout, "LBRACE\n"); col+=yyleng;}
"<="			{if (flag) fprintf(yyout, "LE\n"); col+=yyleng;}
"("				{if (flag) fprintf(yyout, "LPAR\n"); col+=yyleng;}
"["				{if (flag) fprintf(yyout, "LSQ\n"); col+=yyleng;}	
"<"				{if (flag) fprintf(yyout, "LT\n"); col+=yyleng;}
"%"				{if (flag) fprintf(yyout, "MOD\n"); col+=yyleng;}
"!="			{if (flag) fprintf(yyout, "NE\n"); col+=yyleng;}
"!"				{if (flag) fprintf(yyout, "NOT\n"); col+=yyleng;}
"&&"			{if (flag) fprintf(yyout, "AND\n"); col+=yyleng;}
"||"			{if (flag) fprintf(yyout, "OR\n"); col+=yyleng;}
"}"				{if (flag) fprintf(yyout, "RBRACE\n"); col+=yyleng; BEGIN SEMICOLON;}
")"				{if (flag) fprintf(yyout, "RPAR\n"); col+=yyleng; BEGIN SEMICOLON;}
"]"				{if (flag) fprintf(yyout, "RSQ\n"); col+=yyleng; BEGIN SEMICOLON;}
"package"		{if (flag) fprintf(yyout, "PACKAGE\n"); col+=yyleng;}
"return"		{if (flag) fprintf(yyout, "RETURN\n"); col+=yyleng; BEGIN SEMICOLON;}
"else"			{if (flag) fprintf(yyout, "ELSE\n"); col+=yyleng;}
"for"			{if (flag) fprintf(yyout, "FOR\n"); col+=yyleng;}
"if"			{if (flag) fprintf(yyout, "IF\n"); col+=yyleng;}
"var"			{if (flag) fprintf(yyout, "VAR\n"); col+=yyleng;}
"int"			{if (flag) fprintf(yyout, "INT\n"); col+=yyleng;}
"float32"		{if (flag) fprintf(yyout, "FLOAT32\n"); col+=yyleng;}
"bool"			{if (flag) fprintf(yyout, "BOOL\n"); col+=yyleng;}
"string"		{if (flag) fprintf(yyout, "STRING\n"); col+=yyleng;}
"fmt.Println"	{if (flag) fprintf(yyout, "PRINT\n"); col+=yyleng;}
"strconv.Atoi"	{if (flag) fprintf(yyout, "PARSEINT\n"); col+=yyleng;}
"func"			{if (flag) fprintf(yyout, "FUNC\n"); col+=yyleng;}
"os.Args"		{if (flag) fprintf(yyout, "CMDARGS\n"); col+=yyleng;}
"/*"			{col+=yyleng-1; comment_lines = 0; BEGIN BCOMMENT;}
"//"			{col+=yyleng-1; BEGIN LCOMMENT;}
" "             {col+=yyleng;}

"++"|"--"		{if (flag) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"break"         {if (flag) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"case"          {if (flag) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"chan"          {if (flag) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"const"         {if (flag) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"continue"      {if (flag) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"default"       {if (flag) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"defer"         {if (flag) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"fallthrough"   {if (flag) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"go"            {if (flag) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"import"        {if (flag) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"goto"          {if (flag) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"interface"     {if (flag) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"map"           {if (flag) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"range"         {if (flag) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"select"        {if (flag) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"struct"        {if (flag) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"switch"        {if (flag) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"type"          {if (flag) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}

0                               {yyless(0); BEGIN OCTAL;}
[1-9]{number}*|{hexa}          	{if (flag) fprintf(yyout, "INTLIT(%s)\n", yytext); col+=yyleng; BEGIN SEMICOLON;}
({float}{exp}?|{number}+{exp})  {if (flag) fprintf(yyout, "REALLIT(%s)\n", yytext); col+=yyleng; BEGIN SEMICOLON;}
{letter}[a-zA-Z0-9_]*			{if (flag) fprintf(yyout, "ID(%s)\n", yytext); col+=yyleng; BEGIN SEMICOLON;}
\"                              {yyless(0); BEGIN STRING;}

{newline}				    			  {line++; col = 0;}
{whispace}                                {col+=yyleng;}
.				        				  {col+=yyleng; fprintf(yyout, "Line %d, column %d: illegal character (%s)\n", line, col, yytext);}

<BCOMMENT>"*/" 	        				  {line+=comment_lines; col+=comment_cols+1; BEGIN 0;}
<BCOMMENT>{newline}        	              {comment_lines++; comment_cols = 0;}
<BCOMMENT><<EOF>>                         {fprintf(yyout, "Line %d, column %d: unterminated comment\n", line, col); yyterminate();}
<BCOMMENT>.     	        	          {comment_cols+=yyleng;}

<LCOMMENT>{newline} 	        		  {line++; col = 0; BEGIN 0;}
<LCOMMENT>. 	        				  {;}

<SEMICOLON>{newline}                      {if (flag) fprintf(yyout, "SEMICOLON\n"); line++; col = 0; BEGIN 0;}
<SEMICOLON>"/*"|"//"                      {yyless(0); if (flag) fprintf(yyout, "SEMICOLON\n"); col = 0; BEGIN 0;}
<SEMICOLON>{whispace}	                  {col+=yyleng;}
<SEMICOLON>.                              {yyless(0); BEGIN 0;}
<SEMICOLON><<EOF>>                        {if (flag) fprintf(yyout, "SEMICOLON\n"); yyterminate();}


<STRING>{newline}                                          {fprintf(yyout, "Line %d, column %d: unterminated string literal\n", line, col); line++; col = 0; BEGIN 0;}
<STRING>\"(\\(\"|n|r|t|f|\\)|[^\\\"\t\f\r\n])*\"           {if (flag) fprintf(yyout, "STRLIT(%s)\n", yytext); col+=yyleng; BEGIN SEMICOLON;}
<STRING>\".*\\[^\\\"nrtf ].*(\"|{newline})?                {yyless(0); BEGIN DUMMY_QUOTE;}
<STRING>\".*\\                                             {yyless(0); BEGIN DUMMY_QUOTE;}
<STRING><<EOF>>                                            {fprintf(yyout, "Line %d, column %d: unterminated string literal\n", line, col); yyterminate();}
<STRING>.                                                  {col+=yyleng;}

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

<OCTAL>0[0-7]*                             {if (flag) fprintf(yyout, "INTLIT(%s)\n", yytext); col+=yyleng; BEGIN 0;}
<OCTAL>[0-9]+                              {col++; fprintf(yyout, "Line %d, column %d: invalid octal constant (%s)\n", line, col, yytext); col+=yyleng-1; BEGIN 0;}
<OCTAL>.                                   {yyless(0); BEGIN 0;}

%%

int main(int argc, char *argv[]){
    if (argv[1] != 0)
        flag = 1;

    yylex();
    return 0;
}

int yywrap(){
    return 1;
}
