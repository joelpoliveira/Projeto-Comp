#include "print_ast.h"
#include "functions.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>


void print_dots(int depth){
    for (int i = 0; i < depth; i++)
        printf("..");
}

bool check_statement( is_statements_list * head){
    int i = 0;
    for (;head; head = head->next ){
        switch (head->val->type_state)
        {
        case d_statement_list:
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
            if(check_statement(head->val->statement.isl))
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
                print_var_dec(current->val->dec.ivd, depth + 1);
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
    print_var_spec(ivd->ivs, depth - 1);
}


void print_func_dec(is_func_dec* ifd, int depth){
    print_dots(depth);
    printf("FuncHeader\n");

    print_dots(depth + 1);
    printf("Id(%s)\n", ifd->id);
    print_parameter_type(ifd->type, depth + 1);

    print_dots(depth + 1);
    printf("FuncParams\n");
    print_func_params(ifd->ipl, depth + 2);


    print_func_body(ifd->ifb, depth);
}


void print_parameter_type(parameter_type param, int depth){
    //types: d_integer, d_float32, d_string, d_bool, d_var, d_dummy
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
        case d_var:
            print_dots(depth);
            printf("Var\n");
            break;
        default:
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
        printf("Id(%s)\n", current->val->id);

        current = current->next;
    }
}


void print_func_body(is_func_body* ifb, int depth){
    if (ifb == NULL) return;

    is_vars_and_statements_list* current = ifb->ivsl;

    //aqui temos de verificar se tem {} para n dar print sempre que o statement tenha >2 linhas

    print_dots(depth);
    printf("FuncBody\n");

    while (current != NULL) {
        print_var_or_statement(current->val, depth);
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
        print_statement(val->body.is, depth - 1);
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
        printf("Id(%s)\n", current->val);
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
            printf("Assign\n");
            print_assign_statement(is->statement.u_assign, depth + 1);
            break;
        case d_statement_list:
            is_block = check_block(is->statement.isl);
            if (is_block){
                print_dots(depth);
                printf("Block\n");
            }
            print_statement_list(is->statement.isl, depth+(int) is_block);
            break;
        case d_final_statement:
            print_final_statement(is->statement.u_state, depth);
            break;
        default:
            printf("erro print_statement\n");
            break;
    }
}


void print_statement_if(is_if_statement* iifs, int depth){
    if (iifs == NULL) return;
    print_expression_list(iifs->iel, depth);

    print_dots(depth);
    printf("Block\n");

    print_statement_list(iifs->isl, depth + 1);
    
    print_else_statement(iifs->ies, depth - 1);

}

void print_else_statement(is_else_statement* ies, int depth){
    print_dots(depth + 1);
    printf("Block\n");

    if (ies == NULL) return;

    print_statement_list(ies->isl, depth+1); // adicionado +1
}

void print_print_statement(is_print_statement* ips, int depth){
    if (ips == NULL) return;

    print_type type = ips->type_print; // {d_expression, d_str}

    switch (type){
        case d_expression:
            print_expression_list(ips->print.iel, depth);
            break;
        case d_str:
            print_dots(depth);
            printf("StrLit(%s)\n", ips->print.id);
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
    
    print_expression_list(ifs->iel, depth);

    print_dots(depth);
    printf("Block\n");  

    print_statement_list(ifs->isl, depth + 1);
}


void print_return_statement(is_return_statement* irs, int depth){
    if (irs == NULL) return;
    print_expression_list(irs->iel, depth);
}

void print_final_statement(is_final_statement* ifs, int depth) {
    if (ifs == NULL) return;

    final_state_type type = ifs->type_state; 
    //d_function_invoc, d_arguments

    switch (type){
        case d_function_invoc:
            print_dots(depth);
            printf("Call\n");
            print_expression_list(ifs->statement.ifi->iel, depth); //alterado +1 antes 0
            break;
        case d_arguments:
            print_dots(depth);
            printf("ParseArgs\n");

            print_dots(depth+1);
            printf("Id(%s)\n", ifs->statement.ipa->id);

            print_expression_list(ifs->statement.ipa->iel, depth+1);
            break;
        
        default:
            printf("Erro print_final_statement\n");
            break;
    }
}

void print_assign_statement(is_assign_statement* ias, int depth){
    if (ias == NULL) return;
    print_dots(depth);
    printf("Id(%s)\n", ias->id);
    print_expression_list(ias->iel, depth);
}

void print_expression_list(is_expression_list* iel, int depth){
    if (iel == NULL) return;

    is_expression_list*  current = iel;

    expression_type type = iel->type_expr; //expression_type = d_operation, d_expr
    
    switch(type){
        case d_operation:
            print_is_operation(current->op_type, depth);
            print_expression_list(current->next, depth+1);
            print_expression2_list(current->ie2l, depth);
            break;
        case d_expr:
            print_expression2_list(current->ie2l, depth - 1);
            break;
        default:
            printf("erro print_expression_list\n");
            break;
    }
}


void print_expression2_list(is_expression2_list* ie2l, int depth){
    if (ie2l == NULL) return;

    is_expression2_list* current = ie2l;
    expression2_type type = current->type_expression;
    
    switch(type){
        case d_expr_2:
            print_expression2_list(current->next, depth);
            print_final_expression(current->ife, depth + 1);
            break;
        case d_self_oper:
            print_dots(depth);
            print_is_self_operation(current->iso);
            print_expression2_list(current->next, depth + 1);
            break;
        default:
            printf("erro print_expression2_list\n");
            break;
    }
}

void print_is_self_operation(self_operation_type sot){
    switch (sot){
        case d_self_minus:
            printf("Minus\n");
            break;
        case d_self_plus:
            printf("Plus\n");
            break;
        case d_self_not:
            printf("Not\n");
            break;
        case d_self_none:
            break;
        default:
            printf("Erro self_operation\n");
            break;
    }
}

void print_is_operation(operation_type io, int depth){
    if (io == d_none) return;
    //d_or, d_and, d_lt, d_gt, d_eq, d_ne, d_ge, d_le,
    //d_plus, d_minus, d_star, d_div, d_mod
    print_dots(depth);
    switch (io){
        case d_or:
            printf("Or\n");
            break;
        case d_and:
            printf("And\n");
            break;
        case d_lt:
            printf("Lt\n");
            break;
        case d_gt:
            printf("Gt\n");
            break;
        case d_eq:
            printf("Eq\n");
            break;
        case d_ne:
            printf("Ne\n");
            break;
        case d_ge:
            printf("Ge\n");
            break;
        case d_le:
            printf("Le\n");
            break;
        case d_plus:
            printf("Add\n");
            break;
        case d_minus:
            printf("Sub\n");
            break;
        case d_star:
            printf("Mul\n");
            break;
        case d_div:
            printf("Div\n");
            break;
        case d_mod:
            printf("Mod\n");
            break;
        case d_none:
            break;
        default:
            printf("Erro print_is_operation: %d\n", io);
            break;
    }
}


void print_final_expression(is_final_expression * ife, int depth){
    if (ife == NULL) return;
    print_dots(depth);

    switch (ife->type_final_expression){
        case d_intlit:
            printf("IntLit(%s)\n", ife->expr.u_intlit->intlit);
            break;
        case d_reallit:
            printf("RealLit(%s)\n", ife->expr.u_reallit->reallit);
            break;
        case d_id:
            printf("Id(%s)\n", ife->expr.u_id->id);
            break;
        case d_func_inv:
            printf("Call\n");
            print_func_invocation(ife->expr.ifi, depth);
            break;
        case d_expr_final:
            print_expression_list(ife->expr.iel, depth); 
            break;
        default:
            printf("Erro print_final_expression\n");
            break;
    }
}





void print_func_invocation(is_function_invocation * ifi, int depth){
    print_dots(depth+1);
    printf("Id(%s)\n", ifi->id);
    print_expression_list(ifi->iel, depth+1);
}
