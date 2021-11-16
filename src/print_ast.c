#include "print_ast.h"
#include "functions.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


void print_dots(int depth){
    for (int i = 0; i < depth; i++)
        printf("..");
}

void print_ast(is_program* root){
    if (root == NULL) return;

    printf("Program\n");
    print_declarations(root->idlist);
}


void print_declarations(is_declarations_list* idl){
    if (idl == NULL) return;
    


    is_declarations_list* current = idl;
    while (current != NULL) {
        printf("..FuncDecl\n");
        declaration_type type = current->val->type_dec;
        switch (type)
        {
        case d_func_dec:
            print_func_dec(current->val->dec.ifd);
            break;
        case d_var_declaration:
            print_var_dec(current->val->dec.ivd);
        break;

        default:
            printf("Erro func_dec / var_dec\n");
        }
        current = current->next;
    }

}

void print_var_dec(is_var_dec * ivd){
    if (ivd == NULL) return;

    printf("....VarDecl\n");
    print_var_spec(ivd->ivs, 3);
}

void print_func_dec(is_func_dec* ifd){
    printf("....FuncHeader\n");
    printf("......Id(%s)\n", ifd->id);
    print_parameter_type(ifd->type, 3);
    printf("......FuncParams\n");
    print_func_params(ifd->ipl);
    print_func_body(ifd->ifb);
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
            printf("Float\n");
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


void print_func_params(is_parameter* ipl){
    if (ipl == NULL) return;


    printf("........ParamDecl\n");

    is_id_type_list* current = ipl->val;

    while (current != NULL) {
        print_parameter_type(current->val->type_param, 5);
        print_dots(5);
        printf("Id(%s)\n", current->val->id);
        current = current->next;
    }
}


void print_func_body(is_func_body* ifb){
    if (ifb == NULL) return;

    is_vars_and_statements_list* current = ifb->ivsl;

    printf("....FuncBody\n");

    while (current != NULL) {
        print_var_or_statement(current->val, 2);
        current = current->next;
    }
}


void print_var_or_statement(is_var_or_statement* val, int depth){
    if (val == NULL) return;

    //d_var_dec, d_statement
    switch (val->type){
    case d_var_dec:
        print_dots(depth + 1);
        printf("VarDecl\n");
        print_var_spec(val->body.ivd->ivs, depth+2);
        break;

    case d_statement:
        print_statement(val->body.is, depth+1);
        break;
    
    default:
        printf("erro d_var_dec / d_statement\n");
    }
}


void print_var_spec(is_var_spec* ivs, int depth){
    if (ivs == NULL) return;
    

    print_parameter_type(ivs->type, depth);

    is_id_list* current = ivs->iil;
    while (current != NULL){
        print_dots(depth);
        printf("Id(%s)\n", current->val);
        current = current->next;        
    }
}


void print_statement(is_statement* is, int depth) {
    if (is == NULL) return;

    statement_type type = is->type_state;
    //statement_type = {d_if, d_for, d_return, d_print, d_assign,  d_statement_list, d_final_statement}   
    
    switch (type) {
        case d_if:
            print_dots(depth);
            printf("If\n");
            print_statement_if(is->statement.u_if_state, depth);
            break;
        case d_for:
            print_dots(depth);
            printf("For\n");
            print_statement_for(is->statement.u_for_state, depth);
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
            print_assign_statement(is->statement.u_assign, depth);
            break;
        case d_statement_list:
            print_dots(depth);
            printf("StatementList\n");
            print_statement_list(is->statement.isl, depth);
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
    print_expression_list(iifs->iel, depth + 1);


    print_dots(depth + 1);
    printf("Block\n");

    print_statement_list(iifs->isl, depth + 1);

    print_dots(depth + 1);
    printf("Block\n");

    print_else_statement(iifs->ies, depth);

}


void print_expression_list(is_expression_list* iel, int depth){
    if (iel == NULL) return;

    is_expression_list* current = iel;

    expression_type type = iel->type_expr; //expression_type = d_operation, d_expr
    
    switch(type){
        case d_operation:
            print_is_operation(current->op_type, depth);
            print_expression_list(current->next, depth+1);
            print_expression2_list(current->ie2l, depth + 1);
            break;
        case d_expr:
            print_expression2_list(current->ie2l, depth);
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
    
    print_dots(depth);
    switch(type){
        case d_expr_2:
            print_final_expression(current->ife, depth);
            break;
        case d_self_oper:
            print_is_self_operation(current->iso);
            print_expression2_list(current->next, depth);
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
        default:
            printf("Erro print_is_operation: %d\n", io);
            break;
    }
}


void print_final_expression(is_final_expression * ife, int depth){
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
            print_func_invocation(ife->expr.ifi, depth + 1);
            break;
        case d_expr_final:
            print_expression_list(ife->expr.iel, depth); 
            break;
        default:
            printf("Erro print_final_expression\n");
            break;
    }
}

void print_statement_list(is_statements_list* isl, int depth){
    if (isl == NULL) return;

    //TODO LOOP para statements
    is_statements_list * current = isl;
    while( current != NULL){
        print_statement(current->val, depth+1);
        current = current->next;
    }

}


void print_else_statement(is_else_statement* ies, int depth){
    if (ies == NULL) return;
    print_dots(depth - 1);
    printf("Else\n");

    print_dots(depth);
    printf("Block\n");
    print_statement_list(ies->isl, depth);
    print_dots(depth);
    printf("Block\n");
}


void print_statement_for(is_for_statement* ifs, int depth){
    if (ifs == NULL) return;

    print_expression_list(ifs->iel, depth);
    print_statement_list(ifs->isl, depth);
}


void print_return_statement(is_return_statement* irs, int depth){
    if (irs == NULL) return;
    print_expression_list(irs->iel, depth);
}


void print_print_statement(is_print_statement* ips, int depth){
    if (ips == NULL) return;


    print_type type = ips->type_print; // {d_expression, d_str}

    switch (type){
        case d_expression:
            print_expression_list(ips->print.iel, depth);
            break;
        case d_str:
            printf("Print-Str\n");
            //printf("%s\n", ips->print.id);
            break;
        default:
            printf("Erro print_print_statement\n");
            break;
    }

}

void print_assign_statement(is_assign_statement* ias, int depth){
    if (ias == NULL) return;
    print_expression_list(ias->iel, depth);
}


void print_final_statement(is_final_statement* ifs, int depth) {
    if (ifs == NULL) return;

    final_state_type type = ifs->type_state; 
    //d_function_invoc, d_arguments

    switch (type){
        case d_function_invoc:
            print_dots(depth);
            printf("FuncInvocation\n");
            print_expression_list(ifs->statement.ifi->iel, depth);
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


void print_func_invocation(is_function_invocation * ifi, int depth){
    print_dots(depth);
    printf("Id(%s)\n", ifi->id);
    print_expression_list(ifi->iel, depth);
}