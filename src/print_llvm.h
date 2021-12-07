#include "structures.h"

void llvm_program(is_program* ip);
void llvm_declarations_list(is_declarations_list* idl);
void llvm_func_declaration(is_func_dec* ifd);
void llvm_function_body(is_func_body* ifb);
void llvm_vars_and_statements_list(is_vars_and_statements_list* ivsl);
void llvm_var_declaration(is_var_dec* ivd, bool is_global);
void llvm_var_spec(is_var_spec* ivs, bool is_global);
void llvm_is_parameter(is_parameter * ip);