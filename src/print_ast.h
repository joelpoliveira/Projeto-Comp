#include "structures.h"

void print_ast(is_program* root);
void print_declarations(is_declarations_list* idl);
void print_func_dec(is_func_dec* ifd);
void print_parameter_type(parameter_type param, int);
void print_func_params(is_parameter* ipl);
void print_func_body(is_func_body* ifb);
void print_var_or_statement(is_var_or_statement* val, int);
void print_var_spec(is_var_spec* ivs, int);
void print_statement(is_statement* is, int);
void print_statement_if(is_if_statement* iifs, int);
void print_expression_list(is_expression_list* iel, int);
void print_expression2_list(is_expression2_list* iel2, int);
void print_is_operation(operation_type io, int);
void print_final_expression(is_final_expression * ife, int);
void print_statement_list(is_statements_list* ist, int);
void print_else_statement(is_else_statement* ies, int);
void print_statement_for(is_for_statement* ifs, int);
void print_return_statement(is_return_statement* irs, int);
void print_print_statement(is_print_statement* ips, int);
void print_assign_statement(is_assign_statement* ias, int);
void print_final_statement(is_final_statement* ifs, int);
void print_is_self_operation(self_operation_type sot);
void print_func_invocation(is_function_invocation * ifi, int);
void print_var_dec(is_var_dec * ivd);