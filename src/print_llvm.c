#include "print_llvm.h"
#include "symbol_table.h"
#include "semantics.h"
#include "y.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>


// TODO Functions calls (invocation)
// TODO se for float/double + float/double temos de usar fadd em vez de add

extern is_program* program;
int global_counter = 0;
int func_counter = 0;
int string_counter = 0;
int label_counter = 0;
bool declare_print = 0, print_done = 0;
bool declare_atoi = 0, atoi_done = 0;
bool declare_not = 0, not_done = 0;
bool return_in_statement = 0;



bool is_digit(char c){
    return (c>='0' && c<='9') || c=='%';
}

bool is_exponent(char * n){
    for (int i = 0; n[i]!='\0'; i++)
        if (n[i]=='e' || n[i]=='E')
            return 1;
    return 0;
}

bool has_point(char * n){
    for ( int i = 0; n[i]!='\0'; i++)
        if (n[i]=='.')
            return 1;
    return 0;
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

llvm_func_parameters* append_param(llvm_func_parameters * head, char * string, int * nvar_now){
    if (string[0] == '%') *nvar_now = atoi(string+1)+1;
    llvm_func_parameters*new = malloc(sizeof(llvm_func_parameters)), *aux;
    new->var_id = string;
    new->next=NULL;

    if (head == NULL)
        return new;

    for (aux = head ; aux->next; aux = aux->next);
    aux->next = new;
    return head;
}

void free_param_list(llvm_func_parameters * head){
    if (head == NULL) return;

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

    return next == -1 ? nvar_now : next + 1;
}

void llvm_store(id_token* a, id_token* b){
    printf("\tstore ");
    llvm_print_type(a->type);
    printf(" %%%s, ", a->id);
    llvm_print_type(b->type);
    printf("* %%%s\n", b->id);
}

void llvm_expr_store(id_token* a, char* b, table_element ** symtab){
    table_element * temp;

    if (is_digit(a->id[0])){
        printf("\tstore ");
        llvm_print_type(a->type);
        ( is_digit(b[0]) )? printf(" %s, ", b) : printf(" %%%s, ", b);
        llvm_print_type(a->type);

        printf("* %s\n", a->id);
    }
    else{
        if ((temp=search_var(*symtab, a->id))){
            if (temp->is_param){
                printf("\t%%%s%d = ", a->id, ++temp->llvm_count);
                a->type==d_float32? printf("fadd "):printf("add ");
                llvm_print_type(a->type);

                if (temp->llvm_count == 1){
                    printf(" %%%s,", a->id);
                }else
                    printf(" %%%s%d,", a->id, temp->llvm_count-1);

                ( is_digit( b[0] ) ) ? printf(" %s\n", b) : printf(" %%%s\n", b);

            }else{
                printf("\tstore ");
                llvm_print_type(a->type);
                ( is_digit(b[0]) )? printf(" %s, ", b) : printf(" %%%s, ", b);
                llvm_print_type(a->type);

                printf("* %%%s\n", a->id);
            }
        }else{
            printf("\tstore ");
            llvm_print_type(a->type);
            ( is_digit(b[0]) )? printf(" %s, ", b) : printf(" %%%s, ", b);
            llvm_print_type(a->type);

            printf("* @%s\n", a->id);
        }
    }
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
            printf("Erro llvm_print_type\n");
            break;
    }
}


void llvm_program(is_program* ip){
    //print global variables
    for(table_element* current = ip->symtab; current; current = current->next) {
        if (current->type_dec == d_var_declaration){
            printf("@%s = global ", current->id->id);
            llvm_print_type(current->type);
            if (current->type == d_float32)
                printf(" 0.0");
            else
                printf(" 0");
            printf("\n");   
        }
    }

    //inserir strings para dar print a variáveis
    table_element* string = (table_element*) malloc(sizeof(table_element));
    id_token* aux = create_token("\"%d\\n\"", 0, 0);
    string->id = aux;
    string->is_string = 1;
    string->type_dec = d_var_declaration;
    insert_symbol(&program->strings_table, string);

    string = (table_element*) malloc (sizeof(table_element));
    aux = create_token("\"%f\\n\"", 0, 0);
    string->id = aux;
    insert_symbol(&program->strings_table, string);

    string = (table_element*) malloc (sizeof(table_element));
    aux = create_token("\"true\\n\"", 0, 0);
    string->id = aux;
    insert_symbol(&program->strings_table, string);

    string = (table_element*) malloc (sizeof(table_element));
    aux = create_token("\"false\\n\"", 0, 0);
    string->id = aux;
    insert_symbol(&program->strings_table, string);


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
            size += 1;
            i+=2;
        } else
            size++;
    }
    //printf("string_size (%s) = %d\n", s, size);
    return size+1;
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
    label_counter = 1;

    printf("define ");
    llvm_print_type(ifd->type);
    printf(" @%s(", ifd->id->id);

    //parametros
    if (strcmp(ifd->id->id, "main") == 0)
        printf("i32 %%argc, i8** %%argv");
    else
        llvm_is_parameter(&ifd->symtab, ifd->ipl);

    printf(") {\n");

    //printf("entry:\n");

    //body
    func_counter++;
    llvm_function_body(ifd->ifb, &ifd->symtab);

    table_element* tmp = search_symbol(ifd->symtab, "return");
    //printf("=========== %d\n", tmp->type);
    if (tmp->type == d_none){
        printf("\tret void\n");
    }

    printf("}\n\n");

    if (declare_print && !print_done) {
        print_done = 1;
        printf("declare i32 @printf(i8*, ...)\n\n");
    }

    if (declare_atoi && !atoi_done) {
        atoi_done = 1;
        printf("declare i32 @atoi(...)\n\n");
    }

    if (declare_not && !not_done) {
        not_done = 1;
        printf("define i1 @.not(i1 %%in){\n");
        printf("\tbr i1 %%in, label %%not_true, label %%not_false\n");

        printf("not_true:\n");
        printf("\t%%1 = icmp eq i1 1, 0\n");
        printf("\tret i1 %%1\n");

        printf("not_false:\n");
        printf("\t%%2 = icmp eq i1 1, 1\n");
        printf("\tret i1 %%2\n}\n");

    }


}


void llvm_function_body(is_func_body* ifb, table_element ** symtab){
    llvm_vars_and_statements_list(ifb->ivsl, symtab, 1);
}


void llvm_is_parameter(table_element ** symtab, is_parameter * ip) {
    if (ip == NULL) return;

    for (is_id_type_list* temp = ip->val; temp; temp = temp->next){
        llvm_print_type(get_var_id_type(symtab, temp->val->id));
        printf(" %%%s", temp->val->id->id);
        if (temp->next != NULL)
            printf(", ");
    }

}


void llvm_vars_and_statements_list(is_vars_and_statements_list* ivsl, table_element ** symtab, int nvar_now){
    is_vars_and_statements_list* current;

    for(current = ivsl; current != NULL; current = current->next) {
        //{d_var_dec, d_statement} var_or_statement;
        switch (current->val->type){
            case d_var_dec:
                llvm_var_declaration(current->val->body.ivd);
                break;
            case d_statement:
                nvar_now = llvm_statement(current->val->body.is, symtab, nvar_now, label_counter);
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
        //printf("\nTYPE = %d\n", current->val->type);
        llvm_print_type(current->val->type);
        printf("\n");
    }
}


int llvm_statement( is_statement* is, table_element ** symtab, int nvar_now, int counter){
    //{d_if, d_for, d_return, d_print, d_assign, d_statement_list, d_final_statement
    int next = nvar_now;
    //printf("================== Nvar = %d\n", nvar_now);
    switch (is->type_state){
        case d_if:
            next = llvm_if_statement(is->statement.u_if_state, symtab, nvar_now, counter);
            break;
        case d_for:
            next = llvm_for_statement(is->statement.u_for_state, symtab, nvar_now, counter);
            break;
        case d_return:
            next = llvm_return_statement(is->statement.u_return_state, symtab, nvar_now);
            break;
        case d_print:
            next = llvm_print_statement(is->statement.u_print_state, symtab, nvar_now);
            break;
        case d_assign:
            next = llvm_assign_statement(is->statement.u_assign, symtab, nvar_now);
            break;
        case d_statement_list:
            next = llvm_statements_list(is->statement.isl, symtab, nvar_now, counter);
            break;
        case d_final_statement:
            next = llvm_final_statement(is->statement.u_state, symtab, nvar_now);
            break;
        
        default:
            printf("Erro llvm_statement\n");
            break;
    }
    return next;
}


int llvm_if_statement(is_if_statement* ifs, table_element**symtab, int nvar_now, int counter){    
    char* token;
    
    bool _bef_if = 0, _if = 0, _else = 0;
    token = llvm_expression_or_list(ifs->iel, NULL, nvar_now, symtab);

    if ( (_bef_if = (ifs->ies != NULL)) )
        printf("\tbr i1 %s, label %%then%d, label %%else%d\n", token, counter, counter);
    else
        printf("\tbr i1 %s, label %%then%d, label %%ifcont%d\n", token, counter, counter);

    return_in_statement = 0;
    printf("then%d:\n", counter);
    nvar_now = llvm_statements_list(ifs->isl, symtab, token[0] == '%' ? atoi(token+1)+1 : nvar_now, counter + 1);

    if ( (_if = !return_in_statement) )
        printf("\tbr label %%ifcont%d\n", counter);


    if (ifs->ies != NULL){
        return_in_statement = 0;
        printf("else%d:\n", counter);
        nvar_now = llvm_else_statement(ifs->ies, symtab, nvar_now, counter + 1);
        if ((_else = !return_in_statement) )
            printf("\tbr label %%ifcont%d\n", counter);
    }

    if ( _if || _else || !_bef_if)
        printf("ifcont%d:\n", counter);
    //

    //printf("\t%%iftmp = phi i32 [ %%calltmp, %%then ], [ %%calltmp1, %%else ]\n");

    label_counter += counter;

    return nvar_now;
}


int llvm_else_statement(is_else_statement* ies,table_element**symtab, int nvar_now, int counter){
    nvar_now = llvm_statements_list(ies->isl, symtab, nvar_now, counter);

    return nvar_now;
}


int llvm_for_statement(is_for_statement* ifs, table_element**symtab, int nvar_now, int counter){
    printf("\tbr label %%startloop%d\n", counter);
    printf("startloop%d:\n", counter);    

    char* token = llvm_expression_or_list(ifs->iel, NULL, nvar_now, symtab);
    printf("\tbr i1 %s, label %%loop%d, label %%endloop%d\n", token, counter, counter);

    printf("loop%d:\n", counter);

    nvar_now = llvm_statements_list(ifs->isl, symtab, token[0] == '%' ? atoi(token+1)+1 : nvar_now, counter+1);
    printf("\tbr label %%startloop%d\n", counter);

    printf("endloop%d:\n", counter);

    label_counter += counter;

    return nvar_now;
}


int llvm_return_statement(is_return_statement* irs, table_element**symtab, int nvar_now){

    if (irs->iel == NULL){
        //printf("\tret void");
        return nvar_now;
    }

    char * token = llvm_expression_or_list(irs->iel, NULL, nvar_now, symtab);
    printf("\tret ");
    llvm_print_type(irs->iel->expression_type);
    printf(" %s\n", token);

    return_in_statement = 1;

    return token[0] == '%'? atoi(token+1)+1 : nvar_now;
}


int llvm_print_statement(is_print_statement* ips, table_element**symtab, int nvar_now){
    if (ips == NULL) return nvar_now;

    declare_print = 1;
    char * token = "";
    char* type;

    //print_type type = ips->type_print; // {d_expression, d_str}

    switch (ips->type_print){
        case d_expression:
            token = llvm_expression_or_list(ips->print.iel, NULL, nvar_now, symtab);
            
            switch (ips->print.iel->expression_type){
                case d_integer:
                    type = "\"%d\\n\"";
                    nvar_now = llvm_print(type, token, nvar_now, ips->print.iel->expression_type);
                    break;

                case d_float32:
                    type = "\"%f\\n\"";
                    nvar_now = llvm_print(type, token, nvar_now, ips->print.iel->expression_type);
                    break;

                case d_string:
                    type = "\"%s\"";
                    nvar_now = llvm_print(type, token, nvar_now, ips->print.iel->expression_type);
                    break;

                 case d_bool:
                    nvar_now = llvm_print("", token, nvar_now, ips->print.iel->expression_type);
                    break;
                
                default:
                    printf("Erro llvm_print_statement(expression)\n");
                    break;
            }

            break;
        case d_str:
            nvar_now = llvm_print(ips->print.id->id, "", nvar_now, d_string);
            break;
        default:
            printf("Erro llvm_print_statement\n");
            break;
    }
    return nvar_now;
}


int llvm_get_string_num(char* string){
    int num = 0;

    for (table_element* aux = program->strings_table; aux; aux = aux->next) {
        if (strcmp(aux->id->id, string) == 0){
            return num;
        }
        num++;
    }

    return -1;
}


int llvm_print(char* string, char* params, int nvar_now, parameter_type type){
    if ( !strcmp(string, "") && strcmp(params, "") ){
        // é boolean
        
        printf("\tbr i1 %s, label %%print_true%s, label %%print_false%s\n", params, params+1, params+1);

        printf("print_true%s:\n", params+1);
        
        table_element *str = search_symbol(program->strings_table, "\"true\\n\"");
        int num = llvm_get_string_num(str->id->id);
        int size = string_size(str->id->id);

        params[0]=='%'? printf("\t%%%d", atoi(params+1)+1) : printf("\t%%%d", nvar_now);
        printf(" = call i32(i8*, ...) "); 
        printf("@printf (i8* getelementptr inbounds");
        printf("([%d x i8], [%d x i8]* ", size, size);

        if (num == 0)
            printf(" @.str, ");
        else
            printf(" @.str.%d, ", num);

        printf("i32 0, i32 0)");
        printf(")\n");
        printf("\tbr label %%keep%s\n", params+1);

        printf("print_false%s:\n", params+1);
        
        str = search_symbol(program->strings_table, "\"false\\n\"");
        num = llvm_get_string_num(str->id->id);
        size = string_size(str->id->id);

        params[0]=='%'? printf("\t%%%d", atoi(params+1)+2) : printf("\t%%%d", nvar_now+1);
        printf(" = call i32(i8*, ...) "); 
        printf("@printf (i8* getelementptr inbounds");
        printf("([%d x i8], [%d x i8]* ", size, size);

        if (num == 0)
            printf(" @.str, ");
        else
            printf(" @.str.%d, ", num);

        printf("i32 0, i32 0)");
        printf(")\n");
        printf("\tbr label %%keep%s\n", params+1);

        printf("keep%s:\n", params+1);

        return params[0]=='%'? atoi(params+1)+3: nvar_now + 2;
    }else{
        table_element *str = search_symbol(program->strings_table, string);
        if(str == NULL) return params[0]=='%'? atoi(params+1)+1:nvar_now;

        int num = llvm_get_string_num(str->id->id);
        int size = string_size(str->id->id);

        params[0]=='%'? printf("\t%%%d", atoi(params+1)+1) : printf("\t%%%d", nvar_now);
        printf(" = call i32(i8*, ...) "); 
        printf("@printf (i8* getelementptr inbounds");
        printf("([%d x i8], [%d x i8]* ", size, size);

        if (num == 0)
            printf(" @.str, ");
        else
            printf(" @.str.%d, ", num);

        printf("i32 0, i32 0)");

        if ( strcmp(params, "") ){
            printf(", ");
            //type
            llvm_print_type(type);
            printf(" %s", params);
        }

        printf(")\n");

        return params[0]=='%'? atoi(params+1)+2: nvar_now + 1;

    }
}  
  

int llvm_assign_statement(is_assign_statement* ias, table_element**symtab, int nvar_now){
    id_token* token = (id_token*)malloc(sizeof(id_token));
    token = ias->id;

    char * res_token;
    res_token = llvm_expression_or_list(ias->iel, ias->id, nvar_now, symtab);

    //printf("TOKEN = %s\n", token->id);
    llvm_expr_store(token, res_token, symtab);

    return res_token[0] == '%' ? atoi(res_token+1)+1 : nvar_now;
}


int llvm_statements_list(is_statements_list* isl, table_element**symtab, int nvar_now, int counter){
    if (isl == NULL) return nvar_now;
    
    for (is_statements_list * current = isl; current; current = current->next){
        nvar_now = llvm_statement(current->val, symtab, nvar_now, counter);
    }
    return nvar_now;
}


int llvm_final_statement(is_final_statement* ifs, table_element**symtab, int nvar_now){
    if (ifs == NULL) return nvar_now;
    char* token;
    int next;

    switch (ifs->type_state){
        case d_function_invoc:
            nvar_now = llvm_func_invocation(ifs->statement.ifi, symtab, nvar_now);
            break;
        
        case d_arguments:
            // token -> valor em os.Args[token]
            declare_atoi = 1;
            token = llvm_expression_or_list(ifs->statement.ipa->iel, NULL, nvar_now, symtab);
            next = (token[0]=='%') ? atoi(token+1)+1 : nvar_now;
            printf("\t%%%d = getelementptr i8*, i8** %%argv, i32 %s\n", next, token);
            printf("\t%%%d = load i8*, i8** %%%d\n", next+1, next);
            printf("\t%%%d = call i32 (i8*, ...) bitcast (i32 (...)* @atoi to i32 (i8*, ...)*)(i8* %%%d)\n", next+2, next+1);
            printf("\tstore i32 %%%d, i32* %%%s\n", next+2, ifs->statement.ipa->id->id);
            nvar_now = next + 3;
            break;
        
        default:
            printf("Erro llvm_final_statement\n");
            break;
    }
    return nvar_now;
}


void llvm_atoi(char* token, char* s, int size){
    declare_atoi = 1;
    //printf("\t%%%s = call i32 (i32) @atoi([%d x i8*] %s)\n", token, size, s);
    printf("\t%%%s = call i32 (i8*, ...) bitcast (i32 (...)* @atoi to i32 (i8*, ...)*)(i8* %s)\n", token, s);
}

char *  llvm_expression_or_list(is_expression_or_list* ieol, id_token* aux, int nvar_now, table_element**symtab){
    if (ieol == NULL) return NULL;
    
    char * ltoken, * rtoken, *ret;
    if (ieol->is_operation!=NULL){

        ltoken = llvm_expression_or_list(ieol->next_left, aux, nvar_now, symtab);
        rtoken = llvm_expression_and_list(ieol->next_right, aux, ltoken[0]!='%' ? nvar_now: atoi((ltoken+1)) + 1, symtab);   
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
        llvm_print_type(ieol->next_left->expression_type);
        ( is_digit(ltoken[0]) ) ? printf(" %s, ", ltoken) : printf(" %%%s, ", ltoken);
        ( is_digit(rtoken[0]) ) ? printf(" %s\n", rtoken) : printf(" %%%s\n", rtoken);
        
        ret = (char *) malloc( ndigits(next) + 2 );
        sprintf(ret, "%%%d", next);
        return ret;

    }else{
        return llvm_expression_and_list(ieol->next_right, aux, nvar_now, symtab);
    } 
}


char * llvm_expression_and_list(is_expression_and_list* ieal, id_token* aux, int nvar_now, table_element**symtab){
    if (ieal == NULL) return NULL;

    is_expression_and_list* current = ieal;
    next_oper* type = current->is_operation;
    char * ltoken, * rtoken, *ret;

    if(type!=NULL){

        ltoken = llvm_expression_and_list(current->next_left, aux, nvar_now, symtab);
        rtoken = llvm_expression_comp_list(current->next_right, aux,  ltoken[0]!='%' ? nvar_now: atoi((ltoken+1)) + 1, symtab);
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
        llvm_print_type(current->next_left->expression_type);
        ( is_digit(ltoken[0]) ) ? printf(" %s, ", ltoken) : printf(" %%%s, ", ltoken);
        ( is_digit(rtoken[0]) ) ? printf(" %s\n", rtoken) : printf(" %%%s\n", rtoken);
        
        ret = (char *) malloc( ndigits(next) + 2 );
        sprintf(ret, "%%%d", next);
        return ret;
    }else{
        return llvm_expression_comp_list(current->next_right, aux, nvar_now, symtab);
    }  
    return "";  
}


char * llvm_expression_comp_list(is_expression_comp_list * iecl, id_token* aux, int nvar_now, table_element**symtab){
    if (iecl == NULL) return NULL;

    next_oper *type = iecl->oper_comp;
    char * ltoken, *rtoken, *ret;

    if (type != NULL){
        //printf("\tComp type = %d\n", type->oper_type.ct);

        ltoken = llvm_expression_comp_list(iecl->next_left, aux, nvar_now, symtab);
        rtoken = llvm_expression_sum_like_list(iecl->next_right, aux,  ltoken[0]!='%' ? nvar_now: atoi((ltoken+1)) + 1, symtab); 
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


        (iecl->next_left->expression_type==d_integer) ? printf("%%%d = icmp ", next) : printf("%%%d = fcmp ", next);
        switch (type->oper_type.ct){
            case d_lt:
                (iecl->next_left->expression_type==d_integer) ? printf("slt ") : printf("ult ");
                break;
            case d_gt:
                (iecl->next_left->expression_type==d_integer) ? printf("sgt ") : printf("ugt ");
                break;
            case d_ne:
                (iecl->next_left->expression_type==d_integer) ? printf("ne ") : printf("une ");
                break;
            case d_eq:
                (iecl->next_left->expression_type==d_integer) ? printf("eq ") : printf("ueq ");
                break;
            case d_le:
                (iecl->next_left->expression_type==d_integer) ? printf("sle ") : printf("ule ");
                break;
            case d_ge:
                (iecl->next_left->expression_type==d_integer) ? printf("sgt ") : printf("ugt ");
                break;
            default:
                printf("Erro llvm_expression_comp_list\n");
                break;
        }

        llvm_print_type(iecl->next_left->expression_type);
        ( is_digit(ltoken[0]) ) ? printf(" %s, ", ltoken) : printf("%%%s, ", ltoken);
        ( is_digit(rtoken[0]) ) ? printf(" %s\n", rtoken) : printf("%%%s\n", rtoken);
        
        ret = (char *) malloc( ndigits(next) + 2 );
        sprintf(ret, "%%%d", next);
        return ret;
    } else {
        return llvm_expression_sum_like_list(iecl->next_right, aux, nvar_now, symtab);
    }
    return "";
}


char * llvm_expression_sum_like_list(is_expression_sum_like_list * iesl, id_token* aux, int nvar_now, table_element**symtab){
    if (iesl == NULL) return NULL;

    is_expression_sum_like_list * current = iesl;
    next_oper * type = current->oper_sum_like;
    char * ltoken, *rtoken, *ret;

    if (type != NULL){
        ltoken = llvm_expression_sum_like_list(current->next_left, aux, nvar_now, symtab);
        rtoken = llvm_expression_star_like_list(current->next_right, aux, (ltoken[0]!='%') ? nvar_now: atoi((ltoken+1)) + 1, symtab);
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
        switch (type->oper_type.slt){
            case d_plus:
                (current->expression_type == d_integer)? printf("%%%d = add ", next): printf("%%%d = fadd ", next);
                llvm_print_type(current->next_left->expression_type);
                ( is_digit(ltoken[0]) ) ? printf(" %s, ", ltoken) : printf(" %%%s, ", ltoken);
                ( is_digit(rtoken[0]) ) ? printf(" %s\n", rtoken) : printf(" %%%s\n", rtoken);
                break;
            case d_minus:
                (current->expression_type == d_integer)? printf("%%%d = sub ", next): printf("%%%d = fsub ", next);
                llvm_print_type(current->next_left->expression_type);
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
        return llvm_expression_star_like_list(current->next_right, aux, nvar_now, symtab);
    }
    return "";
}


char * llvm_expression_star_like_list(is_expression_star_like_list * iestl, id_token* aux, int nvar_now, table_element**symtab){
    if (iestl == NULL) return NULL;

    is_expression_star_like_list * current = iestl;
    next_oper * type = current->oper_star_like;
    char * ltoken,* rtoken, *ret;
    if (type != NULL){
        ltoken = llvm_expression_star_like_list(current->next_left, aux, nvar_now, symtab);
        rtoken = llvm_self_expression_list(current->next_right, aux,  ltoken[0]!='%' ? nvar_now: atoi((ltoken+1)) + 1, symtab);
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

        switch (type->oper_type.stlt){
            case d_star:
                (current->expression_type==d_integer)? printf("%%%d = mul ", next): printf("%%%d = fmul ", next);
                llvm_print_type(current->next_left->expression_type);
                ( is_digit(ltoken[0]) ) ? printf(" %s, ", ltoken) : printf(" %%%s, ", ltoken);
                ( is_digit(rtoken[0]) ) ? printf(" %s\n", rtoken) : printf(" %%%s\n", rtoken);
                break;
            case d_div:
                (current->expression_type==d_integer)? printf("%%%d = sdiv ", next) : printf("%%%d = fdiv ", next);
                llvm_print_type(current->next_left->expression_type);
                ( is_digit(ltoken[0]) ) ? printf(" %s, ", ltoken) : printf(" %%%s, ", ltoken);
                ( is_digit(rtoken[0]) ) ? printf(" %s\n", rtoken) : printf(" %%%s\n", rtoken);
                break;
            case d_mod:
                printf("%%%d = srem ", next);
                llvm_print_type(current->next_left->expression_type);
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
       return llvm_self_expression_list(current->next_right, aux, nvar_now, symtab);
    }
    return "";
}


char * llvm_self_expression_list(is_self_expression_list * isel, id_token* aux, int nvar_now, table_element**symtab){
    if (isel == NULL) return NULL;
    is_self_expression_list * current = isel;
    next_oper* type = current->self_oper_type;
    char * ltoken, *ret;

    if (type != NULL){
        ltoken = llvm_self_expression_list(current->next_same, aux, nvar_now, symtab);
        printf("\t");

        int next = (ltoken[0]=='%') ? atoi(ltoken+1)+1 : nvar_now;

        switch (type->oper_type.sot){
            case d_self_not:
                declare_not = 1;
                printf("%%%d = call i1 @.not(i1 %s)\n", next, ltoken);

                ret = (char *) malloc( ndigits(next) + 2 );
                sprintf(ret, "%%%d", next);
                return ret;

            case d_self_plus:
                current->next_same->expression_type==d_float32? printf("%%%d = fadd ",next):printf("%%%d = add ", next);
                llvm_print_type(isel->next_same->expression_type);
                ( is_digit(ltoken[0]) ) ? printf(" %s, 0", ltoken) : printf(" %%%s, 0", ltoken);
                ( current->next_same->expression_type==d_float32 ) ? printf(".0\n") : printf("\n"); 

                ret = (char *) malloc( ndigits(next) + 2 );
                sprintf(ret, "%%%d", next);
                return ret;

            case d_self_minus:
                printf("%%%d = mul ", next);
                llvm_print_type(current->next_same->expression_type);
                ( is_digit(ltoken[0]) ) ? printf(" %s, -1\n", ltoken) : printf(" %%%s, -1\n", ltoken);

                ret = (char *) malloc( ndigits(next) + 2 );
                sprintf(ret, "%%%d", next);
                return ret;
        }
        
    } 

    return llvm_final_expression(current->next_final, aux, nvar_now, symtab);

}


char * llvm_final_expression(is_final_expression * ife, id_token* aux, int nvar_now, table_element**symtab){
    if (ife == NULL) return NULL;
    //{d_intlit, d_reallit, d_id, d_func_inv, d_expr_final} 
    char * token;
    

    //printf("====== FINAL EXPRESSION\n");

    switch (ife->type_final_expression){
        case d_intlit:
            token = (char * ) malloc(strlen(ife->expr.u_intlit->intlit->id) + 2);
            //printf("===== %s ===== \n", ife->expr.u_intlit->intlit->id);
            if (ife->expr.u_intlit->intlit->id[0] == '0' && (ife->expr.u_intlit->intlit->id[1] == 'x' || ife->expr.u_intlit->intlit->id[1] == 'X')){
                int hexa = strtol(ife->expr.u_intlit->intlit->id, NULL, 16);
                sprintf(token, "%d", hexa);
            }else if (ife->expr.u_intlit->intlit->id[0] == '0' && ife->expr.u_intlit->intlit->id[0] != '\0'){
                int octal = strtol(ife->expr.u_intlit->intlit->id, NULL, 8);
                sprintf(token, "%d", octal);
            }else
                sprintf(token, "%s", ife->expr.u_intlit->intlit->id);
            return token;

        case d_reallit:
            token = (char * ) malloc(33);
            if (is_exponent(ife->expr.u_reallit->reallit->id)){
                double real = strtof(ife->expr.u_reallit->reallit->id, NULL);
                sprintf(token, "%f", real);
                
            }else{
                if (has_point(ife->expr.u_reallit->reallit->id)){
                    if (ife->expr.u_reallit->reallit->id[0]!='.')
                        sprintf(token, "%s", ife->expr.u_reallit->reallit->id);
                    else
                        sprintf(token, "0%s", ife->expr.u_reallit->reallit->id);
                }else
                    sprintf(token, "%s.0", ife->expr.u_reallit->reallit->id);
            }
            return token;

        case d_id:
            token = (char * ) malloc( ndigits(nvar_now) + 2);
            table_element * temp_var = search_var(*symtab, ife->expr.u_id->id->id);

            if (temp_var == NULL){
                printf("\t%%%d = load ", nvar_now);
                llvm_print_type(ife->expression_type);
                printf(", ");
                llvm_print_type(ife->expression_type);
                printf("* @%s\n", ife->expr.u_id->id->id);
            }else{
                if (temp_var->is_param){
                    (ife->expr.u_id->id->type==d_float32) ? printf("\t%%%d = fadd ", nvar_now) : printf("\t%%%d = add ", nvar_now);
                    llvm_print_type(ife->expr.u_id->id->type);
                    if (temp_var->llvm_count == 0){
                        printf(" %%%s, 0", ife->expr.u_id->id->id);
                        (ife->expr.u_id->id->type==d_float32) ? printf(".0\n"):printf("\n");
                    }
                    else{
                        printf(" %%%s%d, 0", ife->expr.u_id->id->id, temp_var->llvm_count);
                        (ife->expr.u_id->id->type==d_float32) ? printf(".0\n"):printf("\n");
                    }
                }else{
                    printf("\t%%%d = load ", nvar_now);
                    //printf("\nife: %d    aux: %d\n", ife->expr.u_id->id->type, aux->type);
                    llvm_print_type(ife->expr.u_id->id->type);
                    printf(", ");
                    llvm_print_type(ife->expr.u_id->id->type);
                    printf("* %%%s\n", ife->expr.u_id->id->id);
                }
            }

            sprintf(token, "%%%d", nvar_now);
            return token;

        case d_func_inv:
            token = (char * ) malloc(ndigits(nvar_now) + 2);

            llvm_func_parameters * aux = NULL, *aux2;
            for ( is_func_inv_expr_list * param = ife->expr.ifi->iel; param ; param = param->next)
                aux = append_param(aux, llvm_expression_or_list(param->val, NULL, nvar_now, symtab), &nvar_now);
            
            int next = get_next_var(aux, nvar_now);
            printf("\t%%%d = call ", next);

            llvm_print_type(ife->expr.ifi->id->type);
            printf(" @%s(", ife->expr.ifi->id->id);

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
            return llvm_expression_or_list(ife->expr.ieol, NULL, nvar_now, symtab);
            break;
        default:
            printf("Erro llvm_final_expression\n");
            break;
    }
    return "";
}


int llvm_func_invocation(is_function_invocation * ifi, table_element ** symtab, int nvar_now){

    llvm_func_parameters * aux = NULL, *aux2;
    for ( is_func_inv_expr_list * param = ifi->iel; param ; param = param->next)
        aux = append_param(aux, llvm_expression_or_list(param->val, NULL, nvar_now, symtab), &nvar_now);
    
    int next = get_next_var(aux, nvar_now);
    if (ifi->id->type!=d_none)
        printf("\t%%%d = ",next);
    else
        printf("\t");

    printf("call ");
    llvm_print_type(ifi->id->type);
    printf(" @%s(", ifi->id->id);

    aux2 = aux;
    for (is_func_inv_expr_list * param = ifi->iel ; param ; param = param->next, aux2 = aux2->next){
        llvm_print_type(param->val->expression_type);
        printf(" %s", aux2->var_id);
        if(param->next!=NULL) printf(", ");
    }
    printf(")\n");

    free_param_list(aux);

    return (ifi->id->type!=d_none)? next+1:next;
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
