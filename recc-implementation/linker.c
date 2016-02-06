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
#include "linker.h"

void buffered_token_output(struct unsigned_char_list *, struct l2_lexer_token *);
void free_symbol_map(struct linker_state *, struct unsigned_char_ptr_to_struct_linker_symbol_ptr_map *);
unsigned int set_post_linking_offsets(struct linker_state *, struct linker_file * linker_file);
unsigned int set_pre_linking_offsets(struct linker_state *, struct linker_file * linker_file);
unsigned int get_l2_item_size(struct linker_state *, struct l2_item *);
unsigned int parse_hexidecimal_string(struct l2_lexer_token *);
unsigned int parse_decimal_string(struct l2_lexer_token *);
unsigned int get_linker_file_size(struct linker_state *, struct linker_file *);
int struct_linker_file_ptr_cmp_indirect(struct linker_file **, struct linker_file **);
int struct_linker_file_ptr_cmp(struct linker_file *, struct linker_file *);
void linker_file_destroy(struct linker_state *, struct linker_file *);
void add_items_of_type(struct linker_state *, enum l2_token_type, struct struct_l2_item_ptr_list *, struct struct_l2_item_ptr_list *, struct struct_start_end_list *, unsigned int *, struct linker_file *);

void reorder_linker_files(struct linker_state *, struct struct_linker_file_ptr_list *, struct struct_linker_file_ptr_list *, unsigned int);

unsigned int is_non_descending_order(struct struct_linker_file_ptr_list *);

unsigned int is_non_descending_order(struct struct_linker_file_ptr_list * linker_files){
	unsigned int i;
	struct linker_file * prev = 0;
	for(i = 0; i < struct_linker_file_ptr_list_size(linker_files); i++){
		if(i != 0){
			if(struct_linker_file_ptr_list_get(linker_files, i)->linker_file_post_linking_offset < prev->linker_file_post_linking_offset){
				return 0;
			}
		}
		prev = struct_linker_file_ptr_list_get(linker_files, i);
	}
	return 1;
}

int struct_linker_file_ptr_cmp_indirect(struct linker_file ** a, struct linker_file ** b){
	if((*a)->linker_file_post_linking_offset < (*b)->linker_file_post_linking_offset){
		return -1;
	}else if((*a)->linker_file_post_linking_offset > (*b)->linker_file_post_linking_offset){
		return 1;
	}else{
		return 0;
	}
}

int struct_linker_file_ptr_cmp(struct linker_file * a, struct linker_file * b){
	if(a < b){
		return -1;
	}else if(a > b){
		return 1;
	}else{
		return 0;
	}
}

void reorder_linker_files(struct linker_state * state, struct struct_linker_file_ptr_list * linker_files, struct struct_linker_file_ptr_list * reordered_linker_files, unsigned int starting_offset){
	/*  Some linker files must be put at a fixed address, and some are relocatable.
	    Re-order them so that they can be contiguously placed (with possible space in-between)
	*/
	unsigned int i;
	struct struct_linker_file_ptr_list relocatable_linker_files;
	struct struct_linker_file_ptr_list non_relocatable_linker_files;
	unsigned int previous_linker_file_end = starting_offset; /*  Offsets in #words */
	unsigned int current_linker_file_start;
	struct linker_file * end_linker_file = (struct linker_file *)0;

	struct_linker_file_ptr_list_create(&relocatable_linker_files);
	struct_linker_file_ptr_list_create(&non_relocatable_linker_files);

	/*  Sort the linker files into relocatable and non-relocatable */
	for(i = 0; i < struct_linker_file_ptr_list_size(linker_files); i++){
		struct linker_file * file = struct_linker_file_ptr_list_get(linker_files, i);
		switch(file->relocation_type){
			case RELOCATION_TYPE_RELOCATABLE:{
				struct_linker_file_ptr_list_add_end(&relocatable_linker_files, file);
				break;
			}case RELOCATION_TYPE_ADDRESS:{
				struct_linker_file_ptr_list_add_end(&non_relocatable_linker_files, file);
				break;
			}case RELOCATION_TYPE_END:{
				assert(!end_linker_file);
				end_linker_file = file;
				break;
			}default:{
				assert(0 && "Unknown relocation type.");
			}
		}
	}
	
	struct_linker_file_ptr_merge_sort(struct_linker_file_ptr_list_data(&non_relocatable_linker_files), struct_linker_file_ptr_list_size(&non_relocatable_linker_files), struct_linker_file_ptr_cmp_indirect);
	/*  Make sure there is no conflict between non-relocatable linker files */
	{
		struct linker_file * prev = 0;
		for(i = 0; i < struct_linker_file_ptr_list_size(&non_relocatable_linker_files); i++){
			struct linker_file * current = struct_linker_file_ptr_list_get(&non_relocatable_linker_files, i);
			unsigned int prev_offset = prev ? prev->linker_file_post_linking_offset + get_linker_file_size(state, prev) : 0;
			unsigned int current_file_size = get_linker_file_size(state, current);
			unsigned int end_byte = (current->linker_file_post_linking_offset + current_file_size) -1;
			/*  Will it overwrite the previous non-relocatable linker file? */
			assert(prev_offset <= current->linker_file_post_linking_offset);
			
			if(current_file_size){
				printf("Non-relocatable linker file %s must occupy %08X - %08X\n", current->l2_lexer_state->c.filename, 4 * current->linker_file_post_linking_offset, 4*end_byte);
			}else{
				printf("Non-relocatable linker file %s is empty so it is ignored.\n", current->l2_lexer_state->c.filename);
			}
			prev = current;
		}
	}
	/*  Fit in all the relocatable linker files between the non relocatable ones */
	for(i = 0; i < struct_linker_file_ptr_list_size(&non_relocatable_linker_files); i++){
		struct linker_file * non_relocatable = struct_linker_file_ptr_list_get(&non_relocatable_linker_files, i);
		unsigned int non_relocatable_size = get_linker_file_size(state, non_relocatable);
		current_linker_file_start = non_relocatable->linker_file_post_linking_offset;  /*  Offset in # words */
		while(struct_linker_file_ptr_list_size(&relocatable_linker_files)){
			unsigned int j;
			/* Look for an file that will fit */
			for(j = 0; j < struct_linker_file_ptr_list_size(&relocatable_linker_files); j++){
				struct linker_file * relocatable = struct_linker_file_ptr_list_get(&relocatable_linker_files, j);
				unsigned int relocatable_size = get_linker_file_size(state, relocatable);
				if(previous_linker_file_end + relocatable_size <= current_linker_file_start){
					unsigned int start = previous_linker_file_end;
					unsigned int end = start + relocatable_size;
					struct_linker_file_ptr_list_add_end(reordered_linker_files, relocatable);
					if(start == end){
						printf("Linker file %s is empty\n", relocatable->l2_lexer_state->c.filename);
					}else{
						printf("0x%08X to 0x%08X will fit linker file %s before non-relocatable linker file %s, which starts at 0x%08X.\n",4 * start, 4 * end -1, relocatable->l2_lexer_state->c.filename, non_relocatable->l2_lexer_state->c.filename, 4 * current_linker_file_start);
					}
					/* Don't try to add this one multiple times */
					struct_linker_file_ptr_list_remove_all(&relocatable_linker_files, relocatable, struct_linker_file_ptr_cmp);
					previous_linker_file_end = previous_linker_file_end + relocatable_size;
					break; /* List size changed, do iteration again. */
				}
			}
			if(struct_linker_file_ptr_list_size(&relocatable_linker_files) == j){
				/*  None of the relocatable files will fit if we got through the loop */
				/*  Consume the empty region after last relocatable file  */
				if((previous_linker_file_end + (current_linker_file_start - previous_linker_file_end))){
					printf("0x%08X to 0x%08X will be unused.\n", 4 * previous_linker_file_end, 4 * (previous_linker_file_end + (current_linker_file_start - previous_linker_file_end)) -1);
				}
				previous_linker_file_end += current_linker_file_start - previous_linker_file_end;
				break;
			}
		}
		/*  No more of the relocatable files will fit, put the non relocatable file after them */
		struct_linker_file_ptr_list_add_end(reordered_linker_files, non_relocatable);
		/*  Next file needs to know where it can start from */
		printf("0x%08X to 0x%08X will fit non-relocatable linker file %s\n", 4 * previous_linker_file_end, 4 * (previous_linker_file_end + non_relocatable_size) -1, non_relocatable->l2_lexer_state->c.filename);
		previous_linker_file_end = previous_linker_file_end + non_relocatable_size;
	}


	/*  Add all the leftover relocatable linker files at the end */
	for(i = 0; i < struct_linker_file_ptr_list_size(&relocatable_linker_files); i++){
		struct linker_file * relocatable = struct_linker_file_ptr_list_get(&relocatable_linker_files, i);
		struct_linker_file_ptr_list_add_end(reordered_linker_files, relocatable);
	}

	if(end_linker_file){
		printf("Linker file %s must be last.\n", end_linker_file->l2_lexer_state->c.filename);
		struct_linker_file_ptr_list_add_end(reordered_linker_files, end_linker_file);
	}
	/*  Now all the linker files are guaranteed to fit. */


	struct_linker_file_ptr_list_destroy(&relocatable_linker_files);
	struct_linker_file_ptr_list_destroy(&non_relocatable_linker_files);
}

unsigned int parse_decimal_string(struct l2_lexer_token * token){
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

unsigned int parse_hexidecimal_string(struct l2_lexer_token * token){
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

unsigned int get_l2_item_size(struct linker_state * state, struct l2_item * item){
	enum l2_token_type type = item->op_token->type;

	if(type == L2_COLON_CHAR){
		/*  A label */
		return 0;
	}

	if ((type == L2_BLT || type == L2_BEQ) && item->identifier_token && state->build_target_type == BUILD_TARGET_L1_FILE){ /* Gets re-written for long jumps */
		return 4;
	}

	if (type == L2_SW){
		return parse_hexidecimal_string(item->number_token); /* Size depends on number of words skipped */
	}

	return 1; /* 1 word */
}

unsigned int get_linker_file_size(struct linker_state * state, struct linker_file * linker_file){
	unsigned int i;
	unsigned int num_l2_items = struct_l2_item_ptr_list_size(&linker_file->l2_items);
	unsigned int current_offset = 0; /*  Offset in # words */
	for(i = 0; i < num_l2_items; i++){
		struct l2_item * instruction = struct_l2_item_ptr_list_get(&linker_file->l2_items, i);
		current_offset += get_l2_item_size(state, instruction);
	}
	return current_offset;
}

unsigned int set_post_linking_offsets(struct linker_state * state, struct linker_file * linker_file){
	unsigned int i;
	unsigned int num_l2_items = struct_l2_item_ptr_list_size(&linker_file->l2_items);
	unsigned int current_offset = 0; /*  Offset in # words */
	for(i = 0; i < num_l2_items; i++){
		struct l2_item * instruction = struct_l2_item_ptr_list_get(&linker_file->l2_items, i);
		instruction->post_linking_offset = current_offset;
		current_offset += get_l2_item_size(state, instruction);
	}
	return current_offset;
}

unsigned int set_pre_linking_offsets(struct linker_state * state, struct linker_file * linker_file){
	unsigned int i;
	unsigned int num_l2_items = struct_l2_item_ptr_list_size(&linker_file->l2_items);
	unsigned int current_offset = 0; /*  Offset in # words */
	for(i = 0; i < num_l2_items; i++){
		struct l2_item * instruction = struct_l2_item_ptr_list_get(&linker_file->l2_items, i);
		instruction->pre_linking_offset = current_offset;
		current_offset += get_l2_item_size(state, instruction);
	}
	return current_offset;
}

void set_symbol_l2_item_pointer(struct linker_state * state, struct linker_file * linker_file, struct l2_lexer_token * token, struct l2_item * pointer, unsigned int original_index){
	unsigned char * identifier_str = copy_string(token->first_byte, token->last_byte, state->memory_pool_collection);
	struct linker_symbol * internal_symbol = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_exists(&linker_file->internal_symbols, identifier_str) ? unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(&linker_file->internal_symbols, identifier_str) : (struct linker_symbol *)0;
	if(internal_symbol){
		if(internal_symbol->observed_as_implemented){
			printf("Internal label %s re-declared on line %d in file %s\n", identifier_str, linker_file->current_line, linker_file->l2_lexer_state->c.filename);
			assert(0 && "Trying to re-declare label in this linker file.");
		}else{
			internal_symbol->original_l2_item_index = original_index;
			internal_symbol->l2_item = pointer;
			internal_symbol->observed_as_implemented = 1; /*  Observed a label for this symbol in the file */
		}
	}else{
		struct linker_symbol * external_symbol = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_exists(&state->external_symbols, identifier_str) ? unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(&state->external_symbols, identifier_str) : (struct linker_symbol *)0;
		if(external_symbol){
			if(external_symbol->observed_as_implemented){
				printf("Internal label %s re-declared on line %d in file %s\n", identifier_str, linker_file->current_line, linker_file->l2_lexer_state->c.filename);
				assert(0 && "Trying to re-declare label in this linker file.");
			}else{
				external_symbol->l2_item = pointer;
				external_symbol->original_l2_item_index = original_index;
				external_symbol->observed_as_implemented = 1; /*  Observed a label for this symbol in the file */
			}
		}else{
			printf("Undeclared identifier %s on line %d in file %s\n", identifier_str, linker_file->current_line, linker_file->l2_lexer_state->c.filename);
			assert(0 && "Trying to set offset of unknown symbol.");
		}
	}
	heap_memory_pool_free(state->memory_pool_collection->heap_pool, identifier_str);
}

void verify_symbol_declaration(struct linker_state * state, struct linker_file * linker_file, struct l2_lexer_token * token){
	/*  Make sure that a symbol has been declared before it is implemented */
	unsigned char * identifier_str = copy_string(token->first_byte, token->last_byte, state->memory_pool_collection);
	unsigned int internal_symbol = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_exists(&linker_file->internal_symbols, identifier_str);
	unsigned int external_symbol = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_exists(&state->external_symbols, identifier_str);
	if(!(internal_symbol || external_symbol)){
		printf("Undeclared identifier %s on line %d in file %s\n", identifier_str, linker_file->current_line, linker_file->l2_lexer_state->c.filename);
		assert(0 && "Found symbol without forward declaration.");
	}
	heap_memory_pool_free(state->memory_pool_collection->heap_pool, identifier_str);
}

void add_internal_linker_symbol(struct linker_state * state, struct linker_file * linker_file, struct l2_lexer_token * token, unsigned int is_required, unsigned int is_implemented){
	unsigned char * identifier_str = copy_string(token->first_byte, token->last_byte, state->memory_pool_collection);
	struct linker_symbol * new_symbol;
	struct linker_symbol * existing_symbol = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_exists(&linker_file->internal_symbols, identifier_str) ? unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(&linker_file->internal_symbols, identifier_str) : (struct linker_symbol *)0;
	if(existing_symbol){
		assert(!(existing_symbol->is_implemented && is_implemented && "Cannot implement internal symbol multiple times."));
		existing_symbol->is_implemented = existing_symbol->is_implemented ? existing_symbol->is_implemented : is_implemented;
		existing_symbol->is_required = existing_symbol->is_required ? existing_symbol->is_required : is_required;
		existing_symbol->is_external = 0;
		heap_memory_pool_free(state->memory_pool_collection->heap_pool, identifier_str);
		return;
	}
	new_symbol = struct_linker_symbol_memory_pool_malloc(state->memory_pool_collection->struct_linker_symbol_pool);
	new_symbol->is_implemented = is_implemented;
	new_symbol->is_required = is_required;
	new_symbol->is_external = 0;
	new_symbol->observed_as_implemented = 0; /*  We haven't see a lable for this symbol yet */
	new_symbol->parent_linker_file = linker_file;
	unsigned_char_ptr_to_struct_linker_symbol_ptr_map_put(&linker_file->internal_symbols, identifier_str, new_symbol);
}

void add_external_linker_symbol(struct linker_state * state, struct linker_file * linker_file, struct l2_lexer_token * token, unsigned int is_required, unsigned int is_implemented){
	unsigned char * identifier_str = copy_string(token->first_byte, token->last_byte, state->memory_pool_collection);
	struct linker_symbol * new_symbol;
	struct linker_symbol * existing_symbol = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_exists(&state->external_symbols, identifier_str) ? unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(&state->external_symbols, identifier_str) : (struct linker_symbol *)0;
	if(existing_symbol){
		if(existing_symbol->is_implemented && is_implemented){
			printf("Detected duplicate external symbol %s re-implemented on line %d in file %s\n", identifier_str, linker_file->current_line, linker_file->l2_lexer_state->c.filename);
			printf("The previously declared external symbol was declared in file %s\n", existing_symbol->parent_linker_file->l2_lexer_state->c.filename);
			assert(0 && "Cannot implement external symbol multiple times.");
		}
		existing_symbol->is_implemented = existing_symbol->is_implemented ? existing_symbol->is_implemented : is_implemented;
		existing_symbol->is_required = existing_symbol->is_required ? existing_symbol->is_required : is_required;
		existing_symbol->is_external = 1;
		existing_symbol->parent_linker_file = is_implemented ? linker_file : existing_symbol->parent_linker_file;
		heap_memory_pool_free(state->memory_pool_collection->heap_pool, identifier_str);
		return;
	}
	new_symbol = struct_linker_symbol_memory_pool_malloc(state->memory_pool_collection->struct_linker_symbol_pool);
	new_symbol->is_implemented = is_implemented;
	new_symbol->is_required = is_required;
	new_symbol->is_external = 1;
	new_symbol->observed_as_implemented = 0; /*  We haven't see a lable for this symbol yet */
	new_symbol->parent_linker_file = is_implemented ? linker_file : (struct linker_file *)0;
	unsigned_char_ptr_to_struct_linker_symbol_ptr_map_put(&state->external_symbols, identifier_str, new_symbol);
}

void process_statement(struct l2_parser_node *, struct linker_state *, struct linker_file *);

void process_statement(struct l2_parser_node * n, struct linker_state * state, struct linker_file * linker_file){
	switch(n->first_child->type){
		case L2_INSTRUCTION_STATEMENT:{
			struct l2_parser_node * op_code = n->first_child->first_child;
			struct l2_item * new_instruction = struct_l2_item_memory_pool_malloc(state->memory_pool_collection->struct_l2_item_pool);
			new_instruction->op_token = op_code->l2_lexer_token;
			switch(op_code->l2_lexer_token->type){
				/*  Uses case statement fallthrough: */
				case L2_ADD:; case L2_SUB:; case L2_MUL:; case L2_DIV:; case L2_AND:; case L2_OR:{
					new_instruction->rx_token = op_code->next->l2_lexer_token;
					new_instruction->ry_token = op_code->next->next->l2_lexer_token;
					new_instruction->rz_token = op_code->next->next->next->l2_lexer_token;
					break;
				}case L2_LOA:; case L2_STO:; case L2_NOT:; case L2_SHR:; case L2_SHL:{
					new_instruction->rx_token = op_code->next->l2_lexer_token;
					new_instruction->ry_token = op_code->next->next->l2_lexer_token;
					break;
				}case L2_BEQ:; case L2_BLT:{
					struct l2_parser_node * branch_distance;
					new_instruction->rx_token = op_code->next->l2_lexer_token;
					new_instruction->ry_token = op_code->next->next->l2_lexer_token;
					branch_distance = op_code->next->next->next;
					if(branch_distance->first_child->l2_lexer_token->type == L2_IDENTIFIER){
						new_instruction->identifier_token = branch_distance->first_child->l2_lexer_token;
						new_instruction->number_token = 0;
						new_instruction->number_token_is_negative = 0;
						verify_symbol_declaration(state, linker_file, branch_distance->first_child->l2_lexer_token);
					}else if(branch_distance->first_child->l2_lexer_token->type == L2_MINUS_CHAR){
						/*  Next one should be the decimal number */
						assert(branch_distance->first_child->next->l2_lexer_token->type == L2_CONSTANT_DECIMAL);
						new_instruction->number_token = branch_distance->first_child->next->l2_lexer_token;
						new_instruction->identifier_token = 0;
						new_instruction->number_token_is_negative = 1;
					}else if(branch_distance->first_child->l2_lexer_token->type == L2_CONSTANT_DECIMAL){
						new_instruction->number_token = branch_distance->first_child->l2_lexer_token;
						new_instruction->identifier_token = 0;
						new_instruction->number_token_is_negative = 0;
					}else{
						assert(0 && "Unknown branch distance.");
					}
					break;
				}case L2_LL:{
					new_instruction->rx_token = op_code->next->l2_lexer_token;
					new_instruction->number_token_is_negative = 0;
					new_instruction->number_token = op_code->next->next->l2_lexer_token;
					new_instruction->identifier_token = 0;
					break;
				}default:{
					printf("Note type was %s\n", get_l2_token_type_names()[op_code->l2_lexer_token->type]);
					assert(0 && "Unknown instruction.");
				}
			}
			struct_l2_item_ptr_list_add_end(&linker_file->l2_items, new_instruction);
			break;
		}case L2_LABEL_STATEMENT:{
			struct l2_parser_node * terminal = n->first_child->first_child;
			struct l2_lexer_token * identifier = terminal->l2_lexer_token;
			struct l2_item * new_instruction = struct_l2_item_memory_pool_malloc(state->memory_pool_collection->struct_l2_item_pool);
			new_instruction->op_token = terminal->next->l2_lexer_token;
			if(terminal->next->type == L2_TERMINAL && terminal->next->l2_lexer_token->type == L2_COLON_CHAR){
				set_symbol_l2_item_pointer(state, linker_file, identifier, new_instruction, struct_l2_item_ptr_list_size(&linker_file->l2_items));
				new_instruction->identifier_token = identifier;
			}else{
				assert(0 && "Expected colon.");
			}
			struct_l2_item_ptr_list_add_end(&linker_file->l2_items, new_instruction);
			break;
		}case L2_DIRECTIVE_STATEMENT:{
			struct l2_parser_node * directive = n->first_child->first_child;
			switch(directive->type){
				case L2_DEFINE_WORD_DIRECTIVE: {
				}case L2_SKIP_WORDS_DIRECTIVE: {
					struct l2_item * new_instruction = struct_l2_item_memory_pool_malloc(state->memory_pool_collection->struct_l2_item_pool);
					struct l2_lexer_token * value = directive->first_child->next->l2_lexer_token;
					new_instruction->op_token = directive->first_child->l2_lexer_token;
					if(value->type == L2_CONSTANT_HEX){
						new_instruction->number_token_is_negative = 0;
						new_instruction->number_token = value;
						new_instruction->identifier_token = 0;
					}else if(value->type == L2_IDENTIFIER){
						new_instruction->identifier_token = value;
						new_instruction->number_token = 0;
						verify_symbol_declaration(state, linker_file, value);
					}else{
						assert(0 && "Unknown value type.");
					}
					struct_l2_item_ptr_list_add_end(&linker_file->l2_items, new_instruction);
					break;
				}case L2_OFFSET_DIRECTIVE:{
					struct l2_lexer_token * value = directive->first_child->next->l2_lexer_token;
					if(value->type == L2_RELOCATABLE){
						linker_file->relocation_type = RELOCATION_TYPE_RELOCATABLE; /* All symbols are relative */
						linker_file->offset_declared = 1;
					}else if(value->type == L2_END){
						linker_file->relocation_type = RELOCATION_TYPE_END; /* Must be last linker file */
						linker_file->offset_declared = 1;
					}else if(value->type == L2_CONSTANT_HEX){
						unsigned int parsed_value = parse_hexidecimal_string(value);
						linker_file->relocation_type = RELOCATION_TYPE_ADDRESS; /* This linker file must be placed at a fixed location in memory */
						linker_file->linker_file_post_linking_offset = (parsed_value / 4); /*  Offset in # words */
						assert(parsed_value % 4 == 0 && "Value must be evenly divisible by word size.");
						linker_file->offset_declared = 1;
					}else{
						assert(0 && "Expected space.");
					}
					break;
				}case L2_SYMBOL_LINKAGE_DECLARATION:{
					unsigned int is_implemented = 0;
					unsigned int is_required = 0;
					struct l2_parser_node * first = directive->first_child;
					struct l2_parser_node * linkage_domain = (struct l2_parser_node *)0;
					if(first->type == L2_INTERFACE_SPECIFIER){
						if(first->first_child->l2_lexer_token->type == L2_REQUIRES){
							is_required = 1;
						}else if(first->first_child->l2_lexer_token->type == L2_IMPLEMENTS){
							is_implemented = 1;
						}else{
							assert(0 && "Unknown linkage specifier.");
						}
						linkage_domain = first->next;
					}
					if(first->next->type == L2_TERMINAL && first->next->l2_lexer_token->type == L2_COMMA_CHAR){
						if(first->next->next->first_child->l2_lexer_token->type == L2_REQUIRES){
							is_required = 1;
						}else if(first->next->next->first_child->l2_lexer_token->type == L2_IMPLEMENTS){
							is_implemented = 1;
						}else{
							assert(0 && "Unknown linkage specifier.");
						}
						linkage_domain = first->next->next->next;
					}
					if(linkage_domain->first_child->l2_lexer_token->type == L2_INTERNAL){
						add_internal_linker_symbol(state, linker_file, linkage_domain->next->l2_lexer_token, is_required, is_implemented);
					}else if(linkage_domain->first_child->l2_lexer_token->type == L2_EXTERNAL){
						add_external_linker_symbol(state, linker_file, linkage_domain->next->l2_lexer_token, is_required, is_implemented);
					}else{
						assert(0 && "Unknown linkage domain.");
					}
					break;
				}case L2_FUNCTION_DIRECTIVE:; case L2_VARIABLE_DIRECTIVE:; case L2_CONSTANT_DIRECTIVE:{
					struct l2_parser_node * first = directive->first_child;
					struct l2_parser_node * second = first->next;
					struct l2_parser_node * third = second->next;
					enum l2_token_type type = first->l2_lexer_token->type;
					/* TODO:  Need to implement function directive */
					struct linker_object * new_object = (struct linker_object *)malloc(sizeof(struct linker_object));
					new_object->type = type;
					new_object->start_label = copy_string(second->l2_lexer_token->first_byte, second->l2_lexer_token->last_byte, state->memory_pool_collection);
					new_object->end_label = copy_string(third->l2_lexer_token->first_byte, third->l2_lexer_token->last_byte, state->memory_pool_collection);
					struct_linker_object_ptr_list_add_end(&linker_file->object_declarations, new_object);
					break;
				}case L2_START_DIRECTIVE:{
					/* TODO:  Need to implement start directive */
					break;
				}case L2_END_DIRECTIVE:{
					/* TODO:  Need to implement end directive */
					break;
				}case L2_IMPLEMENTED_DIRECTIVE:{
					/* TODO:  Need to implement implemented directive */
					break;
				}case L2_REQUIRED_DIRECTIVE:{
					/* TODO:  Need to implement implemented directive */
					break;
				}default:{
					printf("Note type was %s\n", get_l2_node_type_names()[directive->type]);
					assert(0 && "Unknown directive.");
				}
			}
			break;
		}default:{
			printf("Note type was %s\n", get_l2_node_type_names()[n->type]);
			assert(0 && "Unknown statement.");
		}
	}
}

struct linker_file * create_linker_file(struct linker_state * state, struct l2_parser_state * l2_parser_state, struct unsigned_char_list * file_input){
	struct linker_file * linker_file = (struct linker_file *)malloc(sizeof(struct linker_file));
	struct l2_parser_node * current_node = l2_parser_state->top_node;
	struct_linker_object_ptr_list_create(&linker_file->object_declarations);
	linker_file->offset_declared = 0;
	linker_file->current_line = 1;
	linker_file->l2_parser_state = l2_parser_state;
	linker_file->l2_lexer_state = l2_parser_state->l2_lexer_state;
	linker_file->file_input = file_input;
	unsigned_char_ptr_to_struct_linker_symbol_ptr_map_create(&linker_file->internal_symbols);
	struct_l2_item_ptr_list_create(&linker_file->l2_items);

	while(current_node->type != L2_EPSILON){
		switch (current_node->type){
			case L2_FILE:{
				current_node = current_node->first_child;
				break;
			}case L2_STATEMENT_LIST:{
				current_node = current_node->first_child;
				if(current_node->type == L2_STATEMENT){
					process_statement(current_node, state, linker_file);
					current_node = current_node->next;
				}
				assert(current_node->type == L2_AFTER_STATEMENT || current_node->type == L2_EPSILON);
				break;
			}case L2_AFTER_STATEMENT:{
				current_node = current_node->first_child;
				if(current_node->type == L2_TERMINAL && current_node->l2_lexer_token->type == L2_COMMENT){
					current_node = current_node->next;
				}
				if(current_node->type == L2_TERMINAL && current_node->l2_lexer_token->type == L2_NEWLINE){
					current_node = current_node->next;
				}
				assert(current_node->type == L2_STATEMENT_LIST);
				break;
			}case L2_EPSILON:{
				break;
			}default:{
				printf("node is %s\n", get_l2_node_type_names()[current_node->type]);
				assert(0 && "Unknown l2 parser node type.");
			}
		}
	}

	if(!linker_file->offset_declared){
		printf("On line %d in file %s\n", linker_file->current_line, l2_parser_state->l2_lexer_state->c.filename); assert(0 && "offset not declared..");
	}
	return linker_file;
}

void buffered_token_output(struct unsigned_char_list * buffer, struct l2_lexer_token * t){
	add_string_to_buffer(buffer, t->first_byte, t->last_byte);
}

unsigned int get_relative_symbol_offset(struct linker_state * state, struct linker_file * linker_file, struct linker_symbol * symbol, unsigned int * found){
	(void)linker_file;
	(void)state;
	if(!symbol->observed_as_implemented){
		*found = 0;
		return 0;
	}

	*found = 1;
	return symbol->l2_item->post_linking_offset;
}

unsigned int get_absolute_symbol_offset(struct linker_state * state, unsigned char * identifier, struct linker_file * current_linker_file, unsigned int * found){
	struct linker_symbol * internal_symbol = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_exists(&current_linker_file->internal_symbols, identifier) ? unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(&current_linker_file->internal_symbols, identifier) : (struct linker_symbol *)0;
	*found = 1;
	if(internal_symbol && internal_symbol->is_implemented){
		return get_relative_symbol_offset(state, current_linker_file, internal_symbol, found) + current_linker_file->linker_file_post_linking_offset;
	}else{
		struct linker_symbol * external_symbol = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_exists(&state->external_symbols, identifier) ? unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(&state->external_symbols, identifier) : (struct linker_symbol *)0;
		if(external_symbol && external_symbol->is_external && external_symbol->is_implemented){
			return get_relative_symbol_offset(state, external_symbol->parent_linker_file, external_symbol, found) + external_symbol->parent_linker_file->linker_file_post_linking_offset;
		}
		*found = 0;
		return 0;
	}
}


void output_symbols(struct linker_state * state, struct linker_file * linker_file, struct unsigned_char_list * symbol_buffer, unsigned char * symbol_file){
	if(symbol_file){
		struct unsigned_char_ptr_list external_keys = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_keys(&state->external_symbols);
		struct unsigned_char_ptr_list internal_keys = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_keys(&linker_file->internal_symbols);
		unsigned int external_keys_size = unsigned_char_ptr_list_size(&external_keys);
		unsigned int internal_keys_size = unsigned_char_ptr_list_size(&internal_keys);
		unsigned int g;
		buffered_printf(symbol_buffer, "Internal symbols for file %s:\n", linker_file->l2_lexer_state->c.filename);
		for(g = 0; g < internal_keys_size; g++){
			unsigned char * id = unsigned_char_ptr_list_get(&internal_keys, g);
			unsigned int found;
			unsigned int absolute_offset = get_absolute_symbol_offset(state, id, linker_file, &found);
			if(found){
				buffered_printf(symbol_buffer, "0x%08X %s\n", absolute_offset * 4, id);
			}else{
				printf("Symbol %s is unimplemented.  Symbol referenced in file %s.\n", id, linker_file->l2_lexer_state->c.filename);
			}

		}
		unsigned_char_ptr_list_destroy(&internal_keys);

		buffered_printf(symbol_buffer, "External symbols for file %s:\n", linker_file->l2_lexer_state->c.filename);
		for(g = 0; g < external_keys_size; g++){
			unsigned char * id = unsigned_char_ptr_list_get(&external_keys, g);
			struct linker_symbol * external_symbol = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(&state->external_symbols, id);
			/*  Only output external symbols that are used in this file */
			if(external_symbol->parent_linker_file == linker_file){
				unsigned int found;
				unsigned int absolute_offset = get_absolute_symbol_offset(state, id, external_symbol->parent_linker_file, &found);
				if(found){
					buffered_printf(symbol_buffer, "0x%08X %s\n", absolute_offset * 4, id);
				}else{
					printf("Symbol %s is unimplemented.  Symbol referenced in file %s.\n", id, linker_file->l2_lexer_state->c.filename);
				}
			}
		}
		unsigned_char_ptr_list_destroy(&external_keys);
	}
}


void output_artifacts(struct linker_state * state, struct unsigned_char_list * file_output, struct linker_file * linker_file){
	struct struct_l2_item_ptr_list * l2_items = &linker_file->l2_items;
	unsigned int size = struct_l2_item_ptr_list_size(l2_items);
	struct l2_item ** data = struct_l2_item_ptr_list_data(l2_items);
	unsigned int i;

	for(i = 0; i < size; i++){
		struct l2_item * instruction = data[i];
		enum l2_token_type type = instruction->op_token->type;
		switch(type){
			/* Uses case statement fallthrough */
			case L2_COLON_CHAR:{
				/*  A label */
				if(instruction->identifier_token && state->build_target_type == BUILD_TARGET_L2_FILE){
					unsigned char * ident = copy_string(instruction->identifier_token->first_byte, instruction->identifier_token->last_byte, state->memory_pool_collection);
					struct linker_symbol * symbol = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_exists(&linker_file->internal_symbols, ident) ? unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(&linker_file->internal_symbols, ident) : (struct linker_symbol *)0;
					buffered_token_output(file_output, instruction->identifier_token);
					if(symbol && !symbol->is_external){
						buffered_printf(file_output, "_%p", symbol);
					}
					buffered_puts(file_output, ":\n");
					heap_memory_pool_free(state->memory_pool_collection->heap_pool, ident);
				}else{
					/*  Don't output anything */
				}
				break;
			}case L2_ADD:; case L2_SUB:; case L2_MUL:; case L2_AND:; case L2_OR:; case L2_DIV:{
				buffered_token_output(file_output, instruction->op_token);
				buffered_puts(file_output, " ");
				buffered_token_output(file_output, instruction->rx_token);
				buffered_puts(file_output, " ");
				buffered_token_output(file_output, instruction->ry_token);
				buffered_puts(file_output, " ");
				buffered_token_output(file_output, instruction->rz_token);
				if(state->build_target_type == BUILD_TARGET_L2_FILE){
					buffered_puts(file_output, ";");
				}
				buffered_puts(file_output, "\n");
				break;
			}case L2_LOA:; case L2_STO :; case L2_NOT:; case L2_SHR:; case L2_SHL:{
				buffered_token_output(file_output, instruction->op_token);
				buffered_puts(file_output, " ");
				buffered_token_output(file_output, instruction->rx_token);
				buffered_puts(file_output, " ");
				buffered_token_output(file_output, instruction->ry_token);
				if(state->build_target_type == BUILD_TARGET_L2_FILE){
					buffered_puts(file_output, ";");
				}
				buffered_puts(file_output, "\n");
				break;
			}case L2_LL:{
				unsigned int hex_value = parse_hexidecimal_string(instruction->number_token);
				buffered_token_output(file_output, instruction->op_token);
				buffered_puts(file_output, " ");
				buffered_token_output(file_output, instruction->rx_token);
				buffered_puts(file_output, " ");
				if(!(hex_value <= MAX_LL_CONSTANT)){
					printf("LL constant too large: %X on line %d in file %s\n", hex_value, linker_file->current_line, linker_file->l2_lexer_state->c.filename);
					assert(0);
				}
				buffered_printf(file_output, "0x%X", hex_value);
				if(state->build_target_type == BUILD_TARGET_L2_FILE){
					buffered_puts(file_output, ";");
				}
				buffered_puts(file_output, "\n");
				break;
			}case L2_BEQ:;  case L2_BLT:{
				if(instruction->identifier_token && state->build_target_type == BUILD_TARGET_L1_FILE){
					unsigned char * ident = copy_string(instruction->identifier_token->first_byte, instruction->identifier_token->last_byte, state->memory_pool_collection);
					unsigned int found;
					unsigned int absolute_offset = get_absolute_symbol_offset(state, ident, linker_file, &found);
					heap_memory_pool_free(state->memory_pool_collection->heap_pool, ident);
					buffered_token_output(file_output, instruction->op_token);
					buffered_puts(file_output, " ");
					buffered_token_output(file_output, instruction->rx_token);
					buffered_puts(file_output, " ");
					buffered_token_output(file_output, instruction->ry_token);
					buffered_puts(file_output, " ");
					buffered_puts(file_output, "1");
					buffered_puts(file_output, "\n");
					buffered_puts(file_output, "beq ZR ZR 2\n"); /* If we're not going to branch, skip the 2 far jump l2_items */
					buffered_puts(file_output, "loa PC PC\n");
					if(found){
						buffered_printf(file_output, "DW 0x%X\n", absolute_offset * 4);
					}else{
						buffered_puts(file_output, "UNRESOLVED 0x0\n");
						printf("Leaving symbol %s unresolved in output file %s.\n", ident, state->out_file);
					}
				}else if(instruction->identifier_token && state->build_target_type == BUILD_TARGET_L2_FILE){
					unsigned char * ident = copy_string(instruction->identifier_token->first_byte, instruction->identifier_token->last_byte, state->memory_pool_collection);
					struct linker_symbol * symbol = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_exists(&linker_file->internal_symbols, ident) ? unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(&linker_file->internal_symbols, ident) : (struct linker_symbol *)0;
					buffered_token_output(file_output, instruction->op_token);
					heap_memory_pool_free(state->memory_pool_collection->heap_pool, ident);
					buffered_puts(file_output, " ");
					buffered_token_output(file_output, instruction->rx_token);
					buffered_puts(file_output, " ");
					buffered_token_output(file_output, instruction->ry_token);
					buffered_puts(file_output, " ");
					buffered_token_output(file_output, instruction->identifier_token);
					if(symbol && !symbol->is_external){
						buffered_printf(file_output, "_%p", symbol);
					}
					buffered_puts(file_output, ";\n");
				}else{
					unsigned int constant_value = parse_decimal_string(instruction->number_token);
					buffered_token_output(file_output, instruction->op_token);
					buffered_puts(file_output, " ");
					buffered_token_output(file_output, instruction->rx_token);
					buffered_puts(file_output, " ");
					buffered_token_output(file_output, instruction->ry_token);
					buffered_puts(file_output, " ");
					if(instruction->number_token_is_negative){
						buffered_puts(file_output, "-");
					}
					assert(constant_value <= MAX_BRANCH_POS || (instruction->number_token_is_negative && constant_value <= MAX_BRANCH_NEG));
					buffered_printf(file_output, "%i", constant_value);
					if(state->build_target_type == BUILD_TARGET_L2_FILE){
						buffered_puts(file_output, ";");
					}
					buffered_puts(file_output, "\n");
				}
				break;
			}case L2_DW:{
				if(instruction->identifier_token && state->build_target_type == BUILD_TARGET_L1_FILE){
					unsigned char * ident = copy_string(instruction->identifier_token->first_byte, instruction->identifier_token->last_byte, state->memory_pool_collection);
					unsigned int found;
					unsigned int absolute_offset = get_absolute_symbol_offset(state, ident, linker_file, &found);

					if(found){
						buffered_printf(file_output, "DW 0x%X", absolute_offset * 4);
					}else{
						buffered_puts(file_output, "UNRESOLVED 0x0");
						printf("Leaving symbol %s unresolved in output file %s.\n", ident, state->out_file);
					}

					heap_memory_pool_free(state->memory_pool_collection->heap_pool, ident);
				}else if(instruction->identifier_token && state->build_target_type == BUILD_TARGET_L2_FILE){
					unsigned char * ident = copy_string(instruction->identifier_token->first_byte, instruction->identifier_token->last_byte, state->memory_pool_collection);
					struct linker_symbol * symbol = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_exists(&linker_file->internal_symbols, ident) ? unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(&linker_file->internal_symbols, ident) : (struct linker_symbol *)0;
					buffered_puts(file_output, "DW ");
					buffered_token_output(file_output, instruction->identifier_token);
					if(symbol && !symbol->is_external){
						buffered_printf(file_output, "_%p", symbol);
					}
					heap_memory_pool_free(state->memory_pool_collection->heap_pool, ident);
				}else{
					buffered_puts(file_output, "DW ");
					buffered_token_output(file_output, instruction->number_token);
				}
				if(state->build_target_type == BUILD_TARGET_L2_FILE){
					buffered_puts(file_output, ";");
				}
				buffered_puts(file_output, "\n");
				break;
			}case L2_UNRESOLVED:{
				buffered_puts(file_output, "UNRESOLVED ");
				buffered_token_output(file_output, instruction->number_token);
				buffered_puts(file_output, "\n");
				break;
			}case L2_SW:{
				buffered_token_output(file_output, instruction->op_token);
				buffered_puts(file_output, " ");
				if(instruction->identifier_token){
					assert(0 && "Unexpected lablel with skip words instruction.");
				}else{
					buffered_token_output(file_output, instruction->number_token);
				}
				if(state->build_target_type == BUILD_TARGET_L2_FILE){
					buffered_puts(file_output, ";");
				}
				buffered_puts(file_output, "\n");
				break;
			}default:{
				assert(0 && "Unknown instruction type.");
			}
		}
	}
}

void free_symbol_map(struct linker_state * state, struct unsigned_char_ptr_to_struct_linker_symbol_ptr_map * map){
	struct unsigned_char_ptr_list keys = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_keys(map);
	unsigned int size = unsigned_char_ptr_list_size(&keys);
	unsigned int i;
	for(i = 0; i < size; i++){
		struct_linker_symbol_memory_pool_free(state->memory_pool_collection->struct_linker_symbol_pool, unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(map, unsigned_char_ptr_list_get(&keys, i)));
	}
	/*  Need to do this twice because we're deleting the data under the keys of the map */
	for(i = 0; i < size; i++){
		heap_memory_pool_free(state->memory_pool_collection->heap_pool, unsigned_char_ptr_list_get(&keys, i));
	}
	unsigned_char_ptr_to_struct_linker_symbol_ptr_map_destroy(map);
	unsigned_char_ptr_list_destroy(&keys);
}

void linker_state_create(struct linker_state * state, struct memory_pool_collection * memory_pool_collection, struct unsigned_char_ptr_list * in_files, unsigned char * out_file, unsigned char * symbol_file, enum build_target_type build_target_type, unsigned char * offset, unsigned int page_align_permission_regions, unsigned int only_metadata){
	struct l2_lexer_token t;
	t.first_byte = offset;
	t.last_byte = get_null_terminator(offset) -1;
	state->starting_offset = build_target_type == BUILD_TARGET_L1_FILE ? parse_hexidecimal_string(&t) : 0;
	state->memory_pool_collection = memory_pool_collection;
	state->in_files = in_files;
	state->out_file = out_file;
	state->symbol_file = symbol_file;
	state->build_target_type = build_target_type;
	state->offset = offset;
	state->page_align_permission_regions = page_align_permission_regions;
	state->only_metadata = only_metadata;
	unsigned_char_ptr_to_struct_linker_symbol_ptr_map_create(&state->external_symbols);

	unsigned_char_list_create(&state->file_output);
	unsigned_char_list_create(&state->symbol_output);
	unsigned_char_list_create(&state->l2_lexer_output);
	struct_linker_file_ptr_list_create(&state->linker_files);
	struct_linker_file_ptr_list_create(&state->reordered_linker_files);
	struct_l2_item_ptr_list_create(&state->created_sw_items);
	struct_linker_region_list_create(&state->regions);
}

void linker_file_destroy(struct linker_state * state, struct linker_file * file){
	unsigned int j = 0;
	unsigned int num_symbols = struct_linker_object_ptr_list_size(&file->object_declarations);
	unsigned int num_items = struct_l2_item_ptr_list_size(&file->l2_items);
	struct l2_parser_state * l2_parser_state = file->l2_parser_state;
	struct l2_lexer_state * l2_lexer_state = l2_parser_state->l2_lexer_state;
	for(j = 0; j < num_symbols; j++){
		struct linker_object * obj = struct_linker_object_ptr_list_get(&file->object_declarations, j);
		heap_memory_pool_free(state->memory_pool_collection->heap_pool, obj->start_label);
		heap_memory_pool_free(state->memory_pool_collection->heap_pool, obj->end_label);
		free(obj);
	}
	struct_linker_object_ptr_list_destroy(&file->object_declarations);

	unsigned_char_list_destroy(file->file_input);

	free_symbol_map(state, &file->internal_symbols);

	for(j = 0; j < num_items; j++){
		struct_l2_item_memory_pool_free(state->memory_pool_collection->struct_l2_item_pool, struct_l2_item_ptr_list_get(&file->l2_items, j));
	}
	struct_l2_item_ptr_list_destroy(&file->l2_items);

	destroy_l2_lexer_state(l2_lexer_state);
	destroy_l2_parser_state(l2_parser_state);
	free(file->file_input);
	free(l2_lexer_state);
	free(l2_parser_state);
	free(file);
}

void linker_state_destroy(struct linker_state * state){
	unsigned int i;
	unsigned int num_files = struct_linker_file_ptr_list_size(&state->linker_files);
	unsigned int num_created_sw_items = struct_l2_item_ptr_list_size(&state->created_sw_items);
	free_symbol_map(state, &state->external_symbols);
	unsigned_char_list_destroy(&state->l2_lexer_output);
	unsigned_char_list_destroy(&state->file_output);
	unsigned_char_list_destroy(&state->symbol_output);

	for(i = 0; i < num_created_sw_items; i++){
		struct l2_item * it = struct_l2_item_ptr_list_get(&state->created_sw_items, i);
		heap_memory_pool_free(state->memory_pool_collection->heap_pool, it->number_token->first_byte);
		heap_memory_pool_free(state->memory_pool_collection->heap_pool, it->op_token->first_byte);
		struct_l2_lexer_token_memory_pool_free(state->memory_pool_collection->struct_l2_lexer_token_pool, it->number_token);
		struct_l2_lexer_token_memory_pool_free(state->memory_pool_collection->struct_l2_lexer_token_pool, it->op_token);
		/* it gets destroyed in linker file destroy */
	}
	struct_l2_item_ptr_list_destroy(&state->created_sw_items);

	for(i = 0; i < num_files; i++){
		struct linker_file * file = struct_linker_file_ptr_list_get(&state->linker_files, i);
		linker_file_destroy(state, file);
	}
	struct_linker_file_ptr_list_destroy(&state->linker_files);
	struct_linker_file_ptr_list_destroy(&state->reordered_linker_files);
	struct_linker_region_list_destroy(&state->regions);
}

void set_all_post_linking_offsets(struct linker_state * state){
	unsigned int i;
	unsigned int next_linker_file_post_linking_offset = state->starting_offset;
	/*  Resolve all the post-linking offsets in the re-ordered list of linker files */
	for(i = 0; i < struct_linker_file_ptr_list_size(&state->reordered_linker_files); i++){
		struct linker_file * file = struct_linker_file_ptr_list_get(&state->reordered_linker_files, i);
		switch(file->relocation_type){
			case RELOCATION_TYPE_RELOCATABLE:; case RELOCATION_TYPE_END:{
				/*  If it is non-relocatable, the offset is already decided. */
				file->linker_file_post_linking_offset = next_linker_file_post_linking_offset;
				next_linker_file_post_linking_offset += set_post_linking_offsets(state, file);
				break;
			}case RELOCATION_TYPE_ADDRESS:{
				/* Skip some area between the last file and the non-relocatable file that follows */
				next_linker_file_post_linking_offset = file->linker_file_post_linking_offset + set_post_linking_offsets(state, file); 
				break;
			}default:{
				assert(0 && "Unknown relocation type.");
			}
		}
	}
}

struct l2_lexer_token * make_sw_op_token(struct linker_state *);
struct l2_lexer_token * make_sw_op_token(struct linker_state * state){
	/* TODO: Hopefully these functions can be removed later */
	struct l2_lexer_token * tok = struct_l2_lexer_token_memory_pool_malloc(state->memory_pool_collection->struct_l2_lexer_token_pool);
	unsigned char * str = create_formatted_string(state->memory_pool_collection, 2, "SW");
	tok->type = L2_SW;
	tok->first_byte = str;
	tok->last_byte = &str[1];
	return tok;
}

struct l2_lexer_token * make_hex_number_token(struct linker_state *, unsigned int);
struct l2_lexer_token * make_hex_number_token(struct linker_state * state, unsigned int value){
	struct l2_lexer_token * tok = struct_l2_lexer_token_memory_pool_malloc(state->memory_pool_collection->struct_l2_lexer_token_pool);
	unsigned char * str = create_formatted_string(state->memory_pool_collection, 20, "0x%X", value);
	unsigned char * start = str;
	unsigned char * end = (unsigned char *)0;
	while(*str) {end = str; str++;} /* last byte */
	tok->type = L2_CONSTANT_HEX;
	tok->first_byte = start;
	tok->last_byte = end;
	return tok;
}

void add_items_of_type(struct linker_state * state, enum l2_token_type type, struct struct_l2_item_ptr_list * source, struct struct_l2_item_ptr_list * dest, struct struct_start_end_list * se_list, unsigned int * offset_so_far, struct linker_file * file){
	unsigned int j;
	unsigned int current_offset = 0;
	unsigned int num_start_ends = struct_start_end_list_size(se_list);
	unsigned int words_in_page = (OP_CPU_PAGE_SIZE / 4);
	unsigned int alignment_words_needed;
	unsigned int offset_at_start = *offset_so_far;
	struct l2_item * sw_padding;
	struct linker_region new_region;
	/*  Group regions by type */
	for(j = 0; j < num_start_ends; j++){
		struct start_end current = struct_start_end_list_get(se_list, j);
		struct start_end next;
		j++;
		next = struct_start_end_list_get(se_list, j);
		if(current.source->type == type){
			unsigned int k;
			assert(next.source->type == type);
			for(k = current.symbol->original_l2_item_index; k <= next.symbol->original_l2_item_index; k++){
				struct l2_item * it = struct_l2_item_ptr_list_get(source, k);
				struct_l2_item_ptr_list_add_end(dest, it);
				current_offset += get_l2_item_size(state, it);
			}
		}
	}
	/*  Add the size of the instructions in this region */

	alignment_words_needed = (current_offset % words_in_page == 0) ? 0 : (words_in_page - (current_offset % words_in_page));
	printf("Needed to add 0x%X to page align region.\n", alignment_words_needed);

	sw_padding = struct_l2_item_memory_pool_malloc(state->memory_pool_collection->struct_l2_item_pool);
	sw_padding->op_token = make_sw_op_token(state);
	sw_padding->number_token_is_negative = 0;
	sw_padding->number_token = make_hex_number_token(state, alignment_words_needed);
	sw_padding->identifier_token = 0;
	struct_l2_item_ptr_list_add_end(dest, sw_padding);
	struct_l2_item_ptr_list_add_end(&state->created_sw_items, sw_padding); /* Need for garbage collection later */

	*offset_so_far = *offset_so_far + (current_offset + alignment_words_needed) * 4;
	new_region.source_file = file;
	new_region.start = offset_at_start;
	new_region.length = *offset_so_far - offset_at_start;
	switch(type){
		case L2_FUNCTION:{
			new_region.permissions = LEVEL_1_PAGE_TABLE_ENTRY_EXECUTE_BIT | LEVEL_1_PAGE_TABLE_ENTRY_READ_BIT;
			break;
		}case L2_CONSTANT:{
			new_region.permissions = LEVEL_1_PAGE_TABLE_ENTRY_READ_BIT;
			break;
		}case L2_VARIABLE:{
			new_region.permissions = LEVEL_1_PAGE_TABLE_ENTRY_WRITE_BIT | LEVEL_1_PAGE_TABLE_ENTRY_READ_BIT;
			break;
		}default:{
			assert(0 && "Unknown premission type.");
			break;
		}
	}
	if(new_region.length){ /*  Don't add empty regions. */
		struct_linker_region_list_add_end(&state->regions, new_region);
		printf("Region occupies 0x%X to 0x%X in linker file %s.\n", new_region.start, new_region.start + new_region.length, file->l2_lexer_state->c.filename);
	}
}

void group_and_page_align_linker_files(struct linker_state * state){
	/*  Re-orders linker files so instructions can be grouped based on what type of object they are and adds padding to the end to
	    make sure the size of reach region is divsible by the size of a page. */
	unsigned int i;
	for(i = 0; i < struct_linker_file_ptr_list_size(&state->linker_files); i++){
		unsigned int offset_so_far = 0;
		unsigned int j;
		struct linker_file * file = struct_linker_file_ptr_list_get(&state->linker_files, i);
		struct struct_start_end_list starts_and_ends;
		unsigned int current_offset = 0;
		unsigned int num_start_ends;
		unsigned int num_items = struct_l2_item_ptr_list_size(&file->l2_items);
		struct struct_l2_item_ptr_list new_items; /*  New list of re-ordered items */
		struct_l2_item_ptr_list_create(&new_items);
		struct_start_end_list_create(&starts_and_ends);
		set_pre_linking_offsets(state, file);
		for(j = 0; j < struct_linker_object_ptr_list_size(&file->object_declarations); j++){
			struct start_end se1;
			struct start_end se2;
			struct linker_object * obj = struct_linker_object_ptr_list_get(&file->object_declarations, j);
			struct linker_symbol * start_symbol = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_exists(&file->internal_symbols, obj->start_label) ? unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(&file->internal_symbols, obj->start_label) : unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(&state->external_symbols, obj->start_label);
			struct linker_symbol * end_symbol = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_exists(&file->internal_symbols, obj->end_label) ? unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(&file->internal_symbols, obj->end_label) : unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(&state->external_symbols, obj->end_label);
			assert(start_symbol->observed_as_implemented && "Symbol not implemented");
			assert(end_symbol->observed_as_implemented && "Symbol not implemented");

			se1.symbol = start_symbol;
			se1.is_start = 1;
			se1.source = obj;

			se2.symbol = end_symbol;
			se2.is_start = 0;
			se2.source = obj;

			struct_start_end_list_add_end(&starts_and_ends, se1);
			struct_start_end_list_add_end(&starts_and_ends, se2);

			if(file->relocation_type == RELOCATION_TYPE_ADDRESS){
				/*  Make sure its offset is page aligned (because it can't be changed) */
				assert(((file->linker_file_post_linking_offset * 4) % OP_CPU_PAGE_SIZE) == 0);
			}
		}
		/*  Sort the array of start + ends */
		/*  Requires stable sorting */
		num_start_ends = struct_start_end_list_size(&starts_and_ends);
		struct_start_end_merge_sort(struct_start_end_list_data(&starts_and_ends), num_start_ends, struct_start_end_ptr_compare);
		for(j = 0; j < num_start_ends; j++){
			/*  Check for complete space usage and monotonic and complete array */
			struct start_end current = struct_start_end_list_get(&starts_and_ends, j);
			if(j % 2 == 0){   /*  Even  */
				assert(current.is_start);
				assert((current_offset <= current.symbol->l2_item->pre_linking_offset) && "Not monotonic.");
				if(!((current.symbol->l2_item->pre_linking_offset - current_offset) == 0)){
					printf("In file %s before label %s there were stray instructions.\n", file->l2_lexer_state->c.filename, current.source->start_label);
					assert(0 && "Extra instructions not belonging to any object.");
				}
			}else{  /*  Odd  */
				if(current.is_start){
					printf("In file %s saw start label %s (%dth start or end) but should have seen an end label.\n", file->l2_lexer_state->c.filename, current.source->start_label, j);
					assert(0 && "Unexpected start label.");
				}
			}
			current_offset = current.symbol->l2_item->pre_linking_offset;
		}

		if(num_items){
			/*  Make sure the offset took us to the end of all the items */
			struct l2_item * last_item = struct_l2_item_ptr_list_get(&file->l2_items, num_items -1);
			unsigned int last_item_offset = last_item->pre_linking_offset;
			unsigned int file_size = get_l2_item_size(state, last_item);
			if(!num_start_ends){
				printf("In file %s There should be at least one object in this file since it is not empty.\n", file->l2_lexer_state->c.filename);
				assert(0);
			}
			assert((last_item_offset + file_size) == struct_start_end_list_get(&starts_and_ends, num_start_ends -1).symbol->l2_item->pre_linking_offset);
		}

		add_items_of_type(state, L2_FUNCTION, &file->l2_items, &new_items, &starts_and_ends, &offset_so_far, file);
		add_items_of_type(state, L2_VARIABLE, &file->l2_items, &new_items, &starts_and_ends, &offset_so_far, file);
		add_items_of_type(state, L2_CONSTANT, &file->l2_items, &new_items, &starts_and_ends, &offset_so_far, file);

		/*  Destroy the original ordering */
		struct_l2_item_ptr_list_destroy(&file->l2_items);
		/*  Replace it with the new one*/
		file->l2_items = new_items;

		struct_start_end_list_destroy(&starts_and_ends);
	}
}

void do_link_to_l1(struct linker_state * state){
	unsigned int current_external_id = 0; /*  Id for this external symbol, unique to this file */
	unsigned int i;
	unsigned int next_linker_file_post_linking_offset = state->starting_offset;
	struct unsigned_char_ptr_list external_symbols = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_keys(&state->external_symbols);
	/*  Make functions, variables and constants aligned so we can set different premissions on those sections */
	group_and_page_align_linker_files(state);
	/*  Re-order them to give priority to the placement of fixed offset files */
	reorder_linker_files(state, &state->linker_files, &state->reordered_linker_files, state->starting_offset);

	set_all_post_linking_offsets(state);

	buffered_printf(&state->file_output, "OFFSET 0x%X\n", state->starting_offset);

	for(i = 0; i < struct_linker_region_list_size(&state->regions); i++){
		struct linker_region region = struct_linker_region_list_get(&state->regions, i);
		buffered_printf(&state->file_output, "REGION 0x0;\nSTART 0x%X;\nEND 0x%X;\nPERMISSION 0x%X;\n", (region.source_file->linker_file_post_linking_offset * 4) + region.start, (region.source_file->linker_file_post_linking_offset * 4) + region.start + region.length, region.permissions);
	}

	for(i = 0; i < unsigned_char_ptr_list_size(&external_symbols); i++){
		unsigned char * key = unsigned_char_ptr_list_get(&external_symbols, i);
		unsigned int words_printed = 0;
		unsigned int len = (unsigned int)strlen((char*)key) + 1;
		unsigned int str_words = len / 4;
		unsigned int extra = len - (str_words * 4);
		buffered_printf(&state->file_output, "EXTERNAL 0x%X\n", current_external_id);
		current_external_id++;
		while(words_printed < str_words){
			/*  TODO: Endian dependent code */
			buffered_printf(&state->file_output, "STRING 0x%2X%2X%2X%2X\n", key[words_printed * 4 + 3], key[words_printed * 4 + 2], key[words_printed * 4 + 1], key[words_printed * 4]);
			words_printed++;
		}
		if(extra){
			unsigned int last0 = (words_printed * 4 + 0) < len ? (unsigned int)key[words_printed * 4 + 0] : 0;
			unsigned int last1 = (words_printed * 4 + 1) < len ? (unsigned int)key[words_printed * 4 + 1] : 0;
			unsigned int last2 = (words_printed * 4 + 2) < len ? (unsigned int)key[words_printed * 4 + 2] : 0;
			unsigned int last3 = (words_printed * 4 + 3) < len ? (unsigned int)key[words_printed * 4 + 3] : 0;
			unsigned int all = last0 + (last1 << 8)  + (last2 << 16) + (last3 << 24);
			buffered_printf(&state->file_output, "STRING 0x%8X\n", all);
		}
	}
	unsigned_char_ptr_list_destroy(&external_symbols);

	for(i = 0; i < struct_linker_file_ptr_list_size(&state->reordered_linker_files); i++){
		struct linker_file * file = struct_linker_file_ptr_list_get(&state->reordered_linker_files, i);
		unsigned int current_linker_file_size = get_linker_file_size(state, file);
		if(next_linker_file_post_linking_offset <= file->linker_file_post_linking_offset){
			/* Skip some area between the last file and the non-relocatable file that follows */
			unsigned int unused_start = next_linker_file_post_linking_offset;
			unsigned int unused_end = file->linker_file_post_linking_offset;
			unsigned int sw_size = next_linker_file_post_linking_offset < file->linker_file_post_linking_offset ? file->linker_file_post_linking_offset - next_linker_file_post_linking_offset : 0;
			unsigned int start = next_linker_file_post_linking_offset + sw_size;
			unsigned int end = start + current_linker_file_size;
			if(sw_size){
				printf("0x%08X to 0x%08X is unused space between linker files.\n", 4 * unused_start, 4 * unused_end -1);
			}
			if(start == end){
				printf("Linker file %s is empty.\n", file->l2_lexer_state->c.filename);
			}else{
				printf("0x%08X to 0x%08X is linker file %s.\n", 4 * start, 4 * end -1, file->l2_lexer_state->c.filename);
			}
			file->unused_words_before = sw_size;
			next_linker_file_post_linking_offset = file->linker_file_post_linking_offset + current_linker_file_size;
		}else{
			printf("Impossible overlap in linker files: %s, 0x%08X goes over 0x%08X.\n", file->l2_lexer_state->c.filename, 4 * next_linker_file_post_linking_offset, 4 * file->linker_file_post_linking_offset);
			assert(0 && "This should never happen.");
		}
	}

	if(!state->only_metadata){
		/*  Output all of the files linked together  */
		for(i = 0; i < struct_linker_file_ptr_list_size(&state->reordered_linker_files); i++){
			struct linker_file * file = struct_linker_file_ptr_list_get(&state->reordered_linker_files, i);
			buffered_printf(&state->file_output, "SW 0x%X\n", file->unused_words_before);
			output_artifacts(state, &state->file_output, file);
		}
	}

	for(i = 0; i < struct_linker_file_ptr_list_size(&state->reordered_linker_files); i++){
		struct linker_file * file = struct_linker_file_ptr_list_get(&state->reordered_linker_files, i);
		output_symbols(state, file, &state->symbol_output, state->symbol_file);
	}
}

void do_link_to_l2(struct linker_state * state){
	unsigned int i;
	struct unsigned_char_ptr_list external_symbols = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_keys(&state->external_symbols);
	for(i = 0; i < struct_linker_file_ptr_list_size(&state->linker_files); i++){
		struct_linker_file_ptr_list_add_end(&state->reordered_linker_files, struct_linker_file_ptr_list_get(&state->linker_files, i));
	}
	set_all_post_linking_offsets(state);
	buffered_printf(&state->file_output, "OFFSET %s\n", state->offset);

	for(i = 0; i < struct_linker_file_ptr_list_size(&state->reordered_linker_files); i++){
		struct linker_file * file = struct_linker_file_ptr_list_get(&state->reordered_linker_files, i);
		unsigned int j;
		unsigned int num_objects = struct_linker_object_ptr_list_size(&file->object_declarations);
		for(j = 0; j < num_objects; j++){
			const char * object_type;
			struct linker_object * obj = struct_linker_object_ptr_list_get(&file->object_declarations, j);
			struct linker_symbol * sym_start = (struct linker_symbol *)0;
			struct linker_symbol * sym_end = (struct linker_symbol *)0;
			unsigned char * start = obj->start_label;
			unsigned char * end = obj->end_label;
			if(unsigned_char_ptr_to_struct_linker_symbol_ptr_map_exists(&file->internal_symbols, obj->start_label)){
				sym_start = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(&file->internal_symbols, obj->start_label);
				sym_end = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(&file->internal_symbols, obj->end_label);
			}
			switch(obj->type){
				case L2_FUNCTION:{
					object_type = "FUNCTION";
					break;
				}case L2_VARIABLE:{
					object_type = "VARIABLE";
					break;
				}case L2_CONSTANT:{
					object_type = "CONSTANT";
					break;
				}default:{
					assert(0 && "Unknown type.");
				}
			}
			/*  Re-name any internal symbol labels if necessary */
			if(sym_start){
				buffered_printf(&state->file_output, "%s %s_%p %s_%p;\n", object_type, start, sym_start, end, sym_end);
			}else{
				buffered_printf(&state->file_output, "%s %s %s;\n", object_type, start, end);
			}
		}
	}

	for(i = 0; i < struct_linker_file_ptr_list_size(&state->reordered_linker_files); i++){
		struct linker_file * file = struct_linker_file_ptr_list_get(&state->reordered_linker_files, i);
		struct unsigned_char_ptr_list internal_symbols = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_keys(&file->internal_symbols);
		unsigned int j;

		for(j = 0; j < unsigned_char_ptr_list_size(&internal_symbols); j++){
			unsigned char * key = unsigned_char_ptr_list_get(&internal_symbols, j);
			struct linker_symbol * sym = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(&file->internal_symbols, key);
			if(sym->is_required && sym->is_implemented){
				buffered_puts(&state->file_output, "REQUIRES, IMPLEMENTS");
			}else if(sym->is_required){
				buffered_puts(&state->file_output, "REQUIRES");
			}else if(sym->is_implemented){
				buffered_puts(&state->file_output, "IMPLEMENTS");
			}else{
				assert(0 && "Not expected.");
			}

			buffered_printf(&state->file_output, " INTERNAL %s_%p;\n", key, sym); /*  Make the internal symbol unique by appending the symbol pointer */
		}
		unsigned_char_ptr_list_destroy(&internal_symbols);
	}
	for(i = 0; i < unsigned_char_ptr_list_size(&external_symbols); i++){
		unsigned char * key = unsigned_char_ptr_list_get(&external_symbols, i);
		struct linker_symbol * sym = unsigned_char_ptr_to_struct_linker_symbol_ptr_map_get(&state->external_symbols, key);
		if(sym->is_required && sym->is_implemented){
			buffered_puts(&state->file_output, "REQUIRES, IMPLEMENTS");
		}else if(sym->is_required){
			buffered_puts(&state->file_output, "REQUIRES");
		}else if(sym->is_implemented){
			buffered_puts(&state->file_output, "IMPLEMENTS");
		}else{
			assert(0 && "Not expected.");
		}

		buffered_printf(&state->file_output, " EXTERNAL %s;\n", key);
	}

	unsigned_char_ptr_list_destroy(&external_symbols);
	for(i = 0; i < struct_linker_file_ptr_list_size(&state->reordered_linker_files); i++){
		struct linker_file * file = struct_linker_file_ptr_list_get(&state->reordered_linker_files, i);
		output_artifacts(state, &state->file_output, file);
	}

	for(i = 0; i < struct_linker_file_ptr_list_size(&state->reordered_linker_files); i++){
		struct linker_file * file = struct_linker_file_ptr_list_get(&state->reordered_linker_files, i);
		output_symbols(state, file, &state->symbol_output, state->symbol_file);
	}
}

int do_link(struct memory_pool_collection * memory_pool_collection, struct unsigned_char_ptr_list * in_files, unsigned char * out_file, unsigned char * symbol_file, enum build_target_type build_target_type, unsigned char * offset, unsigned int page_align_permission_regions, unsigned int only_metadata){
	unsigned int i;
	struct linker_state state;

	linker_state_create(&state, memory_pool_collection, in_files, out_file, symbol_file, build_target_type, offset, page_align_permission_regions, only_metadata);

	/*  Load and parse all the linker files */
	for(i = 0; i < unsigned_char_ptr_list_size(state.in_files); i++){
		struct unsigned_char_list tmp;
		struct unsigned_char_list * file_input = (struct unsigned_char_list *)malloc(sizeof(struct unsigned_char_list));
		struct l2_lexer_state * l2_lexer_state = (struct l2_lexer_state *)malloc(sizeof(struct l2_lexer_state));
		struct l2_parser_state * l2_parser_state = (struct l2_parser_state *)malloc(sizeof(struct l2_parser_state));
		struct linker_file * linker_file;
		unsigned_char_list_create(file_input);
		unsigned_char_list_create(&tmp);
		add_file_to_buffer(file_input, (char*)unsigned_char_ptr_list_get(state.in_files, i));

		create_l2_lexer_state(l2_lexer_state, &state.l2_lexer_output, state.memory_pool_collection, unsigned_char_ptr_list_get(state.in_files, i), unsigned_char_list_data(file_input), unsigned_char_list_size(file_input));
		lex_asm(l2_lexer_state);
		create_l2_parser_state(l2_parser_state, memory_pool_collection, l2_lexer_state, &tmp, unsigned_char_list_data(file_input));
		parse_l2(l2_parser_state);
		printf("Linker parsed input file %d '%s'.\n", i, (char *)unsigned_char_ptr_list_get(state.in_files, i));

		linker_file = create_linker_file(&state, l2_parser_state, file_input);

		struct_linker_file_ptr_list_add_end(&state.linker_files, linker_file);
		unsigned_char_list_destroy(&tmp);
	}

	switch(build_target_type){
		case BUILD_TARGET_L1_FILE:{
			do_link_to_l1(&state);
			break;
		}case BUILD_TARGET_L2_FILE:{
			do_link_to_l2(&state);
			break;
		}default:{
			assert(0 && "Case not considered.");
		}
	}

	/*  Clean up all the resources */
	for(i = 0; i < unsigned_char_ptr_list_size(state.in_files); i++){

	}


	/*  Output to a file our final product with all files linked together */
	output_buffer_to_file(&state.file_output, (char*)state.out_file);
	if(state.symbol_file){
		output_buffer_to_file(&state.symbol_output, (char*)state.symbol_file);
	}

	linker_state_destroy(&state);

	return EXIT_SUCCESS;
}
