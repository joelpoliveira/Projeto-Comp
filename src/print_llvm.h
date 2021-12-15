#include "structures.h"

void llvm_store(id_token* a, id_token* b);
void llvm_load(id_token* a);
void llvm_add(id_token* a, id_token* b);
int llvm_print(char* string, char* param, int);
void llvm_string_dec(id_token* id);
int llvm_string_size(char* s);
int string_size(char* s);
int llvm_get_string_num(char* string);
void llvm_print_type(parameter_type type);
void llvm_program(is_program* ip);
void llvm_declarations_list(is_declarations_list* idl);
void llvm_func_declaration(is_func_dec* ifd);
void llvm_function_body(is_func_body* ifb, table_element**);
void llvm_vars_and_statements_list(is_vars_and_statements_list* ivsl,table_element**, int);
void llvm_var_declaration(is_var_dec* ivd);
void llvm_var_spec(is_var_spec* ivs);
void llvm_is_parameter(is_parameter * ip);
int llvm_statement( is_statement* is,table_element**, int, int counter);
int llvm_if_statement(is_if_statement* ifs,table_element**, int, int counter);
int llvm_else_statement(is_else_statement* ies,table_element**, int, int counter);
int llvm_for_statement(is_for_statement* ifs,table_element**, int);
int llvm_return_statement(is_return_statement* irs,table_element**, int);
int llvm_print_statement(is_print_statement* ips,table_element**, int);
int llvm_assign_statement(is_assign_statement* ias,table_element**, int);
int llvm_statements_list(is_statements_list* isl,table_element**, int, int counter);
int llvm_final_statement(is_final_statement* ifs,table_element**, int);
char * llvm_expression_or_list(is_expression_or_list* ieol, id_token* aux, int,table_element**);
char * llvm_expression_and_list(is_expression_and_list* ieal, id_token* aux, int,table_element**);
char * llvm_expression_comp_list(is_expression_comp_list * iecl, id_token* aux, int,table_element**);
char * llvm_expression_sum_like_list(is_expression_sum_like_list * iesl, id_token* aux, int,table_element**);
char * llvm_expression_star_like_list(is_expression_star_like_list * iestl, id_token* aux, int,table_element**);
char * llvm_self_expression_list(is_self_expression_list * isel, id_token* aux, int,table_element**);
char * llvm_final_expression(is_final_expression * ife, id_token* aux, int,table_element**);
void llvm_func_invocation(is_function_invocation * ifi);
