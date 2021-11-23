#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern table_element *symtab;


// Insere um novo identificador na cauda de uma lista ligada de simbolo
table_element *insert_symbol(table_element **symtab, char *str, parameter_type type) {
    table_element *new_symbol = (table_element *)malloc(sizeof(table_element));
    table_element *aux;
    table_element *previous;

    new_symbol->name = strdup(str);
    new_symbol->type = type;
    new_symbol->next = NULL;

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
    }

    return new_symbol;
}


// Procura um identificador, devolve NULL caso nao exista
table_element *search_symbol(table_element *symtab, char *str) {
    table_element *aux;

    for (aux = symtab; aux; aux = aux->next)
        if (strcmp(aux->name, str) == 0)
            return aux;

    return NULL;
}


void show_table(table_element *symtab) {
    table_element *aux;
    printf("\n");

    for (aux = symtab; aux; aux = aux->next)
        printf("symbol %s, type %d\n", aux->name, aux->type);
}

