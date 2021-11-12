#include "structures.h"

is_program* insert_program(is_declarations_list *);
is_declarations_list * insert_declaration(is_declarations_list *, is_declaration *);
is_declaration * insert_var_declaration(is_var_dec* );
is_var_spec * insert_var_specifications(is_id_list *);
is_id_list * insert_var_id(is_id_list * , is_var_id * );
is_declaration * insert_func_declaration(is_parameter * , char* , is_func_body * );
is_id_type_list * insert_id_type(is_id_type_list * , char * , char * );
is_parameter * insert_parameter(char * , char * , is_id_type_list * );
is_vars_and_statements_list * insert_var_dec(is_vars_and_statements_list * , is_var_dec * );
is_vars_and_statements_list * insert_statements(is_vars_and_statements_list * , is_statement * );
is_statement * insert_if_statement(is_expression_list * , is_statements_list *, is_statements_list * );
is_statement * insert_for_statement(is_expression_list * , is_statements_list* );
is_statement * insert_return_statement(is_expression_list * );
is_statement * insert_print_expr_statement(is_expression_list * );
is_statement * insert_print_str_statement(is_str * );
is_statement * insert_assign_statement(char * , is_expression_list * );
is_statement * insert_statements_list(is_statements_list *);
is_statement * insert_final_statement(is_final_statement *);
is_final_statement * insert_final_state_args(is_parse_arguments * );
is_final_statement * insert_final_state_func_inv(is_function_invocation * );
is_statements_list * insert_statement_in_list(is_statements_list *, is_statement * );
is_parse_arguments * insert_parse_args( char * , is_expression_list * );
