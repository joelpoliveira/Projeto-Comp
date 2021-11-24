#include "semantics.h"
#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


void print_already_defined(char* id, int line, int col){
    printf("Line %d, column %d: Symbol %s already defined\n", line, col, id);
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
    table_element* new_symbol = NULL;

    new_symbol = insert_func(symtab, ifd->id->id, ifd->ipl, ifd->type);

    if (new_symbol == NULL){
        print_already_defined(ifd->id->id, ifd->id->line, ifd->id->col);
        return;
    }

    //possilvemente erro aqui |
    //printf("++++++++++++++++%p\n", ifd->symtab);
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
                check_statement(current->val->body.is);
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


void check_statement(is_statement* is){
    //{d_if, d_for, d_return, d_print, d_assign, d_statement_list, d_final_statement
    switch (is->type_state){
        case d_if:
            check_if_statement(is->statement.u_if_state);
            break;
        case d_for:
            check_for_statement(is->statement.u_for_state);
            break;
        case d_return:
            check_return_statement(is->statement.u_return_state);
            break;
        case d_print:
            check_print_statement(is->statement.u_print_state);
            break;
        case d_assign:
            check_assign_statement(is->statement.u_assign);
            break;
        case d_statement_list:
            check_statements_list(is->statement.isl);
            break;
        case d_final_statement:
            check_final_statement(is->statement.u_state);
            break;
        
        default:
            printf("Erro check_statement\n");
            break;
    }
}


void check_if_statement(is_if_statement* ifs){
    
}


void check_for_statement(is_for_statement* ifs){

}


void check_return_statement(is_return_statement* irs){

}


void check_print_statement(is_print_statement* ips){

}


void check_assign_statement(is_assign_statement* ias){

}


void check_statements_list(is_statements_list* isl){

}


void check_final_statement(is_final_statement* ifs){

}