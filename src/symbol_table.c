#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//extern table_element *symtab;


table_element* insert_symbol(table_element **symtab, char* str, table_element* new_symbol){
    table_element *aux;
    table_element *previous;

    if (*symtab) {
        for (aux = *symtab; aux; previous = aux, aux = aux->next) {
            if (strcmp(aux->name, str) == 0) {
                free(new_symbol->name);
                free(new_symbol);
                return NULL;
            }
        }
        previous->next = new_symbol;
    } else {
        *symtab = new_symbol;
        return new_symbol;
    }
}


table_element *insert_func(table_element **symtab, char* str, is_parameter * ip, parameter_type return_type) {
    table_element *new_symbol = (table_element *)malloc(sizeof(table_element));

    //{d_func_dec, d_var_declaration} declaration_type;
    is_id_type_list* current;
    table_element_params* last_param = (table_element_params*)malloc(sizeof(table_element_params));
 
    new_symbol->name = strdup(str);
    new_symbol->type = return_type;
    new_symbol->dec = d_func_dec;
    new_symbol->next = NULL;

    //insert params (linked list)
    if (ip != NULL){
        for (is_id_type_list* current = ip->val; current != NULL; current = current->next) {
            table_element_params* param = (table_element_params*)malloc(sizeof(table_element_params));

            param->name = current->val->id->id;
            param->type = current->val->type_param;
            param->next = NULL;

            if (new_symbol->params == NULL){
                new_symbol->params = param;
                last_param = param;
            } else {
                last_param->next = param;
                last_param = param;
            }
        } 
    } else {
        new_symbol->params = NULL;
    }

    return insert_symbol(symtab, str, new_symbol);
}


table_element *insert_var(table_element **symtab, char* str, parameter_type return_type) {
    table_element *new_symbol = (table_element *)malloc(sizeof(table_element));

    is_id_type_list* current;
    table_element_params* last_param = (table_element_params*)malloc(sizeof(table_element_params));
 
    new_symbol->name = strdup(str);
    new_symbol->type = return_type;
    new_symbol->dec = d_var_declaration;
    new_symbol->next = NULL;

    return insert_symbol(symtab, str, new_symbol);
}


// Procura um identificador, devolve NULL caso nao exista
table_element *search_symbol(table_element *symtab, char *str) {
    table_element *aux;

    for (aux = symtab; aux; aux = aux->next)
        if (strcmp(aux->name, str) == 0)
            return aux;

    return NULL;
}


void symbol_print_type(parameter_type type){
    switch (type) {
        case 0:
            printf("int");
            break;
        case 1:
            printf("float32");
            break;
        case 2:
            printf("string");
            break;
        case 3:
            printf("bool");
            break;
        case 4:
            printf("var");
            break;
        case 5:
            printf("none");
            break;
        default:
            printf("Erro show_table\n");
            break;
    }
}


void show_table(table_element *symtab) {
    //{d_integer, d_float32, d_string, d_bool, d_var, d_dummy}   parameter_type;
    table_element *aux;
    table_element_params *param_list;

    printf("\n===== Global Symbol Table =====\n");

    for (aux = symtab; aux != NULL; aux = aux->next){
        printf("%s\t", aux->name);
        
        switch (aux->dec){
            case d_func_dec:
                printf("(");
                if (aux->params != NULL){
                    for (param_list = aux->params; param_list != NULL; param_list = param_list->next) {
                        symbol_print_type(param_list->type);
                        if (param_list->next != NULL)
                            printf(", ");
                    }
                }
                printf(")\t");
                break;
            case d_var_declaration:
                //nao era preciso um switch mas n me apetece apagar :D
                break;
            default:
            printf("Erro print aux->dec\n");
                break;
        }
        
        symbol_print_type(aux->type);
        printf("\n");
    }

    printf("\n===== Function <function>(type) Symbol Table =====\n");
    
}

