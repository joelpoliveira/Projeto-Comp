#include "structures.h"
#include "symbol_table.h"

void print_already_defined(char* id, int line, int col);
void check_program(is_program* ip);
void check_declarations_list(table_element** symtab, is_declarations_list* idl);
void check_func_declaration(table_element** symtab, is_func_dec* ifd);
void check_var_declaration(table_element** symtab, is_var_dec* ivd);
void check_var_spec(table_element** symtab, is_var_spec* ivs);
