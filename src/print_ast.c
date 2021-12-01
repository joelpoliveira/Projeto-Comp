#include "print_ast.h"
#include "functions.h"
#include "symbol_table.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

extern bool flag_3;
extern is_program * program;


void print_anotation_type(id_token* id){
    //typedef enum {d_integer, d_float32, d_string, d_bool, d_var, d_none}  
    if (flag_3){
        switch (id->type){
            case d_integer:
                printf(" - int");
                break;
            case d_float32:
                printf(" - float32");
                break;
            case d_string:
                printf(" - string");
                break;
             case d_bool:
                printf(" - bool");
                break;
            case d_none:
                //printf(" - none");
                break;
            case d_undef:
                printf(" - undef");
                break;
            default:
                printf("Erro print_anotation_typnonee: %d", id->type);
                break;
        }
    } 
    printf("\n");
}


void print_parameters_type(is_id_type_list * iitl){
    for ( ; iitl; iitl = iitl->next){
            switch (iitl->val->type_param)
            {
            case d_integer:
                printf("int");
                break;
            case d_float32:
                printf("float32");
                break;
            case d_string:
                printf("string");
                break;
            case d_bool:
                printf("bool");
                break;
            case d_var:
            case d_none:
                break;
            default:
                printf("-----------------\n\n-----------------\n");
            }
            if (iitl->next != NULL)
                printf(",");
        
    }
}

void print_dots(int depth){
    for (int i = 0; i < depth; i++)
        printf("..");
}

bool check_statement_in_block( is_statements_list * head){
    int i = 0;
    for (;head; head = head->next ){
        switch (head->val->type_state){
        case d_statement_list:
            if(check_statement_in_block(head->val->statement.isl))
                i++;
            break;
        default:
            i++;
            break;
        }
    }
    return i>=1;
}

bool check_block( is_statements_list * head){
    int i=0;
    for (;head; head = head->next ){
        switch (head->val->type_state)
        {
        case d_statement_list:
            if(check_statement_in_block(head->val->statement.isl))
                i++;
            break;
        default:
            i++;
            break;
        }
    }
    return i >= 2;
}


void print_ast(is_program* root){
    if (root == NULL) return;

    printf("Program\n");
    print_declarations(root->idlist, 1);
}


void print_declarations(is_declarations_list* idl, int depth){
    if (idl == NULL) return;

    is_declarations_list* current = idl;
    while (current != NULL) {
        declaration_type type = current->val->type_dec;
        switch (type){
            case d_func_dec:
                print_dots(depth);
                printf("FuncDecl\n");

                print_func_dec(current->val->dec.ifd, depth + 1);
                break;
            case d_var_declaration:
                print_var_dec(current->val->dec.ivd, depth);
            break;
            default:
                printf("Erro print_declarations\n");
                break;
        }
        current = current->next;
    }

}


void print_var_dec(is_var_dec * ivd, int depth){
    if (ivd == NULL) return;    
    print_var_spec(ivd->ivs, depth);
}


void print_func_dec(is_func_dec* ifd, int depth){
    print_dots(depth);
    printf("FuncHeader\n");

    print_dots(depth + 1);
    printf("Id(%s)\n", ifd->id->id);
    print_parameter_type(ifd->type, depth + 1);

    print_dots(depth + 1);
    printf("FuncParams\n");
    print_func_params(ifd->ipl, depth + 2);

    print_func_body(ifd->ifb, depth);
}

void print_parameter_type(parameter_type param, int depth){
    //types: d_integer, d_float32, d_string, d_bool, d_var, d_none
    switch(param){
        case d_integer:
            print_dots(depth);
            printf("Int\n");
            break;
        case d_float32:
            print_dots(depth);
            printf("Float32\n");
            break;
        case d_string:
            print_dots(depth);
            printf("String\n");
            break;
        case d_bool:
            print_dots(depth);
            printf("Bool\n");
            break;
        // case d_var:
        //     print_dots(depth);
        //     printf("Var\n");
        //     break;
        // case d_none:
        //     print_dots(depth);
        //     printf("None\n");
        //     break;
        default:
            //printf("Erro print_parameter_type: %d\n", param);
            break;
    };
}

void print_func_params(is_parameter* ipl, int depth){
    if (ipl == NULL) return;

    //print_dots(depth);
    //printf("ParamDecl\n");

    is_id_type_list* current = ipl->val;

    while (current != NULL) {
        print_dots(depth);
        printf("ParamDecl\n");

        print_parameter_type(current->val->type_param, depth + 1);

        print_dots(depth + 1);
        printf("Id(%s)\n", current->val->id->id);

        current = current->next;
    }
}


void print_func_body(is_func_body* ifb, int depth){
    if (ifb == NULL) return;

    is_vars_and_statements_list* current = ifb->ivsl;

    print_dots(depth);
    printf("FuncBody\n");

    while (current != NULL) {
        print_var_or_statement(current->val, depth + 1);
        current = current->next;
    }
}


void print_var_or_statement(is_var_or_statement* val, int depth){
    if (val == NULL) return;

    //d_var_dec, d_statement
    switch (val->type){
        case d_var_dec:
            print_var_spec(val->body.ivd->ivs, depth);
            break;

        case d_statement:
            print_statement(val->body.is, depth);
            break;

        default:
            printf("Erro print_var_or_statement\n");
    }
}


void print_var_spec(is_var_spec* ivs, int depth){
    if (ivs == NULL) return;
    
    is_id_list* current = ivs->iil;

    while (current != NULL){
        print_dots(depth);
        printf("VarDecl\n");

        print_parameter_type(ivs->type, depth + 1);

        print_dots(depth + 1);
        printf("Id(%s)\n", current->val->id);
        current = current->next;        
    }
}


void print_statement(is_statement* is, int depth) {
    if (is == NULL) return;

    statement_type type = is->type_state;
    //statement_type = {d_if, d_for, d_return, d_print, d_assign,  d_statement_list, d_final_statement}   
    bool is_block;
    switch (type) {
        case d_if:
            print_dots(depth);
            printf("If\n");

            print_statement_if(is->statement.u_if_state, depth + 1);
            break;
        case d_for:
            print_dots(depth);
            printf("For\n");

            print_statement_for(is->statement.u_for_state, depth + 1);
            break;
        case d_return:
            print_dots(depth);
            printf("Return\n");

            print_return_statement(is->statement.u_return_state, depth + 1);
            break;
        case d_print:
            print_dots(depth);
            printf("Print\n");

            print_print_statement(is->statement.u_print_state, depth + 1);
            break;
        case d_assign:
            print_dots(depth);
            printf("Assign");
            if (flag_3) print_anotation_type(is->statement.u_assign->id);
            
            print_assign_statement(is->statement.u_assign, depth + 1);
            break;
        case d_statement_list:
            is_block = check_block(is->statement.isl);
            if (is_block){
                print_dots(depth);
                printf("Block\n");
            }
            print_statement_list(is->statement.isl, depth + (int) is_block);
            break;
        case d_final_statement:
            print_final_statement(is->statement.u_state, depth );
            break;
        default:
            printf("erro print_statement\n");
            break;
    }
}


void print_statement_if(is_if_statement* iifs, int depth){
    if (iifs == NULL) return;
    print_expression_or_list(iifs->iel, depth);

    print_dots(depth);
    printf("Block\n");

    print_statement_list(iifs->isl, depth + 1);
    
    print_else_statement(iifs->ies, depth);

}

void print_else_statement(is_else_statement* ies, int depth){
    print_dots(depth);
    printf("Block\n");

    if (ies == NULL) return;

    print_statement_list(ies->isl, depth+1); // adicionado +1
}

void print_print_statement(is_print_statement* ips, int depth){
    if (ips == NULL) return;

    print_type type = ips->type_print; // {d_expression, d_str}

    switch (type){
        case d_expression:
            print_expression_or_list(ips->print.iel, depth);
            break;
        case d_str:
            print_dots(depth);
            printf("StrLit(%s)\n", ips->print.id->id);
            break;
        default:
            printf("Erro print_print_statement\n");
            break;
    }

}

void print_statement_list(is_statements_list* isl, int depth){
    if (isl == NULL) return;

    is_statements_list * current = isl;
    
    while( current != NULL){
        print_statement(current->val, depth);
        current = current->next;
    }

}

void print_statement_for(is_for_statement* ifs, int depth){
    if (ifs == NULL) return;
    
    print_expression_or_list(ifs->iel, depth);

    print_dots(depth);
    printf("Block\n");  

    print_statement_list(ifs->isl, depth + 1);
}


void print_return_statement(is_return_statement* irs, int depth){
    if (irs == NULL) return;
    print_expression_or_list(irs->iel, depth);
}

void print_final_statement(is_final_statement* ifs, int depth) {
    if (ifs == NULL) return;

    final_state_type type = ifs->type_state; 
    //d_function_invoc, d_arguments
   
    switch (type){
        case d_function_invoc:
            //printf("======print_final_statement======\n");
            print_dots(depth);
            printf("Call");
            print_anotation_type(ifs->statement.ifi->id);
            print_func_invocation(ifs->statement.ifi, depth +1);

            break;
        case d_arguments:
            print_dots(depth);
            printf("ParseArgs");
            print_anotation_type(ifs->statement.ipa->id);

            print_dots(depth+1);
            printf("Id(%s)", ifs->statement.ipa->id->id);
            print_anotation_type(ifs->statement.ipa->id);

            print_expression_or_list(ifs->statement.ipa->iel, depth+1);
            break;
        
        default:
            printf("Erro print_final_statement\n");
            break;
    }
}

void print_assign_statement(is_assign_statement* ias, int depth){
    if (ias == NULL) return;
    print_dots(depth);
    printf("Id(%s)", ias->id->id);
    print_anotation_type(ias->id);
    print_expression_or_list(ias->iel, depth);
}


void print_expression_or_list(is_expression_or_list* ieol, int depth){
    if (ieol == NULL) return;

    is_expression_or_list*  current = ieol;

    bool type = ieol->is_operation; //expression_type = d_operation, d_expr
    
    if (type){
        print_dots(depth);
        printf("Or");
        if (flag_3) printf(" - bool");
        printf("\n");


        print_expression_or_list(current->next_left, depth+1);
        print_expression_and_list(current->next_right, depth+1);
    } else {
        print_expression_and_list(current->next_right, depth);
        print_expression_or_list(current->next_left, depth);
    }
}


void print_expression_and_list(is_expression_and_list* ieal, int depth){
    if (ieal == NULL) return;

    is_expression_and_list* current = ieal;
    bool type = current->is_operation;
    
    if(type){
        print_dots(depth);
        printf("And");
        if (flag_3) printf(" - bool");
        printf("\n");
        
        print_expression_and_list(current->next_left, depth + 1);
        print_expression_comp_list(current->next_right, depth + 1);
    } else {
        print_expression_comp_list(current->next_right, depth);
        print_expression_and_list(current->next_left, depth);
    }
}

void print_expression_comp_list(is_expression_comp_list * iecl, int depth){
    if (iecl == NULL) return;

    is_expression_comp_list * current = iecl;
    comp_type type = current->oper_comp;

    if (type != d_sum_like){
        print_dots(depth);
        print_comp_type(type);

        print_expression_comp_list(current->next_left, depth + 1);
        print_expression_sum_like_list(current->next_right, depth + 1);
    } else {
        print_expression_sum_like_list(current->next_right, depth);
        print_expression_comp_list(current->next_left, depth);
    }
}


void print_expression_sum_like_list(is_expression_sum_like_list * iesl, int depth){
    if (iesl == NULL) return;

    is_expression_sum_like_list * current = iesl;
    sum_like_type type = current->oper_sum_like;

    if (type != d_star_like){
        print_dots(depth);
        print_sum_like(type);

        print_expression_sum_like_list(current->next_left, depth + 1);
        print_expression_star_like_list(current->next_right, depth + 1);
    } else {
        print_expression_star_like_list(current->next_right, depth);
        print_expression_sum_like_list(current->next_left, depth);
    }
}


void print_expression_star_like_list(is_expression_star_like_list * iestl, int depth){
    if (iestl == NULL) return;

    is_expression_star_like_list * current = iestl;
    star_like_type type = current->oper_star_like;

    if (type != d_self){
        print_dots(depth);
        print_star_like(type);

        print_expression_star_like_list(current->next_left, depth + 1);
        print_self_expression_list(current->next_right, depth + 1);
    } else {
        print_self_expression_list(current->next_right, depth);
        print_expression_star_like_list(current->next_left, depth);
    }
}


void print_self_expression_list(is_self_expression_list * isel, int depth){
    if (isel == NULL) return;

    is_self_expression_list * current = isel;
    self_operation_type type = current->self_oper_type;

    if (type != d_final){
        print_dots(depth);
        print_self_operation_type(type);

        print_self_expression_list(current->next_same, depth + 1);
        print_final_expression(current->next_final, depth + 1);
    } else {
        print_final_expression(current->next_final, depth);
        print_self_expression_list(current->next_same, depth);
    }
}


void print_final_expression(is_final_expression * ife, int depth){
    if (ife == NULL) return;

    switch (ife->type_final_expression){
        case d_intlit:
            print_dots(depth);
            printf("IntLit(%s)", ife->expr.u_intlit->intlit->id);
            print_anotation_type(ife->expr.u_intlit->intlit);
            break;
        case d_reallit:
            print_dots(depth);
            printf("RealLit(%s)", ife->expr.u_reallit->reallit->id);
            print_anotation_type(ife->expr.u_reallit->reallit);
            break;
        case d_id:
            print_dots(depth);
            printf("Id(%s)", ife->expr.u_id->id->id);
            print_anotation_type(ife->expr.u_id->id);
            break;
        case d_func_inv:
            //printf("==== Final Expression =====\n");
            print_dots(depth);
            printf("Call");
            print_anotation_type(ife->expr.ifi->id);
            print_func_invocation(ife->expr.ifi, depth + 1);

            break;
        case d_expr_final:
            print_expression_or_list(ife->expr.ieol, depth); 
            break;
        default:
            printf("Erro print_final_expression\n");
            break;
    }
}


void print_func_invocation(is_function_invocation * ifi, int depth){
    print_dots(depth);
    printf("Id(%s)", ifi->id->id);
    
    if (flag_3){
        is_func_dec * func_dec = get_function_declaration(program, ifi->id->id);
        printf(" - (");
        if (func_dec!= NULL)
            print_parameters_type(func_dec->ipl->val);
        printf(")");
    }
    printf("\n");
    
    is_func_inv_expr_list * current = ifi->iel;
    while ( current ){
        print_expression_or_list(current->val, depth);
        current = current->next;
    }
}


void print_comp_type(comp_type ct){
    //{d_lt, d_gt, d_eq, d_ne, d_ge, d_le, d_sum_like}
    switch (ct){
        case d_lt:
            printf("Lt");
            if (flag_3)printf(" - bool");
            printf("\n");
            break;
        case d_gt:
            printf("Gt");
            if (flag_3)printf(" - bool");
            printf("\n");
            break;
        case d_eq:
            printf("Eq");
            if (flag_3)printf(" - bool");
            printf("\n");
            break;
        case d_ne:
            printf("Ne");
            if (flag_3)printf(" - bool");
            printf("\n");
            break;
        case d_ge:
            printf("Ge");
            if (flag_3)printf(" - bool");
            printf("\n");
            break;
        case d_le:
            printf("Le");
            if (flag_3)printf(" - bool");
            printf("\n");
            break;
        default:
            printf("Erro print_comp_type");
            break;
    }

}


void print_sum_like(sum_like_type slt) {
    //{d_plus, d_minus, d_star_like} sum_like_type

    switch (slt){
        case d_plus:
            printf("Add");
            break;
        case d_minus:
            printf("Sub");
            break;
        default:
            printf("Erro print_sum_type");
            break;
    }
    printf("\n");
}


void print_star_like(star_like_type slt) {
    //d_star, d_div, d_mod, d_self   star_like_type

    switch (slt){
        case d_star:
            printf("Mul");
            break;
        case d_div:
            printf("Div");
            break;
        case d_mod:
            printf("Mod");
            break;
        default:
            printf("Erro print_star_like\n");
            break;
    }
    printf("\n");

}


void print_self_operation_type(self_operation_type sot){
    // d_self_plus, d_self_minus, d_self_not, d_final    self_operation_type;

    switch (sot){
        case d_self_plus:
            printf("Plus");
            break;
        case d_self_minus:
            printf("Minus");
            break;
        case d_self_not:
            printf("Not");
            if (flag_3) printf(" - bool");
            break;
        default:
            printf("Erro print_self_operation_type\n");
            break;
    }
    printf("\n");
}

