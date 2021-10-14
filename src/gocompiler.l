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
newline \n|\r\n|\r
sp_chr  \\(\"|n|r|t|f|\\)

%X SEMICOLON BCOMMENT LCOMMENT STRING DUMMY_QUOTE ILLEGAL_SCP OCTAL

%%

";"				{fprintf(yyout, "SEMICOLON\n"); col+=yyleng;}
","				{fprintf(yyout, "COMMA\n"); col+=yyleng;}
"_"				{fprintf(yyout, "BLANKID\n"); col+=yyleng;}
"="				{fprintf(yyout, "ASSIGN\n"); col+=yyleng;}
"*"				{fprintf(yyout, "STAR\n"); col+=yyleng;}
"/"				{fprintf(yyout, "DIV\n"); col+=yyleng;}
"-"				{fprintf(yyout, "MINUS\n"); col+=yyleng;}
"+"				{fprintf(yyout, "PLUS\n"); col+=yyleng;}
"=="			{fprintf(yyout, "EQ\n"); col+=yyleng;}
">="			{fprintf(yyout, "GE\n"); col+=yyleng;}
">"				{fprintf(yyout, "GT\n"); col+=yyleng;}
"{"				{fprintf(yyout, "LBRACE\n"); col+=yyleng;}
"<="			{fprintf(yyout, "LE\n"); col+=yyleng;}
"("				{fprintf(yyout, "LPAR\n"); col+=yyleng;}
"["				{fprintf(yyout, "LSQ\n"); col+=yyleng;}	
"<"				{fprintf(yyout, "LT\n"); col+=yyleng;}
"%"				{fprintf(yyout, "MOD\n"); col+=yyleng;}
"!="			{fprintf(yyout, "NE\n"); col+=yyleng;}
"!"				{fprintf(yyout, "NOT\n"); col+=yyleng;}
"&&"			{fprintf(yyout, "AND\n"); col+=yyleng;}
"||"			{fprintf(yyout, "OR\n"); col+=yyleng;}
"}"				{fprintf(yyout, "RBRACE\n"); col+=yyleng; BEGIN SEMICOLON;}
")"				{fprintf(yyout, "RPAR\n"); col+=yyleng; BEGIN SEMICOLON;}
"]"				{fprintf(yyout, "RSQ\n"); col+=yyleng; BEGIN SEMICOLON;}
"package"		{fprintf(yyout, "PACKAGE\n"); col+=yyleng;}
"return"		{fprintf(yyout, "RETURN\n"); col+=yyleng; BEGIN SEMICOLON;}
"else"			{fprintf(yyout, "ELSE\n"); col+=yyleng;}
"for"			{fprintf(yyout, "FOR\n"); col+=yyleng;}
"if"			{fprintf(yyout, "IF\n"); col+=yyleng;}
"var"			{fprintf(yyout, "VAR\n"); col+=yyleng;}
"int"			{fprintf(yyout, "INT\n"); col+=yyleng;}
"float32"		{fprintf(yyout, "FLOAT32\n"); col+=yyleng;}
"bool"			{fprintf(yyout, "BOOL\n"); col+=yyleng;}
"string"		{fprintf(yyout, "STRING\n"); col+=yyleng;}
"fmt.Println"	{fprintf(yyout, "PRINT\n"); col+=yyleng;}
"strconv.Atoi"	{fprintf(yyout, "PARSEINT\n"); col+=yyleng;}
"func"			{fprintf(yyout, "FUNC\n"); col+=yyleng;}
"os.Args"		{fprintf(yyout, "CMDARGS\n"); col+=yyleng;}
"/*"			{col+=yyleng-1; comment_lines = 0; BEGIN BCOMMENT;}
"//"			{col+=yyleng-1; BEGIN LCOMMENT;}
" "             {col+=yyleng;}

"++"|"--"		{fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"break"         {fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"case"          {fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"chan"          {fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"const"         {fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"continue"      {fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"default"       {fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"defer"         {fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"fallthrough"   {fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"go"            {fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"import"        {fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"goto"          {fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"interface"     {fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"map"           {fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"range"         {fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"select"        {fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"struct"        {fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"switch"        {fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}
"type"          {fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng;}

0                               {yyless(0); BEGIN OCTAL;}
[1-9]{number}*|{hexa}          	{fprintf(yyout, "INTLIT(%s)\n", yytext); col+=yyleng; BEGIN SEMICOLON;}
({float}{exp}?|{number}+{exp})  {fprintf(yyout, "REALLIT(%s)\n", yytext); col+=yyleng; BEGIN SEMICOLON;}
{letter}[a-zA-Z0-9_]*			{fprintf(yyout, "ID(%s)\n", yytext); col+=yyleng; BEGIN SEMICOLON;}
\"                              {yyless(0); BEGIN STRING;}

{newline}				    			  {line++; col = 0;}
.				        				  {col+=yyleng; if (flag) fprintf(yyout, "Line %d, column %d: illegal character (%s)\n", line, col, yytext);}

<BCOMMENT>"*/" 	        				  {line+=comment_lines; col+=comment_cols+1; BEGIN 0;}
<BCOMMENT>{newline}        	              {comment_lines++; comment_cols = 0;}
<BCOMMENT><<EOF>>                         {if (flag) fprintf(yyout, "Line %d, column %d: unterminated comment\n", line, col); yyterminate();}
<BCOMMENT>.     	        	          {comment_cols+=yyleng;}

<LCOMMENT>{newline} 	        		  {line++; col = 0; BEGIN 0;}
<LCOMMENT>. 	        				  {;}

<SEMICOLON>{newline}                      {fprintf(yyout, "SEMICOLON\n"); line++; col = 0; BEGIN 0;}
<SEMICOLON>"/*"|"//"                      {yyless(0); fprintf(yyout, "SEMICOLON\n"); line++; col = 0; BEGIN 0;}
<SEMICOLON>" "|\t	                      {col+=yyleng;}
<SEMICOLON>.                              {yyless(0); BEGIN 0;}
<SEMICOLON><<EOF>>                        {fprintf(yyout, "SEMICOLON\n"); yyterminate();}


<STRING>{newline}                                          {if (flag) fprintf(yyout, "Line %d, column %d: unterminated string literal\n", line, col); line++; col = 0; BEGIN 0;}
<STRING>\"(\\(\"|n|r|t|f|\\)|[^\\\"\t\f\r\n])*\"           {fprintf(yyout, "STRLIT(%s)\n", yytext); col+=yyleng; BEGIN SEMICOLON;}
<STRING>\".*\\[^\\\"nrtf ].*(\"|{newline})?                {yyless(0); BEGIN DUMMY_QUOTE;}
<STRING>\".*\\                                             {yyless(0); BEGIN DUMMY_QUOTE;}
<STRING><<EOF>>                                            {if (flag) fprintf(yyout, "Line %d, column %d: unterminated string literal\n", line, col); yyterminate();}
<STRING>.                                                  {col+=yyleng;}

<DUMMY_QUOTE>\"                            {col++; comment_cols = 1; BEGIN ILLEGAL_SCP;}
<ILLEGAL_SCP>\"                            {col+=comment_cols; BEGIN 0;}
<ILLEGAL_SCP>{newline}                     {if (flag) fprintf(yyout, "Line %d, column %d: unterminated string literal\n", line, col); line++; col = 0; BEGIN 0;}
<ILLEGAL_SCP>\\(\"|n|r|t|f|\\)             {comment_cols+=yyleng;}
<ILLEGAL_SCP>\\[^\\\"nrtf\r\n ]            {if (flag) fprintf(yyout, "Line %d, column %d: invalid escape sequence (%s)\n", line, col+comment_cols, yytext); comment_cols+=yyleng;}
<ILLEGAL_SCP>\\{newline}                   {if (flag) fprintf(yyout, "Line %d, column %d: invalid escape sequence (%c)\n"
                                                           "Line %d, column %d: unterminated string literal\n", line, col+comment_cols, yytext[0], line, col); line++; col=0; BEGIN 0;}
<ILLEGAL_SCP>\\                            {if (flag) fprintf(yyout, "Line %d, column %d: invalid escape sequence (%s)\n", line, col+comment_cols, yytext); comment_cols+=yyleng;}
<ILLEGAL_SCP>\\" "                         {if (flag) fprintf(yyout, "Line %d, column %d: invalid escape sequence (%s)\n", line, col+comment_cols, yytext); comment_cols+=yyleng;}
<ILLEGAL_SCP><<EOF>>                       {if (flag) fprintf(yyout, "Line %d, column %d: unterminated string literal\n", line, col); yyterminate();}
<ILLEGAL_SCP>.                             {comment_cols+=yyleng;}

<OCTAL>0[0-7]*                             {fprintf(yyout, "INTLIT(%s)\n", yytext); col+=yyleng; BEGIN 0;}
<OCTAL>[0-9]+                              {col++; if (flag) fprintf(yyout, "Line %d, column %d: invalid octal constant (%s)\n", line, col, yytext); col+=yyleng-1; BEGIN 0;}
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
