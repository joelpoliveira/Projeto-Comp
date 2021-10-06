main:
	flex gocompiler.l
	clang -o gocompiler lex.yy.c

.PHONY: clean
clean:
	rm gocompiler lex.yy.c
