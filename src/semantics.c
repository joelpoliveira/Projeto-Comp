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
    //TODO create new symbtab for functions (return new symtab?)
    table_element* new_symbol = NULL;

    new_symbol = insert_func(symtab, ifd->id->id, ifd->ipl, ifd->type);

    if (new_symbol == NULL){
        print_already_defined(ifd->id->id, ifd->id->line, ifd->id->col);
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
