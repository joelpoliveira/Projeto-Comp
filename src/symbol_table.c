#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern is_program * program;

id_token *create_token(char *id, int line, int col) {
    id_token *idt = malloc(sizeof(id_token));

    idt->id = id;
    idt->line = line;
    idt->col = col;
    
    return idt;
}


table_element* insert_symbol(table_element **symtab, char* str, table_element* new_symbol){
    table_element *aux = (table_element*) malloc(sizeof(table_element));
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
        //return new_symbol;
    }
    //printf("================Inserted %s\n", new_symbol->name);
    return new_symbol;
}


table_element *insert_func(table_element **symtab, char* str, is_parameter * ip, parameter_type return_type) {
    table_element *new_symbol = (table_element *)malloc(sizeof(table_element));

    //{d_func_dec, d_var_declaration} declaration_type;
    is_id_type_list* current;
    table_element_params* last_param = (table_element_params*)malloc(sizeof(table_element_params));
 
    new_symbol->name = strdup(str);
    new_symbol->type = return_type;
    new_symbol->type_dec = d_func_dec;
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
    new_symbol->type_dec = d_var_declaration;
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


void print_function_params(table_element *symtab) {
    table_element_params *param_list;
    printf("(");
    if (symtab->params != NULL){
        for (param_list = symtab->params; param_list != NULL; param_list = param_list->next) {
            symbol_print_type(param_list->type);
            if (param_list->next != NULL)
                printf(", ");
        }
    }
    printf(")");
} 


void print_table(table_element *symtab){
    table_element *aux;
    for (aux = symtab; aux != NULL; aux = aux->next){
        printf("%s\t", aux->name);
        
        switch (aux->type_dec){
            case d_func_dec:
                print_function_params(aux);
                printf("\t");
                break;
            case d_var_declaration:
                //nao era preciso um switch mas n me apetece apagar :D
                break;
            default:
            printf("Erro print_table\n");
                break;
        }
        
        symbol_print_type(aux->type);
        printf("\n");
    }
}


// Função para dar return á tabela de simbolos de uma função
//return NULL caso a função não exista
table_element* get_function_table(is_program* ip, char* str){
    for(is_declarations_list* current = ip->idlist; current; current = current->next){
        if(current->val->type_dec == d_func_dec){
            if (strcmp(current->val->dec.ifd->id->id, str) == 0)
                return current->val->dec.ifd->symtab;
        }
    }
    return NULL;
}


void print_symbol_tables(is_program* ip) {
    //{d_integer, d_float32, d_string, d_bool, d_var, d_dummy}   parameter_type;
    table_element *aux;
    table_element_params *param_list;

    //print global table
    printf("\n===== Global Symbol Table =====\n");  
    print_table(ip->symtab);

    //print function tables
    for (aux = ip->symtab; aux != NULL; aux = aux->next){
        if (aux->type_dec == d_func_dec){
            printf("\n===== Function %s ", aux->name);
            print_function_params(aux);
            printf(" Symbol Table =====\n");

            print_table(get_function_table(ip, aux->name));
        }
    }
    
}

