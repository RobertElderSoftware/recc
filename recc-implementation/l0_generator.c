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
#include "l0_generator.h"

/*

A 'Preloader' for producing programming language specific instructions for loading emulators with bytecode

*/

enum l0_language_type get_l0_language_type(char *);
void java_generic_data(FILE *);

static unsigned int java_max_items_per_method = 1000;
static unsigned int count_l2_file_image_size(struct l2_parser_node *, unsigned int *);
static unsigned int count_l2_instruction_image_size(struct l2_parser_node *);
static void output_instruction(struct l2_parser_node *, enum l0_language_type, FILE *, unsigned int, unsigned int);
static void output_data(struct l2_parser_node *, enum l0_language_type, FILE *, unsigned int);
void output_data_open(unsigned int size, char * variable_name, FILE * out, enum l0_language_type language);
static void output_to_l0_file(struct l0_generator_state *, char *, char *, enum l0_language_type);

static void output_to_l0_file(struct l0_generator_state *, char *, char *, enum l0_language_type);

unsigned int parse_decimal_token(struct l2_lexer_token * t){
	unsigned int i = 0;
	unsigned int base = 1;
	unsigned char * c = t->last_byte;
	do{
		i += (((unsigned int)*c - (unsigned int)'0') * base);
		base *= 10;
	}while(c-- != t->first_byte);
	return i;
}

unsigned int parse_hexadecimal_token(struct l2_lexer_token * t){
	unsigned int i = 0;
	unsigned int base = 1;
	unsigned char * c = t->last_byte;
	do{
		unsigned int n;
		if(*c >= '0' && *c <= '9'){
			n = (unsigned int)*c - (unsigned int)'0';
		}else if(*c >= 'A' && *c <= 'F'){
			n = ((unsigned int)*c - (unsigned int)'A') + (unsigned int)10;
		}else{
			assert(0 && "Unknown hex character.");
		}
		i += (n * base);
		base *= 16;
	}while(*(--c) != 'x');
	return i;
}

unsigned int get_register_number(struct l2_lexer_token * t){
	if(*(t->first_byte) == 'P' && *(t->last_byte) == 'C'){
		return 0;
	}else if(*(t->first_byte) == 'S' && *(t->last_byte) == 'P'){
		return 1;
	}else if(*(t->first_byte) == 'F' && *(t->last_byte) == 'P'){
		return 2;
	}else if(*(t->first_byte) == 'Z' && *(t->last_byte) == 'R'){
		return 3;
	}else if(*(t->first_byte) == 'F' && *(t->last_byte) == 'R'){
		return 4;
	}else if(*(t->first_byte) == 'W' && *(t->last_byte) == 'R'){
		return 5;
	}else{
		if(*(t->first_byte) == 'r'){
			struct l2_lexer_token abc;
			abc.first_byte = &t->first_byte[1];
			abc.last_byte = t->last_byte;
			return parse_decimal_token(&abc) + 5;
		}else{
			assert(0 && "Expected register");
			return 0;
		}
	}
}

void output_start_end(char *, FILE *, enum l0_language_type);

void output_start_end(char * variable_name, FILE * out, enum l0_language_type language){
	switch(language){
		case C_LANGUAGE_TYPE:{
			break;
		}case JSONP_LANGUAGE_TYPE:{
			fprintf(out, "%s({\n", variable_name);
			break;
		}case PYTHON_LANGUAGE_TYPE:{
			fprintf(out, "class %s(object):\n    def __init__(self):\n", variable_name);
			break;
		}case JAVA_LANGUAGE_TYPE:{
			fprintf(out, "package OpCPU;\nimport java.lang.reflect.Method;\n\nclass %s implements OpCPUDataInterface {\n", variable_name);
			fprintf(out, "    private static final long itemsPerClass = %uL;\n\n", java_max_items_per_method);
			fprintf(out, "    private long [][] lastArray = null;\n");
			fprintf(out, "    private Long lastIndex = null;\n");
			java_generic_data(out);
			fprintf(out, "}\n");
			break;
		}default:{
			assert(0 && "Should not happen.");
		}
	}
}


void output_data_open(unsigned int size, char * variable_name, FILE * out, enum l0_language_type language){
	switch(language){
		case C_LANGUAGE_TYPE:{
			fprintf(out, "extern unsigned char %s[%d][5];\n", variable_name, size);
			fprintf(out, "unsigned char %s[%d][5] = {\n", variable_name, size);
			break;
		}case JSONP_LANGUAGE_TYPE:{
			fprintf(out, "\"data\":  [\n");
			break;
		}case PYTHON_LANGUAGE_TYPE:{
			fprintf(out, "        self.data = [\n");
			break;
		}case JAVA_LANGUAGE_TYPE:{
			break;
		}default:{
			assert(0 && "Should not happen.");
		}
	}
}


void output_java_method_start(FILE *, unsigned int index);

void output_java_method_start(FILE * out, unsigned int index){
	fprintf(out, "class dataClass%u{\n", index);
	fprintf(out, "    public static long [][] getData(){\n");
	fprintf(out, "        final long data [][] = {\n");
}

void output_java_method_end(FILE *);

void output_java_method_end(FILE * out){
	fprintf(out, "\n");
	fprintf(out, "        };\n");
	fprintf(out, "        return data;\n");
	fprintf(out, "    }\n");
	fprintf(out, "}\n");
}

void output_data_item(unsigned int, unsigned int, FILE *, enum l0_language_type, unsigned int, unsigned int);

void output_data_item(unsigned int type, unsigned int v, FILE * out, enum l0_language_type language, unsigned int index, unsigned int total_items){
	switch(language){
		case C_LANGUAGE_TYPE:{
			unsigned char v1 = (v & 0xFF000000) >> 24;
			unsigned char v2 = (v & 0xFF0000) >> 16;
			unsigned char v3 = (v & 0xFF00) >> 8;
			unsigned char v4 = v & 0xFF;
			fprintf(out, "{0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X}", type, v1, v2, v3, v4);
			break;
		}case JSONP_LANGUAGE_TYPE:{
			fprintf(out, "[0x%02X, 0x%08X]", type, v);
			break;
		}case PYTHON_LANGUAGE_TYPE:{
			fprintf(out, "            [0x%02XL, 0x%08XL]", type, v);
			break;
		}case JAVA_LANGUAGE_TYPE:{
			if(index % java_max_items_per_method == 0){
				output_java_method_start(out, index / java_max_items_per_method);
			}
			fprintf(out, "            {0x%XL, 0x%08XL}", type, v);
			if((index % java_max_items_per_method) == (java_max_items_per_method - 1) || index == total_items -1){
				output_java_method_end(out);
			}else{
				fprintf(out, ",\n");
			}
			break;
		}default:{
			assert(0 && "Should not happen.");
		}
	}
}


void java_generic_data(FILE * out){
	fprintf(out, "\n");
	fprintf(out, "    public long [] getData(Long index){\n");
	fprintf(out, "        try{\n");
	fprintf(out, "            long [][] dataSection;\n");
	fprintf(out, "            Long classIndex = index / itemsPerClass;\n");
	fprintf(out, "            if(!classIndex.equals(this.lastIndex)){\n");
	fprintf(out, "                Class<?> clazz = Class.forName(\"OpCPU.dataClass\" + classIndex);\n");
	fprintf(out, "                Method method = clazz.getMethod(\"getData\");\n");
	fprintf(out, "                dataSection = ((long [][])method.invoke(this));\n");
	fprintf(out, "                this.lastArray = dataSection;\n");
	fprintf(out, "                this.lastIndex = classIndex;\n");
	fprintf(out, "            }else{\n");
	fprintf(out, "                dataSection = this.lastArray;\n");
	fprintf(out, "            }\n");
	fprintf(out, "            long [] f = dataSection[(int)(index - (classIndex * itemsPerClass))];\n");
	fprintf(out, "            if(index %% 20000 == 0){\n");
	fprintf(out, "                System.out.println(\"Loading kernel image...\" + index + \" words loaded.\");\n");
	fprintf(out, "            }\n");
	fprintf(out, "            return f;\n");
	fprintf(out, "        }catch(Exception e){\n");
	fprintf(out, "            System.out.println(\"Exception getting CPU data: \" + e);\n");
	fprintf(out, "            return new long [] {0,0};\n");
	fprintf(out, "        }\n");
	fprintf(out, "    }\n");
}

void output_data_close(FILE * out, enum l0_language_type language);

void output_data_close(FILE * out, enum l0_language_type language){
	switch(language){
		case C_LANGUAGE_TYPE:{
			fprintf(out, "};\n");
			break;
		}case JSONP_LANGUAGE_TYPE:{
			fprintf(out, "]});\n");
			break;
		}case PYTHON_LANGUAGE_TYPE:{
			fprintf(out, "        ]\n");
			break;
		}case JAVA_LANGUAGE_TYPE:{
			break;
		}default:{
			assert(0 && "Should not happen.");
		}
	}
}

static unsigned int count_l2_instruction_image_size(struct l2_parser_node * n){
	switch(n->first_child->type){
		case L2_INSTRUCTION_STATEMENT:{
			return 1;
		}case L2_DIRECTIVE_STATEMENT:{
			struct l2_parser_node * directive = n->first_child->first_child;
			switch(directive->type){
				case L2_DEFINE_WORD_DIRECTIVE: {
					return 1;
				}case L2_SKIP_WORDS_DIRECTIVE: {
					struct l2_lexer_token * value = directive->first_child->next->l2_lexer_token;
					if(value->type == L2_CONSTANT_HEX){
						return parse_hexadecimal_token(value);
					}else{
						assert(0 && "Unknown value type.");
					}
					return 0;
				}case L2_OFFSET_DIRECTIVE:{
					return 0;
				}case L2_STRING_DIRECTIVE:{
					return 0;
				}case L2_LINKAGE_DIRECTIVE:{
					return 0;
				}case L2_FUNCTION_DIRECTIVE:{
					return 0;
				}case L2_VARIABLE_DIRECTIVE:{
					return 0;
				}case L2_CONSTANT_DIRECTIVE:{
					return 0;
				}case L2_START_DIRECTIVE:{
					return 0;
				}case L2_END_DIRECTIVE:{
					return 0;
				}case L2_UNRESOLVED_DIRECTIVE:{
					return 0;
				}case L2_IMPLEMENTED_DIRECTIVE:{
					return 0;
				}case L2_REQUIRED_DIRECTIVE:{
					return 0;
				}case L2_PERMISSION_DIRECTIVE:{
					return 0;
				}case L2_REGION_DIRECTIVE:{
					return 0;
				}default:{
					assert(0 && "Unknown directive.");
				}
			}
			break;
		}default:{
			assert(0 && "Unknown statement.");
		}
	}
}

static unsigned int count_l2_file_image_size(struct l2_parser_node * current_node, unsigned int * num_l0_items){
	unsigned int count = 0;
	assert(current_node->type == L2_FILE);
	while(current_node->type != L2_EPSILON){
		switch (current_node->type){
			case L2_FILE:{
				current_node = current_node->first_child;
				break;
			}case L2_STATEMENT_LIST:{
				current_node = current_node->first_child;
				if(current_node->type == L2_STATEMENT){
					count += count_l2_instruction_image_size(current_node);
					current_node = current_node->next;
					*num_l0_items = *num_l0_items + 1;
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
				assert(0 && "Unknown l2 parser node type.");
			}
		}
	}
	return count;
}

static void output_instruction(struct l2_parser_node * n, enum l0_language_type language, FILE * out, unsigned int position, unsigned int num_l0_items){
	assert(n->type == L2_STATEMENT);
	switch(n->first_child->type){
		case L2_INSTRUCTION_STATEMENT:{
			unsigned int instruction = 0;
			struct l2_parser_node * op_code = n->first_child->first_child;
			/*  Every instruction starts with at least one rigister. */
			instruction += ((1L << ra_OFFSET) * get_register_number(op_code->next->l2_lexer_token));
			if(op_code->l2_lexer_token->type != L2_LL){
				instruction += ((1L << rb_OFFSET) * get_register_number(op_code->next->next->l2_lexer_token));
			}else{
				/*  Uses a constant */
				instruction += parse_hexadecimal_token(op_code->next->next->l2_lexer_token);
			}
			switch(op_code->l2_lexer_token->type){
				case L2_ADD:{ instruction += ADD_OP_CODE; break; }
				case L2_SUB:{ instruction += SUB_OP_CODE; break; }
				case L2_MUL:{ instruction += MUL_OP_CODE; break; }
				case L2_DIV:{ instruction += DIV_OP_CODE; break; }
				case L2_AND:{ instruction += AND_OP_CODE; break; }
				case L2_OR: { instruction += OR_OP_CODE;  break; }
				case L2_LOA:{ instruction += LOA_OP_CODE; break; }
				case L2_STO:{ instruction += STO_OP_CODE; break; }
				case L2_NOT:{ instruction += NOT_OP_CODE; break; }
				case L2_SHR:{ instruction += SHR_OP_CODE; break; }
				case L2_SHL:{ instruction += SHL_OP_CODE; break; }
				case L2_BEQ:{ instruction += BEQ_OP_CODE; break; }
				case L2_BLT:{ instruction += BLT_OP_CODE; break; }
				case L2_LL: { instruction += LL_OP_CODE;  break; }
				default:{
					assert(0 && "Unknown instruction.");
				}
			}
			switch(op_code->l2_lexer_token->type){
				/*  Uses case statement fallthrough */
				case L2_ADD:; case L2_SUB:; case L2_MUL:; case L2_DIV:; case L2_AND:; case L2_OR: { 
					instruction += ((1L << rc_OFFSET) * get_register_number(op_code->next->next->next->l2_lexer_token));
					break;
				}
				case L2_LOA:; case L2_STO:; case L2_NOT:; case L2_SHR:; case L2_SHL:; case L2_BEQ:; case L2_BLT:; case L2_LL: {
					/* No third register */
					break;
				}
				default:{
					assert(0 && "Unknown instruction.");
				}
			}

			if(op_code->l2_lexer_token->type == L2_BEQ || op_code->l2_lexer_token->type == L2_BLT){
				struct l2_parser_node * b = op_code->next->next->next;
				unsigned int constant_is_negative = b->first_child->l2_lexer_token->type == L2_MINUS_CHAR;
				struct l2_lexer_token * number = constant_is_negative ? b->first_child->next->l2_lexer_token : b->first_child->l2_lexer_token;
				unsigned int branch_distance = parse_decimal_token(number);
				if(constant_is_negative){
					instruction += ((BRANCH_DISTANCE_MASK - branch_distance) + 1);
				}else{
					instruction += branch_distance;
				}
			}
			output_data_item(L0_MACHINE_INSTRUCTION, instruction, out, language, position, num_l0_items);
			break;
		}case L2_LABEL_STATEMENT:{
			assert(0 && "Not expected.");
			break;
		}case L2_DIRECTIVE_STATEMENT:{
			struct l2_parser_node * directive = n->first_child->first_child;
			switch(directive->type){
				case L2_DEFINE_WORD_DIRECTIVE: {
					output_data_item(L0_DW_DIRECTIVE, parse_hexadecimal_token(directive->first_child->next->l2_lexer_token), out, language, position, num_l0_items);
					break;
				}case L2_SKIP_WORDS_DIRECTIVE: {
					output_data_item(L0_SW_DIRECTIVE, parse_hexadecimal_token(directive->first_child->next->l2_lexer_token), out, language, position, num_l0_items);
					break;
				}case L2_OFFSET_DIRECTIVE:{
					output_data_item(L0_OFFSET_ADDRESS_DIRECTIVE, parse_hexadecimal_token(directive->first_child->next->l2_lexer_token), out, language, position, num_l0_items);
					break;
				}case L2_SYMBOL_LINKAGE_DECLARATION:{
					assert(0 && "Should not happen.");
					break;
				}case L2_STRING_DIRECTIVE:{
					output_data_item(L0_STRING_DIRECTIVE, parse_hexadecimal_token(directive->first_child->next->l2_lexer_token), out, language, position, num_l0_items);
					break;
				}case L2_LINKAGE_DIRECTIVE:{
					output_data_item(L0_LINKAGE_DIRECTIVE, parse_hexadecimal_token(directive->first_child->next->l2_lexer_token), out, language, position, num_l0_items);
					break;
				}case L2_FUNCTION_DIRECTIVE:{
					assert(directive->first_child->next->l2_lexer_token->type == L2_CONSTANT_HEX);
					output_data_item(L0_FUNCTION_DIRECTIVE, parse_hexadecimal_token(directive->first_child->next->l2_lexer_token), out, language, position, num_l0_items);
					break;
				}case L2_VARIABLE_DIRECTIVE:{
					assert(directive->first_child->next->l2_lexer_token->type == L2_CONSTANT_HEX);
					output_data_item(L0_VARIABLE_DIRECTIVE, parse_hexadecimal_token(directive->first_child->next->l2_lexer_token), out, language, position, num_l0_items);
					break;
				}case L2_CONSTANT_DIRECTIVE:{
					assert(directive->first_child->next->l2_lexer_token->type == L2_CONSTANT_HEX);
					output_data_item(L0_CONSTANT_DIRECTIVE, parse_hexadecimal_token(directive->first_child->next->l2_lexer_token), out, language, position, num_l0_items);
					break;
				}case L2_START_DIRECTIVE:{
					assert(directive->first_child->next->l2_lexer_token->type == L2_CONSTANT_HEX);
					output_data_item(L0_START_DIRECTIVE, parse_hexadecimal_token(directive->first_child->next->l2_lexer_token), out, language, position, num_l0_items);
					break;
				}case L2_END_DIRECTIVE:{
					assert(directive->first_child->next->l2_lexer_token->type == L2_CONSTANT_HEX);
					output_data_item(L0_END_DIRECTIVE, parse_hexadecimal_token(directive->first_child->next->l2_lexer_token), out, language, position, num_l0_items);
					break;
				}case L2_UNRESOLVED_DIRECTIVE:{
					assert(directive->first_child->next->l2_lexer_token->type == L2_CONSTANT_HEX);
					output_data_item(L0_UNRESOLVED_DIRECTIVE, parse_hexadecimal_token(directive->first_child->next->l2_lexer_token), out, language, position, num_l0_items);
					break;
				}case L2_IMPLEMENTED_DIRECTIVE:{
					assert(directive->first_child->next->l2_lexer_token->type == L2_CONSTANT_HEX);
					output_data_item(L0_IMPLEMENTED_DIRECTIVE, parse_hexadecimal_token(directive->first_child->next->l2_lexer_token), out, language, position, num_l0_items);
					break;
				}case L2_REQUIRED_DIRECTIVE:{
					assert(directive->first_child->next->l2_lexer_token->type == L2_CONSTANT_HEX);
					output_data_item(L0_REQUIRED_DIRECTIVE, parse_hexadecimal_token(directive->first_child->next->l2_lexer_token), out, language, position, num_l0_items);
					break;
				}case L2_PERMISSION_DIRECTIVE:{
					assert(directive->first_child->next->l2_lexer_token->type == L2_CONSTANT_HEX);
					output_data_item(L0_PERMISSION_DIRECTIVE, parse_hexadecimal_token(directive->first_child->next->l2_lexer_token), out, language, position, num_l0_items);
					break;
				}case L2_REGION_DIRECTIVE:{
					assert(directive->first_child->next->l2_lexer_token->type == L2_CONSTANT_HEX);
					output_data_item(L0_REGION_DIRECTIVE, parse_hexadecimal_token(directive->first_child->next->l2_lexer_token), out, language, position, num_l0_items);
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

static void output_data(struct l2_parser_node * current_node, enum l0_language_type language, FILE * out, unsigned int num_l0_items){
	unsigned int i = 2; /*  Two previous instructions for num items image size */
	assert(current_node->type == L2_FILE);
	while(current_node->type != L2_EPSILON){
		switch (current_node->type){
			case L2_FILE:{
				current_node = current_node->first_child;
				break;
			}case L2_STATEMENT_LIST:{
				current_node = current_node->first_child;
				if(current_node->type == L2_STATEMENT){
					output_instruction(current_node, language, out, i, num_l0_items);
					current_node = current_node->next;
					if(language != JAVA_LANGUAGE_TYPE){
						if(i != num_l0_items -1){
							fprintf(out, ",");
						}
						fprintf(out, "\n");
					}
					i++;
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
				assert(0 && "Unknown l2 parser node type.");
			}
		}
	}
}

static void output_to_l0_file(struct l0_generator_state * l0_generator_state, char * variable_name, char * out_file, enum l0_language_type language){
	/*  TODO: currently assuming that sizeof(unsigned int) == 4 */
	unsigned int num_l0_items = 2; /*  One for image size directive and one for num items */
	unsigned int num_memory_words = count_l2_file_image_size(l0_generator_state->l2_parser_state.top_node, &num_l0_items);
	unsigned int image_size = 4 * num_memory_words;
	FILE * out = fopen(out_file, "w");

	output_start_end(variable_name, out, language);
	/*  Assemble all the instructions */
	output_data_open(num_l0_items, variable_name, out, language);
	output_data_item(L0_NUM_L0_ITEMS, num_l0_items, out, language, 0, num_l0_items);
	if(language != JAVA_LANGUAGE_TYPE){
		fprintf(out, ",\n");
	}
	output_data_item(L0_IMAGE_SIZE, image_size, out, language, 1, num_l0_items);
	if(language != JAVA_LANGUAGE_TYPE){
		fprintf(out, ",\n");
	}
	output_data(l0_generator_state->l2_parser_state.top_node, language, out, num_l0_items);
	output_data_close(out, language);
	fclose(out);
}

struct l0_generator_state * l0_generator_state_create(struct memory_pool_collection * m, unsigned char * variable_name, unsigned char * in_file, unsigned char * out_file, unsigned char * language_str){
	struct l0_generator_state * l0_generator_state = (struct l0_generator_state *)malloc(sizeof(struct l0_generator_state));
	struct unsigned_char_list lexer_output;
	struct unsigned_char_list parser_output;
	struct unsigned_char_list file_input;
	unsigned_char_list_create(&parser_output);
	unsigned_char_list_create(&lexer_output);
	unsigned_char_list_create(&file_input);

	add_file_to_buffer(&file_input, (char *)in_file);

	create_l2_lexer_state(&l0_generator_state->l2_lexer_state, &lexer_output, m, in_file, (unsigned char *)unsigned_char_list_data(&file_input), unsigned_char_list_size(&file_input));
	lex_asm(&l0_generator_state->l2_lexer_state);
	create_l2_parser_state(&l0_generator_state->l2_parser_state, m, &l0_generator_state->l2_lexer_state, &parser_output, (unsigned char *)unsigned_char_list_data(&file_input));
	parse_l2(&l0_generator_state->l2_parser_state);

	output_to_l0_file(l0_generator_state, (char*)variable_name, (char*)out_file, get_l0_language_type((char*)language_str));

	unsigned_char_list_destroy(&lexer_output);
	unsigned_char_list_destroy(&parser_output);
	unsigned_char_list_destroy(&file_input);
	return l0_generator_state;
}

void l0_generator_state_destroy(struct l0_generator_state * l0_generator_state){
	destroy_l2_lexer_state(&l0_generator_state->l2_lexer_state);
	destroy_l2_parser_state(&l0_generator_state->l2_parser_state);
	free(l0_generator_state);
}

enum l0_language_type get_l0_language_type(char * l){
	if(!strcmp("jsonp", l)){
		return JSONP_LANGUAGE_TYPE;
	}else if(!strcmp("c", l)){
		return C_LANGUAGE_TYPE;
	}else if(!strcmp("python", l)){
		return PYTHON_LANGUAGE_TYPE;
	}else if(!strcmp("java", l)){
		return JAVA_LANGUAGE_TYPE;
	}else{
		assert(0 && "Unknown language");
		return C_LANGUAGE_TYPE;
	}
}
