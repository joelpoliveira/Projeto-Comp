%{
    // Tomás Mendes - 2019232272
    // Joel Oliveira - 2019227468

    #include "y.tab.h"

    int line = 1;
    int comment_lines = 1; 
    int col = 0;
    int comment_cols = 0;

    int flag_1, swaped_line;
%}

letter  [a-zA-Z_]
number  [0-9]
exp     (e|E)[-+]?{number}+
float   ({number}+"."{number}*)|("."{number}+)
hexa	(0(x|X)[0-9a-fA-F]+)
newline \r?\n
sp_chr  \\(\"|n|r|t|f|\\)
whispace (" "|\t)

%X SEMICOLON BCOMMENT LCOMMENT STRING DUMMY_QUOTE OCTAL SEMIBCOMMENT SEMILCOMMENT INVALID_STRING

%%

";"				{if (flag_1) fprintf(yyout, "SEMICOLON\n"); col+=yyleng; return SEMICOLON;}
","				{if (flag_1) fprintf(yyout, "COMMA\n"); col+=yyleng; return COMMA;}
"_"				{if (flag_1) fprintf(yyout, "BLANKID\n"); col+=yyleng; return BLANKID;}
"="				{if (flag_1) fprintf(yyout, "ASSIGN\n"); col+=yyleng; return ASSIGN;}
"*"				{if (flag_1) fprintf(yyout, "STAR\n"); col+=yyleng; return STAR;}
"/"				{if (flag_1) fprintf(yyout, "DIV\n"); col+=yyleng; return DIV; }
"-"				{if (flag_1) fprintf(yyout, "MINUS\n"); col+=yyleng; return MINUS; }
"+"				{if (flag_1) fprintf(yyout, "PLUS\n"); col+=yyleng; return PLUS; }
"=="			{if (flag_1) fprintf(yyout, "EQ\n"); col+=yyleng; return EQ;}
">="			{if (flag_1) fprintf(yyout, "GE\n"); col+=yyleng; return GE;}
">"				{if (flag_1) fprintf(yyout, "GT\n"); col+=yyleng; return GT;}
"{"				{if (flag_1) fprintf(yyout, "LBRACE\n"); col+=yyleng; return LBRACE;}
"<="			{if (flag_1) fprintf(yyout, "LE\n"); col+=yyleng; return LE; }
"("				{if (flag_1) fprintf(yyout, "LPAR\n"); col+=yyleng; return LPAR; }
"["				{if (flag_1) fprintf(yyout, "LSQ\n"); col+=yyleng; return LSQ;}	
"<"				{if (flag_1) fprintf(yyout, "LT\n"); col+=yyleng; return LT;}
"%"				{if (flag_1) fprintf(yyout, "MOD\n"); col+=yyleng; return MOD;}
"!="			{if (flag_1) fprintf(yyout, "NE\n"); col+=yyleng; return NE;}
"!"				{if (flag_1) fprintf(yyout, "NOT\n"); col+=yyleng; return NOT;}
"&&"			{if (flag_1) fprintf(yyout, "AND\n"); col+=yyleng; return AND;}
"||"			{if (flag_1) fprintf(yyout, "OR\n"); col+=yyleng; return OR;}
"}"				{if (flag_1) fprintf(yyout, "RBRACE\n"); col+=yyleng; return RBRACE; BEGIN SEMICOLON;}
")"				{if (flag_1) fprintf(yyout, "RPAR\n"); col+=yyleng; return RPAR; BEGIN SEMICOLON;}
"]"				{if (flag_1) fprintf(yyout, "RSQ\n"); col+=yyleng; return RSQ; BEGIN SEMICOLON;}
"package"		{if (flag_1) fprintf(yyout, "PACKAGE\n"); col+=yyleng; return PACKAGE;}
"return"		{if (flag_1) fprintf(yyout, "RETURN\n"); col+=yyleng; return RETURN; BEGIN SEMICOLON;}
"else"			{if (flag_1) fprintf(yyout, "ELSE\n"); col+=yyleng; return ELSE; }
"for"			{if (flag_1) fprintf(yyout, "FOR\n"); col+=yyleng; return FOR; }
"if"			{if (flag_1) fprintf(yyout, "IF\n"); col+=yyleng; return IF; }
"var"			{if (flag_1) fprintf(yyout, "VAR\n"); col+=yyleng; return VAR; }
"int"			{if (flag_1) fprintf(yyout, "INT\n"); col+=yyleng; return INT;}
"float32"		{if (flag_1) fprintf(yyout, "FLOAT32\n"); col+=yyleng; return FLOAT32;}
"bool"			{if (flag_1) fprintf(yyout, "BOOL\n"); col+=yyleng; return BOOL; }
"string"		{if (flag_1) fprintf(yyout, "STRING\n"); col+=yyleng; return STRING; }
"fmt.Println"	{if (flag_1) fprintf(yyout, "PRINT\n"); col+=yyleng; return PRINT; }
"strconv.Atoi"	{if (flag_1) fprintf(yyout, "PARSEINT\n"); col+=yyleng; return PARSEINT; }
"func"			{if (flag_1) fprintf(yyout, "FUNC\n"); col+=yyleng; return FUNC; }
"os.Args"		{if (flag_1) fprintf(yyout, "CMDARGS\n"); col+=yyleng; return CMDARGS;}
"/*"			{col++; comment_lines = 0; comment_cols = 0; swaped_line = 0; BEGIN BCOMMENT;}
"//"			{col++; BEGIN LCOMMENT;}

"++"|"--"		{if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng; yylval.string =strdup(yytext); return RESERVED; }
"break"         {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng; yylval.string =strdup(yytext); return RESERVED; }
"case"          {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng; yylval.string =strdup(yytext); return RESERVED; }
"chan"          {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng; yylval.string =strdup(yytext); return RESERVED; }
"const"         {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng; yylval.string =strdup(yytext); return RESERVED; }
"continue"      {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng; yylval.string =strdup(yytext); return RESERVED; }
"default"       {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng; yylval.string =strdup(yytext); return RESERVED; }
"defer"         {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng; yylval.string =strdup(yytext); return RESERVED; }
"fallthrough"   {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng; yylval.string =strdup(yytext); return RESERVED; }
"go"            {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng; yylval.string =strdup(yytext); return RESERVED;}
"import"        {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng; yylval.string =strdup(yytext); return RESERVED;}
"goto"          {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng; yylval.string =strdup(yytext); return RESERVED;}
"interface"     {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng; yylval.string =strdup(yytext); return RESERVED;}
"map"           {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng; yylval.string =strdup(yytext); return RESERVED;}
"range"         {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng; yylval.string =strdup(yytext); return RESERVED;}
"select"        {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng; yylval.string =strdup(yytext); return RESERVED;}
"struct"        {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng; yylval.string =strdup(yytext); return RESERVED;}
"switch"        {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng; yylval.string =strdup(yytext); return RESERVED;}
"type"          {if (flag_1) fprintf(yyout, "RESERVED(%s)\n", yytext); col+=yyleng; yylval.string =strdup(yytext); return RESERVED;}

0                               {yyless(0); BEGIN OCTAL;}
[1-9]{number}*|{hexa}          	{if (flag_1) fprintf(yyout, "INTLIT(%s)\n", yytext); col+=yyleng; yylval.string = strdup(yytext); return INTLIT; BEGIN SEMICOLON;}
({float}{exp}?|{number}+{exp})  {if (flag_1) fprintf(yyout, "REALLIT(%s)\n", yytext); col+=yyleng; yylval.string = strdup(yytext); return REALLIT; BEGIN SEMICOLON;}
{letter}[a-zA-Z0-9_]*			{if (flag_1) fprintf(yyout, "ID(%s)\n", yytext); yylval.string =strdup(yytext); return ID; col+=yyleng; BEGIN SEMICOLON;}
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

<SEMICOLON>{newline}                      {if (flag_1) {fprintf(yyout, "SEMICOLON\n"); return SEMICOLON}; line++; col = 0; BEGIN 0;}
<SEMICOLON>{whispace}	                  {col+=yyleng;}
<SEMICOLON>"/*"                           {col++; comment_lines = 0; comment_cols = 0; swaped_line = 0; BEGIN SEMIBCOMMENT;}
<SEMICOLON>"//"                           {BEGIN SEMILCOMMENT;}
<SEMICOLON>.                              {yyless(0); BEGIN 0;}
<SEMICOLON><<EOF>>                        {if (flag_1) {fprintf(yyout, "SEMICOLON\n"); return SEMICOLON;}; yyterminate();}

<SEMIBCOMMENT>"*/" 	        			  {line+=comment_lines; if (swaped_line) col = 0; else col++; col+=comment_cols+yyleng; if (swaped_line) {fprintf(yyout, "SEMICOLON\n"); return SEMICOLON; BEGIN 0;} else BEGIN SEMICOLON;}
<SEMIBCOMMENT>{newline}        	          {comment_lines++; comment_cols = 0; swaped_line=1; }
<SEMIBCOMMENT><<EOF>>                     {fprintf(yyout, "SEMICOLON\n"); fprintf(yyout, "Line %d, column %d: unterminated comment\n", line, col); return SEMICOLON; yyterminate();}
<SEMIBCOMMENT>.     	        	      {comment_cols+=yyleng;}

<SEMILCOMMENT>{newline} 	              {yyless(0); BEGIN SEMICOLON;}
<SEMILCOMMENT><<EOF>>                     {fprintf(yyout, "SEMICOLON\n"); return SEMICOLON; yyterminate();}
<SEMILCOMMENT>. 	        		      {;}

<STRING>{newline}                                   {fprintf(yyout, "Line %d, column %d: unterminated string literal\n", line, col); line++; col = 0; BEGIN 0;}
<STRING>\"(\\(\"|n|r|t|f|\\)|[^\\\"\r\n])*\"        {if (flag_1) fprintf(yyout, "STRLIT(%s)\n", yytext); yylval.string =strdup(yytext); return STRLIT; col+=yyleng; BEGIN SEMICOLON;}
<STRING>\"[^\"\r\n]*\\[^\\\"nrtf][^\"\r\n]*\"       {yyless(0); BEGIN DUMMY_QUOTE;}
<STRING>\"[^\"\r\n]*{newline}?                      {yyless(0); BEGIN DUMMY_QUOTE;}

<DUMMY_QUOTE>\"                            {col++; comment_cols = 1; BEGIN INVALID_STRING;}

<INVALID_STRING>\"                         {col+=comment_cols; BEGIN 0;}
<INVALID_STRING>{newline}                  {fprintf(yyout, "Line %d, column %d: unterminated string literal\n", line, col); line++; col = 0; BEGIN 0;}
<INVALID_STRING>\\(\"|n|r|t|f|\\)          {comment_cols+=yyleng;}
<INVALID_STRING>\\[^\\\"nrtf\r\n]          {fprintf(yyout, "Line %d, column %d: invalid escape sequence (%s)\n", line, col+comment_cols, yytext); comment_cols+=yyleng;}
<INVALID_STRING>\\                         {fprintf(yyout, "Line %d, column %d: invalid escape sequence (%s)\n", line, col+comment_cols, yytext); comment_cols+=yyleng;}
<INVALID_STRING><<EOF>>                    {fprintf(yyout, "Line %d, column %d: unterminated string literal\n", line, col); yyterminate();}
<INVALID_STRING>.                          {comment_cols+=yyleng;}

<OCTAL>0[0-7]*                             {if (flag_1) {fprintf(yyout, "INTLIT(%s)\n", yytext); yylval.string = strdup(yytext); return INTLIT;}col+=yyleng; BEGIN SEMICOLON;}
<OCTAL>[0-9]+                              {col++; fprintf(yyout, "Line %d, column %d: invalid octal constant (%s)\n", line, col, yytext); col+=yyleng-1; BEGIN 0;}
<OCTAL>.                                   {yyless(0); BEGIN 0;}

%%

int yywrap(){
    return 1;
}
