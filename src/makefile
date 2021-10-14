.SILENT:
main:
	flex gocompiler.l
	clang-7 -o gocompiler lex.yy.c

.PHONY: clean
clean:
	rm gocompiler lex.yy.c
