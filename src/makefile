.SILENT:
main:
	flex gocompiler.l
	yacc gocompiler.y -d
	clang -o gocompiler y.tab.c lex.yy.c

.PHONY: clean
clean:
	rm gocompiler y.tab.c lex.yy.c
