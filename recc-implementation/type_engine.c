/*
    Copyright 2016 Robert Elder Software Inc.
    
    Licensed under the Apache License, Version 2.0 (the "License"); you may not 
    use this file except in compliance with the License.  You may obtain a copy 
    of the License at
    
        http://www.apache.org/licenses/LICENSE-2.0
    
    Unless required by applicable law or agreed to in writing, software 
    distributed under the License is distributed on an "AS IS" BASIS, WITHOUT 
    WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the 
    License for the specific language governing permissions and limitations 
    under the License.
*/
#include "type_engine.h"


/*  Private functions:  Do not call externally */
struct aggregate_specifier_id aggregate_specifier_from_any_specifier_id(struct type_engine_state *, enum type_engine_aggregate_specifier_kind, unsigned int);
struct specifier_or_qualifier_id get_type_engine_id_for_any_specifier_or_qualifier(struct type_engine_state *, enum type_engine_specifier_or_qualifier_kind, unsigned int);
struct specifier_or_qualifier_list_item_id get_type_engine_specifier_qualifier_list_id(struct type_engine_state *, enum type_engine_specifier_or_qualifier_list_item_kind, struct specifier_or_qualifier_list_item_id, struct specifier_or_qualifier_id);
struct declarator_part_list_item_id get_type_engine_declarator_part_list_id(struct type_engine_state *, enum type_engine_declarator_part_list_item_kind, struct declarator_part_list_item_id, struct declarator_part_id);
struct declarator_part_id get_type_engine_id_for_declarator_part(struct type_engine_state * state, enum type_engine_declarator_part_kind type, unsigned int index);
struct parameter_list_id get_type_engine_id_for_parameter_list(struct type_engine_state *, enum type_engine_parameter_list_kind, unsigned int);
struct bitfield_or_declarator_id get_type_engine_id_for_bitfield_or_declarator(struct type_engine_state *, enum type_engine_bitfield_or_declarator_kind, unsigned int);
struct general_type_list_item_id get_type_engine_general_type_list_item_id(struct type_engine_state *, enum type_engine_general_type_list_item_kind, struct general_type_list_item_id, struct general_type_id);
struct struct_or_union_specifier_id get_type_engine_id_for_struct_or_union_specifier(struct type_engine_state *, enum type_engine_struct_or_union_specifier_kind, unsigned int);
struct struct_specifier_id get_aggregate_struct_specifier_id(struct type_engine_state *, enum type_engine_struct_specifier_kind, unsigned int);
struct union_specifier_id get_aggregate_union_specifier_id(struct type_engine_state *, enum type_engine_union_specifier_kind, unsigned int);
struct function_part_id get_type_engine_id_for_function_part(struct type_engine_state *, enum type_engine_function_part_kind, unsigned int);
struct array_part_id get_type_engine_id_for_array_part(struct type_engine_state *, enum type_engine_array_part_kind, unsigned int);

static const char * g_extern_string = "extern";
static const char * g_static_string = "static";
static const char * g_auto_string = "auto";
static const char * g_typedef_string = "typedef";
static const char * g_volatile_string = "volatile";
static const char * g_void_string = "void";
static const char * g_char_string = "char";
static const char * g_short_string = "short";
static const char * g_int_string = "int";
static const char * g_long_string = "long";
static const char * g_float_string = "float";
static const char * g_double_string = "double";
static const char * g_const_string = "const";
static const char * g_signed_string = "signed";
static const char * g_unsigned_string = "unsigned";
static const char * g_ptr_string = "*";
static const char * g_open_paren_string = "(";
static const char * g_close_paren_string = ")";
static const char * g_open_square_bracket_string = "[";
static const char * g_close_square_bracket_string = "]";

struct parser_node * destroy_parser_node_tree_and_c_lexer_tokens(struct memory_pool_collection * m, struct parser_node * n){
	if(n){
		if(n->c_lexer_token){
			struct_c_lexer_token_memory_pool_free(m->struct_c_lexer_token_pool, n->c_lexer_token);
		}
		destroy_parser_node_tree_and_c_lexer_tokens(m, n->first_child);
		destroy_parser_node_tree_and_c_lexer_tokens(m, n->next);
		struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n);
	}
	return (struct parser_node *)0;
}

struct parser_node * copy_parser_node_children_only(struct memory_pool_collection * m, struct parser_node * n){
	/*  Create a copy of only the children of n.  Often, we don't want to copy the node's sibilings */
	if(n){
		struct parser_node * new_node = struct_parser_node_memory_pool_malloc(m->struct_parser_node_pool);
		new_node->type = n->type;
		new_node->first_child = copy_parser_node_tree_and_c_lexer_tokens(m, n->first_child);
		new_node->next = (struct parser_node *)0;
		if(n->c_lexer_token){
			new_node->c_lexer_token = struct_c_lexer_token_memory_pool_malloc(m->struct_c_lexer_token_pool);
			new_node->c_lexer_token->type = n->c_lexer_token->type;
			new_node->c_lexer_token->first_byte = n->c_lexer_token->first_byte;
			new_node->c_lexer_token->last_byte = n->c_lexer_token->last_byte;
		}else{
			new_node->c_lexer_token = (struct c_lexer_token *)0;
		}
		return new_node;
	}else{
		return (struct parser_node *)0;
	}
}

struct parser_node * remove_one_pointer_level(struct memory_pool_collection * m, struct parser_node * n){
	assert(n->type == POINTER);
	if(n->first_child->next && n->first_child->next->type == POINTER){
		n->first_child->next = remove_one_pointer_level(m, n->first_child->next);
		return n;
	}else if(n->first_child->next && n->first_child->next->next && n->first_child->next->next->type == POINTER){
		n->first_child->next->next = remove_one_pointer_level(m, n->first_child->next->next);
		return n;
	}else{
		/*  This will delete the pointer and possible type qualifier list */
		destroy_parser_node_tree_and_c_lexer_tokens(m, n);
		return (struct parser_node *)0;
	}
}

struct parser_node * copy_parser_node_tree_and_c_lexer_tokens(struct memory_pool_collection * m, struct parser_node * n){
	/*  Create a copy of every node we can reach from n */
	if(n){
		struct parser_node * node_to_copy = n;
		struct parser_node * top_node = struct_parser_node_memory_pool_malloc(m->struct_parser_node_pool);
		struct parser_node * current_dest_node = top_node;
		while(node_to_copy){
			current_dest_node->type = node_to_copy->type;
			if(node_to_copy->next){
				current_dest_node->next = struct_parser_node_memory_pool_malloc(m->struct_parser_node_pool);
			}else{
				current_dest_node->next = (struct parser_node *)0;
			}
			current_dest_node->first_child = copy_parser_node_tree_and_c_lexer_tokens(m, node_to_copy->first_child);
			if(node_to_copy->c_lexer_token){
				current_dest_node->c_lexer_token = struct_c_lexer_token_memory_pool_malloc(m->struct_c_lexer_token_pool);
				current_dest_node->c_lexer_token->type = node_to_copy->c_lexer_token->type;
				current_dest_node->c_lexer_token->first_byte = node_to_copy->c_lexer_token->first_byte;
				current_dest_node->c_lexer_token->last_byte = node_to_copy->c_lexer_token->last_byte;
			}else{
				current_dest_node->c_lexer_token = (struct c_lexer_token *)0;
			}
			node_to_copy = node_to_copy->next;
			current_dest_node = current_dest_node->next;
		}
		return top_node;
	}else{
		return (struct parser_node *)0;
	}
}

struct parser_node * convert_abstract_declarator_to_pointer_type(struct memory_pool_collection * m, struct parser_node * n){
	if(n == 0){
		return (struct parser_node *)0;
	}

	switch(n->type){
		case DIRECT_ABSTRACT_DECLARATOR:{
			n->first_child = convert_abstract_declarator_to_pointer_type(m, n->first_child);
			n->next = (struct parser_node *)0;
			if(n->first_child){
				return n;
			}else{
				/*  This node does not need to exist if it does not have a child */
				struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n);
				return (struct parser_node *)0;
			}
		}case ABSTRACT_DECLARATOR:{
			assert(!(n->next) && "This function only handles the top-most case of abstract_declarator directly.");
			if(n->first_child->next){
				n->first_child->next = convert_abstract_declarator_to_pointer_type(m, n->first_child->next);
				return n;
			}
			n->first_child = convert_abstract_declarator_to_pointer_type(m, n->first_child);
			if(n->first_child){
				return n;
			}else{
				struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n);
				return (struct parser_node *)0;
			}
		}case POINTER:{
			if(n->next){
				n->next = convert_abstract_declarator_to_pointer_type(m, n->next);
				return n;
			}else{
				return remove_one_pointer_level(m, n);
			}
		}case EPSILON:{
			return n;
		}case TERMINAL:{
			switch(n->c_lexer_token->type){
				case OPEN_PAREN_CHAR:{
					if(n->next->type == ABSTRACT_DECLARATOR){
						n->next->first_child = convert_abstract_declarator_to_pointer_type(m, n->next->first_child);
						if(n->next->first_child){
							/*  abstract_declarator still exists */
							return n;
						}else{
							/*  In this case, we're going to delete the ( abs ) nodes 
							  Determine if the _rest is an epsilon or not */
							if(n->next->next->next->first_child->type == EPSILON){
								/* There is nothing left */
								destroy_parser_node_tree_and_c_lexer_tokens(m, n->next->next->next);
								struct_c_lexer_token_memory_pool_free(m->struct_c_lexer_token_pool, n->next->next->c_lexer_token);
								struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n->next->next);
								struct_c_lexer_token_memory_pool_free(m->struct_c_lexer_token_pool, n->next->c_lexer_token);
								struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n->next);
								struct_c_lexer_token_memory_pool_free(m->struct_c_lexer_token_pool, n->c_lexer_token);
								struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n);
								return (struct parser_node *)0;
							}else{
								/*  The first_child of the _rest becomes the first_child of the parent */
								struct parser_node * rtn = n->next->next->next->first_child;
								struct_c_lexer_token_memory_pool_free(m->struct_c_lexer_token_pool, n->next->next->next->c_lexer_token);
								struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n->next->next->next);
								struct_c_lexer_token_memory_pool_free(m->struct_c_lexer_token_pool, n->next->next->c_lexer_token);
								struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n->next->next);
								struct_c_lexer_token_memory_pool_free(m->struct_c_lexer_token_pool, n->next->c_lexer_token);
								struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n->next);
								struct_c_lexer_token_memory_pool_free(m->struct_c_lexer_token_pool, n->c_lexer_token);
								struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n);
								return rtn;
							}
						}
					}else{
						assert(0 && "Must not have been a pointer type.");
						return (struct parser_node *)0;
					}
				}default:{
					assert(0);
					return (struct parser_node *)0;
				}
			}
		}default:{
			assert(0);
			return (struct parser_node *)0;
		}
	}
}


struct parser_node * convert_abstract_declarator_to_array_type(struct memory_pool_collection * m, struct parser_node * n){
	if(n == 0){
		return (struct parser_node *)0;
	}

	switch(n->type){
		case DIRECT_ABSTRACT_DECLARATOR:{
			n->first_child = convert_abstract_declarator_to_array_type(m, n->first_child);
			n->next = (struct parser_node *)0;
			if(n->first_child){
				return n;
			}else{
				/*  This node does not need to exist if it does not have a child */
				struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n);
				return (struct parser_node *)0;
			}
		}case ABSTRACT_DECLARATOR:{
			assert(!(n->next) && "This function only handles the top-most case of abstract_declarator directly.");
			if(n->first_child->next){
				n->first_child->next = convert_abstract_declarator_to_array_type(m, n->first_child->next);
				return n;
			}
			n->first_child = convert_abstract_declarator_to_array_type(m, n->first_child);
			if(n->first_child){
				return n;
			}else{
				struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n);
				return (struct parser_node *)0;
			}
		}case POINTER:{
			n->next = convert_abstract_declarator_to_array_type(m, n->next);
			return n;
		}case EPSILON:{
			return n;
		}case TERMINAL:{
			switch(n->c_lexer_token->type){
				case OPEN_PAREN_CHAR:{
					if(n->next->type == ABSTRACT_DECLARATOR){
						n->next->first_child = convert_abstract_declarator_to_array_type(m, n->next->first_child);
						if(n->next->first_child){
							/*  abstract_declarator still exists */
							return n;
						}else{
							/*  In this case, we're going to delete the ( abs ) nodes 
							  Determine if the _rest is an epsilon or not */
							if(n->next->next->next->first_child->type == EPSILON){
								/* There is nothing left */
								destroy_parser_node_tree_and_c_lexer_tokens(m, n->next->next->next);
								struct_c_lexer_token_memory_pool_free(m->struct_c_lexer_token_pool, n->next->next->c_lexer_token);
								struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n->next->next);
								struct_c_lexer_token_memory_pool_free(m->struct_c_lexer_token_pool, n->next->c_lexer_token);
								struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n->next);
								struct_c_lexer_token_memory_pool_free(m->struct_c_lexer_token_pool, n->c_lexer_token);
								struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n);
								return (struct parser_node *)0;
							}else{
								/*  The first_child of the _rest becomes the first_child of the parent */
								struct parser_node * rtn = n->next->next->next->first_child;
								struct_c_lexer_token_memory_pool_free(m->struct_c_lexer_token_pool, n->next->next->next->c_lexer_token);
								struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n->next->next->next);
								struct_c_lexer_token_memory_pool_free(m->struct_c_lexer_token_pool, n->next->next->c_lexer_token);
								struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n->next->next);
								struct_c_lexer_token_memory_pool_free(m->struct_c_lexer_token_pool, n->next->c_lexer_token);
								struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n->next);
								struct_c_lexer_token_memory_pool_free(m->struct_c_lexer_token_pool, n->c_lexer_token);
								struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n);
								return rtn;
							}
						}
					}else{
						assert(0);
						return (struct parser_node *)0;
					}
				}case OPEN_SQUARE_BRACKET_CHAR:{
					if(n->next->type == TERMINAL && n->next->c_lexer_token->type == CLOSE_SQUARE_BRACKET_CHAR){
						if(n->next->next->first_child->type == EPSILON){
							/*  This also destroys the _rest and epsilon */
							destroy_parser_node_tree_and_c_lexer_tokens(m, n->next);
							struct_c_lexer_token_memory_pool_free(m->struct_c_lexer_token_pool, n->c_lexer_token);
							struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n);
							return (struct parser_node *)0;
						}else{
							struct parser_node * rtn = n->next->next->first_child;
							struct_c_lexer_token_memory_pool_free(m->struct_c_lexer_token_pool, n->next->next->c_lexer_token);
							struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n->next->next);
							struct_c_lexer_token_memory_pool_free(m->struct_c_lexer_token_pool, n->next->c_lexer_token);
							struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n->next);
							struct_c_lexer_token_memory_pool_free(m->struct_c_lexer_token_pool, n->c_lexer_token);
							struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n);
							return rtn;
						}
					}else if(n->next->type == CONSTANT_EXPRESSION){
						if(n->next->next->next->first_child->type == EPSILON){
							/*  This also destroys the _rest and epsilon */
							destroy_parser_node_tree_and_c_lexer_tokens(m, n->next);
							struct_c_lexer_token_memory_pool_free(m->struct_c_lexer_token_pool, n->c_lexer_token);
							struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n);
							return (struct parser_node *)0;
						}else{
							struct parser_node * rtn = n->next->next->next->first_child;
							struct_c_lexer_token_memory_pool_free(m->struct_c_lexer_token_pool, n->next->next->next->c_lexer_token);
							struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n->next->next->next);
							struct_c_lexer_token_memory_pool_free(m->struct_c_lexer_token_pool, n->next->next->c_lexer_token);
							struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n->next->next);
							/* Destroy the constant expression */
							destroy_parser_node_tree_and_c_lexer_tokens(m, n->next->first_child);
							struct_c_lexer_token_memory_pool_free(m->struct_c_lexer_token_pool, n->next->c_lexer_token);
							struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n->next);
							struct_c_lexer_token_memory_pool_free(m->struct_c_lexer_token_pool, n->c_lexer_token);
							struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n);
							return rtn;
						}
					}else{
						assert(0 && "Must not have been an array type.");
						return (struct parser_node *)0;
					}
				}default:{
					assert(0);
					return (struct parser_node *)0;
				}
			}
		}default:{
			assert(0);
			return (struct parser_node *)0;
		}
	}
}

struct parser_node * convert_abstract_declarator_to_function_type(struct memory_pool_collection * m, struct parser_node * n){
	if(n == 0){
		return (struct parser_node *)0;
	}

	switch(n->type){
		case DIRECT_ABSTRACT_DECLARATOR:{
			n->first_child = convert_abstract_declarator_to_function_type(m, n->first_child);
			n->next = (struct parser_node *)0;
			if(n->first_child){
				return n;
			}else{
				/*  This node does not need to exist if it does not have a child */
				struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n);
				return (struct parser_node *)0;
			}
		}case ABSTRACT_DECLARATOR:{
			assert(!(n->next) && "This function only handles the top-most case of abstract_declarator directly.");
			if(n->first_child->next){
				n->first_child->next = convert_abstract_declarator_to_function_type(m, n->first_child->next);
				return n;
			}
			n->first_child = convert_abstract_declarator_to_function_type(m, n->first_child);
			if(n->first_child){
				return n;
			}else{
				struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n);
				return (struct parser_node *)0;
			}
		}case POINTER:{
			n->next = convert_abstract_declarator_to_function_type(m, n->next);
			return n;
		}case EPSILON:{
			return n;
		}case TERMINAL:{
			switch(n->c_lexer_token->type){
				case OPEN_PAREN_CHAR:{
					if(n->next->type == ABSTRACT_DECLARATOR){
						n->next->first_child = convert_abstract_declarator_to_function_type(m, n->next->first_child);
						if(n->next->first_child){
							/*  abstract_declarator still exists */
							return n;
						}else{
							/*  In this case, we're going to delete the ( abs ) nodes 
							  Determine if the _rest is an epsilon or not */
							if(n->next->next->next->first_child->type == EPSILON){
								/* There is nothing left */
								destroy_parser_node_tree_and_c_lexer_tokens(m, n->next->next->next);
								struct_c_lexer_token_memory_pool_free(m->struct_c_lexer_token_pool, n->next->next->c_lexer_token);
								struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n->next->next);
								struct_c_lexer_token_memory_pool_free(m->struct_c_lexer_token_pool, n->next->c_lexer_token);
								struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n->next);
								struct_c_lexer_token_memory_pool_free(m->struct_c_lexer_token_pool, n->c_lexer_token);
								struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n);
								return (struct parser_node *)0;
							}else{
								/*  The first_child of the _rest becomes the first_child of the parent */
								struct parser_node * rtn = n->next->next->next->first_child;
								struct_c_lexer_token_memory_pool_free(m->struct_c_lexer_token_pool, n->next->next->next->c_lexer_token);
								struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n->next->next->next);
								struct_c_lexer_token_memory_pool_free(m->struct_c_lexer_token_pool, n->next->next->c_lexer_token);
								struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n->next->next);
								struct_c_lexer_token_memory_pool_free(m->struct_c_lexer_token_pool, n->next->c_lexer_token);
								struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n->next);
								struct_c_lexer_token_memory_pool_free(m->struct_c_lexer_token_pool, n->c_lexer_token);
								struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n);
								return rtn;
							}
						}
					}else if(n->next->type == TERMINAL && n->next->c_lexer_token->type == CLOSE_PAREN_CHAR){
						if(n->next->next->first_child->type == EPSILON){
							/*  This also destroys the _rest and epsilon */
							destroy_parser_node_tree_and_c_lexer_tokens(m, n->next);
							struct_c_lexer_token_memory_pool_free(m->struct_c_lexer_token_pool, n->c_lexer_token);
							struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n);
							return (struct parser_node *)0;
						}else{
							struct parser_node * rtn = n->next->next->first_child;
							struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n->next->next);
							struct_c_lexer_token_memory_pool_free(m->struct_c_lexer_token_pool, n->next->c_lexer_token);
							struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n->next);
							struct_c_lexer_token_memory_pool_free(m->struct_c_lexer_token_pool, n->c_lexer_token);
							struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n);
							return rtn;
						}
					}else if(n->next->type == PARAMETER_TYPE_LIST){
						if(n->next->next->next->first_child->type == EPSILON){
							/*  This also destroys the _rest and epsilon */
							destroy_parser_node_tree_and_c_lexer_tokens(m, n->next);
							struct_c_lexer_token_memory_pool_free(m->struct_c_lexer_token_pool, n->c_lexer_token);
							struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n);
							return (struct parser_node *)0;
						}else{
							struct parser_node * rtn = n->next->next->next->first_child;
							struct_c_lexer_token_memory_pool_free(m->struct_c_lexer_token_pool, n->next->next->next->c_lexer_token);
							struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n->next->next->next);
							struct_c_lexer_token_memory_pool_free(m->struct_c_lexer_token_pool, n->next->next->c_lexer_token);
							struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n->next->next);
							struct_c_lexer_token_memory_pool_free(m->struct_c_lexer_token_pool, n->next->c_lexer_token);
							struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n->next);
							struct_c_lexer_token_memory_pool_free(m->struct_c_lexer_token_pool, n->c_lexer_token);
							struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n);
							return rtn;
						}
					}else{
						assert(0 && "Must not have been a function type.");
						return (struct parser_node *)0;
					}
				}default:{
					assert(0);
					return (struct parser_node *)0;
				}
			}
		}default:{
			assert(0);
			return (struct parser_node *)0;
		}
	}
}


struct parser_node * convert_abstract_declarator_to_address_type_h(struct memory_pool_collection * m, struct parser_node * n){
	/*
	The purpose of this function is to take an abstract declarator as input and change it
	to be the address type version of itself (like with the & operator)
	Input1:  int (*(*(void))(void))(int i);
	Output1: int (*(*(*)(void))(void))(int i);
	Input2:  int
	Output2: int *
	*/
	switch (n->type){
		case POINTER:{
			if(n->next){
				n->next = convert_abstract_declarator_to_address_type_h(m, n->next);
				return n;
			}else{
				return add_level_to_pointer(m, n);
			}
		}case ABSTRACT_DECLARATOR:{
			/*  The current node should not change */
			n->first_child = convert_abstract_declarator_to_address_type_h(m, n->first_child);
			return n;
		}case DIRECT_ABSTRACT_DECLARATOR:{
			if(n->first_child->next->type == ABSTRACT_DECLARATOR){
				/*  Don't need an assignment, abstract declarator node should not change */
				convert_abstract_declarator_to_address_type_h(m, n->first_child->next);
				return n;
			}else{
				struct parser_node * pointer = create_pointer_node(m);
				struct parser_node * abstract_declarator = struct_parser_node_memory_pool_malloc(m->struct_parser_node_pool);
				struct parser_node * current_first_child = n->first_child;
				struct parser_node * open_paren = struct_parser_node_memory_pool_malloc(m->struct_parser_node_pool);
				struct parser_node * close_paren = struct_parser_node_memory_pool_malloc(m->struct_parser_node_pool);
				struct parser_node * direct_abstract_declarator_rest = struct_parser_node_memory_pool_malloc(m->struct_parser_node_pool);
				struct c_lexer_token * open_paren_token = struct_c_lexer_token_memory_pool_malloc(m->struct_c_lexer_token_pool);
				struct c_lexer_token * close_paren_token = struct_c_lexer_token_memory_pool_malloc(m->struct_c_lexer_token_pool);
				n->first_child = open_paren;
				/* Set up the '(' token */
				open_paren_token->type = OPEN_PAREN_CHAR;
				open_paren_token->first_byte = (unsigned char *)g_open_paren_string;
				open_paren_token->last_byte = (unsigned char *)(g_open_paren_string + (strlen(g_open_paren_string) -1));
				/* Set up the TERMINAL for '(' token */
				open_paren->c_lexer_token = open_paren_token;
				open_paren->next = abstract_declarator;
				open_paren->first_child = (struct parser_node *)0;
				open_paren->type = TERMINAL;
				/* Set up the abstract declarator */
				abstract_declarator->c_lexer_token = (struct c_lexer_token *)0;
				abstract_declarator->next = close_paren;
				abstract_declarator->first_child = pointer;
				abstract_declarator->type = ABSTRACT_DECLARATOR;
				/* Set up the ')' token */
				close_paren_token->type = CLOSE_PAREN_CHAR;
				close_paren_token->first_byte = (unsigned char *)g_close_paren_string;
				close_paren_token->last_byte = (unsigned char *)(g_close_paren_string + (strlen(g_close_paren_string) -1));
				/* Set up the TERMINAL for ')' token */
				close_paren->c_lexer_token = close_paren_token;
				close_paren->next = direct_abstract_declarator_rest;
				close_paren->first_child = (struct parser_node *)0;
				close_paren->type = TERMINAL;
				/* Move what was here into the new direct_abstract_declarator_rest */
				direct_abstract_declarator_rest->c_lexer_token = (struct c_lexer_token *)0;
				direct_abstract_declarator_rest->next = (struct parser_node *)0;
				direct_abstract_declarator_rest->first_child = current_first_child;
				direct_abstract_declarator_rest->type = DIRECT_ABSTRACT_DECLARATOR_REST;
				return n;
			}
		}default:{
			assert(0 && "Unknown conversion case for address type.");
			return (struct parser_node *)0;
		}
	}
}

struct parser_node * convert_abstract_declarator_to_address_type(struct memory_pool_collection * m, struct parser_node * n){
	assert(!n || n->type == ABSTRACT_DECLARATOR);
	if(!n){
		struct parser_node * abstract_declarator = struct_parser_node_memory_pool_malloc(m->struct_parser_node_pool);
		abstract_declarator->first_child = create_pointer_node(m);
		abstract_declarator->c_lexer_token = (struct c_lexer_token *)0;
		abstract_declarator->next = (struct parser_node *)0;
		abstract_declarator->type = ABSTRACT_DECLARATOR;
		return abstract_declarator;
	}
	n->first_child = convert_abstract_declarator_to_address_type_h(m, n->first_child);
	return n;
}

struct parser_node * create_pointer_node(struct memory_pool_collection * m){
	struct c_lexer_token * ptr_token = struct_c_lexer_token_memory_pool_malloc(m->struct_c_lexer_token_pool);
	struct parser_node * terminal = struct_parser_node_memory_pool_malloc(m->struct_parser_node_pool);
	struct parser_node * pointer = struct_parser_node_memory_pool_malloc(m->struct_parser_node_pool);
	ptr_token->type = MULTIPLY_CHAR;
	ptr_token->first_byte = (unsigned char *)g_ptr_string;
	ptr_token->last_byte = (unsigned char *)(g_ptr_string + (strlen(g_ptr_string) -1));
	terminal->c_lexer_token = ptr_token;
	terminal->first_child = (struct parser_node *)0;
	terminal->next = (struct parser_node *)0;
	terminal->type = TERMINAL;
	pointer->first_child = terminal;
	pointer->c_lexer_token = (struct c_lexer_token *)0;
	pointer->next = (struct parser_node *)0;
	pointer->type = POINTER;
	return pointer;
}

struct parser_node * add_level_to_pointer(struct memory_pool_collection * m, struct parser_node * n){
	/* Takes an existing pointer and adds a pointer onto it */
	struct parser_node * p = create_pointer_node(m);
	p->next = n;
	return p;
}

unsigned int is_pointer(struct memory_pool_collection * m, struct parser_node * n){
	if(!n){
		return 0;
	}
	if(n->type == ABSTRACT_DECLARATOR){
		return is_pointer(m, n->first_child);
	}else if(n->type == POINTER){
		if(n->next){
			return is_pointer(m, n->next);
		}else{
			return 1;
		}
	}else if(n->type == DIRECT_ABSTRACT_DECLARATOR){
		if(n->first_child->next->type == ABSTRACT_DECLARATOR){
			return is_pointer(m, n->first_child->next);
		}else{
			return 0;
		}
	}else{
		assert(0);
		return 0;
	}
}

unsigned int is_function(struct memory_pool_collection * m, struct parser_node * n){
	if(!n){
		return 0;
	}
	if(n->type == ABSTRACT_DECLARATOR){
		return is_function(m, n->first_child);
	}else if(n->type == POINTER){
		if(n->next){
			return is_function(m, n->next);
		}else{
			return 0;
		}
	}else if(n->type == DIRECT_ABSTRACT_DECLARATOR){
		if(n->first_child->type == TERMINAL && n->first_child->c_lexer_token->type == OPEN_PAREN_CHAR){
			if(n->first_child->next->type == ABSTRACT_DECLARATOR){
				return is_function(m, n->first_child->next);
			}else if(n->first_child->next->type == TERMINAL){
				return 1;
			}else if(n->first_child->next->type == PARAMETER_TYPE_LIST){
				return 1;
			}else{
				return 0;
			}
		}else{
			return 0;
		}
	}else{
		assert(0);
		return 0;
	}
}


struct parser_node * get_nth_parameter_declaration_from_parameter_type_list(struct parser_node * n, unsigned int nth){
	/*  Will return the nth (0-based) parameter_declaration from the parameter_type_list */
	switch(n->type){
		case PARAMETER_TYPE_LIST:{
			return get_nth_parameter_declaration_from_parameter_type_list(n->first_child, nth);
		}case PARAMETER_LIST:{
			if(nth == 0){
				return n->first_child;
			}else{
				return get_nth_parameter_declaration_from_parameter_type_list(n->first_child->next, nth - 1);
			}
		}case PARAMETER_LIST_REST:{
			if(nth == 0){
				if(n->first_child->type == EPSILON){
					return (struct parser_node *)0;
				}else{
					return n->first_child->next;
				}
			}else{
				if(n->first_child->type == EPSILON){
					return (struct parser_node *)0;
				}else{
					return get_nth_parameter_declaration_from_parameter_type_list(n->first_child->next->next, nth - 1);
				}
			}
		}default:{
			assert(0);
			return (struct parser_node *)0;
		}
	}
}

struct parser_node * get_parameter_type_list_from_abstract_declarator(struct parser_node * n){
	if(!n){
		return (struct parser_node *)0;
	}
	if(n->type == DECLARATOR || n->type == ABSTRACT_DECLARATOR){
		return get_parameter_type_list_from_abstract_declarator(n->first_child);
	}else if(n->type == POINTER){
		if(n->next){
			return get_parameter_type_list_from_abstract_declarator(n->next);
		}else{
			return (struct parser_node *)0;
		}
	}else if(n->type == DIRECT_DECLARATOR || n->type == DIRECT_ABSTRACT_DECLARATOR){
		if(n->first_child->type == TERMINAL && n->first_child->c_lexer_token->type == OPEN_PAREN_CHAR){
			if(n->first_child->next->type == DECLARATOR || n->first_child->next->type == ABSTRACT_DECLARATOR){
				return get_parameter_type_list_from_abstract_declarator(n->first_child->next);
			}else if(n->first_child->next->type == TERMINAL){
				return (struct parser_node *)0;
			}else if(n->first_child->next->type == PARAMETER_TYPE_LIST){
				return n->first_child->next;
			}else{
				return (struct parser_node *)0;
			}
		}else if(n->first_child->type == TERMINAL && n->first_child->c_lexer_token->type == IDENTIFIER){
			return get_parameter_type_list_from_abstract_declarator(n->first_child->next);
		}else{
			return (struct parser_node *)0;
		}
	}else{
		assert(0);
		return (struct parser_node *)0;
	}
}

struct parser_node * get_constant_expression_from_abstract_declarator(struct parser_node * n){
	if(!n){
		return (struct parser_node *)0;
	}
	if(n->type == ABSTRACT_DECLARATOR){
		return get_constant_expression_from_abstract_declarator(n->first_child);
	}else if(n->type == POINTER){
		if(n->next){
			return get_constant_expression_from_abstract_declarator(n->next);
		}else{
			return (struct parser_node *)0;
		}
	}else if(n->type == DIRECT_ABSTRACT_DECLARATOR){
		if(n->first_child->type == TERMINAL && n->first_child->next->type == ABSTRACT_DECLARATOR){
			return get_constant_expression_from_abstract_declarator(n->first_child->next);
		}else if(n->first_child->type == TERMINAL && n->first_child->c_lexer_token->type == OPEN_SQUARE_BRACKET_CHAR){
			if(n->first_child->next->type == TERMINAL){
				return (struct parser_node *)0;
			}else if(n->first_child->next->type == CONSTANT_EXPRESSION){
				return n->first_child->next;
			}else{
				return (struct parser_node *)0;
			}
		}else{
			return (struct parser_node *)0;
		}
	}else{
		assert(0);
		return (struct parser_node *)0;
	}
}

unsigned int is_parameter_type_list_variadic(struct parser_node * n){
	assert(n->type == PARAMETER_TYPE_LIST);
	return n->first_child->next && n->first_child->next->next && n->first_child->next->next->type == TERMINAL && n->first_child->next->next->c_lexer_token->type == ELLIPSIS;
}

unsigned int is_function_variadic(struct parser_node * n){
	if(!n){
		return 0;
	}
	if(n->type == ABSTRACT_DECLARATOR){
		return is_function_variadic(n->first_child);
	}else if(n->type == POINTER){
		if(n->next){
			return is_function_variadic(n->next);
		}else{
			return 0;
		}
	}else if(n->type == DIRECT_ABSTRACT_DECLARATOR){
		if(n->first_child->type == TERMINAL && n->first_child->c_lexer_token->type == OPEN_PAREN_CHAR){
			if(n->first_child->next->type == ABSTRACT_DECLARATOR){
				return is_function_variadic(n->first_child->next);
			}else if(n->first_child->next->type == TERMINAL){
				return 0;
			}else if(n->first_child->next->type == PARAMETER_TYPE_LIST){
				return is_parameter_type_list_variadic(n->first_child->next);
			}else{
				return 0;
			}
		}else{
			return 0;
		}
	}else{
		assert(0);
		return 0;
	}
}

unsigned int is_function_k_and_r_c_style(struct parser_node * n){
	if(!n){
		return 0;
	}
	if(n->type == ABSTRACT_DECLARATOR){
		return is_function_k_and_r_c_style(n->first_child);
	}else if(n->type == POINTER){
		if(n->next){
			return is_function_k_and_r_c_style(n->next);
		}else{
			return 0;
		}
	}else if(n->type == DIRECT_ABSTRACT_DECLARATOR){
		if(n->first_child->type == TERMINAL && n->first_child->c_lexer_token->type == OPEN_PAREN_CHAR){
			if(n->first_child->next->type == ABSTRACT_DECLARATOR){
				return is_function_k_and_r_c_style(n->first_child->next);
			}else if(n->first_child->next->type == TERMINAL){
				return 1;
			}else{
				return 0;
			}
		}else{
			return 0;
		}
	}else{
		assert(0);
		return 0;
	}
}

unsigned int is_array(struct memory_pool_collection * m, struct parser_node * n){
	if(!n){
		return 0;
	}
	if(n->type == ABSTRACT_DECLARATOR){
		return is_array(m, n->first_child);
	}else if(n->type == POINTER){
		if(n->next){
			return is_array(m, n->next);
		}else{
			return 0;
		}
	}else if(n->type == DIRECT_ABSTRACT_DECLARATOR){
		if(n->first_child->type == TERMINAL && n->first_child->c_lexer_token->type == OPEN_PAREN_CHAR){
			if(n->first_child->next->type == ABSTRACT_DECLARATOR){
				return is_array(m, n->first_child->next);
			}else{
				return 0;
			}
		}else if(n->first_child->type == TERMINAL && n->first_child->c_lexer_token->type == OPEN_SQUARE_BRACKET_CHAR){
			if(n->first_child->next->type == TERMINAL){
				return 1;
			}else if(n->first_child->next->type == CONSTANT_EXPRESSION){
				return 1;
			}else{
				return 0;
			}
		}else{
			return 0;
		}
	}else{
		assert(0);
		return 0;
	}
}

void add_normalized_specifier_to_list(struct memory_pool_collection * m, struct parser_node * n, struct struct_normalized_specifier_ptr_list * list){
	struct normalized_specifier * new_p;
	struct parser_node * first;
	struct parser_node * second;
	if(!n){
		return;
	}

	first = n->first_child;
	second = first ? first->next : (struct parser_node *)0;

	assert(
		n->type == DECLARATION_SPECIFIERS ||
		n->type == SPECIFIER_QUALIFIER_LIST ||
		n->type == TYPE_QUALIFIER_LIST ||
		n->type == TYPE_QUALIFIER_LIST_REST
	);

	new_p = (struct normalized_specifier *)malloc(sizeof(struct normalized_specifier));
	new_p->specifier = copy_parser_node_children_only(m, first);

	switch(first->type){
		case STORAGE_CLASS_SPECIFIER:{
			new_p->type = NORMALIZED_STORAGE_CLASS_SPECIFIER;
			break;
		}case TYPE_SPECIFIER:{
			new_p->type = NORMALIZED_TYPE_SPECIFIER;
			break;
		}case TYPE_QUALIFIER:{
			new_p->type = NORMALIZED_TYPE_QUALIFIER;
			break;
		}default:{
			assert(0 && "Unknown normalized specifier type.");
		}
	}

	struct_normalized_specifier_ptr_list_add_end(list, new_p);

	if( /*  Use recursion to add the rest of the specifiers to the list, if there are more */
		second && (
			second->type == DECLARATION_SPECIFIERS ||
			second->type == SPECIFIER_QUALIFIER_LIST || (
				second->type == TYPE_QUALIFIER_LIST_REST &&
				second->first_child->type != EPSILON
			)
		)
	){
		add_normalized_specifier_to_list(m, second, list);
	}
}



struct struct_normalized_specifier_ptr_list * get_normalized_specifier_list(struct memory_pool_collection * m, struct parser_node * n){
	/*
	This function exists to abstract away the differences in iterating over declaration_specifiers,
	specifier_qualifier_lists, and type_qualifier_lists

	declaration_specifiers
		: storage_class_specifier
		| storage_class_specifier declaration_specifiers
		| type_specifier
		| type_specifier declaration_specifiers
		| type_qualifier
		| type_qualifier declaration_specifiers
		;
	specifier_qualifier_list
		: type_specifier specifier_qualifier_list
		| type_specifier
		| type_qualifier specifier_qualifier_list
		| type_qualifier
		;
	type_qualifier_list
		: type_qualifier type_qualifier_list_rest
		;

	type_qualifier_list_rest
		| type_qualifier type_qualifier_list_rest
		| epsilon
		;
	These all get flattened out to a list of [ storage_class_specifier | type_specifier | type_qualifier ]
	*/
	struct struct_normalized_specifier_ptr_list * rtn = (struct struct_normalized_specifier_ptr_list *)malloc(sizeof(struct struct_normalized_specifier_ptr_list));
	struct_normalized_specifier_ptr_list_create(rtn);
	add_normalized_specifier_to_list(m, n, rtn);
	return rtn;
}

void add_normalized_declarator_to_list(struct memory_pool_collection * m, struct parser_node * n, struct struct_normalized_declarator_ptr_list * list){
	struct normalized_declarator * new_p;

	if(!n || n->first_child->type == EPSILON){
		return;
	}

	new_p = (struct normalized_declarator *)malloc(sizeof(struct normalized_declarator));

	switch(n->type){
		case ENUMERATOR_LIST:{
			new_p->type = NORMALIZED_ENUMERATOR;
			new_p->declarator = copy_parser_node_children_only(m, n->first_child);
			struct_normalized_declarator_ptr_list_add_end(list, new_p);
			add_normalized_declarator_to_list(m, n->first_child->next, list);
			break;
		}case ENUMERATOR_LIST_REST:{
			new_p->type = NORMALIZED_ENUMERATOR;
			new_p->declarator = copy_parser_node_children_only(m, n->first_child->next);
			struct_normalized_declarator_ptr_list_add_end(list, new_p);
			add_normalized_declarator_to_list(m, n->first_child->next->next, list);
			break;
		}case INIT_DECLARATOR_LIST:{
			new_p->type = NORMALIZED_INIT_DECLARATOR;
			new_p->declarator = copy_parser_node_children_only(m, n->first_child);
			struct_normalized_declarator_ptr_list_add_end(list, new_p);
			add_normalized_declarator_to_list(m, n->first_child->next, list);
			break;
		}case INIT_DECLARATOR_LIST_REST:{
			new_p->type = NORMALIZED_INIT_DECLARATOR;
			new_p->declarator = copy_parser_node_children_only(m, n->first_child->next);
			struct_normalized_declarator_ptr_list_add_end(list, new_p);
			add_normalized_declarator_to_list(m, n->first_child->next->next, list);
			break;
		}case STRUCT_DECLARATOR_LIST:{
			new_p->type = NORMALIZED_STRUCT_DECLARATOR;
			new_p->declarator = copy_parser_node_children_only(m, n->first_child);
			struct_normalized_declarator_ptr_list_add_end(list, new_p);
			add_normalized_declarator_to_list(m, n->first_child->next, list);
			break;
		}case STRUCT_DECLARATOR_LIST_REST:{
			new_p->type = NORMALIZED_STRUCT_DECLARATOR;
			new_p->declarator = copy_parser_node_children_only(m, n->first_child->next);
			struct_normalized_declarator_ptr_list_add_end(list, new_p);
			add_normalized_declarator_to_list(m, n->first_child->next->next, list);
			break;
		}case PARAMETER_DECLARATION:{
			if(n->first_child->next){
				switch(n->first_child->next->type){
					case DECLARATOR:{
						new_p->type = NORMALIZED_DECLARATOR;
						break;
					}case ABSTRACT_DECLARATOR:{
						new_p->type = NORMALIZED_ABSTRACT_DECLARATOR;
						break;
					}default:{
						assert(0 && "Unknown declarator.");
					}
				}
				new_p->declarator = copy_parser_node_children_only(m, n->first_child->next);
				assert(new_p->declarator->type == DECLARATOR || new_p->declarator->type == ABSTRACT_DECLARATOR);
				struct_normalized_declarator_ptr_list_add_end(list, new_p);
			}else{
				free(new_p); /*  No declarator */
			}
			break;
		}case FUNCTION_DEFINITION:{
			new_p->type = NORMALIZED_DECLARATOR;
			if(n->first_child->type == DECLARATOR){
				new_p->declarator = copy_parser_node_children_only(m, n->first_child);
			}else if(n->first_child->next->type == DECLARATOR){
				new_p->declarator = copy_parser_node_children_only(m, n->first_child->next);
			}else{
				assert(0 && "Unknown function definition.");
			}
			assert(new_p->declarator->type == DECLARATOR);
			struct_normalized_declarator_ptr_list_add_end(list, new_p);
			break;
		}case DECLARATION:{
			free(new_p); /*  The declarator (if there is one) is deeper in the recursion */
			if(n->first_child->next && n->first_child->next->type == INIT_DECLARATOR_LIST){
				add_normalized_declarator_to_list(m, n->first_child->next, list);
			}
			break;
		}case STRUCT_DECLARATION:{
			free(new_p); /*  The declarator (if there is one) is deeper in the recursion */
			if(n->first_child->next && n->first_child->next->type == STRUCT_DECLARATOR_LIST){
				add_normalized_declarator_to_list(m, n->first_child->next, list);
			}
			break;
		}default:{
			assert(0 && "Unknown normalized declarator type.");
		}
	}
}

struct struct_normalized_declarator_ptr_list * get_normalized_declarator_list(struct memory_pool_collection * m, struct parser_node * n){
	/*
	This function exists to abstract away the differences in iterating over init_declarator_lists,
	struct_declaration_lists

	declaration
		: declaration_specifiers ';'
		| declaration_specifiers init_declarator_list ';'
		;
	init_declarator_list
		: init_declarator init_declarator_list_rest
		;
	init_declarator_list_rest
		: ',' init_declarator init_declarator_list_rest
		| epsilon
		;
	struct_declarator_list
		: struct_declarator struct_declarator_list_rest
		;
	struct_declarator_list_rest
		| ',' struct_declarator struct_declarator_list_rest
		| epsilon
		;
	parameter_declaration
		: declaration_specifiers declarator
		| declaration_specifiers abstract_declarator
		| declaration_specifiers
		;
	function_definition
		: declaration_specifiers declarator declaration_list compound_statement
		| declaration_specifiers declarator compound_statement
		| declarator declaration_list compound_statement
		| declarator compound_statement
		;

	These all get flattened out to a list of [ declarator | abstract_declarator | struct_declarator | ... ]
	*/
	struct struct_normalized_declarator_ptr_list * rtn = (struct struct_normalized_declarator_ptr_list *)malloc(sizeof(struct struct_normalized_declarator_ptr_list));
	struct_normalized_declarator_ptr_list_create(rtn);
	add_normalized_declarator_to_list(m, n, rtn);
	return rtn;
}


struct parser_node * add_outer_direct_abstract_declarator_to_inner_as_rest(struct memory_pool_collection * m, struct parser_node * outer_direct_abstract_declarator, struct parser_node * inner_direct_abstract_declarator){
	/*  Must be called initially with outer_direct_abstract_declarator and inner_direct_abstract_declarator as DIRECT_ABSTRACT_DECLARATORS,
        inner_direct_abstract_declarator can be _rest on recursion */
	if(inner_direct_abstract_declarator->first_child->type == EPSILON){
		struct_parser_node_memory_pool_free(m->struct_parser_node_pool, inner_direct_abstract_declarator->first_child);
		struct_parser_node_memory_pool_free(m->struct_parser_node_pool, inner_direct_abstract_declarator);
		outer_direct_abstract_declarator->type = DIRECT_ABSTRACT_DECLARATOR_REST;
		return outer_direct_abstract_declarator;
	}else{
		struct parser_node * p = inner_direct_abstract_declarator->first_child;
		while(p->next->type != DIRECT_ABSTRACT_DECLARATOR_REST){
			p = p->next;
		}
		p->next = add_outer_direct_abstract_declarator_to_inner_as_rest(m, outer_direct_abstract_declarator, p->next);
		return inner_direct_abstract_declarator;
	}
}

void add_pointer_to_end(struct memory_pool_collection * m , struct parser_node * outer, struct parser_node * inner){
	struct parser_node * n = outer->first_child;
	struct parser_node * prev = n;
	while(n->next){
		/*  Move to end */
		prev = n;
		n = n->next;
	}
	if(n->type == POINTER){
		add_pointer_to_end(m, n, inner);
	}else{
		prev->next = inner;
	}
}

struct parser_node * insert_abstract_declarator(struct memory_pool_collection * m, struct parser_node * outer, struct parser_node * inner){
	/*  Returns the outer abstract declarator, or the outer abstract declarator with a copy of the inner abstract declarator inserted */
	if(!inner){
		/*  Nothing to do. */
		return outer;
	}else if(!outer){
		/*  If there is a declarator it is the inner one. */
		return copy_parser_node_children_only(m, inner);
	}else{
		struct parser_node * outer_direct_abstract_declarator = (outer->first_child->type == DIRECT_ABSTRACT_DECLARATOR) ? outer->first_child : outer->first_child->next;
		struct parser_node * inner_direct_abstract_declarator = (inner->first_child->type == DIRECT_ABSTRACT_DECLARATOR) ? inner->first_child : inner->first_child->next;
		struct parser_node * outer_pointer = outer->first_child->type == POINTER ? outer->first_child : (struct parser_node *)0;
		struct parser_node * inner_pointer = inner->first_child->type == POINTER ? inner->first_child : (struct parser_node *)0;

		struct parser_node * new_direct_abstract_declarator = (struct parser_node *)0;
		struct parser_node * new_pointer = (struct parser_node *)0;
		struct parser_node * unmerged_inner_pointer = (struct parser_node *)0;
		if(outer_direct_abstract_declarator){
			if(outer_direct_abstract_declarator->first_child->next->type == ABSTRACT_DECLARATOR){ /*  If we're at a '(' abstract_declarator ')' */
				struct parser_node * close_paren = outer_direct_abstract_declarator->first_child->next->next;
				struct parser_node * new_abstract_declarator = insert_abstract_declarator(m, outer_direct_abstract_declarator->first_child->next, inner);
				new_abstract_declarator->next = close_paren;
				outer_direct_abstract_declarator->first_child->next = new_abstract_declarator;
				return outer;
			}else{ /*  The identifier would have been next, insert the inner declarator */
				/*  The outer direct_abstract_declarator will go into the _rest most of the inner one we're inserting */
				if(inner_direct_abstract_declarator){
					new_direct_abstract_declarator = add_outer_direct_abstract_declarator_to_inner_as_rest(m, outer_direct_abstract_declarator, copy_parser_node_children_only(m, inner_direct_abstract_declarator));
				}else{
					new_direct_abstract_declarator = outer_direct_abstract_declarator;
				}
			}
		}else if(inner_direct_abstract_declarator){
			new_direct_abstract_declarator = copy_parser_node_children_only(m, inner_direct_abstract_declarator);
		}

		if(outer_pointer && !inner_pointer){
			new_pointer = outer_pointer;
		}else if(!outer_pointer && inner_pointer){
			new_pointer = copy_parser_node_children_only(m, inner_pointer);
		}else if(outer_pointer && inner_pointer && (new_direct_abstract_declarator && new_direct_abstract_declarator->first_child->next->type != ABSTRACT_DECLARATOR)){
			new_pointer = outer_pointer;
			/* In this case we need to put ( ) around the pointer, but only the inner one */
			unmerged_inner_pointer = copy_parser_node_children_only(m, inner_pointer);
			/*  The new pointer will have no next when surrounded by ( ) */
		}else if(outer_pointer && inner_pointer){
			new_pointer = outer_pointer;
			add_pointer_to_end(m, outer_pointer, copy_parser_node_children_only(m, inner_pointer));
		}else{ /* !outer_pointer && !inner_pointer */
			/* Nothing to do */
		}

		/*  Check if we need to add ( ) around any pointers */
		if(inner_pointer && (new_direct_abstract_declarator && new_direct_abstract_declarator->first_child->next->type != ABSTRACT_DECLARATOR)){
			/*  create a new direct abstract declarator, put the pointer in it, then push the new_direct_abstract_declarator into a _rest  */
			struct parser_node * artificial_direct_abstract_declarator = struct_parser_node_memory_pool_malloc(m->struct_parser_node_pool);
			struct parser_node * inserted_open_paren = struct_parser_node_memory_pool_malloc(m->struct_parser_node_pool);
			struct parser_node * inserted_close_paren = struct_parser_node_memory_pool_malloc(m->struct_parser_node_pool);
			struct c_lexer_token * open_paren_token = struct_c_lexer_token_memory_pool_malloc(m->struct_c_lexer_token_pool);
			struct c_lexer_token * close_paren_token = struct_c_lexer_token_memory_pool_malloc(m->struct_c_lexer_token_pool);
			struct parser_node * abstract_declarator_to_insert = struct_parser_node_memory_pool_malloc(m->struct_parser_node_pool);
			struct parser_node * surrounded_pointer = unmerged_inner_pointer ? unmerged_inner_pointer : new_pointer;

			if(!unmerged_inner_pointer){ /* We only put the new_pointer in the () if there is no unmerged inner pointer */
				new_pointer = (struct parser_node *)0; /*  Prevent this pointer from showing up before the direct abstract declarator we're creating */
			}

			artificial_direct_abstract_declarator->type = DIRECT_ABSTRACT_DECLARATOR;
			artificial_direct_abstract_declarator->next = (struct parser_node *)0;
			artificial_direct_abstract_declarator->first_child = inserted_open_paren;
			artificial_direct_abstract_declarator->c_lexer_token = (struct c_lexer_token *)0;
			
			open_paren_token->type = OPEN_PAREN_CHAR;
			open_paren_token->first_byte = (unsigned char *)g_open_paren_string;
			open_paren_token->last_byte = (unsigned char *)g_open_paren_string;

			inserted_open_paren->type = TERMINAL;
			inserted_open_paren->c_lexer_token = open_paren_token;
			inserted_open_paren->next = abstract_declarator_to_insert;
			inserted_open_paren->first_child = (struct parser_node *)0;

			abstract_declarator_to_insert->type = ABSTRACT_DECLARATOR;
			abstract_declarator_to_insert->next = inserted_close_paren;
			abstract_declarator_to_insert->first_child = surrounded_pointer;
			abstract_declarator_to_insert->c_lexer_token = (struct c_lexer_token *)0;
			/*  surrounded_pointer would have been the inner pointer that was copied.  Because we're putting '(' ')' around it, the direct_abstract_declarator is no longer next */
			surrounded_pointer->next = (struct parser_node *)0;

			close_paren_token->type = CLOSE_PAREN_CHAR;
			close_paren_token->first_byte = (unsigned char *)g_close_paren_string;
			close_paren_token->last_byte = (unsigned char *)g_close_paren_string;

			inserted_close_paren->type = TERMINAL;
			inserted_close_paren->c_lexer_token = close_paren_token;
			inserted_close_paren->next = new_direct_abstract_declarator;
			inserted_close_paren->first_child = (struct parser_node *)0;

			/*  The thing that was going to become the new direct abstract declarator will now become a direct_abstract_declarator_rest 
			    This is ok because we make this direct_abstract_declarator doesn't have an abstract_declarator in it  */
			new_direct_abstract_declarator->type = DIRECT_ABSTRACT_DECLARATOR_REST;
			new_direct_abstract_declarator = artificial_direct_abstract_declarator; /* instread it is '(' '*' ')'  */
		}

		if(new_pointer){
			outer->first_child = new_pointer;
			new_pointer->next = new_direct_abstract_declarator;
		}else{
			outer->first_child = new_direct_abstract_declarator;
		}

		return outer;
	}
}

void convert_to_untypedefed_type_description(struct memory_pool_collection * m, struct type_description_reference t){
	/*  Will resolve any typedefed types in the top level specifiers (but not in deeper places like function parameters.) */
	unsigned int num_specifiers = struct_normalized_specifier_ptr_list_size(t.t->specifiers);
	unsigned int i;
	/*  Find the first specifier that is a typedefed type.  */
	for(i = 0; i < num_specifiers; i++){
		struct normalized_specifier * normalized_specifier = struct_normalized_specifier_ptr_list_get(t.t->specifiers, i);
		if(normalized_specifier->type == NORMALIZED_TYPE_SPECIFIER && normalized_specifier->specifier->first_child->type == TERMINAL){
			struct parser_node * child = normalized_specifier->specifier->first_child;
			if(child->c_lexer_token->type == IDENTIFIER){
				unsigned char * ident = copy_string(child->c_lexer_token->first_byte, child->c_lexer_token->last_byte, m);
				struct namespace_object * obj = get_namespace_object_from_closest_namespace(ident, IDENTIFIER_NAMESPACE, t.t->source_scope_level, 0, m);
				struct normalized_declaration_element * element;
				struct type_description_reference typedefed_type;
				assert(t.t->source_scope_level);
				if(!obj || !struct_normalized_declaration_element_ptr_list_size(&obj->elements)){
					printf("Unknown typedefed type: %s\n", ident);
					assert(0 && "Unknown typedefed type.");
				}
				element = struct_normalized_declaration_element_ptr_list_get(&obj->elements, 0);
				typedefed_type = create_type_description_from_normalized_declaration_element(m, element, t.t->context, t.t->source_scope_level, t.t->value_type);
				/*  If there is a corresponding object in the identifier namespace that was declared using a typedef. */
				if(obj && count_specifiers(typedefed_type, TYPEDEF)){
					struct parser_node * abstract_typedefed_type;
					struct parser_node * abstract_t;
					unsigned int num_original_specifiers;
					unsigned int j;
					/*  Remove the identifier specifier from the type we're converting */
					remove_specifier(m, t, 0, IDENTIFIER);
					/*  Remove the typedef specifier from the type used in the typedef declaration */
					remove_specifier(m, typedefed_type, 0, TYPEDEF);
					/*  Number of specifiers in typedef declaration to copy */
					num_original_specifiers = struct_normalized_specifier_ptr_list_size(typedefed_type.t->specifiers);
					for(j = 0; j < num_original_specifiers; j++){
						struct normalized_specifier * src_normalized_specifier = struct_normalized_specifier_ptr_list_get(typedefed_type.t->specifiers, j);
						struct normalized_specifier * dst_normalized_specifier = (struct normalized_specifier *)malloc(sizeof(struct normalized_specifier));
						struct parser_node * specifiers_copy = copy_parser_node_children_only(m, src_normalized_specifier->specifier);
						dst_normalized_specifier->specifier = specifiers_copy;
						dst_normalized_specifier->type = src_normalized_specifier->type;
						struct_normalized_specifier_ptr_list_add_end(t.t->specifiers, dst_normalized_specifier);
					}

					/*  Force the typedef's declarator to be abstract so we can insert into it with target type's declarator */
					abstract_typedefed_type = create_abstract_declarator_from_normalized_declarator(m, typedefed_type.t->declarator);

					/*  Force the target's declarator to be abstract so we can insert it into the typedef's declarator */
					abstract_t = create_abstract_declarator_from_normalized_declarator(m, t.t->declarator);

					if(t.t->declarator->declarator){
						t.t->declarator->declarator->next = (struct parser_node *)0;
						destroy_parser_node_tree_and_c_lexer_tokens(m, t.t->declarator->declarator);
					}
					/*  Insert the target type abstract declarator into the typedefed type. */
					t.t->declarator->declarator = insert_abstract_declarator(m, abstract_typedefed_type, abstract_t);

					/*  For anonymous structures, needs the reference to the source element to point to the original anonymous struct declaration */
					t.t->source_element = typedefed_type.t->source_element;

					/*  Recurse, because the expanded typedef might contain additional typedefed types. */
					convert_to_untypedefed_type_description(m, t);
					destroy_parser_node_tree_and_c_lexer_tokens(m, abstract_t);
				}else{
					assert(0 && "Unable to resolve typedef.");
				}
				destroy_type_description(m, typedefed_type);
				heap_memory_pool_free(m->heap_pool, ident);
			}
		}/* else, no typedefs to resolve */
	}
}

struct namespace_object * get_namespace_object_from_closest_namespace(unsigned char * ident, enum scope_type scope_type, struct scope_level * start_scope, unsigned int require_definition, struct memory_pool_collection * m){
	struct scope_level * current_scope = start_scope;
	(void)m;
	while(current_scope){
		struct namespace_object * current_obj;
		struct unsigned_char_ptr_to_struct_namespace_object_ptr_map * scope_namespace;
		switch (scope_type){
			case IDENTIFIER_NAMESPACE:{
				scope_namespace = &current_scope->identifier_namespace;
				break;
			}case TAG_NAMESPACE:{
				scope_namespace = &current_scope->tag_namespace;
				break;
			}default:{
				assert(0 && "Not covered.");
				scope_namespace = (struct unsigned_char_ptr_to_struct_namespace_object_ptr_map *)0;
			}
		}
		current_obj = get_namespace_object_from_scope_namespace_using_string(scope_namespace, ident);

		/* Sometimes we need to make sure that we get definition, not just a declaration  */
		if(current_obj && (!require_definition || contains_struct_or_union_or_enum_definition(current_obj) || contains_function_definition(current_obj))){
			return current_obj;
		}
		current_scope = current_scope->parent_scope;
	}
	return (struct namespace_object *)0;
}


struct type_description_reference create_type_description_from_normalized_declaration_element(struct memory_pool_collection * m, struct normalized_declaration_element * element, struct parser_node * context, struct scope_level * source_scope_level, enum value_type value_type){
	if(element->normalized_declarator && element->normalized_declarator->type == NORMALIZED_ENUMERATOR){
		/*  For an enumerator, the individual enumerator identifier doesn't have specifiers on it, they're on the parent node */
		struct normalized_declaration_set * declaration_set = create_normalized_declaration_set_from_parser_node(m, element->normalized_declaration_set->parent_normalized_declaration_set->set, (struct normalized_declaration_set *)0);
		struct type_description_reference type = create_type_description_from_normalized_declarator_and_specifiers(m, element->normalized_declarator, declaration_set->normalized_specifiers, context, source_scope_level, value_type, element);
		destroy_normalized_declaration_element_list(m, create_normalized_declaration_element_list(declaration_set)); /* TODO:  Unnecessary, but cleans up memory easily */
		return type;
	}else{
		struct type_description_reference type = create_type_description_from_normalized_declarator_and_specifiers(m, element->normalized_declarator, element->normalized_specifiers, context, source_scope_level, value_type, element);
		return type;
	}
}

void destroy_type_description(struct memory_pool_collection * m, struct type_description_reference description){
	if(description.t->specifiers){
		unsigned int num_specifiers = struct_normalized_specifier_ptr_list_size(description.t->specifiers);
		unsigned int i;
		for(i = 0; i < num_specifiers; i++){
			struct normalized_specifier * normalized_specifier = struct_normalized_specifier_ptr_list_get(description.t->specifiers, i);
			destroy_parser_node_tree_and_c_lexer_tokens(m, normalized_specifier->specifier);
			free(normalized_specifier);
		}
		struct_normalized_specifier_ptr_list_destroy(description.t->specifiers);
		free(description.t->specifiers);
	}
	if(description.t->declarator){
		destroy_parser_node_tree_and_c_lexer_tokens(m, description.t->declarator->declarator);
		free(description.t->declarator);
	}
	struct_type_description_memory_pool_free(m->struct_type_description_pool, description.t);
}

unsigned int count_specifiers(struct type_description_reference description, enum c_token_type t){
	unsigned int rtn = 0;
	unsigned int k;
	for(k = 0; k < struct_normalized_specifier_ptr_list_size(description.t->specifiers); k++){
		struct normalized_specifier * normalized_specifier = struct_normalized_specifier_ptr_list_get(description.t->specifiers, k);
		if(normalized_specifier->specifier->first_child->type == TERMINAL && normalized_specifier->specifier->first_child->c_lexer_token->type == t){
			rtn++;
		}
	}
	return rtn;
}

void remove_specifier(struct memory_pool_collection * m, struct type_description_reference description, unsigned int n, enum c_token_type t){
	/*  Remove the nth (zero based) normalized specifier that has type t */
	unsigned int count = 0;
	unsigned int k;
	validate_specifier_token_type(t);
	for(k = 0; k < struct_normalized_specifier_ptr_list_size(description.t->specifiers); k++){
		struct normalized_specifier * normalized_specifier = struct_normalized_specifier_ptr_list_get(description.t->specifiers, k);
		if(normalized_specifier->specifier->first_child->type == TERMINAL && normalized_specifier->specifier->first_child->c_lexer_token->type == t){
			if(count == n){
				normalized_specifier->specifier->next = (struct parser_node *)0;
				destroy_parser_node_tree_and_c_lexer_tokens(m, normalized_specifier->specifier);
				free(normalized_specifier);
				struct_normalized_specifier_ptr_list_remove_all(description.t->specifiers, normalized_specifier, normalized_specifier_ptr_cmp);
				return;
			}
			count++;
		}
	}
	assert(0 && "Did not find specifier.");
}


struct parser_node * create_abstract_declarator_from_declarator(struct memory_pool_collection * m, struct parser_node * n){
	assert(n->type == DECLARATOR);
	return convert_declarator_to_abstract_declarator(m, copy_parser_node_children_only(m, n));
}

struct parser_node * create_abstract_declarator_from_normalized_declarator(struct memory_pool_collection * m, struct normalized_declarator * declarator){
	/*  Abstract declarators are just like regular declarators, but without an identifier.
	An example of this is what comes after the declaration specifiers when stating parameter types in a function prorotype:
	int foo(int, int (*)(int), double (*a)[3], int [3]);
	In this example the abstract declarators are:
	Arg1: [no abstract declarator]
	Arg2: (*)(int)
	Arg3: (*a)[3]
	Arg4: [3]
	
	*/
	if(!(declarator)){
		return (struct parser_node *)0;
	}

	switch(declarator->type){
		case NORMALIZED_DECLARATOR:{
			struct parser_node * rtn;
			assert(declarator->declarator->type == DECLARATOR);
			rtn = create_abstract_declarator_from_declarator(m, declarator->declarator);
			assert(!rtn || rtn->type == ABSTRACT_DECLARATOR);
			return rtn;
		}case NORMALIZED_ABSTRACT_DECLARATOR:{
			struct parser_node * copy;
			assert(!declarator || !declarator->declarator || declarator->declarator->type == ABSTRACT_DECLARATOR);
			copy = copy_parser_node_children_only(m, declarator->declarator);
			assert(!copy || copy->type == ABSTRACT_DECLARATOR);
			return copy;
		}case NORMALIZED_STRUCT_DECLARATOR:{
			if(declarator->declarator->first_child->type == DECLARATOR){
				struct parser_node * rtn;
				assert(declarator->declarator->first_child->type == DECLARATOR);
				rtn = create_abstract_declarator_from_declarator(m, declarator->declarator->first_child);
				assert(!rtn || rtn->type == ABSTRACT_DECLARATOR);
				return rtn;
			}else{
				return (struct parser_node *)0;
			}
		}case NORMALIZED_INIT_DECLARATOR:{
			struct parser_node * rtn;
			assert(declarator->declarator->first_child->type == DECLARATOR);
			rtn =  create_abstract_declarator_from_declarator(m, declarator->declarator->first_child);
			assert(!rtn || rtn->type == ABSTRACT_DECLARATOR);
			return rtn;
		}case NORMALIZED_ENUMERATOR:{
			return (struct parser_node *)0;
		}default:{
			assert(0);
		}
	}
	return (struct parser_node *)0;
}

struct namespace_object * get_namespace_object_from_scope_namespace_using_string(struct unsigned_char_ptr_to_struct_namespace_object_ptr_map * name, unsigned char * ident){
	if(unsigned_char_ptr_to_struct_namespace_object_ptr_map_exists(name, ident)){
		return unsigned_char_ptr_to_struct_namespace_object_ptr_map_get(name, ident);
	}else{
		return (struct namespace_object *)0;
	}
}

struct c_lexer_token * get_identifier_token_from_declarator(struct parser_node * n){
	/*  This function should be called externally with non-abstract declarators */
	if(n == 0){
		return (struct c_lexer_token *)0;
	}

	switch(n->type){
		case DIRECT_DECLARATOR:{
			return get_identifier_token_from_declarator(n->first_child);
		}case DECLARATOR:{
			return get_identifier_token_from_declarator(n->first_child);
		}case POINTER:{
			return get_identifier_token_from_declarator(n->next);
		}case DIRECT_DECLARATOR_REST:{
			return get_identifier_token_from_declarator(n->first_child);
		}case CONSTANT_EXPRESSION:{
			return get_identifier_token_from_declarator(n->next);
		}case PARAMETER_TYPE_LIST:{
			return get_identifier_token_from_declarator(n->next);
		}case IDENTIFIER_LIST:{
			return get_identifier_token_from_declarator(n->next);
		}case EPSILON:{
			return (struct c_lexer_token *)0;
		}case TERMINAL:{
			switch(n->c_lexer_token->type){
				case IDENTIFIER:{
					return n->c_lexer_token;
				}case OPEN_PAREN_CHAR:{
					if(n->next->type == DECLARATOR){
						/*  There are 2 possible locations for the identifier at this point */
						struct c_lexer_token * n1 = get_identifier_token_from_declarator(n->next);
						struct c_lexer_token * n2 = get_identifier_token_from_declarator(n->next->next);
						return n1 ? n1 : n2;
					}else{
						return get_identifier_token_from_declarator(n->next);
					}
				}case CLOSE_PAREN_CHAR:{
					return get_identifier_token_from_declarator(n->next);
				}case OPEN_SQUARE_BRACKET_CHAR:{
					return get_identifier_token_from_declarator(n->next);
				}case CLOSE_SQUARE_BRACKET_CHAR:{
					return get_identifier_token_from_declarator(n->next);
				}default:{
					assert(0);
					return (struct c_lexer_token *)0;
				}
			}
		}default:{
			assert(0);
			return (struct c_lexer_token *)0;
		}
	}
}


struct parser_node * convert_declarator_to_abstract_declarator(struct memory_pool_collection * m, struct parser_node * n){
	struct parser_node * rtn;
	if(!(!n || n->type == DECLARATOR)){
		assert(!n || n->type == DECLARATOR);
	}
	rtn = convert_declarator_to_abstract_declarator_h(m, n);
	assert(!rtn || rtn->type == ABSTRACT_DECLARATOR);
	return rtn;
}


struct parser_node * convert_declarator_to_abstract_declarator_h(struct memory_pool_collection * m, struct parser_node * n){
	if(n == 0){
		return (struct parser_node *)0;
	}
	switch(n->type){
		case ABSTRACT_DECLARATOR:{
			return n;
		}case DIRECT_DECLARATOR:{
			/* its abstract now */
			n->type = DIRECT_ABSTRACT_DECLARATOR;
			n->first_child = convert_declarator_to_abstract_declarator_h(m, n->first_child);
			n->next = convert_declarator_to_abstract_declarator_h(m, n->next);
			if(n->first_child){
				return n;
			}else{
				/*  This node does not need to exist if it does not have a child */
				struct parser_node * rtn = n->next;
				struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n);
				return rtn;
			}
		}case DECLARATOR:{
			n->type = ABSTRACT_DECLARATOR;
			n->first_child = convert_declarator_to_abstract_declarator_h(m, n->first_child);
			n->next = convert_declarator_to_abstract_declarator_h(m, n->next);
			if(n->first_child){
				return n;
			}else{
				/*  This node does not need to exist if it does not have a child */
				struct parser_node * rtn = n->next;
				struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n);
				return rtn;
			}
		}case POINTER:{
			n->next = convert_declarator_to_abstract_declarator_h(m, n->next);
			return n;
		}case DIRECT_DECLARATOR_REST:{
			n->type = DIRECT_ABSTRACT_DECLARATOR_REST;
			n->first_child = convert_declarator_to_abstract_declarator_h(m, n->first_child);
			return n;
		}case CONSTANT_EXPRESSION:{
			n->next = convert_declarator_to_abstract_declarator_h(m, n->next);
			return n;
		}case PARAMETER_TYPE_LIST:{
			n->next = convert_declarator_to_abstract_declarator_h(m, n->next);
			return n;
		}case IDENTIFIER_LIST:{
			/* identifier lists don't exist in abstract declarators */
			struct parser_node * next = n->next;
			n->next = (struct parser_node *)0;
			destroy_parser_node_tree_and_c_lexer_tokens(m, n);
			return convert_declarator_to_abstract_declarator_h(m, next);
		}case EPSILON:{
			return n;
		}case TERMINAL:{
			switch(n->c_lexer_token->type){
				case IDENTIFIER:{
					if(n->next->first_child->type == EPSILON){
						/*  There is a _rest that just has an epsilon node.  Delete them. */
						destroy_parser_node_tree_and_c_lexer_tokens(m, n->next);
						struct_c_lexer_token_memory_pool_free(m->struct_c_lexer_token_pool, n->c_lexer_token);
						struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n);
						return (struct parser_node *)0;
					}else{
						/*  The first child is no longer the identifier, it is the expansion of the _rest */
						struct parser_node * rtn = convert_declarator_to_abstract_declarator_h(m, n->next->first_child);
						struct_c_lexer_token_memory_pool_free(m->struct_c_lexer_token_pool, n->next->c_lexer_token);
						struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n->next);
						struct_c_lexer_token_memory_pool_free(m->struct_c_lexer_token_pool, n->c_lexer_token);
						struct_parser_node_memory_pool_free(m->struct_parser_node_pool, n);
						return rtn;
					}
				}case OPEN_PAREN_CHAR:{
					/*  Just keep moving along */
					n->next = convert_declarator_to_abstract_declarator_h(m, n->next);
					return n;
				}case CLOSE_PAREN_CHAR:{
					/*  Just keep moving along */
					n->next = convert_declarator_to_abstract_declarator_h(m, n->next);
					return n;
				}case OPEN_SQUARE_BRACKET_CHAR:{
					/*  Just keep moving along */
					n->next = convert_declarator_to_abstract_declarator_h(m, n->next);
					return n;
				}case CLOSE_SQUARE_BRACKET_CHAR:{
					/*  Just keep moving along */
					n->next = convert_declarator_to_abstract_declarator_h(m, n->next);
					return n;
				}default:{
					assert(0);
					return (struct parser_node *)0;
				}
			}
		}default:{
			assert(0);
			return (struct parser_node *)0;
		}
	}
}


struct c_lexer_token * get_identifier_token_from_normalized_declarator(struct normalized_declarator * declarator){
	if(!(declarator)){
		return (struct c_lexer_token *)0;
	}

	switch(declarator->type){
		case NORMALIZED_DECLARATOR:{
			return get_identifier_token_from_declarator(declarator->declarator);
		}case NORMALIZED_ABSTRACT_DECLARATOR:{
			return (struct c_lexer_token *)0;
		}case NORMALIZED_STRUCT_DECLARATOR:{
			if(declarator->declarator->first_child->type == DECLARATOR){
				return get_identifier_token_from_declarator(declarator->declarator->first_child);
			}else{
				return (struct c_lexer_token *)0;
			}
		}case NORMALIZED_INIT_DECLARATOR:{
			return get_identifier_token_from_declarator(declarator->declarator->first_child);
		}case NORMALIZED_ENUMERATOR:{
			return declarator->declarator->first_child->c_lexer_token;
		}default:{
			assert(0);
		}
	}
	return (struct c_lexer_token *)0;
}

struct parser_node * get_function_definition(struct normalized_declaration_set * declaration_set){
	return declaration_set->type == NORMALIZED_FUNCTION_DEFINITION ? declaration_set->set : (struct parser_node *)0;
}

unsigned int is_bitfield(struct parser_node * n){
	return n && n->type == STRUCT_DECLARATOR && (
		n->first_child->type == TERMINAL ||
		(n->first_child->next && n->first_child->next->type == TERMINAL)
	);
}

unsigned int is_struct(struct parser_node * n){
	assert(n->type == STRUCT_OR_UNION_SPECIFIER || n->type == ENUM_SPECIFIER);
	return n->first_child->first_child && n->first_child->first_child->c_lexer_token->type == STRUCT;
}

unsigned int is_union(struct parser_node * n){
	assert(n->type == STRUCT_OR_UNION_SPECIFIER || n->type == ENUM_SPECIFIER);
	return n->first_child->first_child && n->first_child->first_child->c_lexer_token->type == UNION;
}

unsigned int contains_struct_or_union_or_enum_definition(struct namespace_object * obj){
	unsigned int num_elements = struct_normalized_declaration_element_ptr_list_size(&obj->elements);
	unsigned int i;
	for(i = 0; i < num_elements; i++){
		struct normalized_declaration_element * element = struct_normalized_declaration_element_ptr_list_get(&obj->elements, i);
		struct parser_node * struct_or_union_or_enum_specifier = get_struct_or_union_or_enum_specifier(element->normalized_specifiers);
		if(struct_or_union_or_enum_specifier){
			unsigned int its_a_struct = is_struct(struct_or_union_or_enum_specifier);
			unsigned int its_a_union = is_union(struct_or_union_or_enum_specifier);
			unsigned int its_a_enum = is_enum(struct_or_union_or_enum_specifier);
			if(its_a_enum){
				if(get_enumerator_list(struct_or_union_or_enum_specifier)){
					return 1;
				}
			}

			if(its_a_struct || its_a_union){
				if(get_struct_declaration_list(struct_or_union_or_enum_specifier)){
					return 1;
				}
			}
		}
	}
	return 0;
}

unsigned int contains_function_definition(struct namespace_object * obj){
	return get_function_definition_from_namespace_object(obj) != (struct parser_node *)0;
}

struct parser_node * get_function_definition_from_namespace_object(struct namespace_object * obj){
	unsigned int num_elements = struct_normalized_declaration_element_ptr_list_size(&obj->elements);
	unsigned int i;
	for(i = 0; i < num_elements; i++){
		struct normalized_declaration_element * element = struct_normalized_declaration_element_ptr_list_get(&obj->elements, i);
		struct parser_node * rtn;
		if((rtn = get_function_definition(element->normalized_declaration_set))){
			return rtn;
		}
	}
	return (struct parser_node *)0;
}



unsigned int is_enum(struct parser_node * n){
	assert(n->type == STRUCT_OR_UNION_SPECIFIER || n->type == ENUM_SPECIFIER);
	return n->first_child->type == TERMINAL && n->first_child->c_lexer_token->type;
}

unsigned int is_anonymous_struct_or_union_or_enum(struct parser_node * n){
	assert(n->type == STRUCT_OR_UNION_SPECIFIER || n->type == ENUM_SPECIFIER);
	return n->first_child->next->c_lexer_token->type != IDENTIFIER;
}

unsigned int is_struct_or_union_or_enum_definition(struct parser_node * n){
	assert(n->type == STRUCT_OR_UNION_SPECIFIER || n->type == ENUM_SPECIFIER);
	return (
		n->first_child->next->c_lexer_token->type == OPEN_BRACE_CHAR || (
			n->first_child->next->next && 
			n->first_child->next->next->c_lexer_token->type == OPEN_BRACE_CHAR
		)
	);
}

struct parser_node * get_struct_declaration_list(struct parser_node * n){
	assert(n->type == STRUCT_OR_UNION_SPECIFIER);
	if(n->first_child->next->next){
		if(n->first_child->next->next->type == STRUCT_DECLARATION_LIST){
			return n->first_child->next->next;
		}else if(n->first_child->next->next->next){
			if(n->first_child->next->next->next->type == STRUCT_DECLARATION_LIST){
				return n->first_child->next->next->next;
			}
		}
	}
	return (struct parser_node *)0;
}

struct parser_node * get_enumerator_list(struct parser_node * n){
	assert(n->type == ENUM_SPECIFIER);
	if(n->first_child->next->next){
		if(n->first_child->next->next->type == ENUMERATOR_LIST){
			return n->first_child->next->next;
		}else if(n->first_child->next->next->next){
			if(n->first_child->next->next->next->type == ENUMERATOR_LIST){
				return n->first_child->next->next->next;
			}
		}
	}
	return (struct parser_node *)0;
}

struct c_lexer_token * get_struct_or_union_or_enum_tag_token(struct parser_node * n){
	assert(!n || n->type == STRUCT_OR_UNION_SPECIFIER || n->type == ENUM_SPECIFIER);
	return n && n->first_child->next->c_lexer_token->type == IDENTIFIER ? n->first_child->next->c_lexer_token : (struct c_lexer_token *)0;
}

struct parser_node * get_struct_or_union_or_enum_specifier(struct struct_normalized_specifier_ptr_list * list){
	unsigned int len = struct_normalized_specifier_ptr_list_size(list);
	unsigned int i;
	/* TODO:  This will only return the *first* struct or union specifier.  Others are ignored. */
	for(i = 0; i < len; i++){
		struct normalized_specifier * normalized_specifier = struct_normalized_specifier_ptr_list_get(list, i);
		if(normalized_specifier->type == NORMALIZED_TYPE_SPECIFIER){
			if(
				normalized_specifier->specifier->first_child->type == STRUCT_OR_UNION_SPECIFIER ||
				normalized_specifier->specifier->first_child->type == ENUM_SPECIFIER
			){
				return normalized_specifier->specifier->first_child;
			}
		}
	}
	return (struct parser_node *)0;
}

struct normalized_declaration_set * create_normalized_declaration_set_from_parser_node(struct memory_pool_collection * m, struct parser_node * n, struct normalized_declaration_set * parent_set){
	/*  This function gives us a single list of specifiers that modify each element of a list of declarators */
	struct parser_node * specifier_list = (struct parser_node *)0;
	struct normalized_declaration_set * declaration_set = (struct normalized_declaration_set *)malloc(sizeof(struct normalized_declaration_set));

	if(n->type == EXTERNAL_DECLARATION){
		declaration_set->set = n->first_child;
	}else{
		declaration_set->set = n;
	}
	declaration_set->parent_normalized_declaration_set = parent_set;

	switch(declaration_set->set->type){
		case DECLARATION:{
			declaration_set->type = NORMALIZED_DECLARATION;
			break;
		}case FUNCTION_DEFINITION:{
			declaration_set->type = NORMALIZED_FUNCTION_DEFINITION;
			break;
		}case PARAMETER_DECLARATION:{
			declaration_set->type = NORMALIZED_PARAMETER_DECLARATION;
			break;
		}case STRUCT_DECLARATION:{
			declaration_set->type = NORMALIZED_STRUCT_DECLARATION;
			break;
		}case ENUMERATOR_LIST:{
			declaration_set->type = NORMALIZED_ENUMERATOR_LIST;
			break;
		}default:{
			assert(0 && "Unknown node type.");
		}
	}

	if(declaration_set->set->first_child->type == DECLARATION_SPECIFIERS || declaration_set->set->first_child->type == SPECIFIER_QUALIFIER_LIST){
		specifier_list = declaration_set->set->first_child;
	}

	declaration_set->normalized_specifiers = get_normalized_specifier_list(m, specifier_list);
	declaration_set->normalized_declarators = get_normalized_declarator_list(m, declaration_set->set);

	return declaration_set;
}


struct type_description_reference create_type_description_from_normalized_declarator_and_specifiers(struct memory_pool_collection * m, struct normalized_declarator * normalized_declarator, struct struct_normalized_specifier_ptr_list * specifiers, struct parser_node * context, struct scope_level * source_scope_level, enum value_type value_type, struct normalized_declaration_element * source_element){
	/* Take a normalized declarator and specifiers, change any declarator it has into an abstract one, and copy all parser nodes */
	/* Make copies of everything so we can manage memory more consistently */
	struct struct_normalized_specifier_ptr_list * normalized_specifiers_copy = (struct struct_normalized_specifier_ptr_list *)malloc(sizeof(struct struct_normalized_specifier_ptr_list));
	struct normalized_declarator * normalized_declarator_copy = (struct normalized_declarator *)malloc(sizeof(struct normalized_declarator));
	struct type_description_reference new_description;
	unsigned int num_specifiers = struct_normalized_specifier_ptr_list_size(specifiers);
	unsigned int i;
	new_description.t = struct_type_description_memory_pool_malloc(m->struct_type_description_pool);
	normalized_declarator_copy->declarator = create_abstract_declarator_from_normalized_declarator(m, normalized_declarator);
	normalized_declarator_copy->type = NORMALIZED_ABSTRACT_DECLARATOR;
	new_description.t->declarator = normalized_declarator_copy;
	new_description.t->specifiers = normalized_specifiers_copy;
	new_description.t->context = context;
	new_description.t->source_scope_level = source_scope_level;
	new_description.t->value_type = value_type;
	new_description.t->source_element = source_element;
	struct_normalized_specifier_ptr_list_create(normalized_specifiers_copy);
	for(i = 0; i < num_specifiers; i++){
		struct normalized_specifier * src_normalized_specifier = struct_normalized_specifier_ptr_list_get(specifiers, i);
		struct normalized_specifier * dst_normalized_specifier = (struct normalized_specifier *)malloc(sizeof(struct normalized_specifier));
		struct parser_node * specifiers_copy = copy_parser_node_children_only(m, src_normalized_specifier->specifier);
		dst_normalized_specifier->specifier = specifiers_copy;
		dst_normalized_specifier->type = src_normalized_specifier->type;
		struct_normalized_specifier_ptr_list_add_end(normalized_specifiers_copy, dst_normalized_specifier);
	}
	return new_description;
}


void destroy_normalized_declaration_element_list(struct memory_pool_collection * m, struct struct_normalized_declaration_element_ptr_list * list){
	unsigned int num_elements = struct_normalized_declaration_element_ptr_list_size(list);
	unsigned int num_declarators;
	unsigned int num_specifiers;
	unsigned int i;
	struct normalized_declaration_set * declaration_set;

	assert(num_elements);
	/*  Because all declaration elements in a list should share the same declaration set, we can destroy the declaration set after we destroy the list elements */
	declaration_set = struct_normalized_declaration_element_ptr_list_get(list, 0)->normalized_declaration_set;

	/*  In the element list, we just have copies of the pointers from the normalized specifiers and declaratiors on the declaration set. */
	for(i = 0; i < num_elements; i++){
		struct normalized_declaration_element * element = struct_normalized_declaration_element_ptr_list_get(list, i);
		struct_normalized_specifier_ptr_list_destroy(element->normalized_specifiers);
		free(element->normalized_specifiers);
		free(element);
	}
	struct_normalized_declaration_element_ptr_list_destroy(list);
	free(list);

	/*  Now destroy the copies of the parser nodes and lexer tokens, normalized specifier and declarator structs...  */
	num_specifiers = struct_normalized_specifier_ptr_list_size(declaration_set->normalized_specifiers);
	for(i = 0; i < num_specifiers; i++){
		struct normalized_specifier * normalized_specifier = struct_normalized_specifier_ptr_list_get(declaration_set->normalized_specifiers, i);
		if(normalized_specifier){
			destroy_parser_node_tree_and_c_lexer_tokens(m, normalized_specifier->specifier);
		}
		free(normalized_specifier);
	}
	num_declarators = struct_normalized_declarator_ptr_list_size(declaration_set->normalized_declarators);
	for(i = 0; i < num_declarators; i++){
		struct normalized_declarator * normalized_declarator = struct_normalized_declarator_ptr_list_get(declaration_set->normalized_declarators, i);
		if(normalized_declarator){
			destroy_parser_node_tree_and_c_lexer_tokens(m, normalized_declarator->declarator);
		}
		free(normalized_declarator);
	}
	/* Free memory used by the lists */
	struct_normalized_specifier_ptr_list_destroy(declaration_set->normalized_specifiers);
	struct_normalized_declarator_ptr_list_destroy(declaration_set->normalized_declarators);
	/* Free the list structs */
	free(declaration_set->normalized_specifiers);
	free(declaration_set->normalized_declarators);
	/* Free the normalized declaration set */
	free(declaration_set);

}


struct struct_normalized_declaration_element_ptr_list * create_normalized_declaration_element_list(struct normalized_declaration_set * declaration_set){
	struct struct_normalized_declaration_element_ptr_list * list = (struct struct_normalized_declaration_element_ptr_list *)malloc(sizeof(struct struct_normalized_declaration_element_ptr_list));
	unsigned int num_declarators = struct_normalized_declarator_ptr_list_size(declaration_set->normalized_declarators);
	unsigned int i;
	struct_normalized_declaration_element_ptr_list_create(list);

	if(num_declarators == 0){
		/*  If there are no declarators, we still want the declaration specifiers because they can describe structs */
		struct normalized_declaration_element * new_element = (struct normalized_declaration_element *)malloc(sizeof(struct normalized_declaration_element));
		new_element->normalized_specifiers = (struct struct_normalized_specifier_ptr_list *)malloc(sizeof(struct struct_normalized_specifier_ptr_list));
		*(new_element->normalized_specifiers) = struct_normalized_specifier_ptr_list_copy(declaration_set->normalized_specifiers);
		new_element->normalized_declarator = (struct normalized_declarator *)0;
		new_element->normalized_declaration_set = declaration_set;
		struct_normalized_declaration_element_ptr_list_add_end(list, new_element);
	}else{
		for(i = 0; i < num_declarators; i++){
			struct normalized_declaration_element * new_element = (struct normalized_declaration_element *)malloc(sizeof(struct normalized_declaration_element));
			new_element->normalized_specifiers = (struct struct_normalized_specifier_ptr_list *)malloc(sizeof(struct struct_normalized_specifier_ptr_list));
			*(new_element->normalized_specifiers) = struct_normalized_specifier_ptr_list_copy(declaration_set->normalized_specifiers);
			new_element->normalized_declarator = struct_normalized_declarator_ptr_list_get(declaration_set->normalized_declarators,i);
			new_element->normalized_declaration_set = declaration_set;
			struct_normalized_declaration_element_ptr_list_add_end(list, new_element);
		}
	}

	return list;
}


void validate_specifier_token_type(enum c_token_type t){
	assert(t == TYPEDEF || t == VOID || t == CHAR || t == SHORT || t == INT || t == LONG || t == FLOAT || t == DOUBLE || t == SIGNED || t == UNSIGNED || t == IDENTIFIER);
}

int normalized_specifier_ptr_cmp(struct normalized_specifier * a, struct normalized_specifier * b){
	if(a < b){
		return -1;
	}else if(a > b){
		return 1;
	}else{
		return 0;
	}
}

struct normalized_declarator * make_array_brackets(struct memory_pool_collection * m){
	struct normalized_declarator * normalized_declarator = (struct normalized_declarator *)malloc(sizeof(struct normalized_declarator));
	struct parser_node * abstract_declarator = struct_parser_node_memory_pool_malloc(m->struct_parser_node_pool);
	struct parser_node * direct_abstract_declarator = struct_parser_node_memory_pool_malloc(m->struct_parser_node_pool);
	struct parser_node * direct_abstract_declarator_rest = struct_parser_node_memory_pool_malloc(m->struct_parser_node_pool);
	struct parser_node * epsilon = struct_parser_node_memory_pool_malloc(m->struct_parser_node_pool);
	struct parser_node * terminal_1 = struct_parser_node_memory_pool_malloc(m->struct_parser_node_pool);
	struct parser_node * terminal_2 = struct_parser_node_memory_pool_malloc(m->struct_parser_node_pool);
	struct c_lexer_token * open = struct_c_lexer_token_memory_pool_malloc(m->struct_c_lexer_token_pool);
	struct c_lexer_token * close = struct_c_lexer_token_memory_pool_malloc(m->struct_c_lexer_token_pool);

	normalized_declarator->type = NORMALIZED_ABSTRACT_DECLARATOR;
	normalized_declarator->declarator = abstract_declarator;

	/* ABSTRACT_DECLARATOR node */
	abstract_declarator->type = ABSTRACT_DECLARATOR;
	abstract_declarator->first_child = direct_abstract_declarator;
	abstract_declarator->next = (struct parser_node *)0;
	abstract_declarator->c_lexer_token = (struct c_lexer_token *)0;

	/* DIRECT_ABSTRACT_DECLARATOR node */
	direct_abstract_declarator->type = DIRECT_ABSTRACT_DECLARATOR;
	direct_abstract_declarator->first_child = terminal_1;
	direct_abstract_declarator->next = (struct parser_node *)0;
	direct_abstract_declarator->c_lexer_token = (struct c_lexer_token *)0;

	/* DIRECT_ABSTRACT_DECLARATOR_REST node */
	direct_abstract_declarator_rest->type = DIRECT_ABSTRACT_DECLARATOR_REST;
	direct_abstract_declarator_rest->first_child = epsilon;
	direct_abstract_declarator_rest->next = (struct parser_node *)0;
	direct_abstract_declarator_rest->c_lexer_token = (struct c_lexer_token *)0;

	/* EPSILON node */
	epsilon->type = EPSILON;
	epsilon->first_child = (struct parser_node *)0;
	epsilon->next = (struct parser_node *)0;
	epsilon->c_lexer_token = (struct c_lexer_token *)0;

	/* ( node */
	terminal_1->type = TERMINAL;
	terminal_1->first_child = (struct parser_node *)0;
	terminal_1->next = terminal_2;
	terminal_1->c_lexer_token = open;

	/* ) node */
	terminal_2->type = TERMINAL;
	terminal_2->first_child = (struct parser_node *)0;
	terminal_2->next = direct_abstract_declarator_rest;
	terminal_2->c_lexer_token = close;

	open->type = OPEN_SQUARE_BRACKET_CHAR;
	open->first_byte = (unsigned char *)g_open_square_bracket_string;
	open->last_byte = (unsigned char *)g_open_square_bracket_string;

	close->type = CLOSE_SQUARE_BRACKET_CHAR;
	close->first_byte = (unsigned char *)g_close_square_bracket_string;
	close->last_byte = (unsigned char *)g_close_square_bracket_string;

	return normalized_declarator;
}

unsigned char * get_specifier_string(enum c_token_type t){
	switch(t){
		case VOID:{ return (unsigned char *)g_void_string; }
		case CHAR:{ return (unsigned char *)g_char_string; }
		case SHORT:{ return (unsigned char *)g_short_string; }
		case INT:{ return (unsigned char *)g_int_string; }
		case LONG:{ return (unsigned char *)g_long_string; }
		case FLOAT:{ return (unsigned char *)g_float_string; }
		case DOUBLE:{ return (unsigned char *)g_double_string; }
		case SIGNED:{ return (unsigned char *)g_signed_string; }
		case UNSIGNED:{ return (unsigned char *)g_unsigned_string; }
		case TYPEDEF:{ return (unsigned char *)g_typedef_string; }
		case EXTERN:{ return (unsigned char *)g_extern_string; }
		case STATIC:{ return (unsigned char *)g_static_string; }
		case AUTO:{ return (unsigned char *)g_auto_string; }
		case CONST:{ return (unsigned char *)g_const_string; }
		case VOLATILE:{ return (unsigned char *)g_volatile_string; }
		default:{ assert(0 && "Not possible."); return (unsigned char *)0; }
	}
}

void print_type_qualifier(struct type_engine_state * state, struct unsigned_char_list * out, struct type_qualifier_id id, enum type_engine_print_type ptype){
	enum c_token_type v = enum_c_token_type_list_get(&state->simple_type_qualifiers, id.id);
	(void)ptype;
	buffered_printf(out, "%s", get_c_token_type_names()[v]);
}

void print_storage_class_specifier(struct type_engine_state * state, struct unsigned_char_list * out, struct storage_class_specifier_id id, enum type_engine_print_type ptype){
	enum c_token_type t = enum_c_token_type_list_get(&state->storage_class_specifiers, id.id);
	(void)ptype;
	buffered_printf(out, "%s", get_c_token_type_names()[t]);
}

void print_identifier(struct type_engine_state * state, struct unsigned_char_list * out, unsigned int index, enum type_engine_print_type ptype){
	struct c_lexer_token i = struct_c_lexer_token_list_get(&state->identifiers, index);
	unsigned char * c = i.first_byte;
	(void)ptype;
	do{
		buffered_printf(out, "%c", *c);
	}while(c++ != i.last_byte);
}

void print_struct_specifier(struct type_engine_state * state, struct unsigned_char_list * out, unsigned int index, enum type_engine_print_type ptype){
	struct struct_specifier_details details = struct_struct_specifier_details_list_get(&state->struct_specifiers, index);
	switch(details.type){
		case TYPE_ENGINE_NAMED_STRUCT:{
			print_identifier(state, out, details.id, ptype);
			break;
		}case TYPE_ENGINE_ANONYMOUS_STRUCT:{
			buffered_printf(out, "Anonymous id %u", details.id);
			break;
		}default:{
			assert(0 && "Not considered.");
		}
	}
}

void print_union_specifier(struct type_engine_state * state, struct unsigned_char_list * out, unsigned int index, enum type_engine_print_type ptype){
	struct union_specifier_details details = struct_union_specifier_details_list_get(&state->union_specifiers, index);
	switch(details.type){
		case TYPE_ENGINE_NAMED_UNION:{
			print_identifier(state, out, details.id, ptype);
			break;
		}case TYPE_ENGINE_ANONYMOUS_UNION:{
			buffered_printf(out, "Anonymous id %u", details.id);
			break;
		}default:{
			assert(0 && "Not considered.");
		}
	}
}

void print_scoped_struct_or_union_specifier(struct type_engine_state * state, struct unsigned_char_list * out, unsigned int index, enum type_engine_print_type ptype){
	struct scoped_struct_or_union_specifier_details details = struct_scoped_struct_or_union_specifier_details_list_get(&state->scoped_struct_or_union_specifiers, index);
	print_struct_or_union_specifier(state, out, details.struct_or_union_specifier_id.id, ptype);
}

void print_struct_or_union_specifier(struct type_engine_state * state, struct unsigned_char_list * out, unsigned int index, enum type_engine_print_type ptype){
	struct struct_or_union_specifier_details details = struct_struct_or_union_specifier_details_list_get(&state->struct_or_union_specifiers, index);
	switch(details.type){
		case TYPE_ENGINE_STRUCT_SPECIFIER:{
			buffered_printf(out, "struct ");
			print_struct_specifier(state, out, details.id, ptype);
			break;
		}case TYPE_ENGINE_UNION_SPECIFIER:{
			buffered_printf(out, "union ");
			print_union_specifier(state, out, details.id, ptype);
			break;
		}default:{
			assert(0 && "Not considered.");
		}
	}
}

void print_aggregate_specifiers(struct type_engine_state * state, struct unsigned_char_list * out, struct aggregate_specifier_id id, enum type_engine_print_type ptype){
	struct aggregate_specifier_details details = struct_aggregate_specifier_details_list_get(&state->aggregate_specifiers, id.id);
	switch(details.type){
		case TYPE_ENGINE_SIMPLE_SPECIFIER:{
			if(ptype == TYPE_ENGINE_DEBUG){ buffered_printf(out, "TYPE_ENGINE_SIMPLE_SPECIFIER:  "); }
			buffered_printf(out, "%s", get_c_token_type_names()[enum_c_token_type_list_get(&state->simple_type_specifiers, details.id)]);
			break;
		}case TYPE_ENGINE_STRUCT_OR_UNION_SPECIFIER:{
			print_scoped_struct_or_union_specifier(state, out, details.id, ptype);
			break;
		}case TYPE_ENGINE_ENUM_SPECIFIER:{
			buffered_printf(out, "TYPE_ENGINE_ENUM_SPECIFIER: TODO: enum");
			break;
		}case TYPE_ENGINE_TYPENAME_SPECIFIER:{
			buffered_printf(out, "TYPE_ENGINE_TYPENAME_SPECIFIER: typename");
			break;
		}default:{
			assert(0 && "Not considered.");
		}
	}
}

void print_type_specifier_or_qualifier(struct type_engine_state * state, struct unsigned_char_list * out, struct specifier_or_qualifier_id index, enum type_engine_print_type ptype){
	struct specifier_or_qualifier_details details = struct_specifier_or_qualifier_details_list_get(&state->specifier_or_qualifiers, index.id);
	switch(details.type){
		case TYPE_ENGINE_TYPE_SPECIFIER:{
			struct aggregate_specifier_id id;
			if(ptype == TYPE_ENGINE_DEBUG){ buffered_printf(out, "type specifier: "); }
			id.id = details.id;
			print_aggregate_specifiers(state, out, id, ptype);
			break;
		}case TYPE_ENGINE_SIMPLE_QUALIFIER:{
			struct type_qualifier_id id;
			id.id = details.id;
			print_type_qualifier(state, out, id, ptype);
			break;
		}case TYPE_ENGINE_STORAGE_CLASS_SPECIFIER:{
			struct storage_class_specifier_id id;
			id.id = details.id;
			print_storage_class_specifier(state, out, id, ptype);
			break;
		}default:{
			assert(0 && "Not considered.");
		}
	}
}

void print_pointer_part(struct type_engine_state * state, struct unsigned_char_list * out, unsigned int details, enum type_engine_print_type ptype){
	enum type_engine_pointer_part_kind type = (enum type_engine_pointer_part_kind)((details & 0x80000000) >> 31);
	unsigned int sub_id = details & 0x7FFFFFFF;
	switch(type){
		case TYPE_ENGINE_QUALIFIED_POINTER:{
			struct specifier_or_qualifier_list_item_id a;
			a.id = sub_id;
			buffered_printf(out, "*");
			print_specifier_or_qualifier_list(state, out, a, ptype);
			break;
		}case TYPE_ENGINE_UNQUALIFIED_POINTER:{
			buffered_printf(out, "*");
			break;
		}default:{
			assert(0 && "Not considered.");
		}
	}
}

void print_function_part(struct type_engine_state * state, struct unsigned_char_list * out, unsigned int details, enum type_engine_print_type ptype){
	enum type_engine_function_part_kind type = (enum type_engine_function_part_kind)((details & 0x80000000) >> 31);
	(void)ptype;
	(void)state;
	switch(type){
		case TYPE_ENGINE_FUNCTION_K_AND_R_C:{
			buffered_printf(out, "K R ()");
			break;
		}case TYPE_ENGINE_FUNCTION_PROTOTYPE:{
			buffered_printf(out, "(...)");
			break;
		}default:{
			assert(0 && "Not considered.");
		}
	}
}

void print_array_part(struct type_engine_state * state, struct unsigned_char_list * out, unsigned int details, enum type_engine_print_type ptype){
	enum type_engine_array_part_kind type = (enum type_engine_array_part_kind)((details & 0x80000000) >> 31);
	(void)ptype;
	(void)state;
	switch(type){
		case TYPE_ENGINE_CONSTANT_EXPRESSION_ARRAY:{
			buffered_printf(out, "[...]");
			break;
		}case TYPE_ENGINE_FLEXIBLE_ARRAY:{
			buffered_printf(out, "[]");
			break;
		}default:{
			assert(0 && "Not considered.");
		}
	}
}

void print_declarator_in_visual_order(struct type_engine_state * state, struct unsigned_char_list * out, struct declarator_part_list_item_id current_declarator_index, enum type_engine_print_type ptype, unsigned int prev_was_fcn_or_arr){
	struct declarator_part_list_item_details details = struct_declarator_part_list_item_details_list_get(&state->declarator_part_list_items, current_declarator_index.id);
	struct declarator_part_id item_id = details.item_id;
	struct declarator_part_list_item_id prev_id = details.prev_id;
	if(details.type == TYPE_ENGINE_DECLARATOR_PART_LIST_ITEM_LIST_START){
		if(ptype == TYPE_ENGINE_DEBUG){ buffered_printf(out, "Start of List.\n"); }
	}else{
		struct declarator_part_details declarator_part_details = struct_declarator_part_details_list_get(&state->declarator_parts, item_id.id);
		/*  If we have a pointer to a function or an array, we need brackets */
		switch(declarator_part_details.type){
			case TYPE_ENGINE_FUNCTION_DECLARATOR_PART:; case TYPE_ENGINE_ARRAY_DECLARATOR_PART:{
				print_declarator_in_visual_order(state, out, prev_id, ptype, 1);
				print_declarator_part(state, out, item_id, ptype);
				break;
			}case TYPE_ENGINE_POINTER_DECLARATOR_PART:{
				if(prev_was_fcn_or_arr){
					buffered_printf(out, "(");
				}
				print_declarator_part(state, out, item_id, ptype);
				print_declarator_in_visual_order(state, out, prev_id, ptype, 0);
				if(prev_was_fcn_or_arr){
					buffered_printf(out, ")");
				}
				break;
			}default:{
				assert(0 && "Not considered.");
			}
		}

		if(ptype == TYPE_ENGINE_DEBUG){ buffered_printf(out, "\n"); }
	}
}
void print_bitfield_or_declarator(struct type_engine_state * state, struct unsigned_char_list * out, unsigned int index, enum type_engine_print_type ptype){
	struct bitfield_or_declarator_details details = struct_bitfield_or_declarator_details_list_get(&state->bitfield_or_declarators, index);
	switch(details.type){
		case TYPE_ENGINE_ONLY_DECLARATOR:{
			struct declarator_part_list_item_id a;
			a.id = details.id;
			print_declarator_in_visual_order(state, out, a, ptype, 0);
			break;
		}case TYPE_ENGINE_ONLY_BITFIELD:{
			buffered_printf(out, "TODO: bitfield");
			break;
		}case TYPE_ENGINE_BITFIELD_AND_DECLARATOR:{
			buffered_printf(out, "TODO: bitfield and declarator");
			break;
		}default:{
			assert(0 && "Not considered.");
		}
	}
}

void print_declarator_in_spiral_rule_order(struct type_engine_state * state, struct unsigned_char_list * out, struct declarator_part_list_item_id current_declarator_index, enum type_engine_print_type ptype){
	struct declarator_part_list_item_details details = struct_declarator_part_list_item_details_list_get(&state->declarator_part_list_items, current_declarator_index.id);
	if(details.type == TYPE_ENGINE_DECLARATOR_PART_LIST_ITEM_LIST_START){
		if(ptype == TYPE_ENGINE_DEBUG){ buffered_printf(out, "Start of List.\n"); }
	}else{
		print_declarator_in_spiral_rule_order(state, out, details.prev_id, ptype);
		print_declarator_part(state, out, details.item_id, ptype);
		buffered_printf(out, ", ");
		if(ptype == TYPE_ENGINE_DEBUG){ buffered_printf(out, "\n"); }
	}
}

void print_specifier_or_qualifier_list(struct type_engine_state * state, struct unsigned_char_list * out, struct specifier_or_qualifier_list_item_id id, enum type_engine_print_type ptype){
	struct specifier_or_qualifier_list_item_details details = struct_specifier_or_qualifier_list_item_details_list_get(&state->specifier_or_qualifier_list_items, id.id);
	enum type_engine_specifier_or_qualifier_list_item_kind is_start = details.type;
	struct specifier_or_qualifier_id item_id = details.item_id;
	struct specifier_or_qualifier_list_item_id prev_id = details.prev_id;
	if(is_start == TYPE_ENGINE_SPECIFIER_OR_QUALIFIER_LIST_ITEM_LIST_START){
		if(ptype == TYPE_ENGINE_DEBUG){ buffered_printf(out, "Start of List.\n"); }
	}else{
		print_specifier_or_qualifier_list(state, out, prev_id, ptype);
		print_type_specifier_or_qualifier(state, out, item_id, ptype);
		buffered_printf(out, " ");
		if(ptype == TYPE_ENGINE_DEBUG){ buffered_printf(out, "\n"); }
	}
}

void print_general_type_list(struct type_engine_state * state, unsigned char * delimiter, struct unsigned_char_list * out, unsigned int list_id, enum type_engine_print_type ptype, unsigned int current_level){
	struct general_type_list_item_details details = struct_general_type_list_item_details_list_get(&state->general_type_list_items, list_id);
	if(details.type == TYPE_ENGINE_GENERAL_TYPE_LIST_ITEM_LIST_START){
		if(ptype == TYPE_ENGINE_DEBUG){ buffered_printf(out, "Start of List.\n"); }
	}else{
		print_general_type_list(state, delimiter, out, details.prev_id.id, ptype, current_level + 1);
		print_general_type_list_item(state, out, details.item_id.id, ptype);
		if(current_level){
			buffered_printf(out, (char *)delimiter);
		}
		if(ptype == TYPE_ENGINE_DEBUG){ buffered_printf(out, "\n"); }
	}
}

void print_general_type_list_item(struct type_engine_state * state, struct unsigned_char_list * out, unsigned int index, enum type_engine_print_type ptype){
	struct general_type_details details = struct_general_type_details_list_get(&state->general_types, index);
	unsigned int specifier_or_qualifier_list_id = details.specifier_or_qualifier_list_item_id.id;
	unsigned int bitfield_or_declarator = details.bitfield_or_declarator_id.id;
	struct specifier_or_qualifier_list_item_id a;
	a.id = specifier_or_qualifier_list_id;
	print_specifier_or_qualifier_list(state, out, a, ptype);
	print_bitfield_or_declarator(state, out, bitfield_or_declarator, ptype);
}

void print_declarator_part(struct type_engine_state * state, struct unsigned_char_list * out, struct declarator_part_id index, enum type_engine_print_type ptype){
	struct declarator_part_details details = struct_declarator_part_details_list_get(&state->declarator_parts, index.id);
	switch(details.type){
		case TYPE_ENGINE_FUNCTION_DECLARATOR_PART:{
			if(ptype == TYPE_ENGINE_DEBUG){ buffered_printf(out, "Function: "); }
			print_function_part(state, out, struct_function_part_details_list_get(&state->function_parts, details.id).id, ptype);
			break;
		}case TYPE_ENGINE_POINTER_DECLARATOR_PART:{
			if(ptype == TYPE_ENGINE_DEBUG){ buffered_printf(out, "Pointer: "); }
			print_pointer_part(state, out, struct_pointer_part_details_list_get(&state->pointer_parts, details.id).id, ptype);
			break;
		}case TYPE_ENGINE_ARRAY_DECLARATOR_PART:{
			if(ptype == TYPE_ENGINE_DEBUG){ buffered_printf(out, "Array: "); }
			print_array_part(state, out, struct_array_part_details_list_get(&state->array_parts, details.id).id, ptype);
			break;
		}default:{
			assert(0 && "Not considered.");
		}
	}
}

void print_type_engine_state(struct type_engine_state * state, struct unsigned_char_list * out){
	unsigned int i;
	struct struct_aggregate_specifier_details_list aggregate_specifiers_map_keys;
	struct enum_c_token_type_list simple_type_specifiers_map_keys;
	struct enum_c_token_type_list simple_type_qualifiers_map_keys;
	struct enum_c_token_type_list storage_class_specifiers_map_keys;
	struct struct_specifier_or_qualifier_details_list specifier_or_qualifiers_map_keys;
	struct struct_specifier_or_qualifier_list_item_details_list specifier_or_qualifier_list_items_map_keys;
	struct struct_declarator_part_list_item_details_list declarator_part_list_items_map_keys;
	struct struct_c_lexer_token_list identifier_map_keys;
	simple_type_specifiers_map_keys = enum_c_token_type_to_struct_simple_specifier_id_map_keys(&state->simple_type_specifiers_map);
	buffered_printf(out, "Simple Type Specifier Mapping\n");
	buffered_printf(out, "| Key || Index |\n");
	for(i = 0; i < enum_c_token_type_list_size(&simple_type_specifiers_map_keys); i++){
		enum c_token_type key = enum_c_token_type_list_get(&simple_type_specifiers_map_keys, i);
		struct simple_specifier_id value = enum_c_token_type_to_struct_simple_specifier_id_map_get(&state->simple_type_specifiers_map, key);
		buffered_printf(out, "| %u %s |", key, get_c_token_type_names()[key]);
		buffered_printf(out, "| %u   |\n", value.id);
	}
	enum_c_token_type_list_destroy(&simple_type_specifiers_map_keys);

	simple_type_qualifiers_map_keys = enum_c_token_type_to_struct_type_qualifier_id_map_keys(&state->simple_type_qualifiers_map);
	buffered_printf(out, "Simple Type Qualifier Mapping\n");
	buffered_printf(out, "| Key || Index |\n");
	for(i = 0; i < enum_c_token_type_list_size(&simple_type_qualifiers_map_keys); i++){
		enum c_token_type key = enum_c_token_type_list_get(&simple_type_qualifiers_map_keys, i);
		struct type_qualifier_id value = enum_c_token_type_to_struct_type_qualifier_id_map_get(&state->simple_type_qualifiers_map, key);
		buffered_printf(out, "| %u %s |", key, get_c_token_type_names()[key]);
		buffered_printf(out, "| %u   |\n", value.id);
	}
	enum_c_token_type_list_destroy(&simple_type_qualifiers_map_keys);

	storage_class_specifiers_map_keys = enum_c_token_type_to_struct_storage_class_specifier_id_map_keys(&state->storage_class_specifiers_map);
	buffered_printf(out, "Storage Class Specifier Mapping\n");
	buffered_printf(out, "| Key || Index |\n");
	for(i = 0; i < enum_c_token_type_list_size(&storage_class_specifiers_map_keys); i++){
		enum c_token_type key = enum_c_token_type_list_get(&storage_class_specifiers_map_keys, i);
		struct storage_class_specifier_id s = enum_c_token_type_to_struct_storage_class_specifier_id_map_get(&state->storage_class_specifiers_map, key);
		buffered_printf(out, "| %u %s |", key, get_c_token_type_names()[key]);
		buffered_printf(out, "| %u   |\n", s.id);
	}
	enum_c_token_type_list_destroy(&storage_class_specifiers_map_keys);

	aggregate_specifiers_map_keys = struct_aggregate_specifier_details_to_struct_aggregate_specifier_id_map_keys(&state->aggregate_specifiers_map);
	buffered_printf(out, "All Type Specifiers Mapping\n");
	buffered_printf(out, "| Key || Index |\n");
	for(i = 0; i < struct_aggregate_specifier_details_list_size(&aggregate_specifiers_map_keys); i++){
		struct aggregate_specifier_details key = struct_aggregate_specifier_details_list_get(&aggregate_specifiers_map_keys, i);
		struct aggregate_specifier_id value = struct_aggregate_specifier_details_to_struct_aggregate_specifier_id_map_get(&state->aggregate_specifiers_map, key);
		buffered_printf(out, "| ");
		print_aggregate_specifiers(state, out, value, TYPE_ENGINE_DEBUG);
		buffered_printf(out, " |");
		buffered_printf(out, "| %u   |\n", value.id);
	}
	struct_aggregate_specifier_details_list_destroy(&aggregate_specifiers_map_keys);

	specifier_or_qualifiers_map_keys = struct_specifier_or_qualifier_details_to_struct_specifier_or_qualifier_id_map_keys(&state->specifier_or_qualifiers_map);
	buffered_printf(out, "Specifier, Qualifiers Or Storage Classes\n");
	buffered_printf(out, "| Item || Value |\n");
	for(i = 0; i < struct_specifier_or_qualifier_details_list_size(&specifier_or_qualifiers_map_keys); i++){
		struct specifier_or_qualifier_details key = struct_specifier_or_qualifier_details_list_get(&specifier_or_qualifiers_map_keys, i);
		struct specifier_or_qualifier_id value = struct_specifier_or_qualifier_details_to_struct_specifier_or_qualifier_id_map_get(&state->specifier_or_qualifiers_map, key);

		buffered_printf(out, "| %X   |", key);
		buffered_printf(out, "| %u   |\n", value.id);
	}
	struct_specifier_or_qualifier_details_list_destroy(&specifier_or_qualifiers_map_keys);

	specifier_or_qualifier_list_items_map_keys = struct_specifier_or_qualifier_list_item_details_to_struct_specifier_or_qualifier_list_item_id_map_keys(&state->specifier_or_qualifier_list_items_map);
	buffered_printf(out, "Specifier or Qualifier List Items\n");
	buffered_printf(out, "| List || Index Of First Item |\n");
	for(i = 0; i < struct_specifier_or_qualifier_list_item_details_list_size(&specifier_or_qualifier_list_items_map_keys); i++){
		struct specifier_or_qualifier_list_item_details key = struct_specifier_or_qualifier_list_item_details_list_get(&specifier_or_qualifier_list_items_map_keys, i);
		struct specifier_or_qualifier_list_item_id value = struct_specifier_or_qualifier_list_item_details_to_struct_specifier_or_qualifier_list_item_id_map_get(&state->specifier_or_qualifier_list_items_map, key);

		buffered_printf(out, "| ");
		/*TODO:  Incorrect print_type_specifier_or_qualifier_list_item(state, out, key, TYPE_ENGINE_DEBUG);*/
		buffered_printf(out, " |");
		buffered_printf(out, "| %u   |\n", value.id);
	}
	struct_specifier_or_qualifier_list_item_details_list_destroy(&specifier_or_qualifier_list_items_map_keys);

	declarator_part_list_items_map_keys = struct_declarator_part_list_item_details_to_struct_declarator_part_list_item_id_map_keys(&state->declarator_part_list_items_map);
	buffered_printf(out, "Declarator Part List Items\n");
	buffered_printf(out, "| List || Index Of First Item |\n");
	for(i = 0; i < struct_declarator_part_list_item_details_list_size(&declarator_part_list_items_map_keys); i++){
		struct declarator_part_list_item_details key = struct_declarator_part_list_item_details_list_get(&declarator_part_list_items_map_keys, i);
		struct declarator_part_list_item_id value = struct_declarator_part_list_item_details_to_struct_declarator_part_list_item_id_map_get(&state->declarator_part_list_items_map, key);

		buffered_printf(out, "| ");
		/*TODO:  Incorrect print_declarator_part_list_item(state, out, key, TYPE_ENGINE_DEBUG);*/
		buffered_printf(out, " |");
		buffered_printf(out, "| %u   |\n", value);
	}
	struct_declarator_part_list_item_details_list_destroy(&declarator_part_list_items_map_keys);

	identifier_map_keys = struct_c_lexer_token_to_struct_identifier_id_map_keys(&state->identifiers_map);
	buffered_printf(out, "Identifiers Mapping\n");
	buffered_printf(out, "| Key || Index |\n");
	for(i = 0; i < struct_c_lexer_token_list_size(&identifier_map_keys); i++){
		struct c_lexer_token key = struct_c_lexer_token_list_get(&identifier_map_keys, i);
		struct identifier_id value = struct_c_lexer_token_to_struct_identifier_id_map_get(&state->identifiers_map, key);
		unsigned char * c = key.first_byte;
		buffered_printf(out, "| ");
		do{
			buffered_printf(out, "%c", *c);
		}while(c++ != key.last_byte);
		buffered_printf(out, " |");
		buffered_printf(out, "| %u   |\n", value.id);
	}
	struct_c_lexer_token_list_destroy(&identifier_map_keys);
}

void create_type_engine_state(struct type_engine_state * state, struct memory_pool_collection * m){
	(void)m;
	struct_parser_node_ptr_to_struct_scope_level_id_map_create(&state->parser_node_scope_associations, struct_struct_parser_node_ptr_to_struct_scope_level_id_key_value_pair_compare);
	enum_type_engine_operation_type_list_create(&state->reversible_operations);
	struct_identifier_id_list_create(&state->saved_identifier_ids);
	struct_struct_or_union_specifier_id_list_create(&state->saved_struct_or_union_specifier_ids);

	enum_c_token_type_list_create(&state->simple_type_qualifiers);
	enum_c_token_type_list_create(&state->simple_type_specifiers);
	struct_aggregate_specifier_details_list_create(&state->aggregate_specifiers);
	enum_c_token_type_list_create(&state->storage_class_specifiers);
	struct_specifier_or_qualifier_details_list_create(&state->specifier_or_qualifiers);
	struct_specifier_or_qualifier_list_item_details_list_create(&state->specifier_or_qualifier_list_items);
	struct_parameter_list_details_list_create(&state->parameter_lists);
	struct_declarator_part_list_item_details_list_create(&state->declarator_part_list_items);
	struct_declarator_part_details_list_create(&state->declarator_parts);
	struct_pointer_part_details_list_create(&state->pointer_parts);
	struct_function_part_details_list_create(&state->function_parts);
	struct_array_part_details_list_create(&state->array_parts);
	struct_bitfield_or_declarator_details_list_create(&state->bitfield_or_declarators);
	struct_general_type_details_list_create(&state->general_types);
	struct_general_type_list_item_details_list_create(&state->general_type_list_items);
	struct_struct_or_union_specifier_details_list_create(&state->struct_or_union_specifiers);
	struct_scoped_struct_or_union_specifier_details_list_create(&state->scoped_struct_or_union_specifiers);
	struct_union_specifier_details_list_create(&state->union_specifiers);
	struct_struct_specifier_details_list_create(&state->struct_specifiers);
	struct_c_lexer_token_list_create(&state->identifiers);
	struct_parser_node_ptr_list_create(&state->constant_expression_parser_nodes);
	struct_scope_level_ptr_list_create(&state->scope_levels);
	struct_aggregate_specifier_details_to_struct_aggregate_specifier_id_map_create(&state->aggregate_specifiers_map, struct_struct_aggregate_specifier_details_to_struct_aggregate_specifier_id_key_value_pair_compare);
	enum_c_token_type_to_struct_simple_specifier_id_map_create(&state->simple_type_specifiers_map, struct_enum_c_token_type_to_struct_simple_specifier_id_key_value_pair_compare);
	enum_c_token_type_to_struct_type_qualifier_id_map_create(&state->simple_type_qualifiers_map, struct_enum_c_token_type_to_struct_type_qualifier_id_key_value_pair_compare);
	enum_c_token_type_to_struct_storage_class_specifier_id_map_create(&state->storage_class_specifiers_map, struct_enum_c_token_type_to_struct_storage_class_specifier_id_key_value_pair_compare);
	struct_specifier_or_qualifier_details_to_struct_specifier_or_qualifier_id_map_create(&state->specifier_or_qualifiers_map, struct_struct_specifier_or_qualifier_details_to_struct_specifier_or_qualifier_id_key_value_pair_compare);
	struct_specifier_or_qualifier_list_item_details_to_struct_specifier_or_qualifier_list_item_id_map_create(&state->specifier_or_qualifier_list_items_map, struct_struct_specifier_or_qualifier_list_item_details_to_struct_specifier_or_qualifier_list_item_id_key_value_pair_compare);
	struct_declarator_part_list_item_details_to_struct_declarator_part_list_item_id_map_create(&state->declarator_part_list_items_map, struct_struct_declarator_part_list_item_details_to_struct_declarator_part_list_item_id_key_value_pair_compare);
	struct_declarator_part_details_to_struct_declarator_part_id_map_create(&state->declarator_parts_map, struct_struct_declarator_part_details_to_struct_declarator_part_id_key_value_pair_compare);
	struct_pointer_part_details_to_struct_pointer_part_id_map_create(&state->pointer_parts_map, struct_struct_pointer_part_details_to_struct_pointer_part_id_key_value_pair_compare);
	struct_function_part_details_to_struct_function_part_id_map_create(&state->function_parts_map, struct_struct_function_part_details_to_struct_function_part_id_key_value_pair_compare);
	struct_array_part_details_to_struct_array_part_id_map_create(&state->array_parts_map, struct_struct_array_part_details_to_struct_array_part_id_key_value_pair_compare);
	struct_parameter_list_details_to_struct_parameter_list_id_map_create(&state->parameter_lists_map, struct_struct_parameter_list_details_to_struct_parameter_list_id_key_value_pair_compare);
	struct_bitfield_or_declarator_details_to_struct_bitfield_or_declarator_id_map_create(&state->bitfield_or_declarators_map, struct_struct_bitfield_or_declarator_details_to_struct_bitfield_or_declarator_id_key_value_pair_compare);
	struct_general_type_details_to_struct_general_type_id_map_create(&state->general_types_map, struct_struct_general_type_details_to_struct_general_type_id_key_value_pair_compare);
	struct_general_type_list_item_details_to_struct_general_type_list_item_id_map_create(&state->general_type_list_items_map, struct_struct_general_type_list_item_details_to_struct_general_type_list_item_id_key_value_pair_compare);
	struct_struct_or_union_specifier_details_to_struct_struct_or_union_specifier_id_map_create(&state->struct_or_union_specifiers_map, struct_struct_struct_or_union_specifier_details_to_struct_struct_or_union_specifier_id_key_value_pair_compare);
	struct_scoped_struct_or_union_specifier_details_to_struct_scoped_struct_or_union_specifier_id_map_create(&state->scoped_struct_or_union_specifiers_map, struct_struct_scoped_struct_or_union_specifier_details_to_struct_scoped_struct_or_union_specifier_id_key_value_pair_compare);
	struct_union_specifier_details_to_struct_union_specifier_id_map_create(&state->union_specifiers_map, struct_struct_union_specifier_details_to_struct_union_specifier_id_key_value_pair_compare);
	struct_struct_specifier_details_to_struct_struct_specifier_id_map_create(&state->struct_specifiers_map, struct_struct_struct_specifier_details_to_struct_struct_specifier_id_key_value_pair_compare);
	struct_parser_node_ptr_to_struct_parser_node_id_map_create(&state->constant_expression_parser_nodes_map, struct_struct_parser_node_ptr_to_struct_parser_node_id_key_value_pair_compare);
	struct_scope_level_ptr_to_struct_scope_level_id_map_create(&state->scope_levels_map, struct_struct_scope_level_ptr_to_struct_scope_level_id_key_value_pair_compare);
	struct_c_lexer_token_to_struct_identifier_id_map_create(&state->identifiers_map, struct_struct_c_lexer_token_to_struct_identifier_id_key_value_pair_compare);
	unsigned_int_to_void_ptr_map_create(&state->function_definitions.general_type_ids_to_function_definitions, struct_unsigned_int_to_void_ptr_key_value_pair_compare);
	void_ptr_list_create(&state->stored_pointers);
	state->current_scope_depth = 0;
	state->top_scope = create_empty_scope(state, (struct scope_level *)0, SCOPE_LEVEL_TYPE_FILE_SCOPE);
}

void destroy_type_engine_state(struct type_engine_state * state){
	struct unsigned_char_list l;
	unsigned char * c;
	unsigned_char_list_create(&l);
	/*print_type_engine_state(state, &l);*/
	unsigned_char_list_add_end(&l, '\0');
	c = unsigned_char_list_data(&l);
	while(*c){
		putchar(*c);
		c++;
	}
	unsigned_char_list_destroy(&l);

	backtrack_type_engine(state, 0);
	destroy_empty_scope(state->top_scope);
	enum_type_engine_operation_type_list_destroy(&state->reversible_operations);
	struct_identifier_id_list_destroy(&state->saved_identifier_ids);
	struct_struct_or_union_specifier_id_list_destroy(&state->saved_struct_or_union_specifier_ids);

	enum_c_token_type_list_destroy(&state->simple_type_qualifiers);
	enum_c_token_type_list_destroy(&state->simple_type_specifiers);
	struct_aggregate_specifier_details_list_destroy(&state->aggregate_specifiers);
	enum_c_token_type_list_destroy(&state->storage_class_specifiers);
	struct_specifier_or_qualifier_details_list_destroy(&state->specifier_or_qualifiers);
	struct_specifier_or_qualifier_list_item_details_list_destroy(&state->specifier_or_qualifier_list_items);
	struct_parameter_list_details_list_destroy(&state->parameter_lists);
	struct_declarator_part_list_item_details_list_destroy(&state->declarator_part_list_items);
	struct_declarator_part_details_list_destroy(&state->declarator_parts);
	struct_pointer_part_details_list_destroy(&state->pointer_parts);
	struct_function_part_details_list_destroy(&state->function_parts);
	struct_array_part_details_list_destroy(&state->array_parts);
	struct_bitfield_or_declarator_details_list_destroy(&state->bitfield_or_declarators);
	struct_general_type_details_list_destroy(&state->general_types);
	struct_general_type_list_item_details_list_destroy(&state->general_type_list_items);
	struct_struct_or_union_specifier_details_list_destroy(&state->struct_or_union_specifiers);
	struct_scoped_struct_or_union_specifier_details_list_destroy(&state->scoped_struct_or_union_specifiers);
	struct_union_specifier_details_list_destroy(&state->union_specifiers);
	struct_struct_specifier_details_list_destroy(&state->struct_specifiers);
	struct_c_lexer_token_list_destroy(&state->identifiers);
	struct_parser_node_ptr_list_destroy(&state->constant_expression_parser_nodes);
	struct_scope_level_ptr_list_destroy(&state->scope_levels);
	struct_aggregate_specifier_details_to_struct_aggregate_specifier_id_map_destroy(&state->aggregate_specifiers_map);
	enum_c_token_type_to_struct_simple_specifier_id_map_destroy(&state->simple_type_specifiers_map);
	enum_c_token_type_to_struct_type_qualifier_id_map_destroy(&state->simple_type_qualifiers_map);
	enum_c_token_type_to_struct_storage_class_specifier_id_map_destroy(&state->storage_class_specifiers_map);
	struct_specifier_or_qualifier_details_to_struct_specifier_or_qualifier_id_map_destroy(&state->specifier_or_qualifiers_map);
	struct_specifier_or_qualifier_list_item_details_to_struct_specifier_or_qualifier_list_item_id_map_destroy(&state->specifier_or_qualifier_list_items_map);
	struct_declarator_part_list_item_details_to_struct_declarator_part_list_item_id_map_destroy(&state->declarator_part_list_items_map);
	struct_declarator_part_details_to_struct_declarator_part_id_map_destroy(&state->declarator_parts_map);
	struct_pointer_part_details_to_struct_pointer_part_id_map_destroy(&state->pointer_parts_map);
	struct_function_part_details_to_struct_function_part_id_map_destroy(&state->function_parts_map);
	struct_array_part_details_to_struct_array_part_id_map_destroy(&state->array_parts_map);
	struct_parameter_list_details_to_struct_parameter_list_id_map_destroy(&state->parameter_lists_map);
	struct_bitfield_or_declarator_details_to_struct_bitfield_or_declarator_id_map_destroy(&state->bitfield_or_declarators_map);
	struct_general_type_details_to_struct_general_type_id_map_destroy(&state->general_types_map);
	struct_general_type_list_item_details_to_struct_general_type_list_item_id_map_destroy(&state->general_type_list_items_map);
	struct_struct_or_union_specifier_details_to_struct_struct_or_union_specifier_id_map_destroy(&state->struct_or_union_specifiers_map);
	struct_scoped_struct_or_union_specifier_details_to_struct_scoped_struct_or_union_specifier_id_map_destroy(&state->scoped_struct_or_union_specifiers_map);
	struct_union_specifier_details_to_struct_union_specifier_id_map_destroy(&state->union_specifiers_map);
	struct_struct_specifier_details_to_struct_struct_specifier_id_map_destroy(&state->struct_specifiers_map);
	struct_parser_node_ptr_to_struct_parser_node_id_map_destroy(&state->constant_expression_parser_nodes_map);
	struct_scope_level_ptr_to_struct_scope_level_id_map_destroy(&state->scope_levels_map);
	struct_c_lexer_token_to_struct_identifier_id_map_destroy(&state->identifiers_map);
	unsigned_int_to_void_ptr_map_destroy(&state->function_definitions.general_type_ids_to_function_definitions);
	void_ptr_list_destroy(&state->stored_pointers);
	struct_parser_node_ptr_to_struct_scope_level_id_map_destroy(&state->parser_node_scope_associations);
}

unsigned int type_engine_checkpoint(struct type_engine_state * state){
	/*  Returns the number of operations that have been performed so far */
	return enum_type_engine_operation_type_list_size(&state->reversible_operations);
}

void backtrack_type_engine(struct type_engine_state * state, unsigned int target_number){
	assert(enum_type_engine_operation_type_list_size(&state->reversible_operations) >= target_number);
	while(enum_type_engine_operation_type_list_size(&state->reversible_operations) > target_number){
		enum type_engine_operation_type operation = enum_type_engine_operation_type_list_pop_end(&state->reversible_operations);
		undo_type_engine_operation(state, operation);
	}
}

struct parser_node * associate_parser_node_with_scope_level(struct type_engine_state * state, struct scope_level * scope, struct parser_node * n){
	if(scope){
		struct scope_level_id scope_id = get_type_engine_id_for_scope_level(state, scope);
		struct_parser_node_ptr_to_struct_scope_level_id_map_put(&state->parser_node_scope_associations, n, scope_id);
		enum_type_engine_operation_type_list_add_end(&state->reversible_operations, TYPE_ENGINE_ASSOCIATE_PARSER_NODE_WITH_SCOPE_LEVEL);
		void_ptr_list_add_end(&state->stored_pointers, (void *)n);
	}
	return n;
}

struct scope_level * increment_scope_depth(struct type_engine_state * state, enum scope_level_type t){
	enum_type_engine_operation_type_list_add_end(&state->reversible_operations, TYPE_ENGINE_INCREMENT_SCOPE_DEPTH);
	manage_new_scope(state, state->top_scope, state->current_scope_depth, ADD, t);
	state->current_scope_depth = state->current_scope_depth + 1;
	return get_parser_scope_level(state);
}

void decrement_scope_depth(struct type_engine_state * state){
	enum_type_engine_operation_type_list_add_end(&state->reversible_operations, TYPE_ENGINE_DECREMENT_SCOPE_DEPTH);
	assert(state->current_scope_depth > 0);
	state->current_scope_depth = state->current_scope_depth - 1;
}

void undo_type_engine_operation(struct type_engine_state * state, enum type_engine_operation_type o){
	switch (o){
		case TYPE_ENGINE_INCREMENT_SCOPE_DEPTH:{
			state->current_scope_depth = state->current_scope_depth - 1;
			manage_new_scope(state, state->top_scope, state->current_scope_depth, REMOVE, SCOPE_LEVEL_TYPE_FILE_SCOPE);
			break;
		}case TYPE_ENGINE_DECREMENT_SCOPE_DEPTH:{
			state->current_scope_depth = state->current_scope_depth + 1;
			break;
		}case TYPE_ENGINE_INCREMENT_NEXT_ANONYMOUS_TAG_ID:{
			struct scope_level * scope = get_parser_scope_level(state);
			scope->next_anonymous_tag_id = scope->next_anonymous_tag_id - 1;
			break;
		}case TYPE_ENGINE_ADD_GENERAL_TYPE_LIST_ITEM:{
			struct general_type_list_item_details e = struct_general_type_list_item_details_list_pop_end(&state->general_type_list_items);
			struct_general_type_list_item_details_to_struct_general_type_list_item_id_map_remove(&state->general_type_list_items_map, e);
			break;
		}case TYPE_ENGINE_ADD_ALL_SPECIFIER:{
			struct aggregate_specifier_details e = struct_aggregate_specifier_details_list_pop_end(&state->aggregate_specifiers);
			struct_aggregate_specifier_details_to_struct_aggregate_specifier_id_map_remove(&state->aggregate_specifiers_map, e);
			break;
		}case TYPE_ENGINE_ADD_SIMPLE_SPECIFIER:{
			enum c_token_type e = enum_c_token_type_list_pop_end(&state->simple_type_specifiers);
			enum_c_token_type_to_struct_simple_specifier_id_map_remove(&state->simple_type_specifiers_map, e);
			break;
		}case TYPE_ENGINE_ADD_STORAGE_CLASS_SPECIFIER:{
			enum c_token_type t = enum_c_token_type_list_pop_end(&state->storage_class_specifiers);
			enum_c_token_type_to_struct_storage_class_specifier_id_map_remove(&state->storage_class_specifiers_map, t);
			break;
		}case TYPE_ENGINE_ADD_SIMPLE_QUALIFIER:{
			enum c_token_type e = enum_c_token_type_list_pop_end(&state->simple_type_qualifiers);
			enum_c_token_type_to_struct_type_qualifier_id_map_remove(&state->simple_type_qualifiers_map, e);
			break;
		}case TYPE_ENGINE_ADD_SPECIFIER_OR_QUALIFIER:{
			struct specifier_or_qualifier_details e = struct_specifier_or_qualifier_details_list_pop_end(&state->specifier_or_qualifiers);
			struct_specifier_or_qualifier_details_to_struct_specifier_or_qualifier_id_map_remove(&state->specifier_or_qualifiers_map, e);
			break;
		}case TYPE_ENGINE_ADD_DECLARATOR_PART:{
			struct declarator_part_details  e = struct_declarator_part_details_list_pop_end(&state->declarator_parts);
			struct_declarator_part_details_to_struct_declarator_part_id_map_remove(&state->declarator_parts_map, e);
			break;
		}case TYPE_ENGINE_ADD_POINTER_PART:{
			struct pointer_part_details e = struct_pointer_part_details_list_pop_end(&state->pointer_parts);
			struct_pointer_part_details_to_struct_pointer_part_id_map_remove(&state->pointer_parts_map, e);
			break;
		}case TYPE_ENGINE_ADD_ARRAY_PART:{
			struct array_part_details e = struct_array_part_details_list_pop_end(&state->array_parts);
			struct_array_part_details_to_struct_array_part_id_map_remove(&state->array_parts_map, e);
			break;
		}case TYPE_ENGINE_ADD_FUNCTION_PART:{
			struct function_part_details e = struct_function_part_details_list_pop_end(&state->function_parts);
			struct_function_part_details_to_struct_function_part_id_map_remove(&state->function_parts_map, e);
			break;
		}case TYPE_ENGINE_ADD_PARAMETER_LIST:{
			struct parameter_list_details e = struct_parameter_list_details_list_pop_end(&state->parameter_lists);
			struct_parameter_list_details_to_struct_parameter_list_id_map_remove(&state->parameter_lists_map, e);
			break;
		}case TYPE_ENGINE_ADD_BITFIELD_OR_DECLARATOR:{
			struct bitfield_or_declarator_details e = struct_bitfield_or_declarator_details_list_pop_end(&state->bitfield_or_declarators);
			struct_bitfield_or_declarator_details_to_struct_bitfield_or_declarator_id_map_remove(&state->bitfield_or_declarators_map, e);
			break;
		}case TYPE_ENGINE_ADD_GENERAL_TYPE:{
			struct general_type_details e = struct_general_type_details_list_pop_end(&state->general_types);
			struct_general_type_details_to_struct_general_type_id_map_remove(&state->general_types_map, e);
			break;
		}case TYPE_ENGINE_ADD_SCOPE_LEVEL:{
			struct scope_level * e = struct_scope_level_ptr_list_pop_end(&state->scope_levels);
			struct_scope_level_ptr_to_struct_scope_level_id_map_remove(&state->scope_levels_map, e);
			break;
		}case TYPE_ENGINE_ADD_CONSTANT_PARSER_NODE:{
			struct parser_node * e = struct_parser_node_ptr_list_pop_end(&state->constant_expression_parser_nodes);
			struct_parser_node_ptr_to_struct_parser_node_id_map_remove(&state->constant_expression_parser_nodes_map, e);
			break;
		}case TYPE_ENGINE_ADD_SPECIFIER_QUALIFIER_LIST_ITEM:{
			struct specifier_or_qualifier_list_item_details e = struct_specifier_or_qualifier_list_item_details_list_pop_end(&state->specifier_or_qualifier_list_items);
			struct_specifier_or_qualifier_list_item_details_to_struct_specifier_or_qualifier_list_item_id_map_remove(&state->specifier_or_qualifier_list_items_map, e);
			break;
		}case TYPE_ENGINE_ADD_DECLARATOR_PART_LIST_ITEM:{
			struct declarator_part_list_item_details e = struct_declarator_part_list_item_details_list_pop_end(&state->declarator_part_list_items);
			struct_declarator_part_list_item_details_to_struct_declarator_part_list_item_id_map_remove(&state->declarator_part_list_items_map, e);
			break;
		}case TYPE_ENGINE_ADD_STRUCT_OR_UNION_SPECIFIER:{
			struct struct_or_union_specifier_details e = struct_struct_or_union_specifier_details_list_pop_end(&state->struct_or_union_specifiers);
			struct_struct_or_union_specifier_details_to_struct_struct_or_union_specifier_id_map_remove(&state->struct_or_union_specifiers_map, e);
			break;
		}case TYPE_ENGINE_ADD_UNION_SPECIFIER:{
			struct union_specifier_details e = struct_union_specifier_details_list_pop_end(&state->union_specifiers);
			struct_union_specifier_details_to_struct_union_specifier_id_map_remove(&state->union_specifiers_map, e);
			break;
		}case TYPE_ENGINE_ADD_STRUCT_SPECIFIER:{
			struct struct_specifier_details e = struct_struct_specifier_details_list_pop_end(&state->struct_specifiers);
			struct_struct_specifier_details_to_struct_struct_specifier_id_map_remove(&state->struct_specifiers_map, e);
			break;
		}case TYPE_ENGINE_ADD_SCOPED_STRUCT_OR_UNION_SPECIFIER:{
			struct scoped_struct_or_union_specifier_details e = struct_scoped_struct_or_union_specifier_details_list_pop_end(&state->scoped_struct_or_union_specifiers);
			struct_scoped_struct_or_union_specifier_details_to_struct_scoped_struct_or_union_specifier_id_map_remove(&state->scoped_struct_or_union_specifiers_map, e);
			break;
		}case TYPE_ENGINE_CREATE_DECLARATION_NAMESPACE:{
			struct declaration_namespace * d = (struct declaration_namespace *)void_ptr_list_pop_end(&state->stored_pointers);
			unsigned_int_list_destroy(&d->ordered_type_ids);
			unsigned_int_to_unsigned_int_map_destroy(&d->identifier_ids_to_type_ids);
			unsigned_int_to_unsigned_int_map_destroy(&d->type_ids_to_ordered_index);
			free(d);
			break;
		}case TYPE_ENGINE_ADD_ORDERED_TYPE_TO_DECLARATION_NAMESPACE:{
			struct declaration_namespace * dn = (struct declaration_namespace *)void_ptr_list_pop_end(&state->stored_pointers);
			unsigned int general_id = unsigned_int_list_pop_end(&dn->ordered_type_ids);
			unsigned_int_to_unsigned_int_map_remove(&dn->type_ids_to_ordered_index, general_id);
			break;
		}case TYPE_ENGINE_ADD_IDENTIFIER_ID_TO_DECLARATION_NAMESPACE:{
			struct declaration_namespace * dn = (struct declaration_namespace *)void_ptr_list_pop_end(&state->stored_pointers);
			struct identifier_id identifier_id = struct_identifier_id_list_pop_end(&state->saved_identifier_ids);
			unsigned_int_to_unsigned_int_map_remove(&dn->identifier_ids_to_type_ids, identifier_id.id);
			break;
		}case TYPE_ENGINE_ADD_STRUCT_OR_UNION_DEFINITION:{
			struct scope_level * scope = (struct scope_level*)void_ptr_list_pop_end(&state->stored_pointers);
			struct struct_or_union_specifier_id aggregate_specifiers_id = struct_struct_or_union_specifier_id_list_pop_end(&state->saved_struct_or_union_specifier_ids);
			struct struct_or_union_definition * d;
			d = (struct struct_or_union_definition *)unsigned_int_to_void_ptr_map_get(&scope->struct_or_union_definitions.struct_or_union_specifier_ids_to_struct_or_union_definitions, aggregate_specifiers_id.id);
			unsigned_int_to_void_ptr_map_remove(&scope->struct_or_union_definitions.struct_or_union_specifier_ids_to_struct_or_union_definitions, aggregate_specifiers_id.id);
			free(d);
			break;
		}case TYPE_ENGINE_ADD_NAMED_STRUCT_OR_UNION_DECLARATION:{
			struct scope_level * scope = (struct scope_level*)void_ptr_list_pop_end(&state->stored_pointers);
			struct identifier_id identifier_id = struct_identifier_id_list_pop_end(&state->saved_identifier_ids);
			unsigned int struct_or_union_specifier_id = unsigned_int_list_pop_end(&scope->struct_or_union_declarations.ordered_type_ids);
			unsigned_int_to_unsigned_int_map_remove(&scope->struct_or_union_declarations.identifier_ids_to_type_ids, identifier_id.id);
			unsigned_int_to_unsigned_int_map_remove(&scope->struct_or_union_declarations.type_ids_to_ordered_index, struct_or_union_specifier_id);
			break;
		}case TYPE_ENGINE_ASSOCIATE_PARSER_NODE_WITH_SCOPE_LEVEL:{
			struct parser_node * n = (struct parser_node*)void_ptr_list_pop_end(&state->stored_pointers);
			struct_parser_node_ptr_to_struct_scope_level_id_map_remove(&state->parser_node_scope_associations, n);
			break;
		}default:{
			assert(0 && "Unknown operation.");
			break;
		}
	}
}

struct scope_level * get_parser_scope_level_h(struct scope_level * s, unsigned int levels_to_go){
	unsigned int num_sub_scopes = struct_scope_level_ptr_list_size(&s->scopes);
	if(levels_to_go){
		return get_parser_scope_level_h(struct_scope_level_ptr_list_get(&s->scopes, num_sub_scopes -1), levels_to_go -1);
	}else{
		return s;
	}
}

struct scope_level * get_parser_scope_level(struct type_engine_state * state){
	/* Return a pointer to the lowest scope in the last scope level */
	return get_parser_scope_level_h(state->top_scope, state->current_scope_depth);
}

void manage_new_scope(struct type_engine_state * state, struct scope_level * scope, unsigned int levels_to_go, enum add_or_remove add_or_remove, enum scope_level_type t){
	if(levels_to_go){
		manage_new_scope(state, struct_scope_level_ptr_list_get(&scope->scopes, struct_scope_level_ptr_list_size(&scope->scopes) -1), levels_to_go - 1, add_or_remove, t);
	}else{
		if(add_or_remove == ADD){
			struct_scope_level_ptr_list_add_end(&scope->scopes, create_empty_scope(state, scope, t));
		}else if(add_or_remove == REMOVE){
			destroy_empty_scope(struct_scope_level_ptr_list_pop_end(&scope->scopes));
		}else{
			assert(0);
		}
	}
}

unsigned int consume_next_anonymous_tag_id_in_current_parser_scope(struct type_engine_state * state){
	struct scope_level * scope = get_parser_scope_level(state);
	unsigned int rtn = scope->next_anonymous_tag_id;
	scope->next_anonymous_tag_id = rtn + 1;
	enum_type_engine_operation_type_list_add_end(&state->reversible_operations, TYPE_ENGINE_INCREMENT_NEXT_ANONYMOUS_TAG_ID);
	return rtn;
}

struct scope_level * create_empty_scope(struct type_engine_state * state, struct scope_level * parent, enum scope_level_type t){
	struct scope_level * s = (struct scope_level *)malloc(sizeof(struct scope_level));
	(void)state;
	s->type = t;
	s->current_function = (struct namespace_object *)0;
	s->first_local_object = (struct namespace_object *)0;
	s->parent_scope = parent;
	struct_scope_level_ptr_list_create(&s->scopes);
	unsigned_char_ptr_to_struct_namespace_object_ptr_map_create(&s->tag_namespace, struct_unsigned_char_ptr_to_struct_namespace_object_ptr_key_value_pair_compare);
	unsigned_char_ptr_to_struct_namespace_object_ptr_map_create(&s->label_namespace, struct_unsigned_char_ptr_to_struct_namespace_object_ptr_key_value_pair_compare);
	unsigned_char_ptr_to_struct_namespace_object_ptr_map_create(&s->identifier_namespace, struct_unsigned_char_ptr_to_struct_namespace_object_ptr_key_value_pair_compare);
	unsigned_char_ptr_list_create(&s->evaluate_labels);
	unsigned_char_ptr_list_create(&s->end_labels);

	unsigned_int_list_create(&s->enum_declarations.ordered_type_ids);
	unsigned_int_to_unsigned_int_map_create(&s->enum_declarations.identifier_ids_to_type_ids, struct_unsigned_int_to_unsigned_int_key_value_pair_compare);
	unsigned_int_to_unsigned_int_map_create(&s->enum_declarations.type_ids_to_ordered_index, struct_unsigned_int_to_unsigned_int_key_value_pair_compare);

	unsigned_int_list_create(&s->struct_or_union_declarations.ordered_type_ids);
	unsigned_int_to_unsigned_int_map_create(&s->struct_or_union_declarations.identifier_ids_to_type_ids, struct_unsigned_int_to_unsigned_int_key_value_pair_compare);
	unsigned_int_to_unsigned_int_map_create(&s->struct_or_union_declarations.type_ids_to_ordered_index, struct_unsigned_int_to_unsigned_int_key_value_pair_compare);

	unsigned_int_list_create(&s->identifier_declarations.ordered_type_ids);
	unsigned_int_to_unsigned_int_map_create(&s->identifier_declarations.identifier_ids_to_type_ids, struct_unsigned_int_to_unsigned_int_key_value_pair_compare);
	unsigned_int_to_unsigned_int_map_create(&s->identifier_declarations.type_ids_to_ordered_index, struct_unsigned_int_to_unsigned_int_key_value_pair_compare);

	unsigned_int_to_void_ptr_map_create(&s->enum_definitions.general_type_ids_to_enum_definitions, struct_unsigned_int_to_void_ptr_key_value_pair_compare);
	unsigned_int_to_void_ptr_map_create(&s->struct_or_union_definitions.struct_or_union_specifier_ids_to_struct_or_union_definitions, struct_unsigned_int_to_void_ptr_key_value_pair_compare);
	s->next_anonymous_tag_id = 0;
	return s;
}

void destroy_empty_scope(struct scope_level * s){
	unsigned_char_ptr_list_destroy(&s->evaluate_labels);
	unsigned_char_ptr_list_destroy(&s->end_labels);
	unsigned_char_ptr_to_struct_namespace_object_ptr_map_destroy(&s->tag_namespace);
	unsigned_char_ptr_to_struct_namespace_object_ptr_map_destroy(&s->label_namespace);
	unsigned_char_ptr_to_struct_namespace_object_ptr_map_destroy(&s->identifier_namespace);

	unsigned_int_list_destroy(&s->enum_declarations.ordered_type_ids);
	unsigned_int_to_unsigned_int_map_destroy(&s->enum_declarations.identifier_ids_to_type_ids);
	unsigned_int_to_unsigned_int_map_destroy(&s->enum_declarations.type_ids_to_ordered_index);

	unsigned_int_list_destroy(&s->struct_or_union_declarations.ordered_type_ids);
	unsigned_int_to_unsigned_int_map_destroy(&s->struct_or_union_declarations.identifier_ids_to_type_ids);
	unsigned_int_to_unsigned_int_map_destroy(&s->struct_or_union_declarations.type_ids_to_ordered_index);

	unsigned_int_list_destroy(&s->identifier_declarations.ordered_type_ids);
	unsigned_int_to_unsigned_int_map_destroy(&s->identifier_declarations.identifier_ids_to_type_ids);
	unsigned_int_to_unsigned_int_map_destroy(&s->identifier_declarations.type_ids_to_ordered_index);

	unsigned_int_to_void_ptr_map_destroy(&s->enum_definitions.general_type_ids_to_enum_definitions);
	unsigned_int_to_void_ptr_map_destroy(&s->struct_or_union_definitions.struct_or_union_specifier_ids_to_struct_or_union_definitions);
	struct_scope_level_ptr_list_destroy(&s->scopes);
	free(s);
}

struct aggregate_specifier_id aggregate_specifier_from_simple_specifier_id(struct type_engine_state * state, struct simple_specifier_id s){
	return aggregate_specifier_from_any_specifier_id(state, TYPE_ENGINE_SIMPLE_SPECIFIER, s.id);
}

struct aggregate_specifier_id aggregate_specifier_from_typename_specifier_id(struct type_engine_state * state, unsigned int s){
	return aggregate_specifier_from_any_specifier_id(state, TYPE_ENGINE_TYPENAME_SPECIFIER, s);
}

struct aggregate_specifier_id aggregate_specifier_from_enum_specifier_id(struct type_engine_state * state, unsigned int s){
	return aggregate_specifier_from_any_specifier_id(state, TYPE_ENGINE_ENUM_SPECIFIER, s);
}

struct aggregate_specifier_id aggregate_specifier_from_scoped_struct_or_union_specifier_id(struct type_engine_state * state, struct scoped_struct_or_union_specifier_id s){
	return aggregate_specifier_from_any_specifier_id(state, TYPE_ENGINE_STRUCT_OR_UNION_SPECIFIER, s.id);
}

struct aggregate_specifier_id aggregate_specifier_from_any_specifier_id(struct type_engine_state * state, enum type_engine_aggregate_specifier_kind type, unsigned int s){
	struct aggregate_specifier_details details;
	details.type = type;
	details.id = s;
	if(struct_aggregate_specifier_details_to_struct_aggregate_specifier_id_map_exists(&state->aggregate_specifiers_map, details)){
		return struct_aggregate_specifier_details_to_struct_aggregate_specifier_id_map_get(&state->aggregate_specifiers_map, details);
	}else{
		struct aggregate_specifier_id new_id;
		new_id.id = struct_aggregate_specifier_details_list_size(&state->aggregate_specifiers);
		struct_aggregate_specifier_details_to_struct_aggregate_specifier_id_map_put(&state->aggregate_specifiers_map, details, new_id);
		struct_aggregate_specifier_details_list_add_end(&state->aggregate_specifiers, details);
		enum_type_engine_operation_type_list_add_end(&state->reversible_operations, TYPE_ENGINE_ADD_ALL_SPECIFIER);
		return new_id;
	}
}

struct simple_specifier_id get_type_engine_id_for_simple_specifier(struct type_engine_state * state, enum c_token_type t){
	if(enum_c_token_type_to_struct_simple_specifier_id_map_exists(&state->simple_type_specifiers_map, t)){
		return enum_c_token_type_to_struct_simple_specifier_id_map_get(&state->simple_type_specifiers_map, t);
	}else{
		struct simple_specifier_id new_id;
		new_id.id = enum_c_token_type_list_size(&state->simple_type_specifiers);
		enum_c_token_type_to_struct_simple_specifier_id_map_put(&state->simple_type_specifiers_map, t, new_id);
		enum_c_token_type_list_add_end(&state->simple_type_specifiers, t);
		enum_type_engine_operation_type_list_add_end(&state->reversible_operations, TYPE_ENGINE_ADD_SIMPLE_SPECIFIER);
		return new_id;
	}
}

struct storage_class_specifier_id get_type_engine_id_for_storage_class_specifier(struct type_engine_state * state, enum c_token_type t){
	if(enum_c_token_type_to_struct_storage_class_specifier_id_map_exists(&state->storage_class_specifiers_map, t)){
		return enum_c_token_type_to_struct_storage_class_specifier_id_map_get(&state->storage_class_specifiers_map, t);
	}else{
		struct storage_class_specifier_id new_id;
		new_id.id = enum_c_token_type_list_size(&state->storage_class_specifiers);
		enum_c_token_type_to_struct_storage_class_specifier_id_map_put(&state->storage_class_specifiers_map, t, new_id);
		enum_c_token_type_list_add_end(&state->storage_class_specifiers, t);
		enum_type_engine_operation_type_list_add_end(&state->reversible_operations, TYPE_ENGINE_ADD_STORAGE_CLASS_SPECIFIER);
		return new_id;
	}
}

struct type_qualifier_id get_type_engine_id_for_simple_qualifier(struct type_engine_state * state, enum c_token_type t){
	if(enum_c_token_type_to_struct_type_qualifier_id_map_exists(&state->simple_type_qualifiers_map, t)){
		return enum_c_token_type_to_struct_type_qualifier_id_map_get(&state->simple_type_qualifiers_map, t);
	}else{
		struct type_qualifier_id new_id;
		new_id.id = enum_c_token_type_list_size(&state->simple_type_qualifiers);
		enum_c_token_type_to_struct_type_qualifier_id_map_put(&state->simple_type_qualifiers_map, t, new_id);
		enum_c_token_type_list_add_end(&state->simple_type_qualifiers, t);
		enum_type_engine_operation_type_list_add_end(&state->reversible_operations, TYPE_ENGINE_ADD_SIMPLE_QUALIFIER);
		return new_id;
	}
}

struct specifier_or_qualifier_id specifier_or_qualifier_from_aggregate_specifier_id(struct type_engine_state * state, struct aggregate_specifier_id s){
	return get_type_engine_id_for_any_specifier_or_qualifier(state, TYPE_ENGINE_TYPE_SPECIFIER, s.id);
}

struct specifier_or_qualifier_id specifier_or_qualifier_from_type_qualifier_id(struct type_engine_state * state, struct type_qualifier_id s){
	return get_type_engine_id_for_any_specifier_or_qualifier(state, TYPE_ENGINE_SIMPLE_QUALIFIER, s.id);
}

struct specifier_or_qualifier_id specifier_or_qualifier_from_storage_class_specifier_id(struct type_engine_state * state, struct storage_class_specifier_id s){
	return get_type_engine_id_for_any_specifier_or_qualifier(state, TYPE_ENGINE_STORAGE_CLASS_SPECIFIER, s.id);
}

struct specifier_or_qualifier_id get_type_engine_id_for_any_specifier_or_qualifier(struct type_engine_state * state, enum type_engine_specifier_or_qualifier_kind type, unsigned int index){
	struct specifier_or_qualifier_details details;
	details.type = type;
	details.id = index;
	if(struct_specifier_or_qualifier_details_to_struct_specifier_or_qualifier_id_map_exists(&state->specifier_or_qualifiers_map, details)){
		return struct_specifier_or_qualifier_details_to_struct_specifier_or_qualifier_id_map_get(&state->specifier_or_qualifiers_map, details);
	}else{
		struct specifier_or_qualifier_id new_id;
		new_id.id = struct_specifier_or_qualifier_details_list_size(&state->specifier_or_qualifiers);
		struct_specifier_or_qualifier_details_to_struct_specifier_or_qualifier_id_map_put(&state->specifier_or_qualifiers_map, details, new_id);
		struct_specifier_or_qualifier_details_list_add_end(&state->specifier_or_qualifiers, details);
		enum_type_engine_operation_type_list_add_end(&state->reversible_operations, TYPE_ENGINE_ADD_SPECIFIER_OR_QUALIFIER);
		return new_id;
	}
}

struct declarator_part_id declarator_part_from_array_part_id(struct type_engine_state * state, struct array_part_id i){
	return get_type_engine_id_for_declarator_part(state, TYPE_ENGINE_ARRAY_DECLARATOR_PART, i.id);
}

struct declarator_part_id declarator_part_from_function_part_id(struct type_engine_state * state, struct function_part_id i){
	return get_type_engine_id_for_declarator_part(state, TYPE_ENGINE_FUNCTION_DECLARATOR_PART, i.id);
}

struct declarator_part_id declarator_part_from_pointer_part_id(struct type_engine_state * state, struct pointer_part_id i){
	return get_type_engine_id_for_declarator_part(state, TYPE_ENGINE_POINTER_DECLARATOR_PART, i.id);
}

struct declarator_part_id get_type_engine_id_for_declarator_part(struct type_engine_state * state, enum type_engine_declarator_part_kind type, unsigned int index){
	struct declarator_part_details details;
	details.type = type;
	details.id = index;
	if(struct_declarator_part_details_to_struct_declarator_part_id_map_exists(&state->declarator_parts_map, details)){
		return struct_declarator_part_details_to_struct_declarator_part_id_map_get(&state->declarator_parts_map, details);
	}else{
		struct declarator_part_id new_id;
		new_id.id = struct_declarator_part_details_list_size(&state->declarator_parts);
		struct_declarator_part_details_to_struct_declarator_part_id_map_put(&state->declarator_parts_map, details, new_id);
		struct_declarator_part_details_list_add_end(&state->declarator_parts, details);
		enum_type_engine_operation_type_list_add_end(&state->reversible_operations, TYPE_ENGINE_ADD_DECLARATOR_PART);
		return new_id;
	}
}

struct pointer_part_id get_type_engine_id_for_pointer_part(struct type_engine_state * state, enum type_engine_pointer_part_kind type, struct specifier_or_qualifier_list_item_id index){
	struct pointer_part_details details;
	details.id = index.id;
	details.type = type;
	if(struct_pointer_part_details_to_struct_pointer_part_id_map_exists(&state->pointer_parts_map, details)){
		return struct_pointer_part_details_to_struct_pointer_part_id_map_get(&state->pointer_parts_map, details);
	}else{
		struct pointer_part_id new_id;
		new_id.id = struct_pointer_part_details_list_size(&state->pointer_parts);
		struct_pointer_part_details_to_struct_pointer_part_id_map_put(&state->pointer_parts_map, details, new_id);
		struct_pointer_part_details_list_add_end(&state->pointer_parts, details);
		enum_type_engine_operation_type_list_add_end(&state->reversible_operations, TYPE_ENGINE_ADD_POINTER_PART);
		return new_id;
	}
}


struct array_part_id get_type_engine_id_for_constant_expression_array_part(struct type_engine_state * state, unsigned int index){
	return get_type_engine_id_for_array_part(state, TYPE_ENGINE_CONSTANT_EXPRESSION_ARRAY, index);
}

struct array_part_id get_type_engine_id_for_flexible_array_part(struct type_engine_state * state, unsigned int index){
	return get_type_engine_id_for_array_part(state, TYPE_ENGINE_FLEXIBLE_ARRAY, index);
}

struct array_part_id get_type_engine_id_for_array_part(struct type_engine_state * state, enum type_engine_array_part_kind type, unsigned int index){
	struct array_part_details details;
	details.type = type;
	details.id = index;
	if(struct_array_part_details_to_struct_array_part_id_map_exists(&state->array_parts_map, details)){
		return struct_array_part_details_to_struct_array_part_id_map_get(&state->array_parts_map, details);
	}else{
		struct array_part_id new_id;
		new_id.id = struct_array_part_details_list_size(&state->array_parts);
		struct_array_part_details_to_struct_array_part_id_map_put(&state->array_parts_map, details, new_id);
		struct_array_part_details_list_add_end(&state->array_parts, details);
		enum_type_engine_operation_type_list_add_end(&state->reversible_operations, TYPE_ENGINE_ADD_ARRAY_PART);
		return new_id;
	}
}

struct function_part_id get_type_engine_id_for_prototype_function_part(struct type_engine_state * state, unsigned int index){
	return get_type_engine_id_for_function_part(state, TYPE_ENGINE_FUNCTION_PROTOTYPE, index);
}

struct function_part_id get_type_engine_id_for_k_r_function_part(struct type_engine_state * state, unsigned int index){
	return get_type_engine_id_for_function_part(state, TYPE_ENGINE_FUNCTION_K_AND_R_C, index);
}

struct function_part_id get_type_engine_id_for_function_part(struct type_engine_state * state, enum type_engine_function_part_kind type, unsigned int index){
	struct function_part_details details;
	details.id = index;
	details.type = type;
	if(struct_function_part_details_to_struct_function_part_id_map_exists(&state->function_parts_map, details)){
		return struct_function_part_details_to_struct_function_part_id_map_get(&state->function_parts_map, details);
	}else{
		struct function_part_id new_id;
		new_id.id = struct_function_part_details_list_size(&state->function_parts);
		struct_function_part_details_to_struct_function_part_id_map_put(&state->function_parts_map, details, new_id);
		struct_function_part_details_list_add_end(&state->function_parts, details);
		enum_type_engine_operation_type_list_add_end(&state->reversible_operations, TYPE_ENGINE_ADD_FUNCTION_PART);
		return new_id;
	}
}

struct parameter_list_id get_variadic_parameter_list_id(struct type_engine_state * state, struct general_type_list_item_id index){
	return get_type_engine_id_for_parameter_list(state, TYPE_ENGINE_PARAMETER_LIST_VARIADIC, index.id);
}

struct parameter_list_id get_non_variadic_parameter_list_id(struct type_engine_state * state, struct general_type_list_item_id index){
	return get_type_engine_id_for_parameter_list(state, TYPE_ENGINE_PARAMETER_LIST_NON_VARIADIC, index.id);
}

struct parameter_list_id get_type_engine_id_for_parameter_list(struct type_engine_state * state, enum type_engine_parameter_list_kind type, unsigned int index){
	struct parameter_list_details details;
	details.type = type;
	details.id = index;
	if(struct_parameter_list_details_to_struct_parameter_list_id_map_exists(&state->parameter_lists_map, details)){
		return struct_parameter_list_details_to_struct_parameter_list_id_map_get(&state->parameter_lists_map, details);
	}else{
		struct parameter_list_id new_id;
		new_id.id = struct_parameter_list_details_list_size(&state->parameter_lists);
		struct_parameter_list_details_to_struct_parameter_list_id_map_put(&state->parameter_lists_map, details, new_id);
		struct_parameter_list_details_list_add_end(&state->parameter_lists, details);
		enum_type_engine_operation_type_list_add_end(&state->reversible_operations, TYPE_ENGINE_ADD_PARAMETER_LIST);
		return new_id;
	}
}

struct bitfield_or_declarator_id bitfield_or_declarator_from_only_bitfield(struct type_engine_state * state, unsigned int index){
	return get_type_engine_id_for_bitfield_or_declarator(state, TYPE_ENGINE_ONLY_BITFIELD, index);
}

struct bitfield_or_declarator_id bitfield_or_declarator_from_only_declarator(struct type_engine_state * state, struct declarator_part_list_item_id index){
	return get_type_engine_id_for_bitfield_or_declarator(state, TYPE_ENGINE_ONLY_DECLARATOR, index.id);
}

struct bitfield_or_declarator_id bitfield_or_declarator_from_bitfield_and_declarator(struct type_engine_state * state, unsigned int index){
	return get_type_engine_id_for_bitfield_or_declarator(state, TYPE_ENGINE_BITFIELD_AND_DECLARATOR, index);
}

struct bitfield_or_declarator_id get_type_engine_id_for_bitfield_or_declarator(struct type_engine_state * state, enum type_engine_bitfield_or_declarator_kind type, unsigned int index){
	struct bitfield_or_declarator_details details;
	details.type = type;
	details.id = index;
	if(struct_bitfield_or_declarator_details_to_struct_bitfield_or_declarator_id_map_exists(&state->bitfield_or_declarators_map, details)){
		return struct_bitfield_or_declarator_details_to_struct_bitfield_or_declarator_id_map_get(&state->bitfield_or_declarators_map, details);
	}else{
		struct bitfield_or_declarator_id new_id;
		new_id.id = struct_bitfield_or_declarator_details_list_size(&state->bitfield_or_declarators);
		struct_bitfield_or_declarator_details_to_struct_bitfield_or_declarator_id_map_put(&state->bitfield_or_declarators_map, details, new_id);
		struct_bitfield_or_declarator_details_list_add_end(&state->bitfield_or_declarators, details);
		enum_type_engine_operation_type_list_add_end(&state->reversible_operations, TYPE_ENGINE_ADD_BITFIELD_OR_DECLARATOR);
		return new_id;
	}
}

struct scoped_struct_or_union_specifier_id get_type_engine_id_for_scoped_struct_or_union_specifier(struct type_engine_state * state, struct scope_level_id scope_id, struct struct_or_union_specifier_id struct_or_union_specifier_id){
	struct scoped_struct_or_union_specifier_details details;
	details.scope_id = scope_id;
	details.struct_or_union_specifier_id = struct_or_union_specifier_id;
	if(struct_scoped_struct_or_union_specifier_details_to_struct_scoped_struct_or_union_specifier_id_map_exists(&state->scoped_struct_or_union_specifiers_map, details)){
		return struct_scoped_struct_or_union_specifier_details_to_struct_scoped_struct_or_union_specifier_id_map_get(&state->scoped_struct_or_union_specifiers_map, details);
	}else{
		struct scoped_struct_or_union_specifier_id new_id;
		new_id.id = struct_scoped_struct_or_union_specifier_details_list_size(&state->scoped_struct_or_union_specifiers);
		struct_scoped_struct_or_union_specifier_details_to_struct_scoped_struct_or_union_specifier_id_map_put(&state->scoped_struct_or_union_specifiers_map, details, new_id);
		struct_scoped_struct_or_union_specifier_details_list_add_end(&state->scoped_struct_or_union_specifiers, details);
		enum_type_engine_operation_type_list_add_end(&state->reversible_operations, TYPE_ENGINE_ADD_SCOPED_STRUCT_OR_UNION_SPECIFIER);
		return new_id;
	}
}

struct struct_or_union_specifier_id get_struct_specifier_id(struct type_engine_state * state, struct struct_specifier_id index){
	return get_type_engine_id_for_struct_or_union_specifier(state, TYPE_ENGINE_STRUCT_SPECIFIER, index.id);
}

struct struct_or_union_specifier_id get_union_specifier_id(struct type_engine_state * state, struct union_specifier_id index){
	return get_type_engine_id_for_struct_or_union_specifier(state, TYPE_ENGINE_UNION_SPECIFIER, index.id);
}

struct struct_or_union_specifier_id get_type_engine_id_for_struct_or_union_specifier(struct type_engine_state * state, enum type_engine_struct_or_union_specifier_kind type, unsigned int index){
	struct struct_or_union_specifier_details details;
	details.type = type;
	details.id = index;
	if(struct_struct_or_union_specifier_details_to_struct_struct_or_union_specifier_id_map_exists(&state->struct_or_union_specifiers_map, details)){
		return struct_struct_or_union_specifier_details_to_struct_struct_or_union_specifier_id_map_get(&state->struct_or_union_specifiers_map, details);
	}else{
		struct struct_or_union_specifier_id new_id;
		new_id.id = struct_struct_or_union_specifier_details_list_size(&state->struct_or_union_specifiers);
		struct_struct_or_union_specifier_details_to_struct_struct_or_union_specifier_id_map_put(&state->struct_or_union_specifiers_map, details, new_id);
		struct_struct_or_union_specifier_details_list_add_end(&state->struct_or_union_specifiers, details);
		enum_type_engine_operation_type_list_add_end(&state->reversible_operations, TYPE_ENGINE_ADD_STRUCT_OR_UNION_SPECIFIER);
		return new_id;
	}
}


struct union_specifier_id get_named_union_specifier_id(struct type_engine_state * state, struct identifier_id index){
	return get_aggregate_union_specifier_id(state, TYPE_ENGINE_NAMED_UNION, index.id);
}

struct union_specifier_id get_anonymous_union_specifier_id(struct type_engine_state * state, unsigned int index){
	return get_aggregate_union_specifier_id(state, TYPE_ENGINE_ANONYMOUS_UNION, index);
}

struct union_specifier_id get_aggregate_union_specifier_id(struct type_engine_state * state, enum type_engine_union_specifier_kind type, unsigned int index){
	struct union_specifier_details details;
	details.type = type;
	details.id = index;
	if(struct_union_specifier_details_to_struct_union_specifier_id_map_exists(&state->union_specifiers_map, details)){
		return struct_union_specifier_details_to_struct_union_specifier_id_map_get(&state->union_specifiers_map, details);
	}else{
		struct union_specifier_id new_id;
		new_id.id = struct_union_specifier_details_list_size(&state->union_specifiers);
		struct_union_specifier_details_to_struct_union_specifier_id_map_put(&state->union_specifiers_map, details, new_id);
		struct_union_specifier_details_list_add_end(&state->union_specifiers, details);
		enum_type_engine_operation_type_list_add_end(&state->reversible_operations, TYPE_ENGINE_ADD_UNION_SPECIFIER);
		return new_id;
	}
}

struct struct_specifier_id get_named_struct_specifier_id(struct type_engine_state * state, struct identifier_id index){
	return get_aggregate_struct_specifier_id(state, TYPE_ENGINE_NAMED_STRUCT, index.id);
}

struct struct_specifier_id get_anonymous_struct_specifier_id(struct type_engine_state * state, unsigned int index){
	return get_aggregate_struct_specifier_id(state, TYPE_ENGINE_ANONYMOUS_STRUCT, index);
}

struct struct_specifier_id get_aggregate_struct_specifier_id(struct type_engine_state * state, enum type_engine_struct_specifier_kind type, unsigned int index){
	struct struct_specifier_details details;
	details.type = type;
	details.id = index;
	if(struct_struct_specifier_details_to_struct_struct_specifier_id_map_exists(&state->struct_specifiers_map, details)){
		return struct_struct_specifier_details_to_struct_struct_specifier_id_map_get(&state->struct_specifiers_map, details);
	}else{
		struct struct_specifier_id new_id;
		new_id.id = struct_struct_specifier_details_list_size(&state->struct_specifiers);
		struct_struct_specifier_details_to_struct_struct_specifier_id_map_put(&state->struct_specifiers_map, details, new_id);
		struct_struct_specifier_details_list_add_end(&state->struct_specifiers, details);
		enum_type_engine_operation_type_list_add_end(&state->reversible_operations, TYPE_ENGINE_ADD_STRUCT_SPECIFIER);
		return new_id;
	}
}

struct general_type_id get_type_engine_id_for_general_type(struct type_engine_state * state, struct specifier_or_qualifier_list_item_id specifier_or_qualifier_list_id, struct bitfield_or_declarator_id bitfield_or_declarator_id){
	struct general_type_details details;
	details.specifier_or_qualifier_list_item_id = specifier_or_qualifier_list_id;
	details.bitfield_or_declarator_id = bitfield_or_declarator_id;
	if(struct_general_type_details_to_struct_general_type_id_map_exists(&state->general_types_map, details)){
		return struct_general_type_details_to_struct_general_type_id_map_get(&state->general_types_map, details);
	}else{
		struct general_type_id new_id;
		new_id.id = struct_general_type_details_list_size(&state->general_types);
		struct_general_type_details_to_struct_general_type_id_map_put(&state->general_types_map, details, new_id);
		struct_general_type_details_list_add_end(&state->general_types, details);
		enum_type_engine_operation_type_list_add_end(&state->reversible_operations, TYPE_ENGINE_ADD_GENERAL_TYPE);
		return new_id;
	}
}

struct scope_level_id get_type_engine_id_for_scope_level(struct type_engine_state * state, struct scope_level * p){
	if(struct_scope_level_ptr_to_struct_scope_level_id_map_exists(&state->scope_levels_map, p)){
		return struct_scope_level_ptr_to_struct_scope_level_id_map_get(&state->scope_levels_map, p);
	}else{
		struct scope_level_id new_id;
		new_id.id = struct_scope_level_ptr_list_size(&state->scope_levels);
		struct_scope_level_ptr_to_struct_scope_level_id_map_put(&state->scope_levels_map, p, new_id);
		struct_scope_level_ptr_list_add_end(&state->scope_levels, p);
		enum_type_engine_operation_type_list_add_end(&state->reversible_operations, TYPE_ENGINE_ADD_SCOPE_LEVEL);
		return new_id;
	}
}

struct parser_node_id get_type_engine_id_for_parser_node(struct type_engine_state * state, struct parser_node * p){
	if(struct_parser_node_ptr_to_struct_parser_node_id_map_exists(&state->constant_expression_parser_nodes_map, p)){
		return struct_parser_node_ptr_to_struct_parser_node_id_map_get(&state->constant_expression_parser_nodes_map, p);
	}else{
		struct parser_node_id new_id;
		new_id.id = struct_parser_node_ptr_list_size(&state->constant_expression_parser_nodes);
		struct_parser_node_ptr_to_struct_parser_node_id_map_put(&state->constant_expression_parser_nodes_map, p, new_id);
		struct_parser_node_ptr_list_add_end(&state->constant_expression_parser_nodes, p);
		enum_type_engine_operation_type_list_add_end(&state->reversible_operations, TYPE_ENGINE_ADD_CONSTANT_PARSER_NODE);
		return new_id;
	}
}

struct identifier_id get_type_engine_id_for_identifier(struct type_engine_state * state, struct c_lexer_token * t){
	if(struct_c_lexer_token_to_struct_identifier_id_map_exists(&state->identifiers_map, *t)){
		return struct_c_lexer_token_to_struct_identifier_id_map_get(&state->identifiers_map, *t);
	}else{
		/*  This one should never need to backtrack, because the identifier will just be used again somewhere else if we remove it.*/
		struct identifier_id id;
		id.id = struct_c_lexer_token_list_size(&state->identifiers);
		struct_c_lexer_token_to_struct_identifier_id_map_put(&state->identifiers_map, *t, id);
		struct_c_lexer_token_list_add_end(&state->identifiers, *t);
		return id;
	}
}

struct specifier_or_qualifier_list_item_id make_specifier_or_qualifier_list_item(struct type_engine_state * state, struct specifier_or_qualifier_list_item_id a, struct specifier_or_qualifier_id b){
	return get_type_engine_specifier_qualifier_list_id(state, TYPE_ENGINE_SPECIFIER_OR_QUALIFIER_LIST_ITEM_SPECIFIER_OR_QUALIFIER, a, b);
}

struct specifier_or_qualifier_list_item_id specifier_or_qualifier_list_begin(struct type_engine_state * state){
	struct specifier_or_qualifier_list_item_id a;
	struct specifier_or_qualifier_id b;
	a.id = 0;
	b.id = 0;
	return get_type_engine_specifier_qualifier_list_id(state, TYPE_ENGINE_SPECIFIER_OR_QUALIFIER_LIST_ITEM_LIST_START, a, b);
}

struct specifier_or_qualifier_list_item_id get_type_engine_specifier_qualifier_list_id(struct type_engine_state * state, enum type_engine_specifier_or_qualifier_list_item_kind is_start, struct specifier_or_qualifier_list_item_id prev_item_index, struct specifier_or_qualifier_id current_item_index){
	struct specifier_or_qualifier_list_item_details details;
	details.type = is_start;
	details.item_id = current_item_index;
	details.prev_id = prev_item_index;

	if(struct_specifier_or_qualifier_list_item_details_to_struct_specifier_or_qualifier_list_item_id_map_exists(&state->specifier_or_qualifier_list_items_map, details)){
		return struct_specifier_or_qualifier_list_item_details_to_struct_specifier_or_qualifier_list_item_id_map_get(&state->specifier_or_qualifier_list_items_map, details);
	}else{
		struct specifier_or_qualifier_list_item_id new_id;
		new_id.id = struct_specifier_or_qualifier_list_item_details_list_size(&state->specifier_or_qualifier_list_items);
		struct_specifier_or_qualifier_list_item_details_to_struct_specifier_or_qualifier_list_item_id_map_put(&state->specifier_or_qualifier_list_items_map, details, new_id);
		struct_specifier_or_qualifier_list_item_details_list_add_end(&state->specifier_or_qualifier_list_items, details);
		enum_type_engine_operation_type_list_add_end(&state->reversible_operations, TYPE_ENGINE_ADD_SPECIFIER_QUALIFIER_LIST_ITEM);
		return new_id;
	}
}

struct declarator_part_list_item_id make_declarator_part_list_item(struct type_engine_state * state, struct declarator_part_list_item_id a, struct declarator_part_id b){
	return get_type_engine_declarator_part_list_id(state, TYPE_ENGINE_DECLARATOR_PART_LIST_ITEM_DECLARATOR_PART, a, b);
}

struct declarator_part_list_item_id declarator_part_list_begin(struct type_engine_state * state){
	struct declarator_part_list_item_id a;
	struct declarator_part_id b;
	a.id = 0;
	b.id = 0;
	return get_type_engine_declarator_part_list_id(state, TYPE_ENGINE_DECLARATOR_PART_LIST_ITEM_LIST_START, a, b);
}

struct declarator_part_list_item_id get_type_engine_declarator_part_list_id(struct type_engine_state * state, enum type_engine_declarator_part_list_item_kind is_start, struct declarator_part_list_item_id prev_item_index, struct declarator_part_id current_item_index){
	struct declarator_part_list_item_details details;
	details.type = is_start;
	details.item_id = current_item_index;
	details.prev_id = prev_item_index;

	if(struct_declarator_part_list_item_details_to_struct_declarator_part_list_item_id_map_exists(&state->declarator_part_list_items_map, details)){
		return struct_declarator_part_list_item_details_to_struct_declarator_part_list_item_id_map_get(&state->declarator_part_list_items_map, details);
	}else{
		struct declarator_part_list_item_id new_id;
		new_id.id = struct_declarator_part_list_item_details_list_size(&state->declarator_part_list_items);
		struct_declarator_part_list_item_details_to_struct_declarator_part_list_item_id_map_put(&state->declarator_part_list_items_map, details, new_id);
		struct_declarator_part_list_item_details_list_add_end(&state->declarator_part_list_items, details);
		enum_type_engine_operation_type_list_add_end(&state->reversible_operations, TYPE_ENGINE_ADD_DECLARATOR_PART_LIST_ITEM);
		return new_id;
	}
}

struct general_type_list_item_id make_general_type_list_item(struct type_engine_state * state, struct general_type_list_item_id prev, struct general_type_id item){
	return get_type_engine_general_type_list_item_id(state, TYPE_ENGINE_GENERAL_TYPE_LIST_ITEM_GENERAL_TYPE, prev, item);
}

struct general_type_list_item_id get_general_type_list_begin(struct type_engine_state * state){
	struct general_type_list_item_id a;
	struct general_type_id b;
	a.id = 0;
	b.id = 0;
	return get_type_engine_general_type_list_item_id(state, TYPE_ENGINE_GENERAL_TYPE_LIST_ITEM_LIST_START, a, b);
}

struct general_type_list_item_id get_type_engine_general_type_list_item_id(struct type_engine_state * state, enum type_engine_general_type_list_item_kind is_start, struct general_type_list_item_id prev_item_index, struct general_type_id current_item_index){
	struct general_type_list_item_details details;
	details.type = is_start;
	details.prev_id = prev_item_index;
	details.item_id = current_item_index;
	if(struct_general_type_list_item_details_to_struct_general_type_list_item_id_map_exists(&state->general_type_list_items_map, details)){
		return struct_general_type_list_item_details_to_struct_general_type_list_item_id_map_get(&state->general_type_list_items_map, details);
	}else{
		struct general_type_list_item_id new_id;
		new_id.id = struct_general_type_list_item_details_list_size(&state->general_type_list_items);
		struct_general_type_list_item_details_to_struct_general_type_list_item_id_map_put(&state->general_type_list_items_map, details, new_id);
		struct_general_type_list_item_details_list_add_end(&state->general_type_list_items, details);
		enum_type_engine_operation_type_list_add_end(&state->reversible_operations, TYPE_ENGINE_ADD_GENERAL_TYPE_LIST_ITEM);
		return new_id;
	}
}

void add_declarators_to_list(struct type_engine_state * state, struct declarator_part_list_item_id * most_recent_list_id, struct declarator_part_list_item_id current_declarator_id, unsigned int reverse){
	/*  Add the declarators onto the end of the list */
	struct declarator_part_list_item_details details = struct_declarator_part_list_item_details_list_get(&state->declarator_part_list_items, current_declarator_id.id);
	if(details.type == TYPE_ENGINE_DECLARATOR_PART_LIST_ITEM_LIST_START){
		/*  base case */
	}else{
		if(reverse){
			/*  Now add these items to the end of the list */
			*most_recent_list_id = make_declarator_part_list_item(state, *most_recent_list_id, details.item_id);
			/*  Go down to base case, */
			add_declarators_to_list(state, most_recent_list_id, details.prev_id, reverse);
		}else{
			/*  Go down to base case, */
			add_declarators_to_list(state, most_recent_list_id, details.prev_id, reverse);
			/*  Now add these items to the end of the list */
			*most_recent_list_id = make_declarator_part_list_item(state, *most_recent_list_id, details.item_id);
		}
	}
}

struct declarator_part_list_item_id add_pointer_and_direct_declarators_to_list(struct type_engine_state * state, struct declarator_part_list_item_id previous_list_item, struct declarator_part_list_item_id pointer_end_list_item, struct declarator_part_list_item_id direct_declarator_end_list_item){
	/*  This function takes care of the fact that declarators aren't parsed in the same order that they are read in the spiral rule */
	add_declarators_to_list(state, &previous_list_item, direct_declarator_end_list_item, 0);
	add_declarators_to_list(state, &previous_list_item, pointer_end_list_item, 1);
	return previous_list_item;
}

struct declaration_namespace * create_declaration_namespace(struct type_engine_state * state){
	struct declaration_namespace * d = (struct declaration_namespace *)malloc(sizeof(struct declaration_namespace));
	unsigned_int_list_create(&d->ordered_type_ids);
	unsigned_int_to_unsigned_int_map_create(&d->identifier_ids_to_type_ids, struct_unsigned_int_to_unsigned_int_key_value_pair_compare);
	unsigned_int_to_unsigned_int_map_create(&d->type_ids_to_ordered_index, struct_unsigned_int_to_unsigned_int_key_value_pair_compare);
	enum_type_engine_operation_type_list_add_end(&state->reversible_operations, TYPE_ENGINE_CREATE_DECLARATION_NAMESPACE);
	void_ptr_list_add_end(&state->stored_pointers, (void*)d);
	return d;
}

void add_ordered_general_type_to_declaration_namespace(struct type_engine_state * state, struct declaration_namespace * dn, struct general_type_id general_type_id){
	unsigned_int_list_add_end(&dn->ordered_type_ids, general_type_id.id);
	unsigned_int_to_unsigned_int_map_put(&dn->type_ids_to_ordered_index, general_type_id.id, unsigned_int_list_size(&dn->ordered_type_ids));
	enum_type_engine_operation_type_list_add_end(&state->reversible_operations, TYPE_ENGINE_ADD_ORDERED_TYPE_TO_DECLARATION_NAMESPACE);
	void_ptr_list_add_end(&state->stored_pointers, (void*)dn);
}

void add_identifier_id_to_declaration_namespace(struct type_engine_state * state, struct declaration_namespace * dn, struct general_type_id general_type_id, struct identifier_id identifier_id){
	unsigned_int_to_unsigned_int_map_put(&dn->identifier_ids_to_type_ids, identifier_id.id, general_type_id.id);
	struct_identifier_id_list_add_end(&state->saved_identifier_ids, identifier_id);
	enum_type_engine_operation_type_list_add_end(&state->reversible_operations, TYPE_ENGINE_ADD_IDENTIFIER_ID_TO_DECLARATION_NAMESPACE);
	void_ptr_list_add_end(&state->stored_pointers, (void*)dn);
}

unsigned int add_named_struct_or_union_declaration(struct type_engine_state * state, struct identifier_id identifier_id, struct struct_or_union_specifier_id struct_or_union_specifier_id, struct scope_level * scope){
	if(unsigned_int_to_unsigned_int_map_exists(&scope->struct_or_union_declarations.identifier_ids_to_type_ids, identifier_id.id)){
		return 0; /*  Already defined, do nothing */
	}else{
		unsigned int index = unsigned_int_list_size(&scope->struct_or_union_declarations.ordered_type_ids);
		unsigned_int_to_unsigned_int_map_put(&scope->struct_or_union_declarations.identifier_ids_to_type_ids, identifier_id.id, struct_or_union_specifier_id.id);
		unsigned_int_to_unsigned_int_map_put(&scope->struct_or_union_declarations.type_ids_to_ordered_index, struct_or_union_specifier_id.id, index);
		unsigned_int_list_add_end(&scope->struct_or_union_declarations.ordered_type_ids, struct_or_union_specifier_id.id);
		struct_identifier_id_list_add_end(&state->saved_identifier_ids, identifier_id);
		enum_type_engine_operation_type_list_add_end(&state->reversible_operations, TYPE_ENGINE_ADD_NAMED_STRUCT_OR_UNION_DECLARATION);
		void_ptr_list_add_end(&state->stored_pointers, (void*)scope);
		return 0;
	}
}

unsigned int add_struct_or_union_definition(struct type_engine_state * state, struct declaration_namespace * dn, struct struct_or_union_specifier_id struct_or_union_specifier_id, struct scope_level * scope){
	if(unsigned_int_to_void_ptr_map_exists(&scope->struct_or_union_definitions.struct_or_union_specifier_ids_to_struct_or_union_definitions, struct_or_union_specifier_id.id)){
		return 1; /*  Already defined */
	}else{
		struct struct_or_union_definition * new_d = (struct struct_or_union_definition *)malloc(sizeof(struct struct_or_union_definition));
		new_d->declaration_namespace = dn;
		struct_struct_or_union_specifier_id_list_add_end(&state->saved_struct_or_union_specifier_ids, struct_or_union_specifier_id);
		enum_type_engine_operation_type_list_add_end(&state->reversible_operations, TYPE_ENGINE_ADD_STRUCT_OR_UNION_DEFINITION);
		unsigned_int_to_void_ptr_map_put(&scope->struct_or_union_definitions.struct_or_union_specifier_ids_to_struct_or_union_definitions, struct_or_union_specifier_id.id, (void*)new_d);
		void_ptr_list_add_end(&state->stored_pointers, (void*)scope);
		return 0;
	}
}

struct scope_level * get_scope_of_closest_struct_or_union_tag_declaration(struct type_engine_state * state, struct scope_level * starting_scope, struct struct_or_union_specifier_id unscoped_struct_or_union_id){
	if(starting_scope){
		if(unsigned_int_to_unsigned_int_map_exists(&starting_scope->struct_or_union_declarations.type_ids_to_ordered_index, unscoped_struct_or_union_id.id)){
			return starting_scope;
		}else{
			return get_scope_of_closest_struct_or_union_tag_declaration(state, starting_scope->parent_scope, unscoped_struct_or_union_id);
		}
	}else{
		return starting_scope;
	}
}

