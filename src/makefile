
Y_FLAGS = -d #--debug --verbose
C_FLAGS = -g  -Wall
LY_FILES =  y.tab.c lex.yy.c
FILES = functions.c print_ast.c symbol_table.c semantics.c free_tree.c print_llvm.c

.SILENT: # makes not show what commands are executed
main:
	flex gocompiler.l
	yacc gocompiler.y $(Y_FLAGS)
	clang-7	-lm $(C_FLAGS) -o gocompiler $(LY_FILES) $(FILES)

.PHONY: clean
clean:
	rm gocompiler $(LY_FILES) y.output y.tab.h
