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
    printf("\tret ");
    llvm_print_type(ifd->type);
    if (ifd->type != d_none)
        printf(" 0");
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
                //llvm_statement(current->val->body.is, counter);
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
