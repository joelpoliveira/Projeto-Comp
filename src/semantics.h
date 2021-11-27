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
void check_statement(table_element** symtab, is_statement* is);
void check_if_statement(table_element** symtab, is_if_statement* ifs);
void check_else_statement(table_element** symtab, is_else_statement* ies);
void check_for_statement(table_element** symtab, is_for_statement* ifs);
void check_return_statement(table_element** symtab, is_return_statement* irs);
void check_print_statement(table_element** symtab, is_print_statement* ips);
void check_statements_list(table_element** symtab, is_statements_list* isl);
void check_assign_statement(table_element** symtab, is_assign_statement* ial);
void check_final_statement(table_element** symtab, is_final_statement* ifs);
void check_expression_or_list(table_element** symtab, is_expression_or_list* ieol);
void check_expression_and_list(table_element** symtab, is_expression_and_list* ieal);
void check_expression_comp_list(table_element** symtab, is_expression_comp_list * iecl);
void check_expression_sum_like_list(table_element** symtab, is_expression_sum_like_list * iesl);
void check_expression_star_like_list(table_element** symtab, is_expression_star_like_list * iestl);
void check_self_expression_list(table_element** symtab, is_self_expression_list * isel);
void check_final_expression(table_element** symtab, is_final_expression * ife);
void check_func_invocation(table_element** symtab, is_function_invocation * ifi);
void check_id(table_element* symtab, id_token* id);
