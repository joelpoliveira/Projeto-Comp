#include "structures.h"

void llvm_store(id_token* a, id_token* b);
void llvm_print_type(parameter_type type);
void llvm_program(is_program* ip);
void llvm_declarations_list(is_declarations_list* idl);
void llvm_func_declaration(is_func_dec* ifd);
void llvm_function_body(is_func_body* ifb);
void llvm_vars_and_statements_list(is_vars_and_statements_list* ivsl);
void llvm_var_declaration(is_var_dec* ivd, bool is_global);
void llvm_var_spec(is_var_spec* ivs, bool is_global);
void llvm_is_parameter(is_parameter * ip);
void llvm_statement( is_statement* is);
void llvm_if_statement(is_if_statement* ifs);
void llvm_else_statement(is_else_statement* ies);
void llvm_for_statement(is_for_statement* ifs);
void llvm_return_statement(is_return_statement* irs);
void llvm_print_statement(is_print_statement* ips);
void llvm_assign_statement(is_assign_statement* ias);
void llvm_statements_list(is_statements_list* isl);
void llvm_final_statement(is_final_statement* ifs);
id_token* llvm_expression_or_list(is_expression_or_list* ieol);
id_token* llvm_expression_and_list(is_expression_and_list* ieal);
id_token* llvm_expression_comp_list(is_expression_comp_list * iecl);
id_token* llvm_expression_sum_like_list(is_expression_sum_like_list * iesl);
id_token* llvm_expression_star_like_list(is_expression_star_like_list * iestl);
id_token* llvm_self_expression_list(is_self_expression_list * isel);
id_token * llvm_final_expression(is_final_expression * ife);
void llvm_func_invocation(is_function_invocation * ifi);




/* void llvm_program(is_program* ip){
    //llvm_declarations_list(ip->idlist);
    table_element* func_table;
    table_element *symbol;

    //global variables
    for(table_element* aux = ip->symtab; aux; aux = aux->next){
        if (aux->type_dec == d_var_declaration){
            printf("@%s = global ", aux->id->id);
            llvm_print_type(aux->type);
            printf(" 0\n");
        }
    }

    printf("\n");

    //functions
    for(table_element* aux = ip->symtab; aux; aux = aux->next){
        if (aux->type_dec == d_func_dec){
            func_counter = 0;

            func_table = get_function_table(program, aux->id->id);

            //Func declaration
            printf("define ");
            llvm_print_type(aux->type);
            printf(" @%s (", aux->id->id);
            llvm_is_parameter(aux->dec.ifd->ipl);
            printf(") {\n");

            func_counter++;

            //Print func symbols
            for (table_element* temp = func_table; temp; temp = temp->next){
                //ignore return ? 
                if (strcmp(temp->id->id, "return") == 0) continue;
                printf("\t%%%s = alloca ", temp->id->id);
                llvm_print_type(temp->type);
                printf(" ; (%s)", temp->id->id);
                printf("\n");
            }
            
            //print tabela da função para as declarações
            //llvm_function_body(aux->dec.ifd->ifb);
            
            // return
            symbol = search_symbol(func_table, "return");
            printf("\tret ");
            if (aux->type == d_none)
                printf("void");
            else{
                llvm_print_type(symbol->type);
                printf(" 0"); // placeholder ret <tipo> registo
            }

            printf("\n}\n\n");
        }
    }

} */


