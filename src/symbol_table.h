#include "structures.h"

//table_element *insert_symbol(table_element **symtab, char *str, parameter_type type);
table_element *insert_func(table_element **symtab, char* str, is_parameter * ip, parameter_type return_type);
table_element *insert_var(table_element **symtab, char* str, parameter_type return_type);
table_element *search_symbol(table_element *symtab, char *str);
void symbol_print_type(parameter_type type);
void show_table(table_element *symtab);

