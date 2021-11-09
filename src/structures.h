#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct _s27{
        
} is_operation;

typedef enum {d_operation, d_expression} expression_type;

typedef struct _s26{
        expression_type type_expr;
        union {
                is_operation * io;
                is_expression2 * ie2;
        } expression;
} is_expression;

typedef struct _s25 {
        is_expression * ie;
        struct _s25 * next;
} is_expression_list;

typedef struct _s24{
        is_expression_list * iel;
} is_function_invocation;

typedef struct _s23{
        is_expression * ie;
} is_parse_arguments;

typedef enum { d_function, d_arguments} final_state_type;

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
        }
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
        }
} is_parameter;

typedef struct _s4{
        is_parameter * val;
        struct _s4 * next;
} is_parameters_list;

typedef struct _s3 {
        is_parameters_list * ipl;
        is_func_body * ifb;
} is_func_dec;

typedef enum {d_function, d_var_dec} declaration_type;

//a lista de declarações pode ter 2 tipos de elementos. func_dec e var_dec;
typedef struct _s2 {
        declaration_type type_dec;
        union {
                is_func_dec * func_dec;
                //is_var_dec * func_dec;
        } dec;
} is_declaration;

typedef struct _s1 {
        is_declaration* id;
        struct _s1 *next;
} is_declarations_list;

#endif
