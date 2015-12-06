/*
	Copyright 2015 Robert Elder Software Inc.  All rights reserved.

	This software is not currently available under any license, and unauthorized use
	or copying is not permitted.

	This software will likely be available under a common open source license in the
	near future.  Licensing is currently pending feedback from a lawyer.  If you have
	an opinion on this subject you can send it to recc [at] robertelder.org.

	This program comes with ABSOLUTELY NO WARRANTY.  In no event shall Robert Elder
	Software Inc. be liable for incidental or consequential damages in connection with
	use of this software.
*/
#include "linker.h"

void buffered_token_output(struct unsigned_char_list *, struct asm_lexer_token *);
void free_symbol_map(struct linker_state *, struct unsigned_char_ptr_to_struct_linker_symbol_ptr_map *);
unsigned int set_post_linking_offsets(struct linker_state *, struct linker_object * linker_object);
unsigned int get_instruction_size(struct linker_state *, struct asm_instruction *);
unsigned int parse_hexidecimal_string(struct asm_lexer_token *);
unsigned int parse_decimal_string(struct asm_lexer_token *);
unsigned int get_linker_object_size(struct linker_state *, struct linker_object *);
int struct_linker_object_ptr_cmp_indirect(struct linker_object **, struct linker_object **);
int struct_linker_object_ptr_cmp(struct linker_object *, struct linker_object *);

void reorder_linker_objects(struct linker_state *, struct struct_linker_object_ptr_list *, struct struct_linker_object_ptr_list *, unsigned int);

unsigned int is_non_descending_order(struct struct_linker_object_ptr_list *);

unsigned int is_non_descending_order(struct struct_linker_object_ptr_list * linker_objects){
	unsigned int i;
	struct linker_object * prev = 0;
	for(i = 0; i < struct_linker_object_ptr_list_size(linker_objects); i++){
		if(i != 0){
			if(struct_linker_object_ptr_list_get(linker_objects, i)->linker_object_post_linking_offset < prev->linker_object_post_linking_offset){
				return 0;
			}
		}
		prev = struct_linker_object_ptr_list_get(linker_objects, i);
	}
	return 1;
}

int struct_linker_object_ptr_cmp_indirect(struct linker_object ** a, struct linker_object ** b){
	if((*a)->linker_object_post_linking_offset < (*b)->linker_object_post_linking_offset){
		return -1;
	}else if((*a)->linker_object_post_linking_offset > (*b)->linker_object_post_linking_offset){
		return 1;
	}else{
		return 0;
	}
}

int struct_linker_object_ptr_cmp(struct linker_object * a, struct linker_object * b){
	if(a < b){
		return -1;
	}else if(a > b){
		return 1;
	}else{
		return 0;
	}
}

void reorder_linker_objects(struct linker_state * linker_state, struct struct_linker_object_ptr_list * linker_objects, struct struct_linker_object_ptr_list * reordered_linker_objects, unsigned int starting_offset){
	/*  Some linker objects must be put at a fixed address, and some are relocatable.
	    Re-order them so that they can be contiguously placed (with possible space in-between)
	*/
	unsigned int i;
	struct struct_linker_object_ptr_list relocatable_linker_objects;
	struct struct_linker_object_ptr_list non_relocatable_linker_objects;
	unsigned int previous_linker_object_end = starting_offset; /*  Offsets in #words */
	unsigned int current_linker_object_start;

	struct_linker_object_ptr_list_create(&relocatable_linker_objects);
	struct_linker_object_ptr_list_create(&non_relocatable_linker_objects);

	/*  Sort the linker objects into relocatable and non-relocatable */
	for(i = 0; i < struct_linker_object_ptr_list_size(linker_objects); i++){
		struct linker_object * obj = struct_linker_object_ptr_list_get(linker_objects, i);
		if(obj->is_relocatable){
			struct_linker_object_ptr_list_add_end(&relocatable_linker_objects, obj);
		}else{
			struct_linker_object_ptr_list_add_end(&non_relocatable_linker_objects, obj);
		}
	}
	
	struct_linker_object_ptr_merge_sort(struct_linker_object_ptr_list_data(&non_relocatable_linker_objects), struct_linker_object_ptr_list_size(&non_relocatable_linker_objects), struct_linker_object_ptr_cmp_indirect);
	/*  Make sure there is no conflict between non-relocatable linker objects */
	{
		struct linker_object * prev = 0;
		for(i = 0; i < struct_linker_object_ptr_list_size(&non_relocatable_linker_objects); i++){
			struct linker_object * current = struct_linker_object_ptr_list_get(&non_relocatable_linker_objects, i);
			unsigned int prev_offset = prev ? prev->linker_object_post_linking_offset + get_linker_object_size(linker_state, prev) : 0;
			unsigned int current_obj_size = get_linker_object_size(linker_state, current);
			unsigned int end_byte = (current->linker_object_post_linking_offset + current_obj_size) -1;
			/*  Will it overwrite the previous non-relocatable linker object? */
			assert(prev_offset <= current->linker_object_post_linking_offset);
			
			if(current_obj_size){
				printf("Non-relocatable linker object %s must occupy %08X - %08X\n", current->asm_lexer_state->c.filename, 4 * current->linker_object_post_linking_offset, 4*end_byte);
			}else{
				printf("Non-relocatable linker object %s is empty so it is ignored.\n", current->asm_lexer_state->c.filename);
			}
			prev = current;
		}
	}
	/*  Fit in all the relocatable linker objects between the non relocatable ones */
	for(i = 0; i < struct_linker_object_ptr_list_size(&non_relocatable_linker_objects); i++){
		struct linker_object * non_relocatable = struct_linker_object_ptr_list_get(&non_relocatable_linker_objects, i);
		unsigned int non_relocatable_size = get_linker_object_size(linker_state, non_relocatable);
		current_linker_object_start = non_relocatable->linker_object_post_linking_offset;  /*  Offset in # words */
		while(struct_linker_object_ptr_list_size(&relocatable_linker_objects)){
			unsigned int j;
			/* Look for an object that will fit */
			for(j = 0; j < struct_linker_object_ptr_list_size(&relocatable_linker_objects); j++){
				struct linker_object * relocatable = struct_linker_object_ptr_list_get(&relocatable_linker_objects, j);
				unsigned int relocatable_size = get_linker_object_size(linker_state, relocatable);
				if(previous_linker_object_end + relocatable_size <= current_linker_object_start){
					unsigned int start = previous_linker_object_end;
					unsigned int end = start + relocatable_size;
					struct_linker_object_ptr_list_add_end(reordered_linker_objects, relocatable);
					if(start == end){
						printf("Linker object %s is empty\n", relocatable->asm_lexer_state->c.filename);
					}else{
						printf("0x%08X to 0x%08X will fit linker object %s before non-relocatable linker object %s, which starts at 0x%08X.\n",4 * start, 4 * end -1, relocatable->asm_lexer_state->c.filename, non_relocatable->asm_lexer_state->c.filename, 4 * current_linker_object_start);
					}
					/* Don't try to add this one multiple times */
					struct_linker_object_ptr_list_remove_all(&relocatable_linker_objects, relocatable, struct_linker_object_ptr_cmp);
					previous_linker_object_end = previous_linker_object_end + relocatable_size;
					break; /* List size changed, do iteration again. */
				}
			}
			if(struct_linker_object_ptr_list_size(&relocatable_linker_objects) == j){
				/*  None of the relocatable objects will fit if we got through the loop */
				/*  Consume the empty region after last relocatable object  */
				if((previous_linker_object_end + (current_linker_object_start - previous_linker_object_end))){
					printf("0x%08X to 0x%08X will be unused.\n", 4 * previous_linker_object_end, 4 * (previous_linker_object_end + (current_linker_object_start - previous_linker_object_end)) -1);
				}
				previous_linker_object_end += current_linker_object_start - previous_linker_object_end;
				break;
			}
		}
		/*  No more of the relocatable objects will fit, put the non relocatable object after them */
		struct_linker_object_ptr_list_add_end(reordered_linker_objects, non_relocatable);
		/*  Next object needs to know where it can start from */
		printf("0x%08X to 0x%08X will fit non-relocatable linker object %s\n", 4 * previous_linker_object_end, 4 * (previous_linker_object_end + non_relocatable_size) -1, non_relocatable->asm_lexer_state->c.filename);
		previous_linker_object_end = previous_linker_object_end + non_relocatable_size;
	}

	/*  Add all the leftover relocatable linker objects at the end */
	for(i = 0; i < struct_linker_object_ptr_list_size(&relocatable_linker_objects); i++){
		struct linker_object * relocatable = struct_linker_object_ptr_list_get(&relocatable_linker_objects, i);
		struct_linker_object_ptr_list_add_end(reordered_linker_objects, relocatable);
	}
	/*  Now all the linker objects are guaranteed to fit. */

	struct_linker_object_ptr_list_destroy(&relocatable_linker_objects);
	struct_linker_object_ptr_list_destroy(&non_relocatable_linker_objects);
}

unsigned int parse_decimal_string(struct asm_lexer_token * token){
	unsigned int i = 0;
	unsigned int base = 1;
	unsigned char * c = token->last_byte;
	do{
		unsigned int n;
		if(*c >= '0' && *c <= '9'){
			n = *c - '0';
		}else{
			printf("%c\n", *c);
			assert(0 && "Unknown decimal character.");
		}
		i += (n * base);
		base *= 10;
	}while(c-- != token->first_byte);
	return i;
}

unsigned int parse_hexidecimal_string(struct asm_lexer_token * token){
	unsigned int i = 0;
	unsigned int base = 1;
	unsigned char * c = token->last_byte;
	do{
		unsigned int n;
		if(*c >= '0' && *c <= '9'){
			n = *c - '0';
		}else if(*c >= 'A' && *c <= 'F'){
			n = (*c - 'A') + 10;
		}else{
			printf("%c\n", *c);
			assert(0 && "Unknown hex character.");
		}
		i += (n * base);
		base *= 16;
	}while(*(--c) != 'x');
	return i;
}

unsigned int get_instruction_size(struct linker_state * linker_state, struct asm_instruction * instruction){
	enum asm_token_type type = instruction->op_token ? instruction->op_token->type : A_ADD; /*  For label just use A_ADD, then check if op token set */

	if(!instruction->op_token){
		/*  A label */
		return 0;
	}

	if ((type == A_BLT || type == A_BEQ) && instruction->identifier_token && linker_state->build_target_type == BUILD_TARGET_L1_FILE){ /* Gets re-written for long jumps */
		return 4;
	}

	if (type == A_LL && instruction->identifier_token && linker_state->build_target_type == BUILD_TARGET_L1_FILE){ /* Gets re-written for large values */
		return 9;
	}

	if (type == A_SW){
		return parse_hexidecimal_string(instruction->number_token); /* Size depends on number of words skipped */
	}

	return 1; /* 1 word */
}

unsigned int get_linker_object_size(struct linker_state * linker_state, struct linker_object * linker_object){
	unsigned int i;
	unsigned int num_instructions = struct_asm_instruction_ptr_list_size(&linker_object->instructions);
	unsigned int current_offset = 0; /*  Offset in # words */
	for(i = 0; i < num_instructions; i++){
		struct asm_instruction * instruction = struct_asm_instruction_ptr_list_get(&linker_object->instructions, i);
		current_offset += get_instruction_size(linker_state, instruction);
	}
	return current_offset;
}

unsigned int set_post_linking_offsets(struct linker_state * linker_state, struct linker_object * linker_object){
	unsigned int i;
	unsigned int num_instructions = struct_asm_instruction_ptr_list_size(&linker_object->instructions);
	unsigned int current_offset = 0; /*  Offset in # words */
	for(i = 0; i < num_instructions; i++){
		struct asm_instruction * instruction = struct_asm_instruction_ptr_list_get(&linker_object->instructions, i);
		instruction->post_linking_offset = current_offset;
		current_offset += get_instruction_size(linker_state, instruction);
	}
	return current_offset;
}

void set_symbol_instruction_index(struct linker_state * linker_state, struct linker_object * linker_object, struct asm_lexer_token * token, unsigned int index){
	unsigned char * identifier_str = copy_string(token->first_byte, token->last_byte, linker_state->memory_pool_collection);
	struct linker_symbol * internal_symbol = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_exists(&linker_object->internal_symbols, identifier_str) ? unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(&linker_object->internal_symbols, identifier_str) : (struct linker_symbol *)0;
	if(internal_symbol){
		if(internal_symbol->observed_as_implemented){
			printf("Internal label %s re-declared on line %d in file %s\n", identifier_str, linker_object->current_line, linker_object->asm_lexer_state->c.filename);
			assert(0 && "Trying to re-declare label in this linker object.");
		}else{
			internal_symbol->instruction_index = index;
			internal_symbol->observed_as_implemented = 1; /*  Observed a label for this symbol in the file */
		}
	}else{
		struct linker_symbol * external_symbol = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_exists(&linker_state->external_symbols, identifier_str) ? unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(&linker_state->external_symbols, identifier_str) : (struct linker_symbol *)0;
		if(external_symbol){
			if(external_symbol->observed_as_implemented){
				printf("Internal label %s re-declared on line %d in file %s\n", identifier_str, linker_object->current_line, linker_object->asm_lexer_state->c.filename);
				assert(0 && "Trying to re-declare label in this linker object.");
			}else{
				external_symbol->instruction_index = index;
				external_symbol->observed_as_implemented = 1; /*  Observed a label for this symbol in the file */
			}
		}else{
			printf("Undeclared identifier %s on line %d in file %s\n", identifier_str, linker_object->current_line, linker_object->asm_lexer_state->c.filename);
			assert(0 && "Trying to set offset of unknown symbol.");
		}
	}
	heap_memory_pool_free(linker_state->memory_pool_collection->heap_pool, identifier_str);
}

void verify_symbol_declaration(struct linker_state * linker_state, struct linker_object * linker_object, struct asm_lexer_token * token){
	/*  Make sure that a symbol has been declared before it is implemented */
	unsigned char * identifier_str = copy_string(token->first_byte, token->last_byte, linker_state->memory_pool_collection);
	unsigned int internal_symbol = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_exists(&linker_object->internal_symbols, identifier_str);
	unsigned int external_symbol = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_exists(&linker_state->external_symbols, identifier_str);
	if(!(internal_symbol || external_symbol)){
		printf("Undeclared identifier %s on line %d in file %s\n", identifier_str, linker_object->current_line, linker_object->asm_lexer_state->c.filename);
		assert(0 && "Found symbol without forward declaration.");
	}
	heap_memory_pool_free(linker_state->memory_pool_collection->heap_pool, identifier_str);
}

void add_internal_linker_symbol(struct linker_state * linker_state, struct linker_object * linker_object, struct asm_lexer_token * token, unsigned int is_required, unsigned int is_implemented){
	unsigned char * identifier_str = copy_string(token->first_byte, token->last_byte, linker_state->memory_pool_collection);
	struct linker_symbol * new_symbol;
	struct linker_symbol * existing_symbol = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_exists(&linker_object->internal_symbols, identifier_str) ? unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(&linker_object->internal_symbols, identifier_str) : (struct linker_symbol *)0;
	if(existing_symbol){
		assert(!(existing_symbol->is_implemented && is_implemented && "Cannot implement internal symbol multiple times."));
		existing_symbol->is_implemented = existing_symbol->is_implemented ? existing_symbol->is_implemented : is_implemented;
		existing_symbol->is_required = existing_symbol->is_required ? existing_symbol->is_required : is_required;
		existing_symbol->is_external = 0;
		heap_memory_pool_free(linker_state->memory_pool_collection->heap_pool, identifier_str);
		return;
	}
	new_symbol = struct_linker_symbol_memory_pool_malloc(linker_state->memory_pool_collection->struct_linker_symbol_pool);
	new_symbol->is_implemented = is_implemented;
	new_symbol->is_required = is_required;
	new_symbol->is_external = 0;
	new_symbol->observed_as_implemented = 0; /*  We haven't see a lable for this symbol yet */
	new_symbol->parent_linker_object = linker_object;
	unsigned_char_ptr_to_struct_linker_symbol_ptr_map_put(&linker_object->internal_symbols, identifier_str, new_symbol);
}

void add_external_linker_symbol(struct linker_state * linker_state, struct linker_object * linker_object, struct asm_lexer_token * token, unsigned int is_required, unsigned int is_implemented){
	unsigned char * identifier_str = copy_string(token->first_byte, token->last_byte, linker_state->memory_pool_collection);
	struct linker_symbol * new_symbol;
	struct linker_symbol * existing_symbol = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_exists(&linker_state->external_symbols, identifier_str) ? unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(&linker_state->external_symbols, identifier_str) : (struct linker_symbol *)0;
	if(existing_symbol){
		if(existing_symbol->is_implemented && is_implemented){
			printf("Detected duplicate external symbol %s re-implemented on line %d in file %s\n", identifier_str, linker_object->current_line, linker_object->asm_lexer_state->c.filename);
			printf("The previously declared external symbol was declared in file %s\n", existing_symbol->parent_linker_object->asm_lexer_state->c.filename);
			assert(0 && "Cannot implement external symbol multiple times.");
		}
		existing_symbol->is_implemented = existing_symbol->is_implemented ? existing_symbol->is_implemented : is_implemented;
		existing_symbol->is_required = existing_symbol->is_required ? existing_symbol->is_required : is_required;
		existing_symbol->is_external = 1;
		existing_symbol->parent_linker_object = is_implemented ? linker_object : existing_symbol->parent_linker_object;
		heap_memory_pool_free(linker_state->memory_pool_collection->heap_pool, identifier_str);
		return;
	}
	new_symbol = struct_linker_symbol_memory_pool_malloc(linker_state->memory_pool_collection->struct_linker_symbol_pool);
	new_symbol->is_implemented = is_implemented;
	new_symbol->is_required = is_required;
	new_symbol->is_external = 1;
	new_symbol->observed_as_implemented = 0; /*  We haven't see a lable for this symbol yet */
	new_symbol->parent_linker_object = is_implemented ? linker_object : (struct linker_object *)0;
	unsigned_char_ptr_to_struct_linker_symbol_ptr_map_put(&linker_state->external_symbols, identifier_str, new_symbol);
}

struct linker_object * process_assembly(struct linker_state * linker_state, struct asm_lexer_state * asm_lexer_state){
	struct linker_object * linker_object = (struct linker_object *)malloc(sizeof(struct linker_object));
	unsigned int num_tokens = struct_asm_lexer_token_ptr_list_size(&asm_lexer_state->tokens);
	struct asm_lexer_token ** tokens =  struct_asm_lexer_token_ptr_list_data(&asm_lexer_state->tokens);
	unsigned int i = 0;
	unsigned int offset_declared = 0;
	linker_object->is_relocatable = 0; /* Assume false */
	linker_object->current_line = 1;
	linker_object->asm_lexer_state = asm_lexer_state;
	unsigned_char_ptr_to_struct_linker_symbol_ptr_map_create(&linker_object->internal_symbols);
	struct_asm_instruction_ptr_list_create(&linker_object->instructions);
	while(i < num_tokens){
		if(tokens[i]->type == A_ADD || tokens[i]->type == A_SUB || tokens[i]->type == A_MUL || tokens[i]->type == A_AND || tokens[i]->type == A_OR || tokens[i]->type == A_DIV){
			struct asm_instruction * new_instruction = struct_asm_instruction_memory_pool_malloc(linker_state->memory_pool_collection->struct_asm_instruction_pool);
			new_instruction->op_token = tokens[i];
			i++;
			if(tokens[i]->type == A_SPACE){
				i++;
				if(tokens[i]->type == A_REGISTER){
					new_instruction->rx_token = tokens[i];
					i++;
					if(tokens[i]->type == A_SPACE){
						i++;
						if(tokens[i]->type == A_REGISTER){
							new_instruction->ry_token = tokens[i];
							i++;
							if(tokens[i]->type == A_SPACE){
								i++;
								if(tokens[i]->type == A_REGISTER){
									new_instruction->rz_token = tokens[i];
									i++;
									struct_asm_instruction_ptr_list_add_end(&linker_object->instructions, new_instruction);
								}else{ assert(0 && "Expected register."); }
							}else{ assert(0 && "Expected space."); }
						}else{ assert(0 && "Expected register."); }
					}else{ assert(0 && "Expected space."); }
				}else{ assert(0 && "Expected register."); }
			}else{ assert(0 && "Expected space."); }
		}else if(tokens[i]->type == A_LOA || tokens[i]->type == A_STO || tokens[i]->type == A_NOT || tokens[i]->type == A_SHR || tokens[i]->type == A_SHL){
			struct asm_instruction * new_instruction = struct_asm_instruction_memory_pool_malloc(linker_state->memory_pool_collection->struct_asm_instruction_pool);
			new_instruction->op_token = tokens[i];
			i++;
			if(tokens[i]->type == A_SPACE){
				i++;
				if(tokens[i]->type == A_REGISTER){
					new_instruction->rx_token = tokens[i];
					i++;
					if(tokens[i]->type == A_SPACE){
						i++;
						if(tokens[i]->type == A_REGISTER){
							new_instruction->ry_token = tokens[i];
							i++;
							struct_asm_instruction_ptr_list_add_end(&linker_object->instructions, new_instruction);
						}else{ assert(0 && "Expected register."); }
					}else{ assert(0 && "Expected space."); }
				}else{ assert(0 && "Expected register."); }
			}else{ assert(0 && "Expected space."); }
		}else if(tokens[i]->type == A_LL){
			struct asm_instruction * new_instruction = struct_asm_instruction_memory_pool_malloc(linker_state->memory_pool_collection->struct_asm_instruction_pool);
			new_instruction->op_token = tokens[i];
			i++;
			if(tokens[i]->type == A_SPACE){
				i++;
				if(tokens[i]->type == A_REGISTER){
					new_instruction->rx_token = tokens[i];
					i++;
					if(tokens[i]->type == A_SPACE){
						i++;
						if(tokens[i]->type == A_CONSTANT_HEX){
							new_instruction->number_token_is_negative = 0;
							new_instruction->number_token = tokens[i];
							new_instruction->identifier_token = 0;
							i++;
							struct_asm_instruction_ptr_list_add_end(&linker_object->instructions, new_instruction);
						}else if(tokens[i]->type == A_IDENTIFIER){
							new_instruction->identifier_token = tokens[i];
							new_instruction->number_token = 0;
							verify_symbol_declaration(linker_state, linker_object, tokens[i]);
							struct_asm_instruction_ptr_list_add_end(&linker_object->instructions, new_instruction);
							i++;
						}else{ assert(0 && "Expected identifier or hexidecimal constant."); }
					}else{ assert(0 && "Expected space."); }
				}else{ assert(0 && "Expected register."); }
			}else{ assert(0 && "Expected space."); }
		}else if(tokens[i]->type == A_DW || tokens[i]->type == A_SW){
			struct asm_instruction * new_instruction = struct_asm_instruction_memory_pool_malloc(linker_state->memory_pool_collection->struct_asm_instruction_pool);
			new_instruction->op_token = tokens[i];
			i++;
			if(tokens[i]->type == A_SPACE){
				i++;
				if(tokens[i]->type == A_CONSTANT_HEX){
					new_instruction->number_token_is_negative = 0;
					new_instruction->number_token = tokens[i];
					new_instruction->identifier_token = 0;
					i++;
					struct_asm_instruction_ptr_list_add_end(&linker_object->instructions, new_instruction);
				}else if(tokens[i]->type == A_IDENTIFIER){
					new_instruction->identifier_token = tokens[i];
					new_instruction->number_token = 0;
					verify_symbol_declaration(linker_state, linker_object, tokens[i]);
					struct_asm_instruction_ptr_list_add_end(&linker_object->instructions, new_instruction);
					i++;
				}else{ printf("On line %d in file %s\n", linker_object->current_line, asm_lexer_state->c.filename);  assert(0 && "Expected identifier or hexidecimal constant."); }
			}else{ assert(0 && "Expected space."); }
		}else if(tokens[i]->type == A_BEQ || tokens[i]->type == A_BLT){
			struct asm_instruction * new_instruction = struct_asm_instruction_memory_pool_malloc(linker_state->memory_pool_collection->struct_asm_instruction_pool);
			new_instruction->op_token = tokens[i];
			i++;
			if(tokens[i]->type == A_SPACE){
				i++;
				if(tokens[i]->type == A_REGISTER){
					new_instruction->rx_token = tokens[i];
					i++;
					if(tokens[i]->type == A_SPACE){
						i++;
						if(tokens[i]->type == A_REGISTER){
							new_instruction->ry_token = tokens[i];
							i++;
							if(tokens[i]->type == A_SPACE){
								i++;
								if(tokens[i]->type == A_IDENTIFIER){
									new_instruction->identifier_token = tokens[i];
									new_instruction->number_token = 0;
									new_instruction->number_token_is_negative = 0;
									verify_symbol_declaration(linker_state, linker_object, tokens[i]);
									struct_asm_instruction_ptr_list_add_end(&linker_object->instructions, new_instruction);
									i++;

								}else if(tokens[i]->type == A_MINUS_CHAR){
									i++;
									if(tokens[i]->type == A_CONSTANT_DECIMAL){
										new_instruction->number_token = tokens[i];
										new_instruction->identifier_token = 0;
										new_instruction->number_token_is_negative = 1;
										i++;
										struct_asm_instruction_ptr_list_add_end(&linker_object->instructions, new_instruction);
									}else{ assert(0 && "Expected number."); }
								}else if(tokens[i]->type == A_CONSTANT_DECIMAL){
									new_instruction->number_token = tokens[i];
									new_instruction->identifier_token = 0;
									new_instruction->number_token_is_negative = 0;
									i++;
									struct_asm_instruction_ptr_list_add_end(&linker_object->instructions, new_instruction);
								}else{ assert(0 && "Expected number or identifier."); }
							}else{ assert(0 && "Expected space."); }
						}else{ assert(0 && "Expected register."); }
					}else{ assert(0 && "Expected space."); }
				}else{ assert(0 && "Expected register."); }
			}else{ assert(0 && "Expected space."); }
		}else if(tokens[i]->type == A_OFFSET){
			assert(!offset_declared && "OFFSET declared multiple times.");
			i++;
			if(tokens[i]->type == A_SPACE){
				i++;
				if(tokens[i]->type == A_RELOCATABLE){
					i++;
					linker_object->is_relocatable = 1; /* All symbols are relative */
					offset_declared = 1;
				}else if(tokens[i]->type == A_CONSTANT_HEX){
					unsigned int parsed_value = parse_hexidecimal_string(tokens[i]);
					linker_object->is_relocatable = 0; /* This linker object must be placed at a fixed location in memory */
					linker_object->linker_object_post_linking_offset = (parsed_value / 4); /*  Offset in # words */
					assert(parsed_value % 4 == 0 && "Value must be evenly divisible by word size.");
					offset_declared = 1;
					i++;
				}else{ assert(0 && "Expected space."); }
			}else{ assert(0 && "Expected space."); }
		}else if(tokens[i]->type == A_REQUIRES){
			i++;
			if(tokens[i]->type == A_COMMA_CHAR){
				i++;
				if(tokens[i]->type == A_SPACE){
					i++;
					if(tokens[i]->type == A_IMPLEMENTS){
						i++;
						if(tokens[i]->type == A_SPACE){
							i++;
							if(tokens[i]->type == A_INTERNAL){
								i++;
								if(tokens[i]->type == A_SPACE){
									i++;
									if(tokens[i]->type == A_IDENTIFIER){
										add_internal_linker_symbol(linker_state, linker_object, tokens[i], 1, 1);
										i++;
									}else{printf("On line %d in file %s\n", linker_object->current_line, asm_lexer_state->c.filename);  assert(0 && "Expected identifier."); }
								}else{ assert(0 && "Expected space."); }
							}else if(tokens[i]->type == A_EXTERNAL){
								i++;
								if(tokens[i]->type == A_SPACE){
									i++;
									if(tokens[i]->type == A_IDENTIFIER){
										add_external_linker_symbol(linker_state, linker_object, tokens[i], 1, 1);
										i++;
									}else{ assert(0 && "Expected identifier."); }
								}else{ assert(0 && "Expected space."); }
							}else{ assert(0 && "Expected INTERNAL or EXTERNAL."); }

						}else{ assert(0 && "Expected space."); }
					}else{ assert(0 && "Expected REQUIRES."); }
				}else{ assert(0 && "Expected space."); }
			}else if(tokens[i]->type == A_SPACE){
				i++;
				if(tokens[i]->type == A_INTERNAL){
					i++;
					if(tokens[i]->type == A_SPACE){
						i++;
						if(tokens[i]->type == A_IDENTIFIER){
							add_internal_linker_symbol(linker_state, linker_object, tokens[i], 1, 0);
							i++;
						}
					}else{ assert(0 && "Expected space."); }
				}else if(tokens[i]->type == A_EXTERNAL){
					i++;
					if(tokens[i]->type == A_SPACE){
						i++;
						if(tokens[i]->type == A_IDENTIFIER){
							add_external_linker_symbol(linker_state, linker_object, tokens[i], 1, 0);
							i++;
						}
					}else{ assert(0 && "Expected space."); }
				}else{ assert(0 && "Expected INTERNAL or EXTERNAL."); }
			}else{ assert(0 && "Expected space or comma."); }
		}else if(tokens[i]->type == A_IMPLEMENTS){
			i++;
			if(tokens[i]->type == A_COMMA_CHAR){
				i++;
				if(tokens[i]->type == A_SPACE){
					i++;
					if(tokens[i]->type == A_REQUIRES){
						i++;
						if(tokens[i]->type == A_SPACE){
							i++;
							if(tokens[i]->type == A_INTERNAL){
								i++;
								if(tokens[i]->type == A_SPACE){
									i++;
									if(tokens[i]->type == A_IDENTIFIER){
										add_internal_linker_symbol(linker_state, linker_object, tokens[i], 1, 1);
										i++;
									}
								}else{ assert(0 && "Expected space."); }
							}else if(tokens[i]->type == A_EXTERNAL){
								i++;
								if(tokens[i]->type == A_SPACE){
									i++;
									if(tokens[i]->type == A_IDENTIFIER){
										add_external_linker_symbol(linker_state, linker_object, tokens[i], 1, 1);
										i++;
									}
								}else{ assert(0 && "Expected space."); }
							}else{ printf("On line %d\n", linker_object->current_line); assert(0 && "Expected INTERNAL or EXTERNAL."); }
						}else{ assert(0 && "Expected space."); }
					}else{ assert(0 && "Expected REQUIRES."); }
				}else{ assert(0 && "Expected space."); }
			}else if(tokens[i]->type == A_SPACE){
				i++;
				if(tokens[i]->type == A_INTERNAL){
					i++;
					if(tokens[i]->type == A_SPACE){
						i++;
						if(tokens[i]->type == A_IDENTIFIER){
							add_internal_linker_symbol(linker_state, linker_object, tokens[i], 0, 1);
							i++;
						}
					}else{ assert(0 && "Expected space."); }
				}else if(tokens[i]->type == A_EXTERNAL){
					i++;
					if(tokens[i]->type == A_SPACE){
						i++;
						if(tokens[i]->type == A_IDENTIFIER){
							add_external_linker_symbol(linker_state, linker_object, tokens[i], 0, 1);
							i++;
						}
					}else{ assert(0 && "Expected space."); }
				}else{ printf("On line %d\n", linker_object->current_line); assert(0 && "Expected INTERNAL or EXTERNAL."); }
			}else{ assert(0 && "Expected space or comma."); }
		}else if(tokens[i]->type == A_ASM_COMMENT){
			i++;
		}else if(tokens[i]->type == A_NEWLINE){
			i++;
			linker_object->current_line = linker_object->current_line + 1;
		}else if(tokens[i]->type == A_IDENTIFIER){
			struct asm_lexer_token * identifier = tokens[i];
			struct asm_instruction * new_instruction = struct_asm_instruction_memory_pool_malloc(linker_state->memory_pool_collection->struct_asm_instruction_pool);
			new_instruction->op_token = (struct asm_lexer_token *)0;
			i++;
			if(tokens[i]->type == A_COLON_CHAR){
				set_symbol_instruction_index(linker_state, linker_object, identifier, struct_asm_instruction_ptr_list_size(&linker_object->instructions));
				new_instruction->identifier_token = tokens[i-1];
				struct_asm_instruction_ptr_list_add_end(&linker_object->instructions, new_instruction);
				i++;
			}else{ printf("On line %d in file %s\n", linker_object->current_line, asm_lexer_state->c.filename); assert(0 && "Expected colon."); }
		}else{
			printf("On line %d in file %s\n", linker_object->current_line, asm_lexer_state->c.filename); assert(0 && "Unable to process assembly file.");
		}
	}

	if(!offset_declared){
		printf("On line %d in file %s\n", linker_object->current_line, asm_lexer_state->c.filename); assert(0 && "OFFSET not declared..");
	}
	return linker_object;
}

void buffered_token_output(struct unsigned_char_list * buffer, struct asm_lexer_token * t){
	unsigned char * c = t->first_byte;
	while(c != t->last_byte) {
		buffered_printf(buffer, "%c", *c);
		c++;
	}
	buffered_printf(buffer, "%c", *c);
}

unsigned int get_relative_symbol_offset(struct linker_state * linker_state, struct linker_object * linker_object, struct linker_symbol * symbol){
	assert(symbol->observed_as_implemented && "Can't get symbol offset because it doesn't have a label in this linker object.");

	if(symbol->instruction_index < struct_asm_instruction_ptr_list_size(&linker_object->instructions)){
		/*  Label with instruction after it */
		return struct_asm_instruction_ptr_list_get(&linker_object->instructions, symbol->instruction_index)->post_linking_offset;
	}else{
		if(struct_asm_instruction_ptr_list_size(&linker_object->instructions)){
			/*  Label with no instruction after it */
			unsigned int size_minus_one = struct_asm_instruction_ptr_list_size(&linker_object->instructions) -1;
			struct asm_instruction * instruction = struct_asm_instruction_ptr_list_get(&linker_object->instructions, size_minus_one);
			/*  Offset is the offset of the previous instruction plus the size of that instruction */
			return instruction->post_linking_offset + get_instruction_size(linker_state, instruction);
		}else{
			return 0; /*  This file consists of only a label*/
		}
	}
}

unsigned int get_absolute_symbol_offset(struct linker_state * linker_state, unsigned char * identifier, struct linker_object * current_linker_object){
	struct linker_symbol * internal_symbol = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_exists(&current_linker_object->internal_symbols, identifier) ? unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(&current_linker_object->internal_symbols, identifier) : (struct linker_symbol *)0;
	if(internal_symbol && internal_symbol->is_implemented){
		return get_relative_symbol_offset(linker_state, current_linker_object, internal_symbol) + current_linker_object->linker_object_post_linking_offset;
	}else{
		struct linker_symbol * external_symbol = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_exists(&linker_state->external_symbols, identifier) ? unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(&linker_state->external_symbols, identifier) : (struct linker_symbol *)0;
		if(external_symbol && external_symbol->is_external && external_symbol->is_implemented){
			return get_relative_symbol_offset(linker_state, external_symbol->parent_linker_object, external_symbol) + external_symbol->parent_linker_object->linker_object_post_linking_offset;
		}
		printf("Symbol %s was not found in any linker object.  Symbol referenced in file %s.\n", identifier, current_linker_object->asm_lexer_state->c.filename);
		assert(0 && "Symbol was not found in any linker object.");
	}
}

void output_artifacts(struct linker_state * linker_state, struct unsigned_char_list * file_output, struct linker_object * linker_object, struct unsigned_char_list * symbol_buffer, unsigned char * symbol_file){
	struct struct_asm_instruction_ptr_list * instructions = &linker_object->instructions;
	unsigned int size = struct_asm_instruction_ptr_list_size(instructions);
	struct asm_instruction ** data = struct_asm_instruction_ptr_list_data(instructions);
	unsigned int i;

	for(i = 0; i < size; i++){
		struct asm_instruction * instruction = data[i];
		enum asm_token_type type = instruction->op_token ? instruction->op_token->type : A_ADD; /*  TODO:  For now, if its a label, call it an add then check for null later to process output of lable. */

		if(!instruction->op_token){
			/*  A label */
			if(instruction->identifier_token && linker_state->build_target_type == BUILD_TARGET_L2_FILE){
				unsigned char * ident = copy_string(instruction->identifier_token->first_byte, instruction->identifier_token->last_byte, linker_state->memory_pool_collection);
				struct linker_symbol * symbol = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_exists(&linker_object->internal_symbols, ident) ? unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(&linker_object->internal_symbols, ident) : (struct linker_symbol *)0;
				buffered_token_output(file_output, instruction->identifier_token);
				if(symbol && !symbol->is_external){
					buffered_printf(file_output, "_%p", symbol);
				}
				buffered_printf(file_output, ":\n");
				heap_memory_pool_free(linker_state->memory_pool_collection->heap_pool, ident);
			}else{
				/*  Don't output anything */
			}
		}else if(type == A_ADD || type == A_SUB || type == A_MUL || type == A_AND || type == A_OR || type == A_DIV){
			buffered_token_output(file_output, instruction->op_token);
			buffered_printf(file_output, " ");
			buffered_token_output(file_output, instruction->rx_token);
			buffered_printf(file_output, " ");
			buffered_token_output(file_output, instruction->ry_token);
			buffered_printf(file_output, " ");
			buffered_token_output(file_output, instruction->rz_token);
			if(linker_state->build_target_type == BUILD_TARGET_L2_FILE){
				buffered_printf(file_output, ";");
			}
			buffered_printf(file_output, "\n");
		}else if(type == A_LOA || type == A_STO || type == A_NOT || type == A_SHR || type == A_SHL){
			buffered_token_output(file_output, instruction->op_token);
			buffered_printf(file_output, " ");
			buffered_token_output(file_output, instruction->rx_token);
			buffered_printf(file_output, " ");
			buffered_token_output(file_output, instruction->ry_token);
			if(linker_state->build_target_type == BUILD_TARGET_L2_FILE){
				buffered_printf(file_output, ";");
			}
			buffered_printf(file_output, "\n");
		}else if(type == A_LL){
			if(instruction->identifier_token && linker_state->build_target_type == BUILD_TARGET_L1_FILE){
				/*  ll instruction cannot load all 32 bit addresses directoy, so re-writing identifier based ll statements to handle this case  */
				unsigned int possibly_uses_r2 = instruction->rx_token->first_byte[1] == '2';
				unsigned char * ident = copy_string(instruction->identifier_token->first_byte, instruction->identifier_token->last_byte, linker_state->memory_pool_collection);
				unsigned int absolute_offset = get_absolute_symbol_offset(linker_state, ident, linker_object);
				unsigned char * target_register = copy_string(instruction->rx_token->first_byte, instruction->rx_token->last_byte, linker_state->memory_pool_collection);
				unsigned char * temp_register = possibly_uses_r2 ? (unsigned char *)"r1" : (unsigned char *)"r2";
				(void)(absolute_offset + 1);
				heap_memory_pool_free(linker_state->memory_pool_collection->heap_pool, ident);
				buffered_printf(file_output, "sub SP SP WR\n");
				buffered_printf(file_output, "sto SP %s\n", temp_register);
				buffered_printf(file_output, "ll %s 0x%X\n", target_register, (0xFFFF0000 & (absolute_offset * 4)) >> 16);
				buffered_printf(file_output, "ll %s 0x10\n", temp_register);
				buffered_printf(file_output, "shl %s %s\n", target_register, temp_register);
				buffered_printf(file_output, "ll %s 0x%X\n", temp_register, 0xFFFF & (absolute_offset * 4));
				buffered_printf(file_output, "or %s %s %s\n", target_register, target_register, temp_register);
				buffered_printf(file_output, "loa %s SP\n", temp_register);
				buffered_printf(file_output, "add SP SP WR\n");
				heap_memory_pool_free(linker_state->memory_pool_collection->heap_pool, target_register);
			}else if(instruction->identifier_token && linker_state->build_target_type == BUILD_TARGET_L2_FILE){
				unsigned char * ident = copy_string(instruction->identifier_token->first_byte, instruction->identifier_token->last_byte, linker_state->memory_pool_collection);
				struct linker_symbol * symbol = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_exists(&linker_object->internal_symbols, ident) ? unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(&linker_object->internal_symbols, ident) : (struct linker_symbol *)0;
				buffered_token_output(file_output, instruction->op_token);
				heap_memory_pool_free(linker_state->memory_pool_collection->heap_pool, ident);
				buffered_printf(file_output, " ");
				buffered_token_output(file_output, instruction->rx_token);
				buffered_printf(file_output, " ");
				buffered_token_output(file_output, instruction->identifier_token);
				if(symbol && !symbol->is_external){
					buffered_printf(file_output, "_%p", symbol);
				}
				buffered_printf(file_output, ";\n");
			}else{
				unsigned int hex_value = parse_hexidecimal_string(instruction->number_token);
				buffered_token_output(file_output, instruction->op_token);
				buffered_printf(file_output, " ");
				buffered_token_output(file_output, instruction->rx_token);
				buffered_printf(file_output, " ");
				if(!(hex_value <= MAX_LL_CONSTANT)){
					printf("LL constant too large: %X on line %d in file %s\n", hex_value, linker_object->current_line, linker_object->asm_lexer_state->c.filename);
					assert(0);
				}
				buffered_printf(file_output, "0x%X", hex_value);
				if(linker_state->build_target_type == BUILD_TARGET_L2_FILE){
					buffered_printf(file_output, ";");
				}
				buffered_printf(file_output, "\n");
			}
		}else if(type == A_BEQ || type == A_BLT){
			if(instruction->identifier_token && linker_state->build_target_type == BUILD_TARGET_L1_FILE){
				unsigned char * ident = copy_string(instruction->identifier_token->first_byte, instruction->identifier_token->last_byte, linker_state->memory_pool_collection);
				unsigned int absolute_offset = get_absolute_symbol_offset(linker_state, ident, linker_object);
				heap_memory_pool_free(linker_state->memory_pool_collection->heap_pool, ident);
				buffered_token_output(file_output, instruction->op_token);
				buffered_printf(file_output, " ");
				buffered_token_output(file_output, instruction->rx_token);
				buffered_printf(file_output, " ");
				buffered_token_output(file_output, instruction->ry_token);
				buffered_printf(file_output, " ");
				buffered_printf(file_output, "1");
				buffered_printf(file_output, "\n");
				buffered_printf(file_output, "beq ZR ZR 2\n"); /* If we're not going to branch, skip the 2 far jump instructions */
				buffered_printf(file_output, "loa PC PC\n");
				buffered_printf(file_output, "dw 0x%X\n", absolute_offset * 4);
			}else if(instruction->identifier_token && linker_state->build_target_type == BUILD_TARGET_L2_FILE){
				unsigned char * ident = copy_string(instruction->identifier_token->first_byte, instruction->identifier_token->last_byte, linker_state->memory_pool_collection);
				struct linker_symbol * symbol = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_exists(&linker_object->internal_symbols, ident) ? unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(&linker_object->internal_symbols, ident) : (struct linker_symbol *)0;
				buffered_token_output(file_output, instruction->op_token);
				heap_memory_pool_free(linker_state->memory_pool_collection->heap_pool, ident);
				buffered_printf(file_output, " ");
				buffered_token_output(file_output, instruction->rx_token);
				buffered_printf(file_output, " ");
				buffered_token_output(file_output, instruction->ry_token);
				buffered_printf(file_output, " ");
				buffered_token_output(file_output, instruction->identifier_token);
				if(symbol && !symbol->is_external){
					buffered_printf(file_output, "_%p", symbol);
				}
				buffered_printf(file_output, ";\n");
			}else{
				unsigned int constant_value = parse_decimal_string(instruction->number_token);
				buffered_token_output(file_output, instruction->op_token);
				buffered_printf(file_output, " ");
				buffered_token_output(file_output, instruction->rx_token);
				buffered_printf(file_output, " ");
				buffered_token_output(file_output, instruction->ry_token);
				buffered_printf(file_output, " ");
				if(instruction->number_token_is_negative){
					buffered_printf(file_output, "-");
				}
				assert(constant_value <= MAX_BRANCH_POS || (instruction->number_token_is_negative && constant_value <= MAX_BRANCH_NEG));
				buffered_printf(file_output, "%i", constant_value);
				if(linker_state->build_target_type == BUILD_TARGET_L2_FILE){
					buffered_printf(file_output, ";");
				}
				buffered_printf(file_output, "\n");
			}
		}else if(type == A_DW){
			buffered_token_output(file_output, instruction->op_token);
			buffered_printf(file_output, " ");
			if(instruction->identifier_token && linker_state->build_target_type == BUILD_TARGET_L1_FILE){
				unsigned char * ident = copy_string(instruction->identifier_token->first_byte, instruction->identifier_token->last_byte, linker_state->memory_pool_collection);
				unsigned int absolute_offset = get_absolute_symbol_offset(linker_state, ident, linker_object);
				buffered_printf(file_output, "0x%X", absolute_offset * 4);
				heap_memory_pool_free(linker_state->memory_pool_collection->heap_pool, ident);
			}else if(instruction->identifier_token && linker_state->build_target_type == BUILD_TARGET_L2_FILE){
				unsigned char * ident = copy_string(instruction->identifier_token->first_byte, instruction->identifier_token->last_byte, linker_state->memory_pool_collection);
				struct linker_symbol * symbol = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_exists(&linker_object->internal_symbols, ident) ? unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(&linker_object->internal_symbols, ident) : (struct linker_symbol *)0;
				buffered_token_output(file_output, instruction->identifier_token);
				if(symbol && !symbol->is_external){
					buffered_printf(file_output, "_%p", symbol);
				}
				heap_memory_pool_free(linker_state->memory_pool_collection->heap_pool, ident);
			}else{
				buffered_token_output(file_output, instruction->number_token);
			}
			if(linker_state->build_target_type == BUILD_TARGET_L2_FILE){
				buffered_printf(file_output, ";");
			}
			buffered_printf(file_output, "\n");
		}else if(type == A_SW){
			buffered_token_output(file_output, instruction->op_token);
			buffered_printf(file_output, " ");
			if(instruction->identifier_token){
				assert(0 && "Unexpected lablel with skip words instruction.");
			}else{
				buffered_token_output(file_output, instruction->number_token);
			}
			if(linker_state->build_target_type == BUILD_TARGET_L2_FILE){
				buffered_printf(file_output, ";");
			}
			buffered_printf(file_output, "\n");
		}else{
			assert(0 && "Unknown instruction type.");
		}
	}

	if(symbol_file){
		struct unsigned_char_ptr_list external_keys = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_keys(&linker_state->external_symbols);
		struct unsigned_char_ptr_list internal_keys = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_keys(&linker_object->internal_symbols);
		unsigned int external_keys_size = unsigned_char_ptr_list_size(&external_keys);
		unsigned int internal_keys_size = unsigned_char_ptr_list_size(&internal_keys);
		unsigned int g;
		buffered_printf(symbol_buffer, "Internal symbols for file %s:\n", linker_object->asm_lexer_state->c.filename);
		for(g = 0; g < internal_keys_size; g++){
			unsigned char * id = unsigned_char_ptr_list_get(&internal_keys, g);
			unsigned int absolute_offset = get_absolute_symbol_offset(linker_state, id, linker_object);
			buffered_printf(symbol_buffer, "0x%08X %s\n", absolute_offset * 4, id);
		}
		unsigned_char_ptr_list_destroy(&internal_keys);

		buffered_printf(symbol_buffer, "External symbols for file %s:\n", linker_object->asm_lexer_state->c.filename);
		for(g = 0; g < external_keys_size; g++){
			unsigned char * id = unsigned_char_ptr_list_get(&external_keys, g);
			struct linker_symbol * external_symbol = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(&linker_state->external_symbols, id);
			/*  Only output external symbols that are used in this file */
			if(external_symbol->parent_linker_object == linker_object){
				unsigned int absolute_offset = get_absolute_symbol_offset(linker_state, id, external_symbol->parent_linker_object);
				buffered_printf(symbol_buffer, "0x%08X %s\n", absolute_offset * 4, id);
			}
		}
		unsigned_char_ptr_list_destroy(&external_keys);
	}
}

void free_symbol_map(struct linker_state * linker_state, struct unsigned_char_ptr_to_struct_linker_symbol_ptr_map * map){
	struct unsigned_char_ptr_list keys = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_keys(map);
	unsigned int size = unsigned_char_ptr_list_size(&keys);
	unsigned int i;
	for(i = 0; i < size; i++){
		struct_linker_symbol_memory_pool_free(linker_state->memory_pool_collection->struct_linker_symbol_pool, unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(map, unsigned_char_ptr_list_get(&keys, i)));
	}
	/*  Need to do this twice because we're deleting the data under the keys of the map */
	for(i = 0; i < size; i++){
		heap_memory_pool_free(linker_state->memory_pool_collection->heap_pool, unsigned_char_ptr_list_get(&keys, i));
	}
	unsigned_char_ptr_to_struct_linker_symbol_ptr_map_destroy(map);
	unsigned_char_ptr_list_destroy(&keys);
}

void linker_state_create(struct linker_state * linker_state, struct memory_pool_collection * memory_pool_collection, struct unsigned_char_ptr_list * in_files, unsigned char * out_file, unsigned char * symbol_file, enum build_target_type build_target_type){
	linker_state->starting_offset = 0;
	linker_state->memory_pool_collection = memory_pool_collection;
	linker_state->in_files = in_files;
	linker_state->out_file = out_file;
	linker_state->symbol_file = symbol_file;
	linker_state->build_target_type = build_target_type;
	unsigned_char_ptr_to_struct_linker_symbol_ptr_map_create(&linker_state->external_symbols);

	unsigned_char_list_create(&linker_state->file_output);
	unsigned_char_list_create(&linker_state->symbol_output);
	unsigned_char_list_create(&linker_state->asm_lexer_output);
	struct_unsigned_char_list_ptr_list_create(&linker_state->input_file_buffers);
	struct_asm_lexer_state_ptr_list_create(&linker_state->lexer_states);
	struct_linker_object_ptr_list_create(&linker_state->linker_objects);
	struct_linker_object_ptr_list_create(&linker_state->reordered_linker_objects);
}

void linker_state_destroy(struct linker_state * linker_state){
	free_symbol_map(linker_state, &linker_state->external_symbols);
	unsigned_char_list_destroy(&linker_state->asm_lexer_output);
	unsigned_char_list_destroy(&linker_state->file_output);
	unsigned_char_list_destroy(&linker_state->symbol_output);
	struct_asm_lexer_state_ptr_list_destroy(&linker_state->lexer_states);
	struct_unsigned_char_list_ptr_list_destroy(&linker_state->input_file_buffers);
	struct_linker_object_ptr_list_destroy(&linker_state->linker_objects);
	struct_linker_object_ptr_list_destroy(&linker_state->reordered_linker_objects);
}

void set_all_post_linking_offsets(struct linker_state * linker_state){
	unsigned int i;
	unsigned int next_linker_object_post_linking_offset = linker_state->starting_offset;
	/*  Resolve all the post-linking offsets in the re-ordered list of linker objects */
	for(i = 0; i < struct_linker_object_ptr_list_size(&linker_state->reordered_linker_objects); i++){
		struct linker_object * obj = struct_linker_object_ptr_list_get(&linker_state->reordered_linker_objects, i);
		if(obj->is_relocatable){
			/*  If it is non-relocatable, the offset is already decided. */
			obj->linker_object_post_linking_offset = next_linker_object_post_linking_offset;
			next_linker_object_post_linking_offset += set_post_linking_offsets(linker_state, obj);
		}else{
			/* Skip some area between the last object and the non-relocatable object that follows */
			next_linker_object_post_linking_offset = obj->linker_object_post_linking_offset + set_post_linking_offsets(linker_state, obj); 
		}
	}
}

void do_link_to_l1(struct linker_state * linker_state){
	unsigned int i;
	unsigned int next_linker_object_post_linking_offset = linker_state->starting_offset;
	/*  Re-order them to give priority to the placement of fixed offset objects */
	reorder_linker_objects(linker_state, &linker_state->linker_objects, &linker_state->reordered_linker_objects, linker_state->starting_offset);

	set_all_post_linking_offsets(linker_state);

	buffered_printf(&linker_state->file_output, "OFFSET 0x%X\n", linker_state->starting_offset);

	/*  Output all of the objects linked together  */
	for(i = 0; i < struct_linker_object_ptr_list_size(&linker_state->reordered_linker_objects); i++){
		struct linker_object * obj = struct_linker_object_ptr_list_get(&linker_state->reordered_linker_objects, i);
		unsigned int current_linker_object_size = get_linker_object_size(linker_state, obj);
		if(next_linker_object_post_linking_offset <= obj->linker_object_post_linking_offset){
			/* Skip some area between the last object and the non-relocatable object that follows */
			unsigned int unused_start = next_linker_object_post_linking_offset;
			unsigned int unused_end = obj->linker_object_post_linking_offset;
			unsigned int sw_size = next_linker_object_post_linking_offset < obj->linker_object_post_linking_offset ? obj->linker_object_post_linking_offset - next_linker_object_post_linking_offset : 0;
			unsigned int start = next_linker_object_post_linking_offset + sw_size;
			unsigned int end = start + current_linker_object_size;
			if(sw_size){
				printf("0x%08X to 0x%08X is unused space between linker objects.\n", 4 * unused_start, 4 * unused_end -1);
			}
			if(start == end){
				printf("Linker object %s is empty.\n", obj->asm_lexer_state->c.filename);
			}else{
				printf("0x%08X to 0x%08X is linker object %s.\n", 4 * start, 4 * end -1, obj->asm_lexer_state->c.filename);
			}
			next_linker_object_post_linking_offset = obj->linker_object_post_linking_offset + current_linker_object_size;
			buffered_printf(&linker_state->file_output, "sw 0x%X\n", sw_size);
		}else{
			printf("Impossible overlap in linker objects: %s, 0x%08X goes over 0x%08X.\n", obj->asm_lexer_state->c.filename, 4 * next_linker_object_post_linking_offset, 4 * obj->linker_object_post_linking_offset);
			assert(0 && "This should never happen.");
		}
		output_artifacts(linker_state, &linker_state->file_output, obj, &linker_state->symbol_output, linker_state->symbol_file);
	}
}

void do_link_to_l2(struct linker_state * linker_state){
	unsigned int i;
	struct unsigned_char_ptr_list external_symbols = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_keys(&linker_state->external_symbols);
	for(i = 0; i < struct_linker_object_ptr_list_size(&linker_state->linker_objects); i++){
		struct_linker_object_ptr_list_add_end(&linker_state->reordered_linker_objects, struct_linker_object_ptr_list_get(&linker_state->linker_objects, i));
	}
	set_all_post_linking_offsets(linker_state);
	buffered_printf(&linker_state->file_output, "OFFSET RELOCATABLE\n");
	for(i = 0; i < struct_linker_object_ptr_list_size(&linker_state->reordered_linker_objects); i++){
		struct linker_object * obj = struct_linker_object_ptr_list_get(&linker_state->reordered_linker_objects, i);
		struct unsigned_char_ptr_list internal_symbols = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_keys(&obj->internal_symbols);
		unsigned int j;
		for(j = 0; j < unsigned_char_ptr_list_size(&internal_symbols); j++){
			unsigned char * key = unsigned_char_ptr_list_get(&internal_symbols, j);
			struct linker_symbol * sym = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(&obj->internal_symbols, key);
			if(sym->is_required && sym->is_implemented){
				buffered_printf(&linker_state->file_output, "REQUIRES, IMPLEMENTS");
			}else if(sym->is_required){
				buffered_printf(&linker_state->file_output, "REQUIRES");
			}else if(sym->is_implemented){
				buffered_printf(&linker_state->file_output, "IMPLEMENTS");
			}else{
				assert(0 && "Not expected.");
			}

			buffered_printf(&linker_state->file_output, " INTERNAL %s_%p;\n", key, sym); /*  Make the internal symbol unique by appending the symbol pointer */
		}
		unsigned_char_ptr_list_destroy(&internal_symbols);
	}
	for(i = 0; i < unsigned_char_ptr_list_size(&external_symbols); i++){
		unsigned char * key = unsigned_char_ptr_list_get(&external_symbols, i);
		struct linker_symbol * sym = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(&linker_state->external_symbols, key);
		if(sym->is_required && sym->is_implemented){
			buffered_printf(&linker_state->file_output, "REQUIRES, IMPLEMENTS");
		}else if(sym->is_required){
			buffered_printf(&linker_state->file_output, "REQUIRES");
		}else if(sym->is_implemented){
			buffered_printf(&linker_state->file_output, "IMPLEMENTS");
		}else{
			assert(0 && "Not expected.");
		}

		buffered_printf(&linker_state->file_output, " EXTERNAL %s;\n", key);
	}

	unsigned_char_ptr_list_destroy(&external_symbols);
	for(i = 0; i < struct_linker_object_ptr_list_size(&linker_state->reordered_linker_objects); i++){
		struct linker_object * obj = struct_linker_object_ptr_list_get(&linker_state->reordered_linker_objects, i);
		output_artifacts(linker_state, &linker_state->file_output, obj, &linker_state->symbol_output, linker_state->symbol_file);
	}
}

int do_link(struct memory_pool_collection * memory_pool_collection, struct unsigned_char_ptr_list * in_files, unsigned char * out_file, unsigned char * symbol_file, enum build_target_type build_target_type){
	unsigned int i;
	struct linker_state linker_state;

	linker_state_create(&linker_state, memory_pool_collection, in_files, out_file, symbol_file, build_target_type);

	/*  Load and parse all the linker objects */
	for(i = 0; i < unsigned_char_ptr_list_size(linker_state.in_files); i++){
		struct unsigned_char_list * file_input = (struct unsigned_char_list *)malloc(sizeof(struct unsigned_char_list));
		struct asm_lexer_state * asm_lexer_state = (struct asm_lexer_state *)malloc(sizeof(struct asm_lexer_state));
		struct linker_object * linker_object;
		unsigned_char_list_create(file_input);
		add_file_to_buffer(file_input, (char*)unsigned_char_ptr_list_get(linker_state.in_files, i));

		asm_lexer_state->c.memory_pool_collection = linker_state.memory_pool_collection;
		asm_lexer_state->c.buffered_output = &linker_state.asm_lexer_output;

		lex_asm(asm_lexer_state, unsigned_char_ptr_list_get(linker_state.in_files, i), unsigned_char_list_data(file_input), unsigned_char_list_size(file_input));
		linker_object = process_assembly(&linker_state, asm_lexer_state);

		struct_unsigned_char_list_ptr_list_add_end(&linker_state.input_file_buffers, file_input);
		struct_linker_object_ptr_list_add_end(&linker_state.linker_objects, linker_object);
		struct_asm_lexer_state_ptr_list_add_end(&linker_state.lexer_states, asm_lexer_state);
	}

	switch(build_target_type){
		case BUILD_TARGET_L1_FILE:{
			do_link_to_l1(&linker_state);
			break;
		}case BUILD_TARGET_L2_FILE:{
			do_link_to_l2(&linker_state);
			break;
		}default:{
			assert(0 && "Case not considered.");
		}
	}

	/*  Clean up all the resources */
	for(i = 0; i < unsigned_char_ptr_list_size(linker_state.in_files); i++){
		unsigned int j;
		struct unsigned_char_list * file_input = struct_unsigned_char_list_ptr_list_get(&linker_state.input_file_buffers, i);
		struct linker_object * linker_object = struct_linker_object_ptr_list_get(&linker_state.linker_objects, i);
		struct asm_lexer_state * asm_lexer_state = struct_asm_lexer_state_ptr_list_get(&linker_state.lexer_states, i);
		unsigned_char_list_destroy(file_input);
		free(file_input);

		free_symbol_map(&linker_state, &linker_object->internal_symbols);

		for(j = 0; j < struct_asm_instruction_ptr_list_size(&linker_object->instructions); j++){
			struct_asm_instruction_memory_pool_free(linker_state.memory_pool_collection->struct_asm_instruction_pool, struct_asm_instruction_ptr_list_get(&linker_object->instructions, j));
		}
		struct_asm_instruction_ptr_list_destroy(&linker_object->instructions);
		free(linker_object);

		destroy_asm_lexer_state(asm_lexer_state);
		free(asm_lexer_state);
	}


	/*  Output to a file our final product with all objects linked together */
	output_buffer_to_file(&linker_state.file_output, (char*)linker_state.out_file);
	if(linker_state.symbol_file){
		output_buffer_to_file(&linker_state.symbol_output, (char*)linker_state.symbol_file);
	}

	linker_state_destroy(&linker_state);

	return EXIT_SUCCESS;
}
