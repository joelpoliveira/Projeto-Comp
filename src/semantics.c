#include "semantics.h"
#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


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
                //check_var_declaration();
                break;
        default:
            printf("Erro check_declarations_list");
            break;
        }
    }
}


void check_func_declaration(table_element** symtab, is_func_dec* ifd){
    table_element* new_symbol = insert_symbol(symtab, ifd->id->id, ifd->type);

    if (new_symbol == NULL){
        printf("Symbol %s already defined! (line %d, col %d)\n", ifd->id->id, ifd->id->line, ifd->id->col);
    }

}
