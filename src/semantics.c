#include "semantics.h"
#include "symbol_table.h"
#include "y.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//extern table_element* symtab;
extern is_program * program;


void print_already_defined(char* id, int line, int col){
    printf("Line %d, column %d: Symbol %s already defined\n", line, col+1, id);
}


void print_cannot_find(char* id, int line, int col){
    printf("Line %d col %d: Cannot find symbol %s\n", line, col+1, id);
}


void check_program(is_program* ip){
    check_declarations_list(&ip->symtab, ip->idlist);
}


void check_declarations_list(table_element** symtab, is_declarations_list* idl){
    is_declarations_list* current;

    //{d_func_dec, d_var_declaration} declaration_type
    for(current = idl; current != NULL; current = current->next) {
        switch (current->val->type_dec){
            case d_func_dec:
                check_func_declaration(symtab, current->val->dec.ifd);
                break;
            case d_var_declaration:
                check_var_declaration(symtab, current->val->dec.ivd);
                break;
        default:
            printf("Erro check_declarations_list");
            break;
        }
    }
}


void check_func_declaration(table_element** symtab, is_func_dec* ifd){
    
    ifd->symtab = insert_func(symtab, ifd->id->id, ifd->ipl, ifd->type);

    //inserir na tabela de simbolos da função
    check_function_body(&ifd->symtab, ifd->ifb);
}


void check_function_body(table_element** symtab, is_func_body* ifb){
    check_vars_and_statements_list(symtab, ifb->ivsl);
}


void check_vars_and_statements_list(table_element** symtab, is_vars_and_statements_list* ivsl){
    is_vars_and_statements_list* current;

    for(current = ivsl; current != NULL; current = current->next) {
        //{d_var_dec, d_statement} var_or_statement;
        switch (current->val->type){
            case d_var_dec:
                check_var_declaration(symtab, current->val->body.ivd);
                break;
            case d_statement:
                check_statement(symtab, current->val->body.is);
                break;
        default:
            printf("Erro check_vars_and_statements_list");
            break;
        }
    }
}


void check_var_declaration(table_element** symtab, is_var_dec* ivd){
    check_var_spec(symtab, ivd->ivs);
}


void check_var_spec(table_element** symtab, is_var_spec* ivs){
    table_element* new_symbol = NULL;
    parameter_type type = ivs->type;

    for (is_id_list* current = ivs->iil; current != NULL; current = current->next){
        new_symbol = insert_var(symtab, current->val->id, type);

        if (new_symbol == NULL){
            print_already_defined(current->val->id, current->val->line, current->val->col);
        }
    }
}


void check_statement(table_element** symtab, is_statement* is){
    //{d_if, d_for, d_return, d_print, d_assign, d_statement_list, d_final_statement
    switch (is->type_state){
        case d_if:
            check_if_statement(symtab, is->statement.u_if_state);
            break;
        case d_for:
            check_for_statement(symtab, is->statement.u_for_state);
            break;
        case d_return:
            check_return_statement(symtab, is->statement.u_return_state);
            break;
        case d_print:
            check_print_statement(symtab, is->statement.u_print_state);
            break;
        case d_assign:
            check_assign_statement(symtab, is->statement.u_assign);
            break;
        case d_statement_list:
            check_statements_list(symtab, is->statement.isl);
            break;
        case d_final_statement:
            check_final_statement(symtab, is->statement.u_state);
            break;
        
        default:
            printf("Erro check_statement\n");
            break;
    }
}


void check_if_statement(table_element** symtab, is_if_statement* ifs){
    if (ifs == NULL) return;

    check_expression_or_list(symtab, ifs->iel);
    check_statements_list(symtab, ifs->isl);
    check_else_statement(symtab, ifs->ies);
}

void check_else_statement(table_element** symtab, is_else_statement* ies){
    if (ies == NULL) return;

    check_statements_list(symtab, ies->isl);
}


void check_for_statement(table_element** symtab, is_for_statement* ifs){
    check_expression_or_list(symtab, ifs->iel);
    check_statements_list(symtab, ifs->isl);
}


void check_return_statement(table_element** symtab, is_return_statement* irs){
    check_expression_or_list(symtab, irs->iel);
}


void check_print_statement(table_element** symtab, is_print_statement* ips){
     if (ips == NULL) return;

    print_type type = ips->type_print; // {d_expression, d_str}

    switch (type){
        case d_expression:
            check_expression_or_list(symtab, ips->print.iel);
            break;
        case d_str:
            ips->print.id->type = d_string;
            break;
        default:
            printf("Erro check_print_statement\n");
            break;
    }
}


void check_assign_statement(table_element** symtab, is_assign_statement* ias){
    // assign_statement -> a = 2;
    // se existir na table global -> não adicionar á tabela local
    if (search_symbol(program->symtab, ias->id->id) == NULL){
        insert_var(symtab, ias->id->id, d_dummy);
    }
    
    check_expression_or_list(symtab, ias->iel);
}


void check_statements_list(table_element** symtab, is_statements_list* isl){
    if (isl == NULL) return;

    is_statements_list * current = isl;
    
    while( current != NULL){
        check_statement(symtab, current->val);
        current = current->next;
    }
}


void check_final_statement(table_element** symtab, is_final_statement* ifs){
    if (ifs == NULL) return;

    final_state_type type = ifs->type_state;  //d_function_invoc, d_arguments
    is_func_inv_expr_list * current;

    switch (type){
        case d_function_invoc:
            check_func_invocation(symtab, ifs->statement.ifi);

            current = ifs->statement.ifi->iel;
            while ( current ){
                check_expression_or_list(symtab, current->val);
                current = current->next;
            }
        case d_arguments:
            check_id(*symtab, ifs->statement.ipa->id);
            check_expression_or_list(symtab, ifs->statement.ipa->iel);
            break;
        default:
            printf("Erro check_final_statement\n");
            break;
    }
}


void check_expression_or_list(table_element** symtab, is_expression_or_list* ieol){
    if (ieol == NULL) return;
    //expression_type = d_operation, d_expr
    
    if (ieol->is_operation){
        //Do stuff

        check_expression_or_list(symtab, ieol->next_left);
        check_expression_and_list(symtab, ieol->next_right);
    }else{
        check_expression_and_list(symtab, ieol->next_right);
        check_expression_or_list(symtab, ieol->next_left);
    }
}


void check_expression_and_list(table_element** symtab, is_expression_and_list* ieal){

    if (ieal == NULL) return;

    is_expression_and_list* current = ieal;
    bool type = current->is_operation;
    
    if(type){
        //printf("And\n");
        
        check_expression_and_list(symtab, current->next_left);
        check_expression_comp_list(symtab, current->next_right);
    }else{
        check_expression_comp_list(symtab, current->next_right);
        check_expression_and_list(symtab, current->next_left);
    }
}


void check_expression_comp_list(table_element** symtab, is_expression_comp_list * iecl){
    if (iecl == NULL) return;

    // comp_type type = iecl->oper_comp;
    // check_comp_type();

    if (iecl->oper_comp != d_sum_like){
        //print_comp_type(type);
        //Do stuff

        check_expression_comp_list(symtab, iecl->next_left);
        check_expression_sum_like_list(symtab, iecl->next_right);
    }else{

        check_expression_sum_like_list(symtab, iecl->next_right);
        check_expression_comp_list(symtab, iecl->next_left);
    }

}


void check_expression_sum_like_list(table_element** symtab, is_expression_sum_like_list * iesl){
    if (iesl == NULL) return;

    is_expression_sum_like_list * current = iesl;
    sum_like_type type = current->oper_sum_like;

    if (type != d_star_like){
        //print_sum_like(type);

        check_expression_sum_like_list(symtab, current->next_left);
        check_expression_star_like_list(symtab, current->next_right);
    }else{
        check_expression_star_like_list(symtab, current->next_right);
        check_expression_sum_like_list(symtab, current->next_left);
    }
}


void check_expression_star_like_list(table_element** symtab, is_expression_star_like_list * iestl){
    if (iestl == NULL) return;

    is_expression_star_like_list * current = iestl;
    star_like_type type = current->oper_star_like;

    if (type != d_self){
        //print_star_like(type);

        check_expression_star_like_list(symtab, current->next_left);
        check_self_expression_list(symtab, current->next_right);
    }else{
        check_self_expression_list(symtab, current->next_right);
        check_expression_star_like_list(symtab, current->next_left);
    }
}


void check_self_expression_list(table_element** symtab, is_self_expression_list * isel){
    if (isel == NULL) return;

    is_self_expression_list * current = isel;
    self_operation_type type = current->self_oper_type;

    if (type != d_final){
        //print_self_operation_type(type);

        check_self_expression_list(symtab, current->next_same);
        check_final_expression(symtab, current->next_final);
    }else{
        check_final_expression(symtab, current->next_final);
        check_self_expression_list(symtab, current->next_same);
    }
}


void check_final_expression(table_element** symtab, is_final_expression * ife){
    if (ife == NULL) return;

    //{d_intlit, d_reallit, d_id, d_func_inv, d_expr_final} 

    switch (ife->type_final_expression){
        case d_intlit:
            ife->expr.u_intlit->intlit->type = d_integer;
            break;
        case d_reallit:
            ife->expr.u_reallit->reallit->type = d_float32; 
            break;
        case d_id:
            check_id(*symtab, ife->expr.u_id->id);
            break;
        case d_func_inv:
            check_func_invocation(symtab, ife->expr.ifi);
            break;
        case d_expr_final:
            check_expression_or_list(symtab, ife->expr.ieol); 
            break;
        default:
            printf("Erro check_final_expression\n");
            break;
    }
}

// TODO pode ter vários parametros (int, int, bool)
// Aproveitar table_element.type_dec == d_func para ver se é uma função
// e fazer uma lista ligada no id_token.type ?
void check_func_invocation(table_element** symtab, is_function_invocation * ifi){
    table_element* global_symbol = search_symbol(program->symtab, ifi->id->id);
    table_element* func_symbol = search_symbol(*symtab, ifi->id->id);

    //printf("====Check_func_invocation: %s\n====", ifi->id->id);

    // if (global_symbol == NULL && func_symbol == NULL){
    //     print_cannot_find(ifi->id->id, ifi->id->line, ifi->id->col);
    // }
    // else {
    //     ifi->id->type = global_symbol->type;
    // }

    is_func_inv_expr_list * current = ifi->iel;
    while ( current ){
        check_expression_or_list(symtab, current->val);
        current = current->next;
    } 
}

// TODO arranjar esta função
// gera erros de cannot find quando não é suposto
void check_id(table_element* symtab, id_token* id){
    table_element* local_symbol = search_symbol(symtab, id->id);
    table_element* global_symbol = search_symbol(program->symtab, id->id);
    bool in_function_table = 0;
    bool in_global_table = 0;

    //printf("====Check_id: %s\n====", id->id);

    if (local_symbol == NULL) {
        in_function_table = 0;
    } else {
        id->type = local_symbol->type;
    }

    if (global_symbol == NULL) {
        in_global_table = 0;
    } else {
        id->type = global_symbol->type;
    }

    if (!in_function_table && !in_global_table)
        print_cannot_find(id->id, id->line, id->col);
    
}
