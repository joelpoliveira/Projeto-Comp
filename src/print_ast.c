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
            print_statement_for(is->statement.u_for_state);
            break;
        case d_return:
            printf("Return\n");
            print_return_statement(is->statement.u_return_state);
            break;
        case d_print:
            printf("Print\n");
            print_print_statement(is->statement.u_print_state);
            break;
        case d_assign:
            printf("Assign\n");
            print_assign_statement(is->statement.u_assign);
            break;
        case d_statement_list:
            printf("StatementList\n");
            print_statement_list(is->statement.isl);
            break;
        case d_final_statement:
            printf("FinalStatement\n");
            print_final_statement(is->statement.u_state);
            break;
        default:
            printf("Erro print_statement\n");
            break;
    }
}


void print_statement_if(is_if_statement* iifs){
    if (iifs == NULL) return;

    print_expression_list(iifs->iel);
    print_statement_list(iifs->isl);
    print_else_statement(iifs->ies);
}


void print_expression_list(is_expression_list* iel){
    if (iel == NULL) return;

    is_expression_list* current = iel;

    while (current != NULL){
        expression_type type = iel->type_expr; //expression_type = d_operation, d_expr
        printf("Type = %d\n", type);
        switch(type){
            case d_operation:
                print_is_operation(current->op_type);
                //print_expression_list(current->next);
                //print_expression2_list(current->next->ie2l);
                break;
            case d_expr:
                //current->expr.
                //print_is_operation(current->op_type);
                print_expression2_list(current->ie2l);
                break;
            default:
                printf("erro print_expression_list\n");
                break;
        }

        current = current->next;
    }  
}


void print_expression2_list(is_expression2_list* ie2l){
    if (ie2l == NULL) return;

    is_expression2_list* current = ie2l;
    while(current != NULL){
        expression2_type type = current->type_expression;
        switch(type){
            case d_expr_2:
                printf("..........");
                print_final_expression(current->expr.ife);
                break;
            case d_self_oper:
                printf("self_operator\n");
                break;
            default:
                printf("erro print_expression2_list\n");
                break;
        }

        current = current->next;
    }

}


void print_is_operation(operation_type io){
    if (io == d_none) return;
    //d_or, d_and, d_lt, d_gt, d_eq, d_ne, d_ge, d_le,
    //d_plus, d_minus, d_star, d_div, d_mod
    printf("........");
    switch (io->type_operation){
        case d_or:
            printf("Or\n");
            break;
        case d_and:
            printf("And\n");
            break;
        case d_lt:
            printf("LT\n");
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
            printf("Plus\n");
            break;
        case d_minus:
            printf("Minus\n");
            break;
        case d_star:
            printf("Star\n");
            break;
        case d_div:
            printf("Div\n");
            break;
        case d_mod:
            printf("Mod\n");
            break;
        default:
            printf("Erro print_is_operation: %d\n", io);
            break;
    }
}


void print_final_expression(is_final_expression * ife){
    if (ife == NULL) return;

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
        // ife->expr.ifi->id
        //print()
        break;
    case d_expr_final:
        print_expression_list(ife->expr.iel); 
        break;
    default:
        printf("Erro print_final_expression\n");
        break;
    }
}

void print_statement_list(is_statements_list* ist){
    if (ist == NULL) return;
    
    printf("........Block\n");

}


void print_else_statement(is_else_statement* ies){
    if (ies == NULL) return;

    printf("........Else\n");

}


void print_statement_for(is_for_statement* ifs){
    if (ifs == NULL) return;

    print_expression_list(ifs->iel);
    print_statement_list(ifs->isl);
}


void print_return_statement(is_return_statement* irs){
    if (irs == NULL) return;

    print_expression_list(irs->iel);
}


void print_print_statement(is_print_statement* ips){
    if (ips == NULL) return;

    print_type type = ips->type_print; // {d_expression, d_str}

    switch (type){
        case d_expression:
            printf("Print-Expression\n");
            break;
        case d_str:
            printf("Print-Str\n");
            break;
        default:
            printf("Erro print_print_statement\n");
            break;
    }

}

void print_assign_statement(is_assign_statement* ias){
    if (ias == NULL) return;

    printf("id = %s\n", ias->id);
    print_expression_list(ias->iel);

}


void print_final_statement(is_final_statement* ifs) {
    if (ifs == NULL) return;

    final_state_type type = ifs->type_state; 
    //d_function_invoc, d_arguments

    switch (type){
        case d_function_invoc:
            printf("FuncInvocation\n");
            printf("id = %s", ifs->statement.ifi->id);
            print_expression_list(ifs->statement.ifi->iel);
            break;
        case d_arguments:
            printf("Arguments\n");
            printf("id = %s\n", ifs->statement.ipa->id);
            print_expression_list(ifs->statement.ipa->iel);
            break;
        
        default:
            printf("Erro print_final_statement\n");
            break;
    }

}
