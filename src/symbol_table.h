#include "symbols.h"


table_element *insert_symbol(table_element **symtab, char *str, parameter_type type);
table_element *search_symbol(table_element *symtab, char *str);
void show_table(table_element *symtab);

