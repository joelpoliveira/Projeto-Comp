#include "structures.h"
#include "functions.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

is_program * insert_program(is_declarations_list * idl){
	is_program * ip = (is_program*) malloc(sizeof(is_program));
	ip->idlist = idl;

	return ip;
}

is_declarations_list * insert_declaration(is_declarations_list * head, is_declaration * id){
	is_declarations_list * idl = (is_declarations_list*) malloc(sizeof(is_declarations_list));
	is_declarations_list * aux;

	idl->val = id;
	idl->next = NULL;

	if (head == NULL)
		return idl;
	
	for( aux = head; aux->next; aux = aux->next);
	aux->next = idl;

	return head;
}

is_declaration * insert_var_declaration(is_var_spec * ivs){
	is_declaration * id = (is_declaration*) malloc(sizeof(is_declaration));
	id->dec.ivd = (is_var_dec*) malloc(sizeof(is_var_dec)); 
	
	id->type_dec = d_var_dec;
	id->dec.ivd->ivs = ivs;

	return id;
}


is_declaration * insert_func_declaration(is_parameter * iparam, char*type, is_func_body * ifb){
	is_declaration * id = (is_declaration *) malloc(sizeof(is_declaration));
	id->dec.ifd = (is_func_dec*) malloc(sizeof(is_func_dec));

	id->type_dec = d_func_dec;
	id->dec.ifd->ipl = iparam;
	id->dec.ifd->ifb = ifb;
	id->dec.ifd->type = (char *) strdup(type);

	return id;
}

is_var_spec * insert_var_specifications(is_id_list * iis, char * type){
	is_var_spec * ivs = (is_var_spec *) malloc(sizeof(is_var_spec));
	ivs->iil = iis;
	ivs->type = (char *) strdup(type);
	return ivs;
}

is_id_list * insert_var_id(is_id_list * head, is_var_id * ivi ){
	is_id_list * iil = (is_id_list *) malloc(sizeof(is_id_list));
	is_id_list * aux;

	iil->val = ivi;
	iil->next = NULL;

	if (head == NULL)
		return iil;
	
	for (aux = head; aux->next; aux = aux-> next);
	aux->next = iil;

	return head;
}

is_parameter * insert_parameter(char * id, char * type, is_id_type_list * iitl){
	is_id_type_list * new_head = (is_id_type_list *) malloc(sizeof(is_id_type_list));
	new_head->val = (is_id_type *) malloc(sizeof(is_id_type));

	new_head->val->id = (char *) strdup(id);
	if ( !strcmp("INT", type) )
		new_head->val->type_param = d_integer;
	else if ( !strcmp("FLOAT32", type) )
		new_head->val->type_param = d_float32;
	else if ( !strcmp("BOOL", type) )
		new_head->val->type_param = d_bool;
	else if ( !strcmp("STRING", type) )
		new_head->val->type_param = d_str;
	
	new_head->next = iitl;

	is_parameter * ip = (is_parameter *) malloc(sizeof(is_parameter));
	ip->val = new_head;

	return ip;
}

is_id_type_list * insert_id_type(is_id_type_list * head, char * id, char * type){
	is_id_type_list * iitl = (is_id_type_list *) malloc(sizeof(is_id_type_list));
	is_id_type_list * aux;

	is_id_type * new_value = (is_id_type *) malloc(sizeof(is_id_type));
	new_value->id = (char *) strdup(type);
	
	if ( !strcmp("INT", type) )
		new_value->type_param = d_integer;
	else if ( !strcmp("FLOAT32", type) )
		new_value->type_param = d_float32;
	else if ( !strcmp("BOOL", type) )
		new_value->type_param = d_bool;
	else if ( !strcmp("STRING", type) )
		new_value->type_param = d_str;

	iitl->val = new_value;
	iitl->next = NULL;

	if (head == NULL)
		return iitl;

	for (aux = head; aux->next; aux = aux->next);
	aux ->next = iitl;

	return head;
}

is_func_body * insert_func_body(is_vars_and_statements_list * ivsl){
	is_func_body * ifb = (is_func_body * ) malloc(sizeof(ifb));

	ifb->ivsl = ivsl;

	return ifb;
}

is_vars_and_statements_list * insert_var_dec(is_vars_and_statements_list * head, is_var_dec * ivd){
	is_vars_and_statements_list * ivsl = (is_vars_and_statements_list *) malloc(sizeof(is_vars_and_statements_list));
	is_vars_and_statements_list * aux;

	is_var_or_statement * ivs = (is_var_or_statement*) malloc(sizeof(is_var_or_statement));
	ivs->type = d_var;
	ivs->body.ivd = ivd;

	ivsl->val = ivs;
	ivsl->next = NULL;

	if (head == NULL)
		return ivsl;

	for(aux = head; aux->next; aux = aux->next);
	aux->next = ivsl;

	return head;
}


is_vars_and_statements_list * insert_statements(is_vars_and_statements_list * head, is_statement * is){
	is_vars_and_statements_list * ivsl = (is_vars_and_statements_list *) malloc(sizeof(is_vars_and_statements_list));
	is_vars_and_statements_list * aux;

	is_var_or_statement * ivs = (is_var_or_statement*) malloc(sizeof(is_var_or_statement));
	ivs->type = d_statement;
	ivs->body.is = is;

	ivsl->val = ivs;
	ivsl->next = NULL;

	if (head == NULL)
		return ivsl;

	for(aux = head; aux->next; aux = aux->next);
	aux->next = ivsl;

	return head;
}

is_statement * insert_if_statement(is_expression_list * iel, is_statements_list * if_isl, is_statements_list * else_isl){
	is_statement * is = (is_statement* ) malloc(sizeof(is_statement));
	is_if_statement * iis = (is_if_statement *) malloc(sizeof(is_if_statement));
	
	iis->iel = iel;
	iis->isl = if_isl;
	
	if(else_isl != NULL){
		iis->ies = (is_else_statement *) malloc(sizeof(is_else_statement));
		iis->ies->isl = else_isl;
	}else
		iis->ies = NULL;
	
	is->type_state = d_if;
	is->statement.u_if_state = iis;

	return is;
}

is_statement * insert_for_statement(is_expression_list * iel, is_statements_list* isl){
	is_statement * is = (is_statement* ) malloc(sizeof(is_statement));
	is_for_statement * ifs = (is_for_statement *) malloc(sizeof(is_for_statement));

	ifs->iel = iel;
	ifs->isl = isl;

	is->type_state = d_for;
	is->statement.u_for_state = ifs;

	return is;
}

is_statement * insert_return_statement(is_expression_list * iel){
	is_statement * is = (is_statement*) malloc(sizeof(is_statement));
	is_return_statement * irs = (is_return_statement*) malloc(sizeof(is_return_statement));

	irs->iel = iel;
	is->type_state = d_return;
	is->statement.u_return_state = irs;

	return is;
}

is_statement * insert_print_expr_statement(is_expression_list * iel){
	is_statement * is = (is_statement *) malloc(sizeof(is_statement));
	is_print_statement * ips = (is_print_statement *) malloc(sizeof(is_print_statement));

	ips->type_print = d_expression;
	ips->print.iel = iel;

	is->type_state = d_print;
	is->statement.u_print_state = ips;

	return is;
}


is_statement * insert_print_str_statement(is_str * istr){
	is_statement * is = (is_statement *) malloc(sizeof(is_statement));
	is_print_statement * ips = (is_print_statement *) malloc(sizeof(is_print_statement));

	ips->type_print = d_str;
	ips->print.u_str_state = istr;

	is->type_state = d_print;
	is->statement.u_print_state = ips;

	return is;
}

is_statement * insert_assign_statement(char * id, is_expression_list * iel){
	is_statement * is = (is_statement *) malloc(sizeof(is_statement));
	is_assign_statement * ias = (is_assign_statement*) malloc(sizeof(is_assign_statement));

	ias->id = (char *) strdup(id);
	ias->iel = iel;

	is->type_state = d_assign;
	is->statement.u_assign = ias;

	return is;
}

is_statement * insert_statements_list(is_statements_list * isl){
	is_statement * is = (is_statement *) malloc(sizeof(is_statement));

	is->type_state = d_statement_list;
	is->statement.isl = isl;

	return is;
}

is_statement * insert_final_statement(is_final_statement * ifs){
	is_statement * is = (is_statement*) malloc( sizeof(is_statement));

	is->type_state = d_final_statement;
	is->statement.u_state = ifs;

	return is;
}

is_final_statement * insert_final_state_args(is_parse_arguments * ipa){
	is_final_statement * ifs = (is_final_statement *) malloc(sizeof(is_final_statement));

	ifs->type_state = d_arguments;
	ifs->statement.ipa = ipa;

	return ifs;
}

is_final_statement * insert_final_state_func_inv(is_function_invocation * ifi){
	is_final_statement * ifs = (is_final_statement *) malloc(sizeof(is_final_statement));
	
	ifs->type_state = d_func_invoc;
	ifs->statement.ifi = ifi;

	return ifs;
}

is_statements_list * insert_statement_in_list(is_statements_list * head, is_statement * is){
	is_statements_list * isl = (is_statements_list *) malloc(sizeof(is_statements_list));
	is_statements_list * aux;

	isl->next = NULL;
	isl->val = is;

	if (head == NULL)
		return isl;

	for (aux = head; aux->next; aux = aux->next);
	aux->next = isl;

	return head;
}

is_parse_arguments * insert_parse_args( char * id, is_expression_list * iel){
	is_parse_arguments * ipa = (is_parse_arguments*) malloc(sizeof(is_parse_arguments));

	ipa->id = (char * )strdup(id);
	ipa->iel = iel;

	return ipa;
}

is_function_invocation * insert_func_inv(is_expression_list * iel, is_expression_list * head){

}
