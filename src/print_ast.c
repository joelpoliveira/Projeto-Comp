#include "print_ast.h"
#include "functions.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


void print_ast(is_program* root){
    if (root == NULL) return;

    printf("Program\n..");
    print_declarations(root->idlist);
}


void print_declarations(is_declarations_list* idl){
    if (idl == NULL) return;
    
    printf("FuncDecl\n");
    printf("..");

    is_declarations_list* current = idl;
    while (current != NULL) {
        declaration_type type = current->val->type_dec;
        switch (type)
        {
        case d_func_dec:
            print_func_dec(current->val->dec.ifd);
            break;
        case d_var_declaration:
            //print_var_dec(current->val->dec.ivd)
        break;

        default:
            printf("Erro func_dec / var_dec\n");
        }
        current = current->next;
    }

}


void print_func_dec(is_func_dec* ifd){
    printf("..FuncHeader\n");
    printf("....Id(%s)\n", ifd->id);
    print_parameter_type(ifd->type);
    print_func_params(ifd->ipl);
    print_func_body(ifd->ifb);
}


void print_parameter_type(parameter_type param){
    //types: d_integer, d_float32, d_string, d_bool, d_var, d_dummy
    printf("....");
    switch(param){
        case d_integer:
            printf("Int\n");
            break;
        case d_float32:
            printf("Float\n");
            break;
        case d_string:
            printf("String\n");
            break;
        case d_bool:
            printf("Bool\n");
            break;
        case d_var:
            printf("Var\n");
            break;
        default:
            break;
    };
}


void print_func_params(is_parameter* ipl){
    if (ipl == NULL) return;

    printf("....FuncParams\n");
    printf("......ParamDecl\n");

    is_id_type_list* current = ipl->val;

    while (current != NULL) {
        printf("....");
        print_parameter_type(current->val->type_param);
        printf("........Id(%s)\n", current->val->id);
        current = current->next;
    }
}


void print_func_body(is_func_body* ifb){
    if (ifb == NULL) return;

    is_vars_and_statements_list* current = ifb->ivsl;

    printf("....FuncBody\n");

    while (current != NULL) {
        printf("......");
        print_var_or_statement(current->val);

        current = current->next;
    }
}


void print_var_or_statement(is_var_or_statement* val){
    if (val == NULL) return;

    //d_var_dec, d_statement
    switch (val->type){
    case d_var_dec:
        print_var_spec(val->body.ivd->ivs);
        break;

    case d_statement:
        print_statement(val->body.is);
        break;
    
    default:
        printf("erro d_var_dec / d_statement\n");
    }
}


void print_var_spec(is_var_spec* ivs){
    if (ivs == NULL) return;

    print_parameter_type(ivs->type);

    is_id_list* current = ivs->iil;

    while (current != NULL){
        printf(".......Id(%s)\n", current->val);
        current = current->next;        
    }
}


void print_statement(is_statement* is) {
    if (is == NULL) return;

    statement_type type = is->type_state;
    //statement_type = {d_if, d_for, d_return, d_print, d_assign,  d_statement_list, d_final_statement}   

    switch (type) {
        case d_if:
            printf("If\n");
            print_statement_if(is->statement.u_if_state);
            break;
        case d_for:
            printf("For\n");
            break;
        case d_return:
            printf("Return\n");
            break;
        case d_print:
            printf("Print\n");
            break;
        case d_assign:
            printf("Assign\n");
            break;
        case d_statement_list:
            printf("StatementList\n");
            break;
        case d_final_statement:
            printf("FinalStatement\n");
            break;
        default:
            printf("erro print_statement\n");
            break;
    }
}


void print_statement_if(is_if_statement* iifs){
    if (iifs == NULL) return;

    print_expression_list(iifs->iel);

}


void print_expression_list(is_expression_list* iel){
    if (iel == NULL) return;

    expression2_type type = iel->type_expr;
    //d_self_oper, d_expr_2

    switch(type){
        case d_self_oper:
            printf("self_operator\n");
            print_is_operation(iel->expr.io);
            break;
        case d_expr_2:
            printf("expression2\n");
            print_expression2_list(iel->expr.ie2l);
            break;
        default:
            printf("erro print_expression_list\n");
    }
}


void print_expression2_list(is_expression2_list* iel2){
    if (iel2 == NULL) return;
    //TODO
}


void print_is_operation(is_operation* io){
    if (io == NULL) return;
    //TODO
}

