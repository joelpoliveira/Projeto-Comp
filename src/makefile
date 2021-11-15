.SILENT:
main:
	flex gocompiler.l
	yacc gocompiler.y -d #--debug --verbose
	gcc -o gocompiler y.tab.c lex.yy.c functions.c print_ast.c -g
	
.PHONY: clean
clean:
	rm gocompiler y.tab.c lex.yy.c y.output
