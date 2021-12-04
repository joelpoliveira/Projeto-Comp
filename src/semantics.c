#include "semantics.h"
#include "symbol_table.h"
//#include "print_ast.h"
#include "y.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//#define DEBUG 1

//extern table_element* symtab;
extern is_program * program;



void print_already_defined(char* id, int line, int col){
    printf("Line %d, column %d: Symbol %s already defined\n", line, col+1, id);
}


void print_cannot_find(char* id, int line, int col){
    printf("Line %d col %d: Cannot find symbol %s\n", line, col+1, id);
}



void print_parameter_type_(parameter_type type){
    switch (type){
        case d_integer:
            printf("int");
            break;
        case d_float32:
            printf("float32");
            break;
        case d_bool:
            printf("bool");
            break;
        case d_string:
            printf("string");
            break;
        case d_undef:
        case d_none:
            printf("undef");
            break;
        default:
            break;
    }
}

void print_cannot_find_function_(table_element ** symtab, is_function_invocation * ifi){
    printf("Line %d, column %d: Cannot find symbol %s(", ifi->id->line, ifi->id->col+1, ifi->id->id);
//    id_token * token;
    is_func_inv_expr_list * ieoll;
    for (ieoll = ifi->iel; ieoll; ieoll = ieoll->next){
        check_expression_or_list(symtab, ieoll->val);
        print_parameter_type_(ieoll->val->expression_type);
        if (ieoll->next!=NULL)
            printf(",");
    }
    printf(")\n");
}

void print_cannot_find_function_diff_params(table_element ** symtab, is_id_type_list * iitl, int index, is_function_invocation * ifi){
    printf("Line %d, column %d: Cannot find symbol %s(", ifi->id->line, ifi->id->col+1, ifi->id->id);
    //id_token * token;
    is_func_inv_expr_list * ieoll;
    int counter = 0;
    for (ieoll = ifi->iel; ieoll; ieoll = ieoll->next){
        if (counter <= index){
            print_parameter_type_(ieoll->val->expression_type);
            iitl = iitl->next;
        }else{
            check_expression_or_list(symtab, ieoll->val);
            print_parameter_type_(ieoll->val->expression_type);
        }
        counter++;
        if (ieoll->next!=NULL)
            printf(",");
    }
    printf(")\n");
}

char * comp_str(comp_type type){
    switch (type)
    {
    case d_lt:
        return "<";
    case d_gt:
        return ">";
    case d_eq:
        return "==";
    case d_ne:
        return "!=";
    case d_ge:
        return ">=";
    case d_le:
        return "<=";
    default:
        return "";
    }
}

char * sum_str(sum_like_type type){
    switch (type)
    {
    case d_plus:
        return "+";
    case d_minus:
        return "-";
    default:
        return "";
    }
}

char * star_str(star_like_type type){
    switch (type)
    {
    case d_star:
        return "*";
    case d_div:
        return "/";
    case d_mod:
        return "%%";
    default:
        return "";
    }
}

char * self_str(self_operation_type type){
    switch (type)
    {
    case d_self_not:
        return "!";
    case d_self_plus:
        return "+";
    case d_self_minus:
        return "-";
    default:
        return "";
    }
}

void print_oper_cannot_apply(next_oper * op, char * oper, parameter_type ltype, parameter_type rtype){
    printf("Line %d, column %d: Operator %s cannot be applied to types ", op->line, op->col+1, oper);
    print_parameter_type_(ltype);
    printf(", ");
    print_parameter_type_(rtype);
    printf("\n");
    
}

void print_oper_cannot_apply_self(next_oper * op, char * oper, parameter_type type){
    printf("Line %d, column %d: Operator %s cannot be applied to type ", op->line, op->col+1, oper);
    print_parameter_type_(type);
    printf("\n");
    
}

void check_program(is_program* ip){
    check_declarations_list(&ip->symtab, ip->idlist);
}


void check_declarations_list(table_element** symtab, is_declarations_list* idl){
    is_declarations_list* current;
    is_func_dec* func_dec;

    //Inserir todas as declarações globais em 1º
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

    // Inserir nas tabelas de funçoes depois de fazer tudo na global
    for (table_element* aux = program->symtab; aux; aux = aux->next){
        if (aux->type_dec == d_func_dec){
            func_dec = get_function_declaration(program, aux->id->id);
            check_function_body(&func_dec->symtab ,func_dec->ifb);
        }
    }

}


void check_func_declaration(table_element** symtab, is_func_dec* ifd){
    ifd->symtab = insert_func(symtab, ifd->id, ifd->ipl, ifd->type, ifd);
}


void check_function_body(table_element** symtab, is_func_body* ifb){
    check_vars_and_statements_list(symtab, ifb->ivsl);
}


void check_vars_and_statements_list(table_element** symtab, is_vars_and_statements_list* ivsl){
    is_vars_and_statements_list* current;

    for(current = ivsl; current != NULL; current = current->next) {
        //{d_var_dec, d_statement} var_or_statement;
        switch (current->val->type){
            case d_var_dec:
                check_var_declaration(symtab, current->val->body.ivd);
                break;
            case d_statement:
                check_statement(symtab, current->val->body.is);
                break;
        default:
            printf("Erro check_vars_and_statements_list");
            break;
        }
    }
}


void check_var_declaration(table_element** symtab, is_var_dec* ivd){
    check_var_spec(symtab, ivd->ivs);
}


void check_var_spec(table_element** symtab, is_var_spec* ivs){
    table_element* new_symbol = NULL;
    parameter_type type = ivs->type;
    //printf("Type: %d \n", type);
    for (is_id_list* current = ivs->iil; current != NULL; current = current->next){
        new_symbol = insert_var(symtab, current->val, type);

        /* if (new_symbol == NULL){
            print_already_defined(current->val->id, current->val->line, current->val->col);
        } */
    }
}


void check_statement(table_element** symtab, is_statement* is){
    //{d_if, d_for, d_return, d_print, d_assign, d_statement_list, d_final_statement
    switch (is->type_state){
        case d_if:
            check_if_statement(symtab, is->statement.u_if_state);
            break;
        case d_for:
            check_for_statement(symtab, is->statement.u_for_state);
            break;
        case d_return:
            check_return_statement(symtab, is->statement.u_return_state);
            break;
        case d_print:
            check_print_statement(symtab, is->statement.u_print_state);
            break;
        case d_assign:
            check_assign_statement(symtab, is->statement.u_assign);
            break;
        case d_statement_list:
            check_statements_list(symtab, is->statement.isl);
            break;
        case d_final_statement:
            check_final_statement(symtab, is->statement.u_state);
            break;
        
        default:
            printf("Erro check_statement\n");
            break;
    }
}


void check_if_statement(table_element** symtab, is_if_statement* ifs){
    if (ifs == NULL) return;

    check_expression_or_list(symtab, ifs->iel);

    id_token * ltoken = check_expression_or_list(symtab, ifs->iel);
    //TODO linha e coluna na mensagem de erro
    if (ifs->iel != NULL) {
        if(ifs->iel->expression_type != d_bool){
            printf("Line %d, column %d: Incompatible type ", ltoken->line, ltoken->col+1);
            symbol_print_type(ifs->iel->expression_type);
            printf(" in if statement\n");
        }
    }

    check_statements_list(symtab, ifs->isl);
    check_else_statement(symtab, ifs->ies);
}


void check_else_statement(table_element** symtab, is_else_statement* ies){
    if (ies == NULL) return;

    check_statements_list(symtab, ies->isl);
}


void check_for_statement(table_element** symtab, is_for_statement* ifs){
   
    id_token * ltoken = check_expression_or_list(symtab, ifs->iel);
    //TODO linha e coluna na mensagem de erro

    if (ifs->iel != NULL) {
        if (ifs->iel->expression_type != d_bool){
            printf("Line %d, column %d: Incompatible type ", ltoken->line, ltoken->col + 1);
            symbol_print_type(ifs->iel->expression_type);
            printf(" in for statement\n");
        }
    }
    
    check_statements_list(symtab, ifs->isl);
}


void check_return_statement(table_element** symtab, is_return_statement* irs){
    table_element* temp = search_symbol(*symtab, "return");
    //TODO linha e coluna na mensagem de erro
    
    id_token * ltoken = check_expression_or_list(symtab, irs->iel);
    
    if (temp != NULL && irs->iel != NULL){
        if ( (irs->iel->expression_type != temp->type && temp->type!=d_none) 
        || (irs->iel->expression_type == d_undef && temp->type!=d_none)){
            printf("Line %d, column %d: Incompatible type ", ltoken->line, ltoken->col+1);
            symbol_print_type(irs->iel->expression_type);
            printf(" in return statement\n");
        }   
    }

}


void check_print_statement(table_element** symtab, is_print_statement* ips){
     if (ips == NULL) return;

    print_type type = ips->type_print; // {d_expression, d_str}

    switch (type){
        case d_expression:
            #ifdef DEBUG
            printf("======== check_final_statement(expression) ========\n");
            #endif
            
            check_expression_or_list(symtab, ips->print.iel);
            if (ips->print.iel->expression_type==d_undef){
                printf("Line %d, column %d: Incompatible type ", ips->print.id->line, ips->print.id->col+1);
                print_parameter_type_(ips->print.iel->expression_type);
                printf(" in fmt.Println statement\n");
                return;
            }
            break;
        case d_str:
            #ifdef DEBUG
            printf("======== check_final_statement(d_str) ========\n");
            #endif
            break;
        default:
            printf("Erro check_print_statement\n");
            break;
    }
}


void check_assign_statement(table_element** symtab, is_assign_statement* ias){

    if (!search_in_tables(symtab, ias->id))
        printf("Line %d, column %d: Cannot find symbol %s\n", ias->id->line, ias->id->col+1, ias->id->id);

    // Se a variável estiver declarada antes com um tipo
    // não pode ser atribuido outro tipo
    check_expression_or_list(symtab, ias->iel);

    // TODO Linha e coluna
    if (ias->iel != NULL) {
        if (ias->iel->expression_type != ias->id->type
        || ias->iel->expression_type == d_undef 
        || ias->id->type == d_undef){
            printf("Line %d, column %d: Operator = cannot be applied to types ", ias->loc->line, ias->loc->col + 1);
            symbol_print_type(ias->id->type);
            printf(", ");
            symbol_print_type(ias->iel->expression_type);
            printf("\n");
        }
    }
}


void check_statements_list(table_element** symtab, is_statements_list* isl){
    if (isl == NULL) return;
    
    for (is_statements_list * current = isl; current; current = current->next){
        check_statement(symtab, current->val);
    }

}


void check_final_statement(table_element** symtab, is_final_statement* ifs){
    if (ifs == NULL) return;

    final_state_type type = ifs->type_state;  //d_function_invoc, d_arguments

    switch (type){
        case d_function_invoc:
            #ifdef DEBUG
            printf("======== check_final_statement(invocation) ========\n");
            #endif
            check_func_invocation(symtab, ifs->statement.ifi);

            break;
        case d_arguments:
            check_id(symtab, ifs->statement.ipa->id);
            #ifdef DEBUG
            printf("======== check_final_statement(arguments): %s ========\n", ifs->statement.ipa->id->id);
            #endif
            
            //TODO check que expression_or_list não é um inteiro
            check_expression_or_list(symtab, ifs->statement.ipa->iel);

            if (ifs->statement.ipa->id->type!=d_integer || ifs->statement.ipa->iel->expression_type!=d_integer){
                printf("Line %d, column %d: Operator strconv.Atoi cannot be applied to types ", ifs->statement.ipa->loc->line, ifs->statement.ipa->loc->col + 1);
                symbol_print_type(ifs->statement.ipa->id->type);
                printf(", ");
                symbol_print_type(ifs->statement.ipa->iel->expression_type);
                printf("\n");
            }
            break;
        default:
            printf("Erro check_final_statement\n");
            break;
    }
}


id_token* check_expression_or_list(table_element** symtab, is_expression_or_list* ieol){
    if (ieol == NULL) return NULL;

    id_token * ltoken;

    if (ieol->is_operation!=NULL){
        ltoken = check_expression_or_list(symtab, ieol->next_left);
        check_expression_and_list(symtab, ieol->next_right);

        if (ieol->next_left->expression_type != d_bool && ieol->next_right->expression_type != d_bool){
            print_oper_cannot_apply(ieol->is_operation, "||", ieol->next_left->expression_type, ieol->next_right->expression_type);
            ieol->expression_type = d_undef;
        }
        else
            ieol->expression_type = d_bool;

        return ltoken;
    }else{
        ltoken=check_expression_and_list(symtab, ieol->next_right);

        ieol->expression_type = ieol->next_right->expression_type;
        return ltoken;

    } 
}


id_token* check_expression_and_list(table_element** symtab, is_expression_and_list* ieal){
    if (ieal == NULL) return NULL;

    is_expression_and_list* current = ieal;
    next_oper* type = current->is_operation;
    id_token*ltoken;

    if(type!=NULL){
        ltoken=check_expression_and_list(symtab, current->next_left);
        check_expression_comp_list(symtab, current->next_right);

        if (current->next_left->expression_type != d_bool && current->next_right->expression_type != d_bool){
            print_oper_cannot_apply(type, "&&", current->next_left->expression_type, current->next_right->expression_type);
            current->expression_type = d_undef;
        }
        else
            current->expression_type = d_bool;

        return ltoken;
    }else{
        ltoken=check_expression_comp_list(symtab, current->next_right);

        current->expression_type = current->next_right->expression_type;

        return ltoken;
    }    
}


id_token* check_expression_comp_list(table_element** symtab, is_expression_comp_list * iecl){
    if (iecl == NULL) return NULL;

    next_oper *type = iecl->oper_comp;
    id_token* ltoken;

    if (type != NULL){
        ltoken = check_expression_comp_list(symtab, iecl->next_left);
        check_expression_sum_like_list(symtab, iecl->next_right);

        if ( type->oper_type.ct == d_eq || type->oper_type.ct == d_ne ){
            if (iecl->next_left->expression_type == d_undef || iecl->next_right->expression_type == d_undef){
                print_oper_cannot_apply(type, comp_str(type->oper_type.ct), iecl->next_left->expression_type, iecl->next_right->expression_type);
                iecl->expression_type = d_undef;
            }else if(iecl->next_left->expression_type != iecl->next_right->expression_type){
                print_oper_cannot_apply(type, comp_str(type->oper_type.ct), iecl->next_left->expression_type, iecl->next_right->expression_type);
                iecl->expression_type = d_undef;
            }else 
                iecl->expression_type = d_bool;
        }else{
            if (iecl->next_left->expression_type == d_undef || iecl->next_right->expression_type == d_undef){
                print_oper_cannot_apply(type, comp_str(type->oper_type.ct), iecl->next_left->expression_type, iecl->next_right->expression_type);
                iecl->expression_type = d_undef;

            }else if(iecl->next_left->expression_type == d_bool || iecl->next_right->expression_type == d_bool){
                print_oper_cannot_apply(type, comp_str(type->oper_type.ct), iecl->next_left->expression_type, iecl->next_right->expression_type);
                iecl->expression_type = d_undef;

            }else if(iecl->next_left->expression_type != iecl->next_right->expression_type){
                print_oper_cannot_apply(type, comp_str(type->oper_type.ct), iecl->next_left->expression_type, iecl->next_right->expression_type);
                iecl->expression_type = d_undef;

            }else
                iecl->expression_type = d_bool;
        }

        return ltoken;
        
    }else{
        ltoken=check_expression_sum_like_list(symtab, iecl->next_right);

        iecl->expression_type = iecl->next_right->expression_type;
        return ltoken;
    }
}


id_token* check_expression_sum_like_list(table_element** symtab, is_expression_sum_like_list * iesl){
    if (iesl == NULL) return NULL;

    is_expression_sum_like_list * current = iesl;
    next_oper * type = current->oper_sum_like;
    id_token * ltoken;

    if (type != NULL){
        ltoken=check_expression_sum_like_list(symtab, current->next_left);
        check_expression_star_like_list(symtab, current->next_right);

        if (current->next_left->expression_type == d_undef || current->next_right->expression_type == d_undef)
            current->expression_type = d_undef;

        else if (current->next_left->expression_type == d_bool || current->next_right->expression_type == d_bool){
            print_oper_cannot_apply(type, sum_str(type->oper_type.slt), current->next_left->expression_type, current->next_right->expression_type);
            current->expression_type = d_undef;
        }

        else if (current->next_left->expression_type != current->next_right->expression_type){
            print_oper_cannot_apply(type, sum_str(type->oper_type.slt), current->next_left->expression_type, current->next_right->expression_type);
            current->expression_type = d_undef;
        }

        else
            current->expression_type = current->next_right->expression_type;

        return ltoken;
    }else{
        ltoken=check_expression_star_like_list(symtab, current->next_right);
        //check_expression_sum_like_list(symtab, current->next_left);

        current->expression_type = current->next_right->expression_type;
        return ltoken;
    }
}


id_token* check_expression_star_like_list(table_element** symtab, is_expression_star_like_list * iestl){
    if (iestl == NULL) return NULL;

    is_expression_star_like_list * current = iestl;
    next_oper * type = current->oper_star_like;
    id_token*ltoken;

    if (type != NULL){
        ltoken=check_expression_star_like_list(symtab, current->next_left);
        check_self_expression_list(symtab, current->next_right);

        if (current->next_left->expression_type == d_undef || current->next_right->expression_type == d_undef){
            print_oper_cannot_apply(type, star_str(type->oper_type.stlt), current->next_left->expression_type, current->next_right->expression_type);
            current->expression_type = d_undef;
        }

        else if (current->next_left->expression_type == d_bool || current->next_right->expression_type == d_bool){
            print_oper_cannot_apply(type, star_str(type->oper_type.stlt), current->next_left->expression_type, current->next_right->expression_type);
            current->expression_type = d_undef;
        }

        else if (current->next_left->expression_type != current->next_right->expression_type){
            print_oper_cannot_apply(type, star_str(type->oper_type.stlt), current->next_left->expression_type, current->next_right->expression_type);
            current->expression_type = d_undef;
        }

        else
            current->expression_type = current->next_right->expression_type;
        
        return ltoken;
    }else{
        ltoken=check_self_expression_list(symtab, current->next_right);

        current->expression_type = current->next_right->expression_type;
        return ltoken;
    }
}


id_token* check_self_expression_list(table_element** symtab, is_self_expression_list * isel){
    if (isel == NULL) return NULL;

    is_self_expression_list * current = isel;
    next_oper* type = current->self_oper_type;
    id_token*ltoken;

    if (type != NULL){
        ltoken=check_self_expression_list(symtab, current->next_same);
        check_final_expression(symtab, current->next_final);
        if (type->oper_type.sot == d_self_not){
            if (current->next_same->expression_type != d_bool){
                print_oper_cannot_apply_self(type, self_str(type->oper_type.sot), current->next_same->expression_type);
                current->expression_type = d_undef;
                
            }else{
                current->expression_type = current->next_same->expression_type;
            }
            
        }else{
            if (current->next_same->expression_type == d_bool || current->next_same->expression_type == d_undef){
                print_oper_cannot_apply_self(type, self_str(type->oper_type.sot), current->next_same->expression_type);
                current->expression_type = d_undef;
            }else{
                current->expression_type = current->next_same->expression_type;
            }
        }
        return ltoken;
    }else{
        ltoken=check_final_expression(symtab, current->next_final);
    
        current->expression_type = current->next_final->expression_type;
        return ltoken;
    }
}


id_token * check_final_expression(table_element** symtab, is_final_expression * ife){
    if (ife == NULL) return NULL;
    //{d_intlit, d_reallit, d_id, d_func_inv, d_expr_final} 

    id_token * token;

    switch (ife->type_final_expression){
        case d_intlit:
            #ifdef DEBUG
            printf("======== check_final_expression(intlit) ========\n");
            #endif
            ife->expr.u_intlit->intlit->type = d_integer;
            ife->expression_type = d_integer;
            token = ife->expr.u_intlit->intlit;
            return token;
        case d_reallit:
            #ifdef DEBUG
            printf("======== check_final_expression(reallit) ========\n");
            #endif
            ife->expr.u_reallit->reallit->type = d_float32; 
            ife->expression_type = d_float32;
            token = ife->expr.u_reallit->reallit;
            return token;
        case d_id:
            #ifdef DEBUG
            printf("======== check_final_expression(id) ========\n");
            #endif
            ife->expression_type = get_id_type(symtab, ife->expr.u_id->id);
            //ife->expr.u_id->id->uses++;
            token = ife->expr.u_id->id;
            return token;
        case d_func_inv:
            #ifdef DEBUG
            printf("======== check_final_expression(invocation) ========\n");
            #endif
            check_func_invocation(symtab, ife->expr.ifi);
            ife->expression_type = get_id_type(symtab, ife->expr.ifi->id); 
            token = ife->expr.ifi->id;
            printf("----- %s, %d \n", ife->expr.ifi->id->id, ife->expr.ifi->id->type);
            return token;
        case d_expr_final:
            #ifdef DEBUG
            printf("======== check_final_expression(final) ========\n");
            #endif
            token = check_expression_or_list(symtab, ife->expr.ieol);
            ife->expression_type = ife->expr.ieol->expression_type;
            return token; 
        default:
            printf("Erro check_final_expression\n");
            break;
    }
    return NULL;
}


void check_func_invocation(table_element** symtab, is_function_invocation * ifi){
    
    check_inv_parameters(symtab, ifi);
    
}

table_element * get_table_elem(table_element ** symtab, id_token * id){
    table_element* local_symbol = search_symbol(*symtab, id->id);
    table_element* global_symbol = search_symbol(program->symtab, id->id);

    if (local_symbol == NULL) {
        return global_symbol;
    } else {
        return local_symbol;
    }
}

parameter_type get_id_type(table_element** symtab, id_token * id){
    table_element * temp = get_table_elem(symtab, id);
    #ifdef DEBUG
    printf("Token: %s -- Type: %d -- Type.id.type: %d\n", id->id, temp->type, temp->id->type);
    #endif

    return (temp == NULL)? d_undef : temp->type;
}


void check_id(table_element** symtab, id_token* id){
    if (!search_in_tables(symtab, id))
        printf("Line %d, column %d: Cannot find symbol %s\n", id->line, id->col+1, id->id);
}


// Procurar simbolo na tabela local e global
// Se existir, definir o tipo do id para anotação na AST
bool search_in_tables(table_element **symtab, id_token* id){
    table_element* local_symbol = search_symbol(*symtab, id->id);
    table_element* global_symbol = search_symbol(program->symtab, id->id);
    bool in_function_table = 1;
    bool in_global_table = 1;

    if (local_symbol == NULL) {
        in_function_table = 0;
    } else {
        id->type = local_symbol->type;
        //local_symbol->id->uses++;
        #ifdef DEBUG
        printf("Found on Local -> %s: type: %d - symbol type: %d / uses: %d\n", local_symbol->id->id, id->type, local_symbol->type, local_symbol->id->uses);
        #endif
    }

    if (in_function_table == 0){
        if (global_symbol == NULL) 
            in_global_table = 0;
        else{
            id->type = global_symbol->type;
            //global_symbol->id->uses++;
            #ifdef DEBUG
            printf("Found on Global -> %s: type: %d - symbol type: %d / uses: %d\n", global_symbol->id->id, id->type, global_symbol->type, global_symbol->id->uses);
            #endif
        } 
    }

    if (!in_function_table && !in_global_table){
        id->type = d_undef;
        return 0;
    }

    return 1;
}

void check_params(table_element ** symtab, table_element* symbol, is_function_invocation * ifi){
    is_parameter * param_list = symbol->dec.ifd->ipl;
    if (param_list == NULL && ifi->iel == NULL) {ifi->id->type = symbol->type; return;}
    else if (param_list == NULL && ifi->iel != NULL) {print_cannot_find_function_(symtab, ifi); ifi->id->type = d_undef;}
    else if (param_list !=NULL && ifi->iel == NULL){print_cannot_find_function_(symtab, ifi); ifi->id->type = d_undef;}
    else{
        is_func_inv_expr_list * ieoll;
        is_id_type_list * param_elem = param_list->val;
        int counter = 0;
        for (ieoll = ifi->iel; ieoll; ieoll = ieoll->next, param_elem = param_elem->next){
            check_expression_or_list(symtab, ieoll->val);
            
            if ( ieoll->val->expression_type == param_elem->val->type_param){
                counter++;
            }else{
                print_cannot_find_function_diff_params(symtab, param_list->val, counter, ifi);
                ifi->id->type = d_undef;
                return;
            }
        }
    }
    ifi->id->type = symbol->type;

}

void check_inv_parameters(table_element **symtab, is_function_invocation * ifi){
    table_element* local_symbol = search_symbol(*symtab, ifi->id->id);
    table_element* global_symbol = search_symbol(program->symtab, ifi->id->id);
    if (local_symbol != NULL){
        if (local_symbol->type_dec == d_var_declaration){
            print_cannot_find_function_(symtab, ifi);
            ifi->id->type = d_undef;
        }else{
            check_params(symtab, local_symbol, ifi);
        }
    }else if (global_symbol!=NULL){
        if (global_symbol->type_dec == d_var_declaration){
            print_cannot_find_function_(symtab, ifi);
            ifi->id->type = d_undef;
        }else{
            check_params(symtab, global_symbol, ifi);
        }
    }else{
        print_cannot_find_function_(symtab, ifi);
        ifi->id->type = d_undef;
    }
}
