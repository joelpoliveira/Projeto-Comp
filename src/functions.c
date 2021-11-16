#include "structures.h"
#include "functions.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void print(is_expression_list * iel){
	if (iel == NULL) return;
	is_expression_list * aux;

	printf("=======START===========\n");
	for(aux = iel; aux; aux = aux->next){
		printf("Exp --> "); printf("%d\n", aux->type_expr);
		if (aux->type_expr == 0){
			printf("\tkey --> "); printf("%d\n", aux->op_type);
		}
	}
	printf("=============END=======\n");
}

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
	
	id->type_dec = d_var_declaration;
	id->dec.ivd->ivs = ivs;

	return id;
}


is_declaration * insert_func_declaration(char * id, is_parameter * iparam, parameter_type type, is_func_body * ifb){
	is_declaration * idec = (is_declaration *) malloc(sizeof(is_declaration));
	idec->dec.ifd = (is_func_dec*) malloc(sizeof(is_func_dec));
	idec->type_dec = d_func_dec;

	idec->dec.ifd->id = (char*) strdup(id);
	idec->dec.ifd->ipl = iparam;
	idec->dec.ifd->ifb = ifb;
	idec->dec.ifd->type = type;

	return idec;
}

is_var_spec * insert_var_specifications(char * id, is_id_list * iis, parameter_type type){
	is_var_spec * ivs = (is_var_spec *) malloc(sizeof(is_var_spec));
	is_id_list * new_head = (is_id_list*)malloc(sizeof(is_id_list));

	new_head->val = (char * ) strdup(id);
	new_head->next = iis;

	ivs->iil = new_head;
	ivs->type = type;
	return ivs;
}

is_id_list * insert_var_id(is_id_list * head, char * id ){
	is_id_list * iil = (is_id_list *) malloc(sizeof(is_id_list));
	is_id_list * aux;

	iil->val = (char *) strdup(id);
	iil->next = NULL;

	if (head == NULL)
		return iil;
	
	for (aux = head; aux->next; aux = aux-> next);
	aux->next = iil;

	return head;
}

is_parameter * insert_parameter(char * id, parameter_type type, is_id_type_list * iitl){
	is_id_type_list * new_head = (is_id_type_list *) malloc(sizeof(is_id_type_list));
	new_head->val = (is_id_type *) malloc(sizeof(is_id_type));

	new_head->val->id = (char *) strdup(id);
	new_head->val->type_param = type;
	
	new_head->next = iitl;

	is_parameter * ip = (is_parameter *) malloc(sizeof(is_parameter));
	ip->val = new_head;

	return ip;
}

is_id_type_list * insert_id_type(is_id_type_list * head, char * id, parameter_type type){
	is_id_type_list * iitl = (is_id_type_list *) malloc(sizeof(is_id_type_list));
	is_id_type_list * aux;

	is_id_type * new_value = (is_id_type *) malloc(sizeof(is_id_type));
	new_value->id = (char *) strdup(id);
	new_value->type_param = type;

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
	ivs->type = d_var_dec;
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


is_statement * insert_print_str_statement(char * id){
	is_statement * is = (is_statement *) malloc(sizeof(is_statement));
	is_print_statement * ips = (is_print_statement *) malloc(sizeof(is_print_statement));

	ips->type_print = d_str;
	ips->print.id = (char*)strdup(id);

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
	
	ifs->type_state = d_function_invoc;
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

is_function_invocation * insert_func_inv(char * id, is_expression_list * head, is_expression_list * iel){
	is_function_invocation * ifi = (is_function_invocation * ) malloc(sizeof(is_function_invocation));
	is_expression_list * aux ;

	if (iel!=NULL){
		for (aux = head; aux->next; aux= aux->next); 
		aux->next = iel;
	}

	ifi->id = (char *) strdup(id);
	ifi->iel = head;

	return ifi;
}

is_expression_list * insert_expression(is_expression_list * head, is_expression_list * iel){
	is_expression_list * aux;

	if (head == NULL){
		return iel;
	}

	for (aux = head; aux->next; aux = aux->next);
	aux->next = iel;

	return head;
}

is_expression_list * insert_first_oper(is_expression_list * head, operation_type type, is_expression2_list * ie2l){
	is_expression_list * iel = malloc(sizeof(is_expression_list));
	is_expression_list * aux;

	iel->next = head;
	iel->type_expr = d_operation;
	iel->op_type = type;
	iel->ie2l = ie2l;

	//printf("<< First Oper >>\n");
	//print(iel);
	return iel;
}

is_expression_list * insert_first_expr(is_expression2_list * ie2l){
	is_expression_list * head = malloc(sizeof(is_expression_list));
	is_expression_list * aux;

	head->next = NULL;
	head->type_expr = d_expr;	
	head->op_type = d_none;
	head->ie2l = ie2l;

	//printf("<< First Expr >>\n");
	//print(head);
	return head;
	
}

is_expression2_list * insert_second_oper(is_expression2_list * head, self_operation_type iso){
	is_expression2_list * ie2l =(is_expression2_list*) malloc(sizeof(is_expression2_list));
	is_expression2_list * aux;

	ie2l->next = NULL;
	ie2l->iso = iso;
	ie2l->type_expression = d_self_oper;

	if( head == NULL )
		return ie2l;
	
	for (aux = head; aux->next; aux=aux->next);
	aux->next = ie2l;

	return head;
}

is_expression2_list * insert_second_expr(is_final_expression * ife){
	is_expression2_list * ie2l =(is_expression2_list*) malloc(sizeof(is_expression2_list));
	ie2l->type_expression = d_expr_2;
	ie2l->iso = d_self_none;
	ie2l->ife = ife;
	ie2l->next = NULL;

	return ie2l;
}

is_final_expression * insert_intlit(char * id){
	is_final_expression * ife = (is_final_expression *)malloc(sizeof(is_final_expression));
	is_intlit * intlit = (is_intlit*)malloc(sizeof(is_intlit));
	intlit->intlit = (char *) strdup(id);

	ife->type_final_expression = d_intlit;
	ife->expr.u_intlit = intlit;

	return ife;
}

is_final_expression * insert_real(char * id){
	is_final_expression * ife = (is_final_expression *)malloc(sizeof(is_final_expression));
	is_reallit * real = (is_reallit *) malloc(sizeof(is_reallit));
	real->reallit = (char *)strdup(id);

	ife->type_final_expression = d_reallit;
	ife->expr.u_reallit = real;

	return ife;
}

is_final_expression * insert_id(char * id){
	is_final_expression * ife = (is_final_expression *)malloc(sizeof(is_final_expression));
	is_id * iid = (is_id*)malloc(sizeof(is_id));
	iid->id = (char *) strdup(id);

	ife->type_final_expression = d_id;
	ife->expr.u_id = iid;

	return ife;
}

is_final_expression * insert_final_func_inv(is_function_invocation * ifi){
	is_final_expression * ife = (is_final_expression *)malloc(sizeof(is_final_expression));

	ife->type_final_expression = d_func_inv;
	ife->expr.ifi = ifi;

	return ife;
}

is_final_expression * insert_final_expr(is_expression_list*iel){
	is_final_expression * ife = (is_final_expression *)malloc(sizeof(is_final_expression));

	ife->type_final_expression = d_expr_final;
	ife->expr.iel = iel;

	//print(iel);
	return ife;
}

operation_type insert_oper(char * oper){
	if( !strcmp(oper, "OR") )
		return d_or;
	else if( !strcmp(oper, "AND") )
		return d_and;	
	else if( !strcmp(oper, "LT") )
		return d_lt;
	else if( !strcmp(oper, "GT") )
		return d_gt;
	else if( !strcmp(oper, "EQ") )
		return d_eq;
	else if( !strcmp(oper, "NE") )
		return d_ne;
	else if( !strcmp(oper, "GE") )
		return d_ge;
	else if( !strcmp(oper, "LE") )
		return d_le;
	else if( !strcmp(oper, "PLUS") )
		return d_plus;
	else if( !strcmp(oper, "MINUS") )
		return d_minus;
	else if( !strcmp(oper, "STAR") )
		return d_star;
	else if( !strcmp(oper, "DIV") )
		return d_div;
	else if( !strcmp(oper, "MOD") )
		return d_mod;
	return d_none;
}

self_operation_type insert_self_oper(char * oper){
	if( !strcmp(oper, "PLUS") )
		return d_self_plus;
	else if( !strcmp(oper, "MINUS") )
		return d_self_minus;
	else if( !strcmp(oper, "NOT") )
		return d_self_not;
	return d_self_none;
}

parameter_type insert_type(char * type){
	if ( !strcmp(type, "INT") )
		return d_integer;
	else if ( !strcmp(type, "FLOAT") )
		return d_float32;
	else if ( !strcmp(type, "BOOL") )
		return d_bool;
	return d_string;
}
