#include "structures.h"

void print_ast(is_program* root);
void print_declarations(is_declarations_list* idl);
void print_func_dec(is_func_dec* ifd);
void print_parameter_type(parameter_type param);
void print_func_params(is_parameter* ipl);
void print_func_body(is_func_body* ifb);
void print_var_or_statement(is_var_or_statement* val);
void print_var_spec(is_var_spec* ivs);
void print_statement(is_statement* is);
void print_statement_if(is_if_statement* iifs);
void print_expression_list(is_expression_list* iel);
void print_expression2_list(is_expression2_list* iel2);
void print_is_operation(operation_type io);
void print_final_expression(is_final_expression * ife);
void print_statement_list(is_statements_list* ist);
void print_else_statement(is_else_statement* ies);
void print_statement_for(is_for_statement* ifs);
void print_return_statement(is_return_statement* irs);
void print_print_statement(is_print_statement* ips);
void print_assign_statement(is_assign_statement* ias);
void print_final_statement(is_final_statement* ifs);

