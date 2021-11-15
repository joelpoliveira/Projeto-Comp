.SILENT:
main:
	flex gocompiler.l
	yacc gocompiler.y -d #--debug --verbose
<<<<<<< HEAD
	clang-7 -g -o gocompiler y.tab.c lex.yy.c functions.c print_ast.c
=======
	gcc -o gocompiler y.tab.c lex.yy.c functions.c print_ast.c -g
>>>>>>> origin/main
	
.PHONY: clean
clean:
	rm gocompiler y.tab.c lex.yy.c y.output
