#include "structures.h"

is_program* insert_program(is_declarations_list *);
is_declarations_list * insert_declaration(is_declarations_list *, is_declaration *);
is_declaration * insert_var_declaration(is_var_dec* );
is_var_spec * insert_var_specifications(is_id_list *);
is_id_list * insert_var_id(is_id_list * , is_var_id * );
is_declaration * insert_func_declaration(is_parameters_list * , char* , is_func_body * );
