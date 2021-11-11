#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct _s39{
        char * id;
} is_var_id;

typedef struct _s38{
        is_var_id * val;
        struct _s38 * next;
} is_id_list;

typedef struct _s37{
        is_id_list * iil;
        char * type;
} is_var_spec;

typedef struct _s36{
        is_var_spec * ivs;
} is_var_dec;

typedef struct _s35{
        is_vars_and_statements_list * ivsl;
} is_func_body;

typedef struct _s34{
        char * id;
} is_id;

typedef struct _s33{
        char * reallit;
} is_reallit;

typedef struct _s32{
        char * intlit;
} is_intlit;

typedef enum {d_intlit, d_reallit, d_id, d_func_invoc, d_expr} final_expression_type;
typedef struct _s31{
        final_expression_type type_final_expression;
        union{
                is_intlit * u_intlit;
                is_reallit * u_reallit;
                is_id * u_id;
                is_function_invocation * ifi;
                is_expression_list * iel;
        } expr;
} is_final_expression;

typedef enum {d_plus, d_minus, d_ne} self_operation_type;
typedef struct _s30{
        self_operation_type type_operation;
        is_final_expression * ifs;
} is_self_operation;

typedef enum {d_self_oper, d_expression} expression2_type;
typedef struct _s29{
        expression2_type type_expression;
        union {
                is_self_operation * iso;
                is_final_expression * ifs;
        } expr;
} is_expression2;

typedef struct _s28{
        is_expression2 * ie2;
        is_expression2_list * next;
} is_expression2_list;

typedef enum {d_or, d_and, d_lt, d_gt, d_eq, d_ne, d_ge,
                d_plus, d_minus, d_star, d_div, d_mod} operation_type;
typedef struct _s27{
        operation_type type_operation;
        is_expression2_list * ie2l;
} is_operation;

//se for d_expression is operation é null então só há expression_2
typedef enum {d_operation, d_expression} expression_type;

typedef struct _s26{
        expression_type type_expr;
        union {
                is_operation * io;
                is_expression2_list * ie2l;
        } expr;
} is_expression;

typedef struct _s25 {
        is_expression * ie;
        struct _s25 * next;
} is_expression_list;

typedef struct _s24{
        is_expression_list * iel;
} is_function_invocation;

typedef struct _s23{
        is_expression_list * iel;
} is_parse_arguments;

typedef enum { d_function_invoc, d_arguments} final_state_type;

typedef struct _s22{
        final_state_type type_state;
        union{
                is_function_invocation * ifi;
                is_parse_arguments * ipa;
        } statement;
} is_final_statement;

typedef struct _s21{
        is_expression * ie;
} is_assign_statement;

typedef struct _s20{
        char * id;
} is_str;

typedef enum {d_expression, d_str} print_type;

typedef struct _s19{
        print_type * type_print;
        union{
                is_expression * ie;
                is_str * u_str_state;
        } print;
} is_print_statement;

typedef struct _s18{
        is_expression * ie;
} is_return_statement;

typedef struct _s17{
        is_expression * ie;
        is_statements_list * isl;
} is_for_statement;

typedef struct _s16{
        is_statements_list;
} is_else_statement;

typedef struct _s15 {
        is_expression * ie;
        is_statements_list * isl;
        is_else_statement * ies;
} is_if_statement;

typedef enum {d_if, d_for, d_return, d_print, d_assign, d_statement_list, d_final_statement} statement_type;

typedef struct _s14 {
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

typedef struct _s13{
     is_statement * val;
     struct _s21 * next;   
} is_statements_list;

typedef enum {d_var, d_statement} var_or_statement;

typedef struct _s12{
        var_or_statement type;
        union {
                is_var_dec * ivd;
                is_statement * is;
        } body;
} is_var_or_statement;

typedef struct _s11{
        is_var_or_statement * val;
        struct _s11 * next;
} is_vars_and_statements_list;

typedef struct _s10{
        char * id;
} is_var_param;

typedef struct _s9{
        char * id;
} is_bool_param;

typedef struct _s8{
        char * id;
} is_str_param;

typedef struct _s7 {
        char * id;
} is_float32_param;

typedef struct _s6{
        char * id;
} is_integer_param;
//possiveis tipos de variavel INT, FLOAT32, 
typedef enum {d_integer, d_float32, d_str, d_bool, d_var} parameter_type;

typedef struct _s5{
        parameter_type type_param;
        union {
                is_integer_param* u_integer_param;
                is_float32_param * u_float32_param;
                is_str_param * u_str_param;
                is_bool_param * u_bool_param;
                is_var_param * u_var_param;
        } param;
} is_parameter;

typedef struct _s4{
        is_parameter * val;
        struct _s4 * next;
} is_parameters_list;

typedef struct _s3 {
        is_parameters_list * ipl;
        is_func_body * ifb;
        char * type;
} is_func_dec;

typedef enum {d_func_dec, d_var_dec} declaration_type;

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
