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
        print_func_dec(idl->val->dec.ifd);
        //print_var_dec(idl->val->dec.ivd);
        current = current->next;
    }

}

void print_func_dec(is_func_dec* ifd){
    printf("..FuncHeader\n");
    printf("....Id(%s)\n" , ifd->id ); // SEGMENTAION FAULT AQUI
    print_parameter_type(ifd->type); // SEGMENTAION FAULT AQUI
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
    is_id_type_list* current = ipl->val;
    while (current != NULL) {
        printf("......");
        print_parameter_type(current->val->type_param);
        printf("%s\n", current->val->id);
        current = current->next;
    }
}

void print_func_body(is_func_body* ifb){
    is_vars_and_statements_list* current = ifb->ivsl;
    while (current != NULL) {
        printf("......");
        print_var_or_statement(current->val);
        current = current->next;
    }
}

void print_var_or_statement(is_var_or_statement* val){
    //d_var_dec, d_statement
    switch (val->type){
    case d_var_dec:
        //print_var_dec(val->body.ivd);
        print_var_spec(val->body.ivd->ivs);
        break;

    case d_statement:
        print_statement(val->body.is);
        break;
    }
}

void print_var_spec(is_var_spec* ivs){
    is_var_spec* current = ivs;
    while (current != NULL){
        //TODO
        current = current->iil->next;
    }
}

void print_statement(is_statement* is) {

}




