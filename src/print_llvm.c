#include "print_llvm.h"
#include "symbol_table.h"
#include "y.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


extern is_program* program;
int global_counter = 0;
int func_counter = 0;


void llvm_print_type(parameter_type type){
    switch (type) {
        case d_integer:
            printf("i32");
            break;
        case d_float32:
            printf("i32");
            break;
        case d_string:
            printf("i4");
            break;
        case d_bool:
            printf("i1");
            break;
        case d_none:
            printf("void");
            break;
        case d_undef:
            break;
        default:
            break;
    }
}


void llvm_program(is_program* ip){
    llvm_declarations_list(ip->idlist);
}


void llvm_declarations_list(is_declarations_list* idl){
    int counter = 0;

    for(is_declarations_list* current = idl; current != NULL; current = current->next) {
        switch (current->val->type_dec){
            case d_func_dec:
                llvm_func_declaration(current->val->dec.ifd);
                break;
            case d_var_declaration:
                llvm_var_declaration(current->val->dec.ivd, true);
                break;
        default:
            printf("Erro llvm_declarations_list");
            break;
        }
    }
}


void llvm_func_declaration(is_func_dec* ifd){
    func_counter = 0;
    table_element* symbol;

    printf("define ");
    llvm_print_type(ifd->type);
    printf(" @%s(", ifd->id->id);

    //parametros
    llvm_is_parameter(ifd->ipl);

    printf(") {\n");

    //body
    func_counter++;
    llvm_function_body(ifd->ifb);

    //return
    symbol = search_symbol(program->symtab ,ifd->id->id);
    printf("\tret ");
    llvm_print_type(ifd->type);
    if (ifd->type != d_none)
        printf(" %d", symbol->type);

    printf("\n}\n");
}


void llvm_function_body(is_func_body* ifb){
    llvm_vars_and_statements_list(ifb->ivsl);
}


void llvm_is_parameter(is_parameter * ip) {
    if (ip == NULL) return;

    for (is_id_type_list* temp = ip->val; temp; temp = temp->next){
        llvm_print_type(temp->val->id->type);
        printf(" %%%d", func_counter++);
        if (temp->next != NULL)
            printf(", ");
    }

}


void llvm_vars_and_statements_list(is_vars_and_statements_list* ivsl){
    is_vars_and_statements_list* current;

    for(current = ivsl; current != NULL; current = current->next) {
        //{d_var_dec, d_statement} var_or_statement;
        switch (current->val->type){
            case d_var_dec:
                llvm_var_declaration(current->val->body.ivd, false);
                break;
            case d_statement:
                llvm_statement(current->val->body.is);
                break;
        default:
            printf("Erro llvm_vars_and_statements_list");
            break;
        }
    }
}


void llvm_var_declaration(is_var_dec* ivd, bool is_global){
    llvm_var_spec(ivd->ivs, is_global);
}


void llvm_var_spec(is_var_spec* ivs, bool is_global){

    for (is_id_list* current = ivs->iil; current; current = current->next){

        if(is_global){
            printf("@%s = global ", current->val->id);
            llvm_print_type(current->val->type);
            printf(" 0\n");
        } else {
            printf("\t%%%d = alloca ", func_counter++);
            llvm_print_type(current->val->type);
            //printf("(%s)", current->val->id);
            printf("\n");
        }
    }
}


void llvm_statement( is_statement* is){
    //{d_if, d_for, d_return, d_print, d_assign, d_statement_list, d_final_statement
    switch (is->type_state){
        case d_if:
            //llvm_if_statement(is->statement.u_if_state);
            break;
        case d_for:
            //llvm_for_statement(is->statement.u_for_state);
            break;
        case d_return:
            llvm_return_statement(is->statement.u_return_state);
            break;
        case d_print:
            //llvm_print_statement(is->statement.u_print_state);
            break;
        case d_assign:
            //llvm_assign_statement(is->statement.u_assign);
            break;
        case d_statement_list:
            //llvm_statements_list(is->statement.isl);
            break;
        case d_final_statement:
            //llvm_final_statement(is->statement.u_state);
            break;
        
        default:
            printf("Erro llvm_statement\n");
            break;
    }
}


void llvm_if_statement(is_if_statement* ifs){
    if (ifs == NULL) return;

    // id_token * ltoken = llvm_expression_or_list(ifs->iel);
    // bool error_ocurred = llvm_or_err(ifs->iel);

    // if (ifs->iel != NULL) {
    //     if(ifs->iel->expression_type != d_bool && !error_ocurred){
    //         printf("Line %d, column %d: Incompatible type ", ltoken->line, ltoken->col+1);
    //         symbol_print_type(ifs->iel->expression_type);
    //         printf(" in if statement\n");
    //     }
    // }

    // llvm_statements_list(ifs->isl);
    // llvm_else_statement(ifs->ies);
}


void llvm_else_statement(is_else_statement* ies){
    if (ies == NULL) return;

    //llvm_statements_list(ies->isl);
}


void llvm_for_statement(is_for_statement* ifs){
   
    // id_token * ltoken = llvm_expression_or_list(ifs->iel);
    // bool error_ocurred = llvm_or_err(ifs->iel);

    // if (ifs->iel != NULL) {
    //     if (ifs->iel->expression_type != d_bool && !error_ocurred){
    //         printf("Line %d, column %d: Incompatible type ", ltoken->line, ltoken->col + 1);
    //         symbol_print_type(ifs->iel->expression_type);
    //         printf(" in for statement\n");
    //     }
    // }
    
    // llvm_statements_list(ifs->isl);
}


void llvm_return_statement(is_return_statement* irs){
    // table_element* temp = search_symbol(*"return");
    //printf("-----RETURN----\n");
    
    // id_token * ltoken = llvm_expression_or_list(irs->iel);
    // bool error_ocurred = llvm_or_err(irs->iel);
    // if (temp != NULL && irs->iel != NULL){
    //     if ( ( 
    //             (irs->iel->expression_type != temp->type) 
    //             || (irs->iel->expression_type == d_undef) 
    //         ) && !error_ocurred){
    //         printf("Line %d, column %d: Incompatible type ", ltoken->line, ltoken->col+1);
    //         symbol_print_type(irs->iel->expression_type);
    //         printf(" in return statement\n");
    //     }   
    // }

}


void llvm_print_statement(is_print_statement* ips){
    if (ips == NULL) return;

    // print_type type = ips->type_print; // {d_expression, d_str}
    // id_token * token;
    // switch (type){
    //     case d_expression:
    //         #ifdef DEBUG
    //         printf("======== llvm_print_statement(expression): %s ========\n", ips->print.id->id);
    //         #endif
            
    //         token = llvm_expression_or_list(ips->print.iel);
    //         llvm_or_err(ips->print.iel);
    //         if (ips->print.iel->expression_type==d_undef){
    //             printf("Line %d, column %d: Incompatible type ", token->line, token->col+1);
    //             print_parameter_type_(ips->print.iel->expression_type);
    //             printf(" in fmt.Println statement\n");
    //             return;
    //         }
    //         break;
    //     case d_str:
    //         #ifdef DEBUG
    //         printf("======== llvm_print_statement(d_str) ========\n");
    //         #endif
    //         break;
    //     default:
    //         printf("Erro llvm_print_statement\n");
    //         break;
    // }
}


void llvm_assign_statement(is_assign_statement* ias){

    
}


void llvm_statements_list(is_statements_list* isl){
    if (isl == NULL) return;
    
    for (is_statements_list * current = isl; current; current = current->next){
        //llvm_statement(current->val);
    }

}


void llvm_final_statement(is_final_statement* ifs){
    if (ifs == NULL) return;

    
}
