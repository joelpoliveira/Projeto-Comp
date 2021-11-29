#.SILENT:
main:
	flex gocompiler.l
	yacc gocompiler.y -d #--debug --verbose
	clang-7 -o gocompiler y.tab.c lex.yy.c functions.c free_tree.c print_ast.c

.PHONY: clean
clean:
	rm gocompiler y.tab.c lex.yy.c y.output
