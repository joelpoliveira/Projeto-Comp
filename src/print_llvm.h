#include "structures.h"

void llvm_program(is_program* ip);
void llvm_declarations_list(is_declarations_list* idl);
void llvm_func_declaration(is_func_dec* ifd);
void llvm_function_body(is_func_body* ifb);
void llvm_vars_and_statements_list(is_vars_and_statements_list* ivsl);
void llvm_var_declaration(is_var_dec* ivd, bool is_global);
void llvm_var_spec(is_var_spec* ivs, bool is_global);
void llvm_is_parameter(is_parameter * ip);
void llvm_statement( is_statement* is);
void llvm_if_statement(is_if_statement* ifs);
void llvm_else_statement(is_else_statement* ies);
void llvm_for_statement(is_for_statement* ifs);
void llvm_return_statement(is_return_statement* irs);
void llvm_print_statement(is_print_statement* ips);
void llvm_assign_statement(is_assign_statement* ias);
void llvm_statements_list(is_statements_list* isl);
void llvm_final_statement(is_final_statement* ifs);
