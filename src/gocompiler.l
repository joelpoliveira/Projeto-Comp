%{
    // Tomás Mendes - 2019232272
    // Joel Oliveira - 2019227468
%}

letter  [a-zA-Z_]
number  [0-9]
exp     (e|E)[-+]?{number}+
float   ({number}+"."{number}*)|({number}*"."{number}+)
hexa	(0(x|X)[0-9a-fA-F]+)
octal   (0[0-7]+)
newline \n|\r\n|\r
sp_chr  \\(\"|n|r|t|f|\\)

%X SEMICOLON BCOMMENT LCOMMENT STRING DUMMY_QUOTE ILLEGAL_SCP

%%

";"				{fprintf(yyout, "SEMICOLON\n");}
","				{fprintf(yyout, "COMMA\n");}
"_"				{fprintf(yyout, "BLANKID\n");}
"="				{fprintf(yyout, "ASSIGN\n");}
"*"				{fprintf(yyout, "STAR\n");}
"/"				{fprintf(yyout, "DIV\n");}
"-"				{fprintf(yyout, "MINUS\n");}
"+"				{fprintf(yyout, "PLUS\n");}
"=="			{fprintf(yyout, "EQ\n");}
">="			{fprintf(yyout, "GE\n");}
">"				{fprintf(yyout, "GT\n");}
"{"				{fprintf(yyout, "LBRACE\n");}
"<="			{fprintf(yyout, "LE\n");}
"("				{fprintf(yyout, "LPAR\n");}
"["				{fprintf(yyout, "LSQ\n");}	
"<"				{fprintf(yyout, "LT\n");}
"%"				{fprintf(yyout, "MOD\n");}
"!="			{fprintf(yyout, "NE\n");}
"!"				{fprintf(yyout, "NOT\n");}
"&&"			{fprintf(yyout, "AND\n");}
"||"			{fprintf(yyout, "OR\n");}
"}"				{fprintf(yyout, "RBRACE\n"); BEGIN SEMICOLON;}
")"				{fprintf(yyout, "RPAR\n"); BEGIN SEMICOLON;}
"]"				{fprintf(yyout, "RSQ\n"); BEGIN SEMICOLON;}
"package"		{fprintf(yyout, "PACKAGE\n");}
"return"		{fprintf(yyout, "RETURN\n"); BEGIN SEMICOLON;}
"else"			{fprintf(yyout, "ELSE\n");}
"for"			{fprintf(yyout, "FOR\n");}
"if"			{fprintf(yyout, "IF\n");}
"var"			{fprintf(yyout, "VAR\n");}
"int"			{fprintf(yyout, "INT\n");}
"float32"		{fprintf(yyout, "FLOAT32\n");}
"bool"			{fprintf(yyout, "BOOL\n");}
"string"		{fprintf(yyout, "STRING\n");}
"fmt.Println"	{fprintf(yyout, "PRINT\n");}
"strconv.Atoi"	{fprintf(yyout, "PARSEINT\n");}
"func"			{fprintf(yyout, "FUNC\n");}
"os.Args"		{fprintf(yyout, "CMDARGS\n");}
"/*"			{fprintf(yyout, ""); BEGIN BCOMMENT;}
"//"			{fprintf(yyout, ""); BEGIN LCOMMENT;}
" "             {fprintf(yyout, "");}

"++"|"--"		{fprintf(yyout, "RESERVED(%s)\n", yytext);}
"break"         {fprintf(yyout, "RESERVED(%s)\n", yytext);}
"case"          {fprintf(yyout, "RESERVED(%s)\n", yytext);}
"chan"          {fprintf(yyout, "RESERVED(%s)\n", yytext);}
"const"         {fprintf(yyout, "RESERVED(%s)\n", yytext);}
"continue"      {fprintf(yyout, "RESERVED(%s)\n", yytext);}
"default"       {fprintf(yyout, "RESERVED(%s)\n", yytext);}
"defer"         {fprintf(yyout, "RESERVED(%s)\n", yytext);}
"fallthrough"   {fprintf(yyout, "RESERVED(%s)\n", yytext);}
"go"            {fprintf(yyout, "RESERVED(%s)\n", yytext);}
"goto"          {fprintf(yyout, "RESERVED(%s)\n", yytext);}
"import"        {fprintf(yyout, "RESERVED(%s)\n", yytext);}
"interface"     {fprintf(yyout, "RESERVED(%s)\n", yytext);}
"map"           {fprintf(yyout, "RESERVED(%s)\n", yytext);}
"range"         {fprintf(yyout, "RESERVED(%s)\n", yytext);}
"select"        {fprintf(yyout, "RESERVED(%s)\n", yytext);}
"struct"        {fprintf(yyout, "RESERVED(%s)\n", yytext);}
"switch"        {fprintf(yyout, "RESERVED(%s)\n", yytext);}
"type"          {fprintf(yyout, "RESERVED(%s)\n", yytext);}

{number}+|{octal}|{hexa}	    {fprintf(yyout, "INTLIT(%s)\n", yytext); BEGIN SEMICOLON;}
({float}|{number}+){exp}?       {fprintf(yyout, "REALLIT(%s)\n", yytext); BEGIN SEMICOLON;}
{letter}[a-zA-Z0-9_]*			{fprintf(yyout, "ID(%s)\n", yytext); BEGIN SEMICOLON;}
\"                              {yyless(0); BEGIN STRING;}

{newline}				    			  {;}
.				        				  {fprintf(yyout, "INVALID(%s)\n", yytext);}

<BCOMMENT>"*/" 	        				  {BEGIN 0;}
<BCOMMENT>{newline}|.	        	      {;}

<LCOMMENT>{newline} 	        		  {BEGIN 0;}
<LCOMMENT>. 	        				  {;}

<SEMICOLON>{newline}                      {fprintf(yyout, "SEMICOLON\n"); BEGIN 0;}
<SEMICOLON>\s	                          {;}
<SEMICOLON>.                              {yyless(0); BEGIN 0;}


<STRING>{newline}                                   {fprintf(yyout, "[ERRO]\n"); BEGIN 0;}
<STRING>\"(\\(\"|n|r|t|f|\\)|[^\\\"nrtf\n\r\t\f])*\"    {fprintf(yyout, "STRLIT(\"%s\")\n", yytext); BEGIN SEMICOLON;}
<STRING>\".*\\[^\\\"nrtf][^\r\n\"]*(\"|{newline})   {yyless(0); BEGIN DUMMY_QUOTE;}
<STRING>.                                           {;}

<DUMMY_QUOTE>\"                                     {BEGIN ILLEGAL_SCP;}
<ILLEGAL_SCP>\"                                     {BEGIN SEMICOLON;}
<ILLEGAL_SCP>{newline}                              {fprintf(yyout, "[ERRO]\n"); BEGIN 0;}
<ILLEGAL_SCP>\\[^\\\"nrtf\r\n]                          {fprintf(yyout, "Line <line>, column <col>: illegal escape sequence (%s)\n", yytext);}
<ILLEGAL_SCP>.                                      {;}

%%

int main(){
    yylex();
    return 0;
}

int yywrap(){
    return 1;
}
