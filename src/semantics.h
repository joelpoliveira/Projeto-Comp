#include "structures.h"
#include "symbol_table.h"

void check_program(is_program* ip);
void check_declarations_list(table_element** symtab, is_declarations_list* idl);
void check_func_declaration(table_element** symtab, is_func_dec* ifd);
