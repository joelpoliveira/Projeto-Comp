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
void print_is_operation(is_operation* io);
void print_final_expression(is_final_expression * ife);




