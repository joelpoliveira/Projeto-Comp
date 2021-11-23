.SILENT:

Y_FLAGS = -d #--debug --verbose
C_FLAGS = -g # -Wall 

main:
	flex gocompiler.l
	yacc gocompiler.y $(Y_FLAGS)
	gcc $(C_FLAGS) -o gocompiler y.tab.c lex.yy.c functions.c print_ast.c

.PHONY: clean
clean:
	rm gocompiler y.tab.c lex.yy.c y.output
