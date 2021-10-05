%{
	int x = 0;
%}

%X COMMENT

%%

%%

int main(){
	yylex();
	return 0;
}

int yywrap(){
	return 1;
}