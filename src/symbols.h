#ifndef SYMBOLS_H
#define SYMBOLS_H

#include "structures.h"


typedef struct _t1 {
    char *name;
    parameter_type type;
    struct _t1 *next;
} table_element;


#endif