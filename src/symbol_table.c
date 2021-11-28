#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


extern is_program * program;
//extern table_element* symtab;


id_token *create_token(char *id, int line, int col) {
    id_token *idt = malloc(sizeof(id_token));

    idt->id = id;
    idt->line = line;
    idt->col = col;
    
    return idt;
}


table_element* insert_symbol(table_element **symtab,  table_element* new_symbol){
    table_element *aux = (table_element*) malloc(sizeof(table_element));
    table_element *previous;

    if (*symtab) {
        for (aux = *symtab; aux; previous = aux, aux = aux->next) {
            //printf("++++++++ %s\n", aux->name);
            if (strcmp(aux->name, new_symbol->name) == 0) {
                free(new_symbol);
                //printf("--------Alredy existed\n");
                return NULL;
            }
        }
        previous->next = new_symbol;
    } else {
        *symtab = new_symbol;
    }
    //printf("================Inserted %s\n", new_symbol->name);
    return new_symbol;
}


table_element *insert_func(table_element **symtab, char* str, is_parameter * ip, parameter_type return_type) {
    table_element *new_symbol = (table_element *)malloc(sizeof(table_element));
    table_element* func_table = NULL;

    new_symbol->name = strdup(str);
    new_symbol->is_param = 0;
    new_symbol->type = return_type;
    new_symbol->type_dec = d_func_dec;
    new_symbol->next = NULL;

    //inserir nome da função
    new_symbol = insert_symbol(symtab, new_symbol);
    if (new_symbol == NULL){
        return NULL;
        free(func_table);
    }

    //inserir return
    new_symbol = (table_element *)malloc(sizeof(table_element));
    new_symbol->name = "return";
    new_symbol->is_param = 0;
    new_symbol->type = return_type;
    //new_symbol->type_dec = NULL;
    new_symbol->next = NULL;
    insert_symbol(&func_table, new_symbol);
    
    //insert parametros
    if (ip != NULL){
        for (is_id_type_list* current = ip->val; current ; current = current->next) {
            new_symbol = (table_element *)malloc(sizeof(table_element));

            new_symbol->name = strdup(current->val->id->id);
            new_symbol->is_param = 1;
            new_symbol->type = return_type;
            new_symbol->type_dec = d_func_dec;
            new_symbol->next = NULL;

            //search_symbol(program->symtab, str);
            new_symbol = insert_symbol(&func_table, new_symbol);
            if (new_symbol == NULL){
                return NULL;
            }
        } 
    } 

    return func_table;
}


table_element *insert_var(table_element **symtab, char* str, parameter_type return_type) {
    table_element *new_symbol = (table_element *)malloc(sizeof(table_element));

    new_symbol->name = strdup(str);
    new_symbol->is_param = 0;
    new_symbol->type = return_type;
    new_symbol->type_dec = d_var_declaration;
    new_symbol->next = NULL;

    return insert_symbol(symtab, new_symbol);
}


// Procura um identificador, devolve NULL caso nao exista
table_element *search_symbol(table_element *symtab, char *str) {
    table_element *aux;

    for (aux = symtab; aux; aux = aux->next){
        //printf("======= %s\n", aux->name);
        if (strcmp(aux->name, str) == 0){
            return aux;
        }
    }

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


void print_function_params_type(table_element *symtab) {
    table_element* aux;
    printf("(");
    for (aux = symtab; aux ; aux = aux->next) {
        //printf("++++++%s", aux->name);
        if(aux->is_param){
            symbol_print_type(aux->type);
            if (aux->next != NULL && aux->next->is_param)
                printf(", ");
        }
    }
    printf(")");
} 


void print_global_table(table_element *symtab){
    table_element *aux;
    for (aux = symtab; aux != NULL; aux = aux->next){
        printf("%s\t", aux->name);

        if (aux->type_dec == d_func_dec){
            print_function_params_type(get_function_table(program, aux->name));
            printf("\t");
        }
        
        symbol_print_type(aux->type);
        printf("\n");
    }
}

void print_function_table(table_element *symtab){
    table_element *aux;

    for (aux = symtab; aux != NULL; aux = aux->next){
        printf("%s\t\t", aux->name); // id
        symbol_print_type(aux->type); //tipo
        if (aux->is_param)
            printf("\tparam");
        printf("\n");
    }
}


// Função para dar return á tabela de simbolos de uma função
// return NULL caso a função não exista
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

    //print global table
    printf("===== Global Symbol Table =====\n");  
    print_global_table(ip->symtab);

    //print function tables
    for (aux = ip->symtab; aux != NULL; aux = aux->next){
        if (aux->type_dec == d_func_dec){
            printf("\n===== Function %s", aux->name);
            print_function_params_type(get_function_table(ip, aux->name));
            printf(" Symbol Table =====\n");

            print_function_table(get_function_table(ip, aux->name));
        }
    }
    
}

