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
#include "preloader.h"

/*

A 'Preloader' for producing programming language specific instructions for loading emulators with bytecode

*/


static unsigned int java_max_items_per_method = 1000;
static unsigned int num_instruction_types = 16;
static unsigned int instruction_template_length = 4;

enum instruction_type {
	ADD_INSTRUCTION,
	SUB_INSTRUCTION,
	MUL_INSTRUCTION,
	DIV_INSTRUCTION,
	BEQ_INSTRUCTION,
	BLT_INSTRUCTION,
	LOA_INSTRUCTION,
	STO_INSTRUCTION,
	LL_INSTRUCTION,
	AND_INSTRUCTION,
	OR_INSTRUCTION,
	NOT_INSTRUCTION,
	SHR_INSTRUCTION,
	SHL_INSTRUCTION,
	DW_INSTRUCTION,
	SW_INSTRUCTION
};

struct instruction_register{
	unsigned int register_number;
};

struct instruction{
	struct instruction_register ** r;
	enum instruction_type type;
	unsigned int constant_is_negative;
	unsigned int constant;
	unsigned int pad;
};

struct l1_file{
	struct instruction ** instructions;
	unsigned int is_relocatable;
	unsigned int offset;
	unsigned int num_instructions;
	unsigned int pad;
};

struct parser_state{
	char * in_bytes;
	unsigned int size_buffer;
	unsigned int input_size;
	unsigned int buffer_position;
	unsigned int pad;
};

struct preloader_state {
	struct l1_file * l1_f;
	struct parser_state * state;
};

enum instruction_part_type{
	REGISTER,
	HEXIDECIMAL_CONSTANT,
	DECIMAL_CONSTANT,
	NOTHING
};

static unsigned int instruction_templates [16][4] = {
	{ADD_INSTRUCTION, REGISTER, REGISTER, REGISTER},
	{SUB_INSTRUCTION, REGISTER, REGISTER, REGISTER},
	{MUL_INSTRUCTION, REGISTER, REGISTER, REGISTER},
	{DIV_INSTRUCTION, REGISTER, REGISTER, REGISTER},
	{BEQ_INSTRUCTION, REGISTER, REGISTER, DECIMAL_CONSTANT},
	{BLT_INSTRUCTION, REGISTER, REGISTER, DECIMAL_CONSTANT},
	{LOA_INSTRUCTION, REGISTER, REGISTER, NOTHING},
	{STO_INSTRUCTION, REGISTER, REGISTER, NOTHING},
	{LL_INSTRUCTION, REGISTER, HEXIDECIMAL_CONSTANT, NOTHING},
	{AND_INSTRUCTION, REGISTER, REGISTER, REGISTER},
	{OR_INSTRUCTION, REGISTER, REGISTER, REGISTER},
	{NOT_INSTRUCTION, REGISTER, REGISTER, NOTHING},
	{SHR_INSTRUCTION, REGISTER, REGISTER, NOTHING},
	{SHL_INSTRUCTION, REGISTER, REGISTER, NOTHING},
	{DW_INSTRUCTION, HEXIDECIMAL_CONSTANT, NOTHING, NOTHING},
	{SW_INSTRUCTION, HEXIDECIMAL_CONSTANT, NOTHING, NOTHING}
};

static unsigned int instruction_op_codes [14][2] = {
	{ADD_INSTRUCTION, 0u  << OP_CODE_OFFSET },
	{SUB_INSTRUCTION, 1u  << OP_CODE_OFFSET },
	{MUL_INSTRUCTION, 2u  << OP_CODE_OFFSET },
	{DIV_INSTRUCTION, 3u  << OP_CODE_OFFSET },
	{AND_INSTRUCTION, 4u  << OP_CODE_OFFSET },
	{OR_INSTRUCTION , 5u  << OP_CODE_OFFSET },
	{NOT_INSTRUCTION, 6u  << OP_CODE_OFFSET },
	{LOA_INSTRUCTION, 7u  << OP_CODE_OFFSET },
	{STO_INSTRUCTION, 8u  << OP_CODE_OFFSET },
	{SHR_INSTRUCTION, 9u  << OP_CODE_OFFSET },
	{SHL_INSTRUCTION, 10u << OP_CODE_OFFSET },
	{BEQ_INSTRUCTION, 11u << OP_CODE_OFFSET },
	{BLT_INSTRUCTION, 12u << OP_CODE_OFFSET },
	{LL_INSTRUCTION , 13u << OP_CODE_OFFSET }
};

struct first_and_last_byte{
	char * first_byte;
	char * last_byte;
};

static int load_file(struct parser_state *, char *);
static unsigned int len(char *);
static unsigned int accept_character(char, struct parser_state *);
static unsigned int parse_decimal_string(struct first_and_last_byte *);
static unsigned int parse_hexidecimal_string(struct first_and_last_byte *);
static unsigned int accept_decimal_digit(struct parser_state *);
static unsigned int accept_hexidecimal_digit(struct parser_state *);
static struct first_and_last_byte * accept_word(const char *, struct parser_state *);
static unsigned int accept_newline(struct parser_state *);
static struct first_and_last_byte * accept_decimal_constant(struct parser_state *);
static struct first_and_last_byte * accept_hexidecimal_constant(struct parser_state *);
static struct instruction_register * accept_register(struct parser_state *);
static unsigned int get_instruction_op_code(enum instruction_type);
static unsigned int * get_instruction_template(enum instruction_type);
static struct instruction * accept_instruction(struct parser_state *);
static struct l1_file * l1_file(struct parser_state *);
static unsigned int do_exponent(unsigned int, unsigned int);
static void setup_preloader_state(struct preloader_state *, char *, char *, enum language_type);

static int load_file(struct parser_state * state, char * in_file){
	FILE *f = NULL;
	int c = 0;
	if(!(f = fopen(in_file, "rb"))){
		printf("Failed to open file %s for read.\n", in_file);
		return 1;
	}

	while (c != EOF) {
		c = getc(f);
		if(c == EOF)
			break;
		if(state->input_size == state->size_buffer){
			state->in_bytes = (char*)realloc(state->in_bytes, state->size_buffer * 2);
			state->size_buffer = state->size_buffer * 2;
		}
		(state->in_bytes)[state->input_size] = (char)c;
		state->input_size += 1;
	}
	fclose(f);
	return 0;
}

static unsigned int len(char * str){
	unsigned int i = 0;
	while(*str){
		str++;
		i++;
	}
	return i;
}

static unsigned int accept_character(char c, struct parser_state * state){
	if(state->in_bytes[state->buffer_position] == c){
		state->buffer_position = state->buffer_position + 1;
		return 1;
	}else{
		return 0;
	}
}

static unsigned int parse_decimal_string(struct first_and_last_byte * flb){
	unsigned int i = 0;
	unsigned int base = 1;
	char * c = flb->last_byte;
	do{
		i += (((unsigned int)*c - (unsigned int)'0') * base);
		base *= 10;
	}while(c-- != flb->first_byte);
	return i;
}

static unsigned int parse_hexidecimal_string(struct first_and_last_byte * flb){
	unsigned int i = 0;
	unsigned int base = 1;
	char * c = flb->last_byte;
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

static unsigned int accept_decimal_digit(struct parser_state * state){
	return (
		accept_character('0', state) ||
		accept_character('1', state) ||
		accept_character('2', state) ||
		accept_character('3', state) ||
		accept_character('4', state) ||
		accept_character('5', state) ||
		accept_character('6', state) ||
		accept_character('7', state) ||
		accept_character('8', state) ||
		accept_character('9', state)
	);
}

static unsigned int accept_hexidecimal_digit(struct parser_state * state){
	return (
		accept_decimal_digit(state) ||
		accept_character('A', state) ||
		accept_character('B', state) ||
		accept_character('C', state) ||
		accept_character('D', state) ||
		accept_character('E', state) ||
		accept_character('F', state)
	);
}

static struct first_and_last_byte * accept_word(const char * str, struct parser_state * state){
	unsigned int length = len((char*)str);
	unsigned int i;
	struct first_and_last_byte * flb = (struct first_and_last_byte *)malloc(sizeof(struct first_and_last_byte));
	unsigned int checkpoint = state->buffer_position;
	flb->first_byte = &state->in_bytes[state->buffer_position];
	for(i = 0; i < length; i++){
		if(!accept_character(str[i], state)){
			free(flb);
			state->buffer_position = checkpoint;
			return 0;
		}
	}
	flb->last_byte = &state->in_bytes[state->buffer_position + (length -1)];
	return flb;
}

static unsigned int accept_newline(struct parser_state * state){
	if(accept_character('\x0a', state)){
		accept_character('\x0d', state);
		return 1;
	}else if(accept_character('\x0d', state)){
		accept_character('\x0a', state);
		return 1;
	}else{
		return 0;
	}
}

static struct first_and_last_byte * accept_decimal_constant(struct parser_state * state){
	struct first_and_last_byte * flb = (struct first_and_last_byte *)malloc(sizeof(struct first_and_last_byte));
	flb->first_byte = &state->in_bytes[state->buffer_position];
	while(state->buffer_position < state->input_size && accept_decimal_digit(state)){
	}
	flb->last_byte = &state->in_bytes[state->buffer_position -1];
	return flb;
}

static struct first_and_last_byte * accept_hexidecimal_constant(struct parser_state * state){
	struct first_and_last_byte * flb = (struct first_and_last_byte *)malloc(sizeof(struct first_and_last_byte));
	flb->first_byte = &state->in_bytes[state->buffer_position];
	if(!accept_character('0', state)){
		free(flb);
		return 0;
	}

	if(!accept_character('x', state)){
		free(flb);
		return 0;
	}
	
	if(!accept_hexidecimal_digit(state)){
		free(flb);
		return 0;
	}

	while(state->buffer_position < state->input_size && accept_hexidecimal_digit(state)){
	}

	flb->last_byte = &state->in_bytes[state->buffer_position -1];
	return flb;
}

static struct instruction_register * accept_register(struct parser_state * state){
	struct instruction_register * instruction_register = (struct instruction_register *)malloc(sizeof(struct instruction_register));
	struct first_and_last_byte * flb;
	if((flb = accept_word("PC", state))){
		instruction_register->register_number = 0;
	}else if((flb = accept_word("SP", state))){
		instruction_register->register_number = 1;
	}else if((flb = accept_word("FP", state))){
		instruction_register->register_number = 2;
	}else if((flb = accept_word("ZR", state))){
		instruction_register->register_number = 3;
	}else if((flb = accept_word("FR", state))){
		instruction_register->register_number = 4;
	}else if((flb = accept_word("WR", state))){
		instruction_register->register_number = 5;
	}else{
		if(accept_character('r', state)){
			struct first_and_last_byte * abc;
			if((abc = accept_decimal_constant(state))){
				instruction_register->register_number = parse_decimal_string(abc) + 5;
				free(abc);
			}else{
				assert(0 && "Expected register number");
			}
		}else{
			assert(0 && "Expected register");
		}
	}
	free(flb);
	return instruction_register;
}

static unsigned int get_instruction_op_code(enum instruction_type type){
	unsigned int i;
	for(i = 0; i < num_instruction_types -1; i++){
		if(instruction_op_codes[i][0] == type){
			return instruction_op_codes[i][1];
		}
	}
	assert(0 && "Not possible.");
}

static unsigned int * get_instruction_template(enum instruction_type type){
	unsigned int i;
	for(i = 0; i < num_instruction_types; i++){
		if(instruction_templates[i][0] == type){
			return instruction_templates[i];
		}
	}
	assert(0 && "Not possible.");
}

static struct instruction * accept_instruction(struct parser_state * state){
	struct instruction * instruction = (struct instruction *)malloc(sizeof(struct instruction));
	struct first_and_last_byte * flb;
	unsigned int * instruction_template;
	unsigned int i;
	instruction->r = (struct instruction_register **)malloc(sizeof(struct instruction_register *) * 4);
	instruction->r[0] = 0;
	instruction->r[1] = 0;
	instruction->r[2] = 0;
	instruction->r[3] = 0;

	if((flb = accept_word("add", state))){
		instruction->type = ADD_INSTRUCTION;
	}else if((flb = accept_word("sub", state))){
		instruction->type = SUB_INSTRUCTION;
	}else if((flb = accept_word("mul", state))){
		instruction->type = MUL_INSTRUCTION;
	}else if((flb = accept_word("div", state))){
		instruction->type = DIV_INSTRUCTION;
	}else if((flb = accept_word("beq", state))){
		instruction->type = BEQ_INSTRUCTION;
	}else if((flb = accept_word("blt", state))){
		instruction->type = BLT_INSTRUCTION;
	}else if((flb = accept_word("loa", state))){
		instruction->type = LOA_INSTRUCTION;
	}else if((flb = accept_word("sto", state))){
		instruction->type = STO_INSTRUCTION;
	}else if((flb = accept_word("and", state))){
		instruction->type = AND_INSTRUCTION;
	}else if((flb = accept_word("not", state))){
		instruction->type = NOT_INSTRUCTION;
	}else if((flb = accept_word("shr", state))){
		instruction->type = SHR_INSTRUCTION;
	}else if((flb = accept_word("shl", state))){
		instruction->type = SHL_INSTRUCTION;
	}else if((flb = accept_word("or", state))){
		instruction->type = OR_INSTRUCTION;
	}else if((flb = accept_word("ll", state))){
		instruction->type = LL_INSTRUCTION;
	}else if((flb = accept_word("dw", state))){
		instruction->type = DW_INSTRUCTION;
	}else if((flb = accept_word("sw", state))){
		instruction->type = SW_INSTRUCTION;
	}else{
		free(instruction->r);
		free(instruction);
		return 0;
	}
	free(flb);

	instruction_template = get_instruction_template(instruction->type);

	for(i = 1; i < instruction_template_length; i++){
		if(instruction_template[i] == REGISTER){
			accept_character(' ', state);
			instruction->r[i-1] = accept_register(state);
		}else if(instruction_template[i] == DECIMAL_CONSTANT){
			struct first_and_last_byte * flb_dc;
			accept_character(' ', state);
			if(accept_character('-', state)){
				instruction->constant_is_negative = 1; 
			}else{
				instruction->constant_is_negative = 0; 
			}
			flb_dc = accept_decimal_constant(state);
			instruction->constant = parse_decimal_string(flb_dc);
			free(flb_dc);
		}else if(instruction_template[i] == HEXIDECIMAL_CONSTANT){
			struct first_and_last_byte * flb_hc;
			accept_character(' ', state);
			flb_hc = accept_hexidecimal_constant(state);
			instruction->constant = parse_hexidecimal_string(flb_hc);
			instruction->constant_is_negative = 0; 
			free(flb_hc);
		}else if(instruction_template[i] == NOTHING){
			/* Do nothing */
		}else{
			assert(0 && "Should not happen.");
		}
	}

	return instruction;
}

static struct l1_file * l1_file(struct parser_state * state){
	struct l1_file * l1_f = (struct l1_file *)malloc(sizeof(struct l1_file));
	struct first_and_last_byte * offset_word;
	l1_f->num_instructions = 0;
	l1_f->instructions = 0;
	if((offset_word = accept_word("OFFSET", state))){
		if(accept_character(' ', state)){
			struct first_and_last_byte * offset_address;
			if((offset_address = accept_hexidecimal_constant(state))){
				l1_f->offset = parse_hexidecimal_string(offset_address);
				while(accept_newline(state)){
					struct instruction * instruction;
					if((instruction = accept_instruction(state))){
						l1_f->num_instructions = l1_f->num_instructions + 1;
						l1_f->instructions = (struct instruction **)realloc(l1_f->instructions, l1_f->num_instructions * sizeof(struct instruction *));
						l1_f->instructions[l1_f->num_instructions -1] = instruction;
					}
				}
				free(offset_word);
				free(offset_address);
				return l1_f;
			}else{
				assert(0 && "Expected hexidecimal constant.");
			}
		}else{
			assert(0 && "Expected space.");
		}
	}else{
		assert(0 && "Expected word OFFSET.");
	}
	free(offset_word);
	return 0;
}

static unsigned int do_exponent(unsigned int base, unsigned int power){
	if(power == 0){
		return 1;
	}else{
		return base * do_exponent(base, power -1);
	}
}

void output_start_end(unsigned int, unsigned int, char *, FILE *, enum language_type);

void output_start_end(unsigned int start, unsigned int end, char * variable_name, FILE * out, enum language_type language){
	switch(language){
		case C_LANGUAGE_TYPE:{
			unsigned char s1 = (start & 0xFF000000) >> 24;
			unsigned char s2 = (start & 0xFF0000) >> 16;
			unsigned char s3 = (start & 0xFF00) >> 8;
			unsigned char s4 = start & 0xFF;
			unsigned char e1 = (end & 0xFF000000) >> 24;
			unsigned char e2 = (end & 0xFF0000) >> 16;
			unsigned char e3 = (end & 0xFF00) >> 8;
			unsigned char e4 = end & 0xFF;
			fprintf(out, "extern unsigned char %s_start[4];\nunsigned char %s_start[4] = {0x%02X, 0x%02X, 0x%02X, 0x%02X};\n", variable_name, variable_name, s1, s2, s3, s4);
			fprintf(out, "extern unsigned char %s_end[4];\nunsigned char %s_end[4] = {0x%02X, 0x%02X, 0x%02X, 0x%02X};\n", variable_name, variable_name, e1, e2, e3, e4);
			break;
		}case JSONP_LANGUAGE_TYPE:{
			fprintf(out, "%s({\"data_start\" : 0x%08X, \"data_end\" : 0x%08X,\n", variable_name, start, end);
			break;
		}case PYTHON_LANGUAGE_TYPE:{
			fprintf(out, "class %s(object):\n    def __init__(self):\n        self.data_start = 0x%08XL\n        self.data_end = 0x%08XL\n", variable_name, start, end);
			break;
		}case JAVA_LANGUAGE_TYPE:{
			fprintf(out, "package OpCPU;\n\nclass %s implements OpCPUDataInterface {\n    private static final long dataStart = 0x%08XL;\n    private static final long dataEnd = 0x%08XL;\n", variable_name, start, end);
			break;
		}default:{
			assert(0 && "Should not happen.");
		}
	}
}

void output_data_open(unsigned int size, char * variable_name, FILE * out, enum language_type language);

void output_data_open(unsigned int size, char * variable_name, FILE * out, enum language_type language){
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
			fprintf(out, "    private static final long itemsPerMethod = %uL;\n\n", java_max_items_per_method);
			break;
		}default:{
			assert(0 && "Should not happen.");
		}
	}
}


void output_java_method_start(FILE *, unsigned int index);

void output_java_method_start(FILE * out, unsigned int index){
	fprintf(out, "    public long [][] getData%u(){\n", index);
	fprintf(out, "        final long data [][] = {\n");
}

void output_java_method_end(FILE *);

void output_java_method_end(FILE * out){
	fprintf(out, "\n");
	fprintf(out, "        };\n");
	fprintf(out, "        return data;\n");
	fprintf(out, "    }\n");
}

void output_data_item(unsigned int, unsigned int, FILE *, enum language_type, unsigned int, unsigned int);

void output_data_item(unsigned int type, unsigned int v, FILE * out, enum language_type language, unsigned int index, unsigned int total_items){
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

void java_generic_data(FILE *);

void java_generic_data(FILE * out){
	fprintf(out, "\n");
	fprintf(out, "    public Long getDataStart(){\n");
	fprintf(out, "        return dataStart;\n");
	fprintf(out, "    }\n");
	fprintf(out, "\n");
	fprintf(out, "    public Long getDataEnd(){\n");
	fprintf(out, "        return dataEnd;\n");
	fprintf(out, "    }\n");
	fprintf(out, "\n");
	fprintf(out, "    public long [] getData(Long index){\n");
	fprintf(out, "        try{\n");
	fprintf(out, "            Long methodIndex = index / itemsPerMethod;\n");
	fprintf(out, "            return ((long [][])this.getClass().getMethod(\"getData\" + methodIndex).invoke(this))[(int)(index - (methodIndex * itemsPerMethod))];\n");
	fprintf(out, "        }catch(Exception e){\n");
	fprintf(out, "            System.out.println(\"Exception getting CPU data.\");\n");
	fprintf(out, "            return new long [] {0,0};\n");
	fprintf(out, "        }\n");
	fprintf(out, "    }\n");
}

void output_data_close(FILE * out, enum language_type language);

void output_data_close(FILE * out, enum language_type language){
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
			java_generic_data(out);
			fprintf(out, "}");
			break;
		}default:{
			assert(0 && "Should not happen.");
		}
	}
}

static void setup_preloader_state(struct preloader_state * preloader_state, char * variable_name, char * out_file, enum language_type language){
	/*  TODO: currently assuming that sizeof(unsigned int) == 4 */
	unsigned int i;
	unsigned int * current_template;
	unsigned int num_skip_words_directives = 0;
	unsigned int skipped_words = 0;
	unsigned int start = preloader_state->l1_f->offset;
	unsigned int end;
	unsigned int num_memory_words;
	FILE * out = fopen(out_file, "w");

	for(i = 0; i < preloader_state->l1_f->num_instructions; i++){
		if(preloader_state->l1_f->instructions[i]->type == SW_INSTRUCTION){
			num_skip_words_directives += 1;
			skipped_words += preloader_state->l1_f->instructions[i]->constant;
		}
	}
	num_memory_words = (preloader_state->l1_f->num_instructions - num_skip_words_directives) + skipped_words;
	end = start + (4 * num_memory_words);

	output_start_end(start, end, variable_name, out, language);
	/*  Assemble all the instructions */
	output_data_open(preloader_state->l1_f->num_instructions, variable_name, out, language);
	for(i = 0; i < preloader_state->l1_f->num_instructions; i++){
		unsigned int ins = 0;
		if(preloader_state->l1_f->instructions[i]->type == DW_INSTRUCTION){
			ins += preloader_state->l1_f->instructions[i]->constant;
		}else if(preloader_state->l1_f->instructions[i]->type == SW_INSTRUCTION){
		}else{
			unsigned int k;
			ins += get_instruction_op_code(preloader_state->l1_f->instructions[i]->type);
			current_template = get_instruction_template(preloader_state->l1_f->instructions[i]->type);
			for(k = 1; k < instruction_template_length; k++){
				if(current_template[k] == REGISTER){
					ins += ((1L << ra_OFFSET) / do_exponent(1u << BITS_PER_REGISTER, k -1)) * preloader_state->l1_f->instructions[i]->r[k-1]->register_number;
				}else if(current_template[k] == DECIMAL_CONSTANT || current_template[k] == HEXIDECIMAL_CONSTANT){
					/* It was a dw, an ll, a beq or a blt */
					if(preloader_state->l1_f->instructions[i]->constant_is_negative){
						ins += ((BRANCH_DISTANCE_MASK - preloader_state->l1_f->instructions[i]->constant) + 1);
					}else{
						ins += preloader_state->l1_f->instructions[i]->constant;
					}
				}else if(current_template[k] == NOTHING){
					/* Do nothing */
				}else{
					assert(0 && "unknown template");
				}
			}
		}
		if(preloader_state->l1_f->instructions[i]->type != SW_INSTRUCTION){
			output_data_item(0, ins, out, language, i, preloader_state->l1_f->num_instructions);
		}else{
			unsigned int c = preloader_state->l1_f->instructions[i]->constant;
			output_data_item(1, c, out, language, i, preloader_state->l1_f->num_instructions);
		}
		if(language != JAVA_LANGUAGE_TYPE){
			if(i != preloader_state->l1_f->num_instructions -1){
				fprintf(out, ",");
			}
			fprintf(out, "\n");
		}
	}
	output_data_close(out, language);
	fclose(out);
}

struct preloader_state * preloader_state_create(char * variable_name, char * in_file, char * out_file, enum language_type language){
	struct preloader_state * preloader_state = (struct preloader_state *)malloc(sizeof(struct preloader_state));
	preloader_state->state = (struct parser_state *)malloc(sizeof(struct parser_state));
	preloader_state->state->in_bytes = (char *)malloc(1);
	preloader_state->state->input_size = 0;
	preloader_state->state->size_buffer = 1;
	preloader_state->state->buffer_position = 0;
	load_file(preloader_state->state, in_file);
	preloader_state->state->in_bytes[preloader_state->state->input_size] = 0;
	if((preloader_state->l1_f = l1_file(preloader_state->state))){
		setup_preloader_state(preloader_state, variable_name, out_file, language);
	}else{
		assert(0 && "Unable to parser instructions.");
	}
	return preloader_state;
}

void preloader_state_destroy(struct preloader_state * preloader_state){
	unsigned int i;
	free(preloader_state->state->in_bytes);
	free(preloader_state->state);
	for(i = 0; i < preloader_state->l1_f->num_instructions; i++){
		free(preloader_state->l1_f->instructions[i]->r[0]);
		free(preloader_state->l1_f->instructions[i]->r[1]);
		free(preloader_state->l1_f->instructions[i]->r[2]);
		free(preloader_state->l1_f->instructions[i]->r[3]);
		free(preloader_state->l1_f->instructions[i]->r);
		free(preloader_state->l1_f->instructions[i]);
	}
	free(preloader_state->l1_f->instructions);
	free(preloader_state->l1_f);
	free(preloader_state);
}

enum language_type get_language_type(char *);
enum language_type get_language_type(char * l){
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

int main(int argc, char ** argv){
	struct preloader_state * preloader_state;

	assert(argc == 5 && "Preloader was invoked with the wrong number of arguments.");
	preloader_state = preloader_state_create(argv[1], argv[2], argv[3], get_language_type(argv[4]));

	preloader_state_destroy(preloader_state);
	return 0;
}
