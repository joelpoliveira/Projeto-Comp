#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


extern is_program * program;
//extern table_element* symtab;


id_token *create_token(char *id, int line, int col) {
    id_token *idt = (id_token*)malloc(sizeof(id_token));

    idt->id = id;
    idt->line = line;
    idt->col = col;
    idt->uses = 0;

    return idt;
}

next_oper* create_next_oper(int line, int col){
    next_oper* nop = (next_oper*) malloc(sizeof(next_oper));

    nop->line = line;
    nop->col = col;

    return nop;
}

location * create_location(int line, int col){
    location * loc = (location*) malloc(sizeof(location));
    loc->line=line;
    loc->col=col;
    return loc;
}

table_element* insert_symbol(table_element **symtab,  table_element* new_symbol){
    table_element *aux = (table_element*) malloc(sizeof(table_element));
    table_element *previous;

    if (*symtab) {
        for (aux = *symtab; aux; previous = aux, aux = aux->next) {
            //printf("++++++++ %s\n", aux->name);
            if (strcmp(aux->id->id, new_symbol->id->id) == 0) {
                printf("Line %d, column %d: Symbol %s already defined\n", new_symbol->id->line, new_symbol->id->col, new_symbol->id->id);
                free(new_symbol);
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


table_element *insert_func(table_element **symtab, id_token* id, is_parameter * ip, parameter_type return_type) {
    table_element *new_symbol = (table_element *)malloc(sizeof(table_element));
    table_element* func_table = NULL;
    //id_token* temp = id;

    new_symbol->id = id;
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
    new_symbol->id = create_token("return", 0, 0);
    new_symbol->is_param = 0;
    new_symbol->type = return_type;
    //new_symbol->type_dec = NULL;
    new_symbol->next = NULL;
    insert_symbol(&func_table, new_symbol);
    
    //insert parametros
    if (ip != NULL){
        for (is_id_type_list* current = ip->val; current ; current = current->next) {
            new_symbol = (table_element *)malloc(sizeof(table_element));
            //temp = (table_element*)malloc(sizeof(table_element));
            //temp = current->val->id;

            new_symbol->id = current->val->id;
            new_symbol->is_param = 1;
            new_symbol->type = current->val->type_param;
            new_symbol->type_dec = d_var_declaration;
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


table_element *insert_var(table_element **symtab, id_token* id, parameter_type return_type) {
    table_element *new_symbol = (table_element *)malloc(sizeof(table_element));
    //id_token* temp = id;

    new_symbol->id = id;
    new_symbol->is_param = 0;
    new_symbol->type = return_type;
    new_symbol->type_dec = d_var_declaration;
    new_symbol->next = NULL;

    return insert_symbol(symtab, new_symbol);
}


// Procura um identificador, devolve NULL caso nao exista
table_element *search_symbol(table_element *symtab, char *str) {

    for (table_element *aux = symtab; aux; aux = aux->next){
        //printf("======= %s\n", aux->name);
        if (strcmp(aux->id->id, str) == 0){
            aux->id->uses++;
            return aux;
        }
    }

    return NULL;
}


void symbol_print_type(parameter_type type){
    switch (type) {
        case d_integer:
            printf("int");
            break;
        case d_float32:
            printf("float32");
            break;
        case d_string:
            printf("string");
            break;
        case d_bool:
            printf("bool");
            break;
        case d_var:
            printf("var");
            break;
        case d_none:
            printf("none");
            break;
        case d_undef:
            printf("undef");
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
                printf(",");
        }
    }
    printf(")");
} 


void print_global_table(table_element *symtab){
    table_element *aux;
    for (aux = symtab; aux != NULL; aux = aux->next){
        printf("%s\t", aux->id->id);

        if (aux->type_dec == d_func_dec){
            print_function_params_type(get_function_table(program, aux->id->id));
            printf("\t");
        } else
            printf("\t"); 
        
        symbol_print_type(aux->type);
        printf("\n");
    }
}

void print_function_table(table_element *symtab){
    table_element *aux;

    for (aux = symtab; aux != NULL; aux = aux->next){
        printf("%s\t\t", aux->id->id); // id
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


is_func_dec * get_function_declaration(is_program* ip, char* str){
    for(is_declarations_list* current = ip->idlist; current; current = current->next){
        if(current->val->type_dec == d_func_dec){
            if (strcmp(current->val->dec.ifd->id->id, str) == 0)
                return current->val->dec.ifd;
        }
    }
    return NULL;
}


void print_symbol_tables(is_program* ip) {
    //{d_integer, d_float32, d_string, d_bool, d_var, d_none}   parameter_type;
    table_element *aux;

    //print global table
    printf("===== Global Symbol Table =====\n");  
    print_global_table(ip->symtab);

    //print function tables
    for (aux = ip->symtab; aux != NULL; aux = aux->next){
        if (aux->type_dec == d_func_dec){
            printf("\n===== Function %s", aux->id->id);
            print_function_params_type(get_function_table(ip, aux->id->id));
            printf(" Symbol Table =====\n");

            print_function_table(get_function_table(ip, aux->id->id));
        }
    }
    
}


void print_never_used_errors (is_program* ip){
    table_element* aux;

    //Global table
    for(table_element* symtab = ip->symtab; symtab; symtab = symtab->next) {
        if (symtab->type_dec  == d_func_dec){
            for (aux = get_function_table(ip, symtab->id->id); aux; aux = aux->next) {
                if (strcmp(aux->id->id, "return") == 0) continue; // ignore return 

                if (aux->id->uses == 0 && !aux->is_param) {
                    //printf("(%s) ", symtab->id->id);
                    printf("Line %d, column %d: Symbol %s declared but never used\n", aux->id->line, aux->id->col+1,aux->id->id);
                }
            }  
        } 
    }
}

