#include "structures.h"
#include "symbol_table.h"

void print_already_defined(char* id, int line, int col);
void check_program(is_program* ip);
void check_declarations_list(table_element** symtab, is_declarations_list* idl);
void check_func_declaration(table_element** symtab, is_func_dec* ifd);
void check_function_body(table_element** symtab, is_func_body* ifb);
void check_vars_and_statements_list(table_element** symtab, is_vars_and_statements_list* ivsl);
void check_var_declaration(table_element** symtab, is_var_dec* ivd);
void check_var_spec(table_element** symtab, is_var_spec* ivs);
void check_statement(is_statement* is);
void check_if_statement(is_if_statement* ifs);
void check_for_statement(is_for_statement* ifs);
void check_return_statement(is_return_statement* irs);
void check_print_statement(is_print_statement* ips);
void check_assign_statement(is_assign_statement* ias);
void check_statements_list(is_statements_list* isl);
void check_final_statement(is_final_statement* ifs);
