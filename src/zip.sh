# File to zip all files that are needed for compilation
# to send to Mooshak
zip gocompiler.zip \
	gocompiler.l\
 	gocompiler.y \
	functions.c \
	functions.h \
	structures.h \
	print_ast.c \
	print_ast.h \
	semantics.c \
	semantics.h \
	symbol_table.c \
	symbol_table.h \
	free_tree.c \
	free_tree.h \
	print_llvm.c \
	print_llvm.h
