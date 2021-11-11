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


is_declaration * insert_func_declaration(is_parameters_list * ipl, char*type, is_func_body * ifb){
	is_declaration * id = (is_declaration *) malloc(sizeof(is_declaration));
	id->dec.ifd = (is_func_dec*) malloc(sizeof(is_func_dec));

	id->type_dec = d_func_dec;
	id->dec.ifd->ipl = ipl;
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

