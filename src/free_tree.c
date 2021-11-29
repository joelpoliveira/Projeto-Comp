#include "free_tree.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void free_ast(is_program* root){
    if (root == NULL) return;

    free_declarations(root->idlist);
	free(root);
}


void free_declarations(is_declarations_list* idl){
    if (idl == NULL) return;

    is_declarations_list* current = idl;

    while (current != NULL) {

        declaration_type type = current->val->type_dec;

        switch (type){
            case d_func_dec:
            
                free_func_dec(current->val->dec.ifd);
                break;

            case d_var_declaration:
                free_var_dec(current->val->dec.ivd);
            break;

            default:
                break;
        }
        
		current = current->next;
		free(idl);
		idl = current;
    }
}


void free_var_dec(is_var_dec * ivd){
    if (ivd == NULL) return;    
    
    free_var_spec(ivd->ivs);
	free(ivd);
}


void free_func_dec(is_func_dec* ifd){
	if ( ifd == NULL) return;

	free(ifd->id);
    free_func_params(ifd->ipl);
    free_func_body(ifd->ifb);
	free(ifd);
    
}

void free_func_params(is_parameter* ipl){
    if (ipl == NULL) return;

    is_id_type_list* current = ipl->val,* aux;

    while (current != NULL) {

		free(current->val->id);
		aux = current;
        current = current->next;
		free(aux);
    }
	free(ipl);

}


void free_func_body(is_func_body* ifb){
    if (ifb == NULL) return;

    is_vars_and_statements_list* current = ifb->ivsl;


    while (current != NULL) {
        free_var_or_statement(current->val);
        current = current->next;
		free(ifb->ivsl);
		ifb->ivsl = current;
    }
	free(ifb);
}


void free_var_or_statement(is_var_or_statement* val){
    if (val == NULL) return;
    
    switch (val->type){
        case d_var_dec:
            free_var_spec(val->body.ivd->ivs);
            break;

        case d_statement:
            free_statement(val->body.is);
            break;

        default:
            break;
    }
	free(val);
}


void free_var_spec(is_var_spec* ivs){
    if (ivs == NULL) return;
    
    is_id_list* current = ivs->iil;

    while (current != NULL){
		
		free(current->val);
        current = current->next;        
		free(ivs->iil);
		ivs->iil = current;
    }
	free(ivs);
}


void free_statement(is_statement* is) {
    if (is == NULL) return;

    statement_type type = is->type_state;
    switch (type) {
        case d_if:
            free_if(is->statement.u_if_state);
            break;
        case d_for:
            free_for(is->statement.u_for_state);
            break;
        case d_return:
            free_return(is->statement.u_return_state);
            break;
        case d_print:
            free_print(is->statement.u_print_state);
            break;
        case d_assign:
            free_assign(is->statement.u_assign);
            break;
        case d_statement_list:
            free_statement_list(is->statement.isl);
            break;
        case d_final_statement:
            free_final_statement(is->statement.u_state);
            break;
        default:
            break;
    }
	free(is);
}


void free_if(is_if_statement* iifs){
    if (iifs == NULL) return;

    free_expression_or(iifs->iel);

    free_statement_list(iifs->isl);
    
    free_else(iifs->ies);
	free(iifs);
}

void free_else(is_else_statement* ies){
    if (ies == NULL) return;
	
    free_statement_list(ies->isl); // adicionado +1
	free(ies);
}

void free_print(is_print_statement* ips){
    if (ips == NULL) return;

    print_type type = ips->type_print; // {d_expression, d_str}

    switch (type){
        case d_expression:
            free_expression_or(ips->print.iel);
            break;
        case d_str:
			free(ips->print.id);
            break;
        default:
            break;
    }
	free(ips);
}

void free_statement_list(is_statements_list* isl){
    if (isl == NULL) return;

    is_statements_list * current = isl;
    
    while( current != NULL){
        free_statement(current->val);
        current = current->next;
		free(isl);
		isl = current;
    }

}

void free_for(is_for_statement* ifs){
    if (ifs == NULL) return;
    
    free_expression_or(ifs->iel);

    free_statement_list(ifs->isl);
	free(ifs);
}


void free_return(is_return_statement* irs){
    if (irs == NULL) return;
    free_expression_or(irs->iel);
    free(irs);
}

void free_final_statement(is_final_statement* ifs) {
    if (ifs == NULL) return;

    final_state_type type = ifs->type_state; 

    is_func_inv_expr_list * current;
    switch (type){
        case d_function_invoc:
            free(ifs->statement.ifi->id);
            current = ifs->statement.ifi->iel;

            while ( current ){
                free_expression_or(current->val); //alterado +1 antes 0
                current = current->next;
                free(ifs->statement.ifi->iel);
                ifs->statement.ifi->iel = current;
            }

            free(ifs->statement.ifi);
            break;
        case d_arguments:
            free_parse_args(ifs->statement.ipa);
            break;
        
        default:
            break;
    }
    free(ifs);
}

void free_parse_args(is_parse_arguments * ipa){
    if ( ipa == NULL) return;
    
    free(ipa->id);
    free_expression_or(ipa->iel);
    free(ipa);
}

void free_assign(is_assign_statement* ias){
    if (ias == NULL) return;
    free(ias->id);
    free_expression_or(ias->iel);
    free(ias);
}

void free_expression_or(is_expression_or_list* ieol){
    if (ieol == NULL) return;

    free_expression_or(ieol->next_left);
    free_expression_and(ieol->next_right);
    free(ieol);    
}


void free_expression_and(is_expression_and_list* ieal){
    if (ieal == NULL) return;
  
    free_expression_and(ieal->next_left);
    free_expression_comp(ieal->next_right);
    free(ieal);
}

void free_expression_comp(is_expression_comp_list * iecl){
    if (iecl == NULL) return;

    free_expression_comp(iecl->next_left);
    free_expression_sum(iecl->next_right);
    free(iecl);
    
}

void free_expression_sum(is_expression_sum_like_list * iesl){
    if (iesl == NULL) return;
    free_expression_sum(iesl->next_left);
    free_expression_star(iesl->next_right);
    free(iesl);
    
}

void free_expression_star(is_expression_star_like_list * iestl){
    if (iestl == NULL) return;

    free_expression_star(iestl->next_left);
    free_expression_self(iestl->next_right);
    free(iestl);
}

void free_expression_self(is_self_expression_list * isel){
    if (isel == NULL) return;
    
    free_expression_self(isel->next_same);
    free_expression_final(isel->next_final);
    free(isel);
}


void free_expression_final(is_final_expression * ife){
    if (ife == NULL) return;
    switch (ife->type_final_expression){
        case d_intlit:
            free(ife->expr.u_intlit->intlit);
            free(ife->expr.u_intlit);
            break;
        case d_reallit:
            free(ife->expr.u_reallit->reallit);
            free(ife->expr.u_reallit);
            break;
        case d_id:
            free(ife->expr.u_id->id);
            free(ife->expr.u_id);
            break;
        case d_func_inv:
            
            free_func_invocation(ife->expr.ifi);
            break;
        case d_expr_final:
            free_expression_or(ife->expr.ieol); 
            break;
        default:
            break;
    }
    free(ife);
}

void free_func_invocation(is_function_invocation * ifi){
    if ( ifi == NULL) return;

    free(ifi->id);
    is_func_inv_expr_list * current = ifi->iel;

    while ( current ){
        free_expression_or(current->val);
        current = current->next;
        free(ifi->iel);
        ifi->iel = current;
    }
    free(ifi);
}

