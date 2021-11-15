#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef enum {d_intlit, d_reallit, d_id, d_func_inv, d_expr_final} final_expression_type;
typedef enum {d_self_plus, d_self_minus, d_self_not, d_self_none}               self_operation_type;
typedef enum {d_self_oper, d_expr_2}                               expression2_type;
typedef enum {d_or, d_and, d_lt, d_gt, d_eq, d_ne, d_ge, d_le,
                d_plus, d_minus, d_star, d_div, d_mod, d_none}             operation_type;
typedef enum {d_operation, d_expr}                                 expression_type;
typedef enum { d_function_invoc, d_arguments}                      final_state_type;
typedef enum {d_expression, d_str}                                 print_type;
typedef enum {d_if, d_for, d_return, d_print, d_assign, 
                d_statement_list, d_final_statement}               statement_type;
typedef enum {d_var_dec, d_statement}                              var_or_statement;
typedef enum {d_integer, d_float32, d_string, d_bool, d_var, d_dummy}       parameter_type;
typedef enum {d_func_dec, d_var_declaration}                       declaration_type;

typedef struct _s0 is_program;
typedef struct _s1 is_declarations_list;
typedef struct _s2 is_declaration;
typedef struct _s3 is_func_dec;
typedef struct _s4 is_parameter;
typedef struct _s5 is_id_type_list;
typedef struct _s6 is_id_type;
typedef struct _s7 is_vars_and_statements_list;
typedef struct _s8 is_var_or_statement;
typedef struct _s9 is_statements_list;
typedef struct _s10 is_statement;
typedef struct _s11 is_if_statement;
typedef struct _s12 is_else_statement;
typedef struct _s13 is_for_statement;
typedef struct _s14 is_return_statement;
typedef struct _s15 is_print_statement;
typedef struct _s16 is_assign_statement;
typedef struct _s17 is_final_statement;
typedef struct _s18 is_parse_arguments;
typedef struct _s19 is_function_invocation;
typedef struct _s20 is_expression_list;
typedef struct _s21 is_operation;
typedef struct _s22 is_expression2_list;
typedef struct _s23 is_self_operation;
typedef struct _s24 is_final_expression;
typedef struct _s25 is_intlit;
typedef struct _s26 is_reallit;
typedef struct _s27 is_id;
typedef struct _s28 is_func_body;
typedef struct _s29 is_var_dec;
typedef struct _s30 is_var_spec;
typedef struct _s31 is_id_list;



typedef struct _s31{
        char * val;
        struct _s31 * next;
} is_id_list;

typedef struct _s30{
        is_id_list * iil;
        parameter_type type;
} is_var_spec;

typedef struct _s29{
        is_var_spec * ivs;
} is_var_dec;

typedef struct _s28{
        is_vars_and_statements_list * ivsl;
} is_func_body;

typedef struct _s27{
        char * id;
} is_id;

typedef struct _s26{
        char * reallit;
} is_reallit;

typedef struct _s25{
        char * intlit;
} is_intlit;

typedef struct _s24{
        final_expression_type type_final_expression;
        union{
                is_intlit * u_intlit;
                is_reallit * u_reallit;
                is_id * u_id;
                is_function_invocation * ifi;
                is_expression_list * iel;
        } expr;
} is_final_expression;

typedef struct _s22{
        expression2_type type_expression;
        self_operation_type iso;
        is_final_expression * ife;
        struct _s22 * next;
} is_expression2_list;
//se for d_expression is operation é null então só há expression_2

typedef struct _s20 {
        expression_type type_expr;
        operation_type op_type;
        is_expression2_list * ie2l;
        struct _s20 * next;
} is_expression_list;

typedef struct _s19{
        char * id;
        is_expression_list * iel;
} is_function_invocation;

typedef struct _s18{
        char * id;
        is_expression_list * iel;
} is_parse_arguments;

typedef struct _s17{
        final_state_type type_state;
        union{
                is_function_invocation * ifi;
                is_parse_arguments * ipa;
        } statement;
} is_final_statement;

typedef struct _s16{
        char * id;
        is_expression_list * iel;
} is_assign_statement;

typedef struct _s15{
        print_type type_print;
        union{
                is_expression_list * iel;
                char * id;
        } print;
} is_print_statement;

typedef struct _s14{
        is_expression_list * iel;
} is_return_statement;

typedef struct _s13{
        is_expression_list * iel;
        is_statements_list * isl;
} is_for_statement;

typedef struct _s12{
        is_statements_list * isl;
} is_else_statement;

typedef struct _s11 {
        is_expression_list * iel;
        is_statements_list * isl;
        is_else_statement * ies;
} is_if_statement;

typedef struct _s10 {
        statement_type type_state;
        union{
                is_if_statement * u_if_state;
                is_for_statement * u_for_state;
                is_return_statement * u_return_state;
                is_print_statement * u_print_state;
                is_assign_statement * u_assign;
                is_statements_list * isl;
                is_final_statement * u_state;
        } statement;
} is_statement;

typedef struct _s9{
     is_statement * val;
     struct _s9 * next;   
} is_statements_list;

typedef struct _s8{
        var_or_statement type;
        union {
                is_var_dec * ivd;
                is_statement * is;
        } body;
} is_var_or_statement;

typedef struct _s7{
        is_var_or_statement * val;
        struct _s7 * next;
} is_vars_and_statements_list;

//possiveis tipos de variavel INT, FLOAT32, bool, var , string
typedef struct _s6{
        parameter_type type_param;
        char * id;
} is_id_type;

typedef struct _s5{
        is_id_type * val;
        struct _s5 * next;
} is_id_type_list;

typedef struct _s4{
        is_id_type_list * val;
} is_parameter;

typedef struct _s3 {
        is_parameter * ipl;
        is_func_body * ifb;
        parameter_type type;
        char* id;
} is_func_dec;

//a lista de declarações pode ter 2 tipos de elementos. func_dec e var_dec;
typedef struct _s2 {
        declaration_type type_dec;
        union {
                is_func_dec * ifd;
                is_var_dec * ivd;
        } dec;
} is_declaration;

typedef struct _s1 {
        is_declaration* val;
        struct _s1 *next;
} is_declarations_list;

typedef struct _s0{
        is_declarations_list * idlist;
} is_program;
#endif
