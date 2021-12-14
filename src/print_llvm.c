#include "print_llvm.h"
#include "symbol_table.h"
#include "y.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>


extern is_program* program;
int global_counter = 0;
int func_counter = 0;
int string_counter = 0;
bool declare_print = 0, print_done = 0;

// store b->type %a, b->type* %b
// store a in b*

bool is_digit(char c){
    return (c>='0' && c<='9') || c=='%';
}

size_t ndigits(int number){
    if (number == 0) return 1;

    size_t digits = 0;
    for (;number;number/=10)
        digits++;
    return digits;
}

int _max(int a, int b){
    return (a>b)?a:b;
}

llvm_func_parameters* append_param(llvm_func_parameters * head, char * string){
    llvm_func_parameters*new = malloc(sizeof(llvm_func_parameters));
    new->var_id = string;
    new->next=NULL;

    if (head == NULL)
        return new;
    for (;head->next; head = head->next);
    head->next = new;
    return head;
}

void free_param_list(llvm_func_parameters * head){
    llvm_func_parameters * aux;
    for (aux = head->next; aux; aux = aux->next){
        free(head);
        head = aux;
    }
}

int get_next_var(llvm_func_parameters * head, int nvar_now){
    int next = -1;
    for (llvm_func_parameters * aux = head; aux; aux = aux->next){
        if (aux->var_id[0] == '%'){
            next = atoi(aux->var_id+1);
        }
    }
    return next==-1?nvar_now:next+1;
}

void llvm_store(id_token* a, id_token* b){
    printf("\tstore ");
    llvm_print_type(a->type);
    printf(" %%%s, ", a->id);
    llvm_print_type(b->type);
    printf("* %%%s\n", b->id);
}

void llvm_expr_store(id_token* a, char* b){
    printf("\tstore ");
    llvm_print_type(a->type);
    ( is_digit(b[0]) )? printf(" %s, ", b) : printf(" %%%s, ", b);
    llvm_print_type(a->type);
    ( is_digit(a->id[0]) )? printf("* %s\n", a->id) : printf("* %%%s\n", a->id);
}

// %<num> = load a->type, a->type* %a
void llvm_load(id_token* a){
    printf("\t%%%d = load ", func_counter++);
    llvm_print_type(a->type);
    printf(", ");
    llvm_print_type(a->type);
    printf("* %%%s\n", a->id);
}


// %<num> = add a->type, %a, %b
void llvm_add(id_token* a, id_token* b){
    printf("\t%%%d = add ", func_counter++);
    llvm_print_type(a->type);
    printf(" %%%s, %%%s\n", b->id, a->id);
}


void llvm_mul(id_token* a){
    printf("\t%%%d = mul ", func_counter++);
    llvm_print_type(a->type);
    printf(" %%%d, %s\n", func_counter - 2, a->id);
}


void llvm_div(id_token* a){
    printf("\t%%%d = div ", func_counter++);
    llvm_print_type(a->type);
    printf(" %%%d, %s\n", func_counter - 2, a->id);
}



void llvm_print_type(parameter_type type){
    switch (type) {
        case d_integer:
            printf("i32");
            break;
        case d_float32:
            printf("double");
            break;
        case d_string:
            printf("i8");
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
    //print global variables
    for(table_element* current = ip->symtab; current; current = current->next) {
        if (current->type_dec == d_var_declaration){
            printf("@%s = global ", current->id->id);
            llvm_print_type(current->type);
            printf(" 0");
            printf("\n");   
        }
    }

    //Print strings declarations
    for(table_element* current = ip->strings_table; current != NULL; current = current->next){
        llvm_string_dec(current->id);
    }


    string_counter = 0;
    printf("\n");
    llvm_declarations_list(ip->idlist);
}


void llvm_string_dec(id_token* id){
    int size = strlen(id->id) - 2; // vem com aspas
    char aux[size];//, tmp[size*2];
    char* tmp = (char*)calloc(sizeof(char), size*2); // TODO fix this. aumentar dinamicamente
    //table_element *str = search_symbol(program->strings_table, aux);
    
    strcpy(aux, id->id);

    int i = 0;
    int x = 0; // ofset para tmp se encontrar \n em aux pq tmp vai ter um char a mais
    while (aux[i] != '\0') {

        if (aux[i] == '\\' && aux[i+1] == 'n'){
            strcat(tmp, "\\0A");
            i++;
            x++;
        } else
            tmp[i + x] = aux[i];
        i++;
    }

    tmp[i+x-1] = 0;

    if (string_counter == 0)
        printf("@.str");
    else
        printf("@.str.%d", string_counter); 

    printf(" = private unnamed_addr constant [%d x i8] c%s", string_size(aux), tmp);

    printf("\\00\"\n");

    string_counter++;

    //Alterar a string da tabela para ter o /0A
    //sprintf(id->id, "%s", tmp);
}


int llvm_string_size(char* s){
    int size = 0;

    for(int i = 0; s[i] != 0; i++){
        if (s[i+2] != 0) {
            if (s[i] == '\"') continue;

            if(s[i] == '\\' && s[i+1] == '0' && s[i+2] == 'A'){
                size++;
                i+=2;
            } if (s[i] == '\\' && s[i+1] == '0' && s[i+2] == '0') {
                size++;
                i+=2;
            } else
                size++;
        }

    }

    return size;
}

//abc\n123\n


int string_size(char* s){
    int size = 0;

    for(int i = 0; s[i] != 0; i++){
        if (s[i] == '\"') continue;
        
        if (s[i] == '\\' && s[i+1] == 'n'){
            size += 2;
            i+=2;
        } else
            size++;
    }

    return size;
}


void llvm_declarations_list(is_declarations_list* idl){
    for(is_declarations_list* current = idl; current; current = current->next) {
        if (current->val->type_dec == d_func_dec){
            llvm_func_declaration(current->val->dec.ifd);
        }
    }
}


void llvm_func_declaration(is_func_dec* ifd){
    func_counter = 0;
    table_element* symbol;

    printf("define ");
    llvm_print_type(ifd->type);
    printf(" @%s(", ifd->id->id);

    //parametros
    llvm_is_parameter(ifd->ipl);

    printf(") {\n");

    //printf("entry:\n");

    //body
    func_counter++;
    llvm_function_body(ifd->ifb);

    printf("\n}\n\n");

    if (declare_print && !print_done) {
        print_done = 1;
        printf("declare i32 @printf(i8*, ...)\n\n");
    }
}


void llvm_function_body(is_func_body* ifb){
    llvm_vars_and_statements_list(ifb->ivsl);
}


void llvm_is_parameter(is_parameter * ip) {
    if (ip == NULL) return;

    for (is_id_type_list* temp = ip->val; temp; temp = temp->next){
        llvm_print_type(temp->val->id->type);
        printf(" %%%s", temp->val->id->id);
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
                llvm_var_declaration(current->val->body.ivd);
                break;
            case d_statement:
                llvm_statement(current->val->body.is);
                break;
        default:
            printf("Erro llvm_vars_and_statements_list");
            break;
        }
    }
}


void llvm_var_declaration(is_var_dec* ivd){
    llvm_var_spec(ivd->ivs);
}


void llvm_var_spec(is_var_spec* ivs){

    for (is_id_list* current = ivs->iil; current; current = current->next){

        printf("\t%%%s = alloca ", current->val->id);
        llvm_print_type(current->val->type);
        //printf("(%s)", current->val->id);
        printf("\n");

    }
}


void llvm_statement( is_statement* is){
    //{d_if, d_for, d_return, d_print, d_assign, d_statement_list, d_final_statement
    switch (is->type_state){
        case d_if:
            llvm_if_statement(is->statement.u_if_state);
            break;
        case d_for:
            //llvm_for_statement(is->statement.u_for_state);
            break;
        case d_return:
            llvm_return_statement(is->statement.u_return_state);
            break;
        case d_print:
            llvm_print_statement(is->statement.u_print_state);
            break;
        case d_assign:
            llvm_assign_statement(is->statement.u_assign);
            break;
        case d_statement_list:
            //llvm_statements_list(is->statement.isl);
            break;
        case d_final_statement:
            //llvm_final_statement(is->statement.u_state);
            break;
        
        default:
            printf("Erro llvm_statement\n");
            break;
    }
}


void llvm_if_statement(is_if_statement* ifs){
    if (ifs == NULL) return;

    //llvm_expression_or_list(ifs->iel);
    // if x then else
    printf("\tbr i1 %%x, label %%then, label %%else\n");

    printf("then:\n");
    llvm_statements_list(ifs->isl);

    printf("else\n");
    llvm_else_statement(ifs->ies);
}


void llvm_else_statement(is_else_statement* ies){
    if (ies == NULL) return;

    //llvm_statements_list(ies->isl);
}


void llvm_for_statement(is_for_statement* ifs){
   
    // id_token * llvm_expression_or_list(ifs->iel);
    // bool error_ocurred = llvm_or_err(ifs->iel);

    // if (ifs->iel != NULL) {
    //     if (ifs->iel->expression_type != d_bool && !error_ocurred){
    //         printf("Line %d, column %d: Incompatible type ", ltoken->line, ltoken->col + 1);
    //         symbol_print_type(ifs->iel->expression_type);
    //         printf(" in for statement\n");
    //     }
    // }
    
    // llvm_statements_list(ifs->isl);
}


void llvm_return_statement(is_return_statement* irs){
    table_element* symbol;
    //return
    //symbol = search_symbol(program->symtab ,ifd->id->id);
    //llvm_load();
    printf("\tret 0");
    // llvm_print_type(ifd->type);
    // if (ifd->type != d_none)
    //     printf(" %d", symbol->type);

}


void llvm_print_statement(is_print_statement* ips){
    if (ips == NULL) return;

    declare_print = 1;
    int num;

    //print_type type = ips->type_print; // {d_expression, d_str}

    switch (ips->type_print){
        case d_expression:
            llvm_expression_or_list(ips->print.iel, NULL, 1);
            break;
        case d_str:
            num = llvm_get_string_num(ips->print.id->id);
            if (num != -1)
                llvm_print(num, ips->print.id->id);
            else
                printf("Não existe\n");
            break;
        default:
            printf("Erro llvm_print_statement\n");
            break;
    }
}


int llvm_get_string_num(char* string){
    table_element* aux;
    int num = 0;

    for (table_element* aux = program->strings_table; aux; aux = aux->next) {
        if (strcmp(aux->id->id, string) == 0){
            return num;
        }
        num++;
    }

    return -1;
}


void llvm_print(int num, char* string){
    table_element *str = search_symbol(program->strings_table, string);
    if(str == NULL) return;

    int size = string_size(str->id->id);

    printf("\t%%%d = call i32(i8*, ...) ", func_counter++); 
    printf("@printf(i8* getelementptr inbounds");
    printf("([%d x i8], [%d x i8]* ", size, size);
    if (num == 0)
        printf(" @.str)");
    else
        printf(" @.str.%d), ", num);
    printf("i64 0, i64 0))\n");
    
}  


void llvm_assign_statement(is_assign_statement* ias){
    id_token* token = (id_token*)malloc(sizeof(id_token));
    token = ias->id;

    char * res_token;
    res_token = llvm_expression_or_list(ias->iel, ias->id, 1);
    
    
    //printf("\t%%%s = add ", token->id);
    //llvm_print_type(token->type);
    //(is_digit(res_token[0]))? printf(" %s, 0\n", res_token): printf(" %%%s, 0\n", res_token);

    
    //sprintf(token->id, "%d", func_counter-1);
    llvm_expr_store(token, res_token);
}


void llvm_statements_list(is_statements_list* isl){
    if (isl == NULL) return;
    
    for (is_statements_list * current = isl; current; current = current->next){
        llvm_statement(current->val);
    }

}


void llvm_final_statement(is_final_statement* ifs){
    if (ifs == NULL) return;

    
}


char *  llvm_expression_or_list(is_expression_or_list* ieol, id_token* aux, int nvar_now){
    if (ieol == NULL) return NULL;
    
    char * ltoken, * rtoken, *ret;
    if (ieol->is_operation!=NULL){

        ltoken = llvm_expression_or_list(ieol->next_left, aux, nvar_now);
        rtoken = llvm_expression_and_list(ieol->next_right, aux, ltoken[0]!='%' ? nvar_now: atoi((ltoken+1)) + 1);   
        printf("\t");
        int next;
        if (ltoken[0]=='%' && rtoken[0]=='%')
            next = _max(atoi( (rtoken + 1)) + 1, atoi( ( ltoken + 1 ) ) + 1);
        else if (ltoken[0] == '%')
            next = atoi(ltoken+1)+1;
        else if (rtoken[0]=='%')
            next = atoi(rtoken+1)+1;
        else
            next = nvar_now;

        printf("%%%d = or ", next);
        llvm_print_type(ieol->expression_type);
        ( is_digit(ltoken[0]) ) ? printf(" %s, ", ltoken) : printf(" %%%s, ", ltoken);
        ( is_digit(rtoken[0]) ) ? printf(" %s\n", rtoken) : printf(" %%%s\n", rtoken);
        
        ret = (char *) malloc( ndigits(next) + 2 );
        sprintf(ret, "%%%d", next);
        return ret;

    }else{
        return llvm_expression_and_list(ieol->next_right, aux, nvar_now);
    } 
}


char * llvm_expression_and_list(is_expression_and_list* ieal, id_token* aux, int nvar_now){
    if (ieal == NULL) return NULL;

    is_expression_and_list* current = ieal;
    next_oper* type = current->is_operation;
    char * ltoken, * rtoken, *ret;

    if(type!=NULL){

        ltoken = llvm_expression_and_list(current->next_left, aux, nvar_now);
        rtoken = llvm_expression_comp_list(current->next_right, aux,  ltoken[0]!='%' ? nvar_now: atoi((ltoken+1)) + 1);
        printf("\t");
        int next;
        if (ltoken[0]=='%' && rtoken[0]=='%')
            next = _max(atoi( (rtoken + 1)) + 1, atoi( ( ltoken + 1 ) ) + 1);
        else if (ltoken[0] == '%')
            next = atoi(ltoken+1)+1;
        else if (rtoken[0]=='%')
            next = atoi(rtoken+1)+1;
        else
            next = nvar_now;

        printf("%%%d = and ", next);
        llvm_print_type(current->expression_type);
        ( is_digit(ltoken[0]) ) ? printf(" %s, ", ltoken) : printf(" %%%s, ", ltoken);
        ( is_digit(rtoken[0]) ) ? printf(" %s\n", rtoken) : printf(" %%%s\n", rtoken);
        
        ret = (char *) malloc( ndigits(next) + 2 );
        sprintf(ret, "%%%d", next);
        return ret;
    }else{
        return llvm_expression_comp_list(current->next_right, aux, nvar_now);
    }  
    return "";  
}


char * llvm_expression_comp_list(is_expression_comp_list * iecl, id_token* aux, int nvar_now){
    if (iecl == NULL) return NULL;

    next_oper *type = iecl->oper_comp;
    char * ltoken, *rtoken, *ret;

    if (type != NULL){
        //printf("\tComp type = %d\n", type->oper_type.ct);

        ltoken = llvm_expression_comp_list(iecl->next_left, aux, nvar_now);
        rtoken = llvm_expression_sum_like_list(iecl->next_right, aux,  ltoken[0]!='%' ? nvar_now: atoi((ltoken+1)) + 1); 
        printf("\t");

        int next;
        if (ltoken[0]=='%' && rtoken[0]=='%')
            next = _max(atoi( (rtoken + 1)) + 1, atoi( ( ltoken + 1 ) ) + 1);
        else if (ltoken[0] == '%')
            next = atoi(ltoken+1)+1;
        else if (rtoken[0]=='%')
            next = atoi(rtoken+1)+1;
        else
            next = nvar_now;


        printf("%%%d = icmp ", next);
        switch (type->oper_type.ct)
        {
        case d_lt:
            (iecl->expression_type==d_integer)? printf("ult "):printf("slt ");
            break;
        case d_gt:
            (iecl->expression_type==d_integer)? printf("ugt "):printf("sgt ");
            break;
        case d_ne:
            printf("ne ");
            break;
        case d_eq:
            printf("eq ");
            break;
        case d_le:
            (iecl->expression_type==d_integer)? printf("ule "):printf("sle ");
            break;
        case d_ge:
            (iecl->expression_type==d_integer)? printf("ugt "):printf("sgt ");
            break;
        default:
            break;
        }
        ( is_digit(ltoken[0]) ) ? printf(" %s, ", ltoken) : printf(" %%%s, ", ltoken);
        ( is_digit(rtoken[0]) ) ? printf(" %s\n", rtoken) : printf(" %%%s\n", rtoken);
        
        ret = (char *) malloc( ndigits(next) + 2 );
        sprintf(ret, "%%%d", next);
        return ret;
    }else{
        return llvm_expression_sum_like_list(iecl->next_right, aux, nvar_now);
    }
    return "";
}


char * llvm_expression_sum_like_list(is_expression_sum_like_list * iesl, id_token* aux, int nvar_now){
    if (iesl == NULL) return NULL;

    is_expression_sum_like_list * current = iesl;
    next_oper * type = current->oper_sum_like;
    char * ltoken, *rtoken, *ret;

    if (type != NULL){
        ltoken = llvm_expression_sum_like_list(current->next_left, aux, nvar_now );
        rtoken = llvm_expression_star_like_list(current->next_right, aux, (ltoken[0]!='%') ? nvar_now: atoi((ltoken+1)) + 1);
        printf("\t");
        int next;
        if (ltoken[0]=='%' && rtoken[0]=='%')
            next = _max(atoi( (rtoken + 1)) + 1, atoi( ( ltoken + 1 ) ) + 1);
        else if (ltoken[0] == '%')
            next = atoi(ltoken+1)+1;
        else if (rtoken[0]=='%')
            next = atoi(rtoken+1)+1;
        else
            next = nvar_now;
        switch (type->oper_type.slt)
        {
        case d_plus:
            (current->expression_type == d_integer)? printf("%%%d = add ", next): printf("%%%d = fadd ", next);
            llvm_print_type(current->expression_type);
            ( is_digit(ltoken[0]) ) ? printf(" %s, ", ltoken) : printf(" %%%s, ", ltoken);
            ( is_digit(rtoken[0]) ) ? printf(" %s\n", rtoken) : printf(" %%%s\n", rtoken);
            break;
        case d_minus:
            (current->expression_type == d_integer)? printf("%%%d = sub ", next): printf("%%%d = fsub ", next);
            llvm_print_type(current->expression_type);
            ( is_digit(ltoken[0]) ) ? printf(" %s, ", ltoken) : printf(" %%%s, ", ltoken);
            ( is_digit(rtoken[0]) ) ? printf(" %s\n", rtoken) : printf(" %%%s\n", rtoken);
            break;
        default:
            break;
        }

        ret = (char *) malloc( ndigits(next) + 2 );

        sprintf(ret, "%%%d", next);

        return ret;

    }else{
        return llvm_expression_star_like_list(current->next_right, aux, nvar_now);
    }
    return "";
}


char * llvm_expression_star_like_list(is_expression_star_like_list * iestl, id_token* aux, int nvar_now){
    if (iestl == NULL) return NULL;

    is_expression_star_like_list * current = iestl;
    next_oper * type = current->oper_star_like;
    char * ltoken,* rtoken, *ret;
    if (type != NULL){
        ltoken = llvm_expression_star_like_list(current->next_left, aux, nvar_now);
        rtoken = llvm_self_expression_list(current->next_right, aux,  ltoken[0]!='%' ? nvar_now: atoi((ltoken+1)) + 1);
        printf("\t");

        int next;
        if (ltoken[0]=='%' && rtoken[0]=='%')
            next = _max(atoi( (rtoken + 1)) + 1, atoi( ( ltoken + 1 ) ) + 1);
        else if (ltoken[0] == '%')
            next = atoi(ltoken+1)+1;
        else if (rtoken[0]=='%')
            next = atoi(rtoken+1)+1;
        else
            next = nvar_now;

        switch (type->oper_type.stlt)
        {
        case d_star:
            (current->expression_type==d_integer)? printf("%%%d = mul ", next): printf("%%%d = fmul", next);
            llvm_print_type(current->expression_type);
            ( is_digit(ltoken[0]) ) ? printf(" %s, ", ltoken) : printf(" %%%s, ", ltoken);
            ( is_digit(rtoken[0]) ) ? printf(" %s\n", rtoken) : printf(" %%%s\n", rtoken);
            break;
        case d_div:
            (current->expression_type==d_integer)? printf("%%%d = sdiv ", next) : printf("%%%d = fdiv ", next);
            llvm_print_type(current->expression_type);
            ( is_digit(ltoken[0]) ) ? printf(" %s, ", ltoken) : printf(" %%%s, ", ltoken);
            ( is_digit(rtoken[0]) ) ? printf(" %s\n", rtoken) : printf(" %%%s\n", rtoken);
            break;
        case d_mod:
            printf("%%%d = srem ", next);
            llvm_print_type(current->expression_type);
            ( is_digit(ltoken[0]) ) ? printf(" %s, ", ltoken) : printf(" %%%s, ", ltoken);
            ( is_digit(rtoken[0]) ) ? printf(" %s\n", rtoken) : printf(" %%%s\n", rtoken);
            break;
        default:
            break;
        }

        ret = (char *) malloc( ndigits(next) + 2 );
        sprintf(ret, "%%%d", next);
        return ret;

    }else{
       return llvm_self_expression_list(current->next_right, aux, nvar_now);
    }
    return "";
}

char * llvm_self_expression_list(is_self_expression_list * isel, id_token* aux, int nvar_now){
    if (isel == NULL) return NULL;
    is_self_expression_list * current = isel;
    next_oper* type = current->self_oper_type;
    char * ltoken, *ret;

    if (type != NULL){
        ltoken = llvm_self_expression_list(current->next_same, aux, nvar_now);
        printf("\t");

        int next = (ltoken[0]=='%') ? atoi(ltoken+1)+1 : nvar_now;

        switch (type->oper_type.sot)
        {
        case d_self_not:

            break;
        case d_self_plus:
            break;
        case d_self_minus:
            printf("%%%d = mul ", next);
            llvm_print_type(current->expression_type);
            ( is_digit(ltoken[0]) ) ? printf(" %s, -1\n", ltoken) : printf(" %%%s, -1\n", ltoken);
            break;
        }
        
        ret = (char *) malloc( ndigits(next) + 2 );
        sprintf(ret, "%%%d", next);
        return ret;
        
    }else{
        return llvm_final_expression(current->next_final, aux, nvar_now);
    }
}


char * llvm_final_expression(is_final_expression * ife, id_token* aux, int nvar_now){
    if (ife == NULL) return NULL;
    //{d_intlit, d_reallit, d_id, d_func_inv, d_expr_final} 
    char * token;

    switch (ife->type_final_expression){
        case d_intlit:
            token = (char * ) malloc(strlen(ife->expr.u_intlit->intlit->id) + 2);
            sprintf(token, "%s", ife->expr.u_intlit->intlit->id);
            return token;
        case d_reallit:
            token = (char * ) malloc(strlen(ife->expr.u_reallit->reallit->id) + 1);
            sprintf(token, "%s", ife->expr.u_reallit->reallit->id);
            return token;
        case d_id:
            token = (char * ) malloc( ndigits(nvar_now) + 2);

            printf("\t%%%d = load ", nvar_now);
            llvm_print_type(ife->expr.u_id->id->type);
            printf(", ");
            llvm_print_type(ife->expr.u_id->id->type);
            printf("* %%%s\n", ife->expr.u_id->id->id);

            sprintf(token, "%%%d", nvar_now);
            return token;
        case d_func_inv:
            token = (char * ) malloc(ndigits(nvar_now) + 2);

            llvm_func_parameters * aux = NULL, *aux2;
            for ( is_func_inv_expr_list * param = ife->expr.ifi->iel; param; param = param->next)
                aux = append_param(aux, llvm_expression_or_list(param->val, NULL, nvar_now));
            
            int next = get_next_var(aux, nvar_now);
            printf("\t%%%d = call ", next);
            llvm_print_type(ife->expr.ifi->id->type);
            printf("@%s(", ife->expr.ifi->id->id);

            aux2 = aux;
            for (is_func_inv_expr_list * param = ife->expr.ifi->iel ; param ; param = param->next, aux2 = aux2->next){
                llvm_print_type(param->val->expression_type);
                printf(" %s", aux2->var_id);
                if(param->next!=NULL) printf(", ");
            }
            printf(")\n");

            free_param_list(aux);

            sprintf(token, "%%%d", next);
            return token;

        case d_expr_final:
            return llvm_expression_or_list(ife->expr.ieol, NULL, nvar_now);
            break;
        default:
            printf("Erro llvm_final_expression\n");
            break;
    }
    return "";
}

void llvm_func_invocation(is_function_invocation * ifi){
    printf("\tcall ");
    llvm_print_type(ifi->id->type);
    printf(" @%s()\n", ifi->id->id);

    //llvm_inv_parameters(ifi);
}

char * llvm_self_str(self_operation_type type){
    switch (type){
        case d_self_not:
            return "fneg";
    //      case d_
        default:
            return "";
            break;
    }
}
