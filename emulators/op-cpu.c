/*
	Copyright 2014 Robert Elder Software Inc.  All rights reserved.

	This software is not currently available under any license, and unauthorized use
	or copying is not permitted.

	This software will likely be available under a common open source license in the
	near future.  Licensing is currently pending feedback from a lawyer.  If you have
	an opinion on this subject you can send it to recc [at] robertelder.org.

	This program comes with ABSOLUTELY NO WARRANTY.  In no event shall Robert Elder
	Software Inc. be liable for incidental or consequential damages in connection with
	use of this software.
*/
#include "op-cpu.h"

/*

C89 implemention of op-cpu emulator.

*/

static unsigned int literal22bitmask = 0x003FFFFF;
static unsigned int UART1_OUT = 0x300000;
static unsigned int UART1_IN = 0x300010;
static unsigned int IRQ_HANDLER = 0x300020;
static unsigned int TIMER_PERIOD = 0x300030;

static unsigned int PC_index = 0;
static unsigned int SP_index = 1;
static unsigned int FP_index = 2;
static unsigned int ZR_index = 3;
static unsigned int FR_index = 4;
static unsigned int WR_index = 5;

static unsigned int HALTED_BIT = 1 << 0;
static unsigned int GLOBAL_INTERRUPT_ENABLE_BIT = 1 << 1;
static unsigned int RTE_BIT = 1 << 2;
static unsigned int TIMER1_ENABLE_BIT = 1 << 3;
static unsigned int TIMER1_ASSERTED_BIT = 1 << 4;
static unsigned int UART1_OUT_ENABLE_BIT = 1 << 5;
static unsigned int UART1_OUT_ASSERTED_BIT = 1 << 6;
static unsigned int UART1_IN_ENABLE_BIT = 1 << 7;
static unsigned int UART1_IN_ASSERTED_BIT = 1 << 8;
static unsigned int UART1_OUT_READY_BIT = 1 << 9;
static unsigned int UART1_IN_READY_BIT = 1 << 10;

static unsigned int num_instruction_types = 15;

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
	DW_INSTRUCTION
};

struct instruction_register{
	unsigned int register_number;
};

struct instruction{
	enum instruction_type type;
	unsigned int constant_is_negative;
	unsigned int constant;
	struct instruction_register ** r;
};

struct l1_file{
	unsigned int is_relocatable;
	unsigned int offset;
	unsigned int num_instructions;
	struct instruction ** instructions;
};

struct parser_state{
	char * in_bytes;
	unsigned int size_buffer;
	unsigned int input_size;
	unsigned int buffer_position;
};

struct virtual_machine {
	unsigned int cycles_executed;
	unsigned int num_memory_words;
	unsigned int num_registers;
	unsigned int * memoryuint32;
	unsigned int * registeruint32;
	struct l1_file * l1_f;
	struct parser_state * state;
};

enum instruction_part_type{
	REGISTER,
	HEXIDECIMAL_CONSTANT,
	DECIMAL_CONSTANT,
	NOTHING
};

static unsigned int instruction_templates [15][5] = {
	{ADD_INSTRUCTION, REGISTER, REGISTER, REGISTER, NOTHING},
	{SUB_INSTRUCTION, REGISTER, REGISTER, REGISTER, NOTHING},
	{MUL_INSTRUCTION, REGISTER, REGISTER, REGISTER, NOTHING},
	{DIV_INSTRUCTION, REGISTER, REGISTER, REGISTER, REGISTER},
	{BEQ_INSTRUCTION, REGISTER, REGISTER, DECIMAL_CONSTANT, NOTHING},
	{BLT_INSTRUCTION, REGISTER, REGISTER, DECIMAL_CONSTANT, NOTHING},
	{LOA_INSTRUCTION, REGISTER, REGISTER, NOTHING, NOTHING},
	{STO_INSTRUCTION, REGISTER, REGISTER, NOTHING, NOTHING},
	{LL_INSTRUCTION, REGISTER, HEXIDECIMAL_CONSTANT, NOTHING, NOTHING},
	{AND_INSTRUCTION, REGISTER, REGISTER, REGISTER, NOTHING},
	{OR_INSTRUCTION, REGISTER, REGISTER, REGISTER, NOTHING},
	{NOT_INSTRUCTION, REGISTER, REGISTER, NOTHING, NOTHING},
	{SHR_INSTRUCTION, REGISTER, REGISTER, NOTHING, NOTHING},
	{SHL_INSTRUCTION, REGISTER, REGISTER, NOTHING, NOTHING},
	{DW_INSTRUCTION, HEXIDECIMAL_CONSTANT, NOTHING, NOTHING, NOTHING}
};

static unsigned int instruction_op_codes [15][2] = {
	{ADD_INSTRUCTION, 0x00000000 },
	{SUB_INSTRUCTION, 0x10000000 },
	{MUL_INSTRUCTION, 0x20000000 },
	{DIV_INSTRUCTION, 0x30000000 },
	{BEQ_INSTRUCTION, 0x40000000 },
	{BLT_INSTRUCTION, 0x50000000 },
	{LOA_INSTRUCTION, 0x60000000 },
	{STO_INSTRUCTION, 0x70000000 },
	{LL_INSTRUCTION,  0x80000000 },
	{AND_INSTRUCTION, 0x90000000 },
	{OR_INSTRUCTION,  0xA0000000 },
	{NOT_INSTRUCTION, 0xB0000000 },
	{SHR_INSTRUCTION, 0xC0000000 },
	{SHL_INSTRUCTION, 0xD0000000 }
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
static struct first_and_last_byte * accept_word(char *, struct parser_state *);
static unsigned int accept_newline(struct parser_state *);
static struct first_and_last_byte * accept_decimal_constant(struct parser_state *);
static struct first_and_last_byte * accept_hexidecimal_constant(struct parser_state *);
static struct instruction_register * accept_register(struct parser_state *);
static enum instruction_type lookup_instruction_op_code(unsigned int);
static unsigned int get_instruction_op_code(enum instruction_type);
static unsigned int * get_instruction_template(enum instruction_type);
static struct instruction * accept_instruction(struct parser_state *);
static struct l1_file * l1_file(struct parser_state *);
static unsigned int do_exponent(unsigned int, unsigned int);
static void do_interrupt(struct virtual_machine *);
static void fetch_decode_execute(struct virtual_machine *);
static void setup_virtual_machine(struct virtual_machine *);

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
		(state->in_bytes)[state->input_size] = c;
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
		i += ((*c - '0') * base);
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
			n = *c - '0';
		}else if(*c >= 'A' && *c <= 'F'){
			n = (*c - 'A') + 10;
		}else{
			printf("%c\n", n);
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

static struct first_and_last_byte * accept_word(char * str, struct parser_state * state){
	unsigned int length = len(str);
	unsigned int i;
	struct first_and_last_byte * flb = malloc(sizeof(struct first_and_last_byte));
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
	struct first_and_last_byte * flb = malloc(sizeof(struct first_and_last_byte));
	flb->first_byte = &state->in_bytes[state->buffer_position];
	while(state->buffer_position < state->input_size && accept_decimal_digit(state)){
	}
	flb->last_byte = &state->in_bytes[state->buffer_position -1];
	return flb;
}

static struct first_and_last_byte * accept_hexidecimal_constant(struct parser_state * state){
	struct first_and_last_byte * flb = malloc(sizeof(struct first_and_last_byte));
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
	struct instruction_register * instruction_register = malloc(sizeof(struct instruction_register));
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

static enum instruction_type lookup_instruction_op_code(unsigned int op_code){
	unsigned int i;
	for(i = 0; i < num_instruction_types -1; i++){
		if(instruction_op_codes[i][1] == op_code){
			return instruction_op_codes[i][0];
		}
	}
	assert(0 && "Not possible.");
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
	struct instruction * instruction = malloc(sizeof(struct instruction));
	struct first_and_last_byte * flb;
	unsigned int * instruction_template;
	unsigned int i;
	instruction->r = malloc(sizeof(struct instruction_register *) * 4);
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
	}else{
		free(instruction->r);
		free(instruction);
		return 0;
	}
	free(flb);

	instruction_template = get_instruction_template(instruction->type);

	for(i = 1; i < 5; i++){
		if(instruction_template[i] == REGISTER){
			accept_character(' ', state);
			instruction->r[i-1] = accept_register(state);
		}else if(instruction_template[i] == DECIMAL_CONSTANT){
			struct first_and_last_byte * flb;
			accept_character(' ', state);
			if(accept_character('-', state)){
				instruction->constant_is_negative = 1; 
			}else{
				instruction->constant_is_negative = 0; 
			}
			flb = accept_decimal_constant(state);
			instruction->constant = parse_decimal_string(flb);
			free(flb);
		}else if(instruction_template[i] == HEXIDECIMAL_CONSTANT){
			struct first_and_last_byte * flb;
			accept_character(' ', state);
			flb = accept_hexidecimal_constant(state);
			instruction->constant = parse_hexidecimal_string(flb);
			instruction->constant_is_negative = 0; 
			free(flb);
		}else if(instruction_template[i] == NOTHING){
			/* Do nothing */
		}else{
			assert(0 && "Should not happen.");
		}
	}

	return instruction;
}

static struct l1_file * l1_file(struct parser_state * state){
	struct l1_file * l1_f = malloc(sizeof(struct l1_file));
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
						l1_f->instructions = realloc(l1_f->instructions, l1_f->num_instructions * sizeof(struct instruction));
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

static void do_interrupt(struct virtual_machine * vm){
	/*  Disable global interrupts */
	vm->registeruint32[FR_index] = vm->registeruint32[FR_index] & ~GLOBAL_INTERRUPT_ENABLE_BIT;
	/*  Branch to irq handler */
	vm->registeruint32[SP_index] = vm->registeruint32[SP_index] - vm->registeruint32[WR_index]; /* SP = SP -4 */
	vm->memoryuint32[vm->registeruint32[SP_index] / sizeof(unsigned int)] = vm->registeruint32[PC_index]; /* [SP] = PC */
	vm->registeruint32[PC_index] = vm->memoryuint32[IRQ_HANDLER / sizeof(unsigned int)];
}

static void fetch_decode_execute(struct virtual_machine * vm){
	unsigned int current_inst = vm->memoryuint32[vm->registeruint32[PC_index] / sizeof(unsigned int)];
	int branch_dist = current_inst & 0x7FFF;
	unsigned int literal22bit = literal22bitmask & current_inst;
	unsigned int ra = (0x0FC00000 & current_inst) / 0x400000;
	unsigned int rb = (0x003F0000 & current_inst) / 0x10000;
	unsigned int rc = (0x0000FC00 & current_inst) / 0x400;
	unsigned int rd = (0x000003F0 & current_inst) / 0x10;
	enum instruction_type op_type = lookup_instruction_op_code(current_inst & 0xF0000000);
	branch_dist = current_inst & 0x8000 ? -branch_dist : branch_dist;
	vm->registeruint32[PC_index] += sizeof(unsigned int);
	assert(vm->registeruint32[PC_index] / sizeof(unsigned int) < vm->num_memory_words);

	switch(op_type){
		case ADD_INSTRUCTION:{
			vm->registeruint32[ra] = vm->registeruint32[rb] + vm->registeruint32[rc];
			break;
		}case SUB_INSTRUCTION:{
			vm->registeruint32[ra] = vm->registeruint32[rb] - vm->registeruint32[rc];
			break;
		}case MUL_INSTRUCTION:{
			vm->registeruint32[ra] = vm->registeruint32[rb] * vm->registeruint32[rc];
			break;
		}case DIV_INSTRUCTION:{
			unsigned int c = vm->registeruint32[rc];
			unsigned int d = vm->registeruint32[rd];
			vm->registeruint32[ra] = c / d;
			vm->registeruint32[rb] = c % d;
			break;
		}case BEQ_INSTRUCTION:{
			if(vm->registeruint32[ra] == vm->registeruint32[rb]){
				vm->registeruint32[PC_index] += sizeof(unsigned int) * branch_dist;
			}
			break;
		}case BLT_INSTRUCTION:{
			if(vm->registeruint32[ra] < vm->registeruint32[rb]){
				vm->registeruint32[PC_index] += sizeof(unsigned int) * branch_dist;
			}
			break;
		}case LOA_INSTRUCTION:{
			assert(vm->registeruint32[rb] / sizeof(unsigned int) < vm->num_memory_words);
			vm->registeruint32[ra] = vm->memoryuint32[vm->registeruint32[rb]/sizeof(unsigned int)];
			break;
		}case STO_INSTRUCTION:{
			assert(vm->registeruint32[ra] / sizeof(unsigned int) < vm->num_memory_words);
			vm->memoryuint32[vm->registeruint32[ra]/sizeof(unsigned int)] = vm->registeruint32[rb];
			break;
		}case LL_INSTRUCTION:{
			vm->registeruint32[ra] = literal22bit;
			break;
		}case AND_INSTRUCTION:{
			vm->registeruint32[ra] = vm->registeruint32[rb] & vm->registeruint32[rc];
			break;
		}case OR_INSTRUCTION:{
			vm->registeruint32[ra] = vm->registeruint32[rb] | vm->registeruint32[rc];
			break;
		}case NOT_INSTRUCTION:{
			vm->registeruint32[ra] = ~vm->registeruint32[rb];
			break;
		}case SHR_INSTRUCTION:{
			vm->registeruint32[ra] = vm->registeruint32[ra] >> vm->registeruint32[rb];
			break;
		}case SHL_INSTRUCTION:{
			vm->registeruint32[ra] = vm->registeruint32[ra] << vm->registeruint32[rb];
			break;
		}default:{
			assert(0 && "Illegial op code: ");
		}
	}
}

unsigned int is_halted(struct virtual_machine * vm){
	return vm->registeruint32[FR_index] & HALTED_BIT;
}

unsigned int vm_putc(struct virtual_machine * vm, unsigned int c){
	if(vm->registeruint32[FR_index] & UART1_IN_READY_BIT){
		return 1; /* Item was NOT input */
	}
	/*  Set the flag bit to indicate there is data */
	vm->registeruint32[FR_index] = vm->registeruint32[FR_index] | UART1_IN_READY_BIT;
	/*  Assert the interrupt */
	vm->registeruint32[FR_index] = vm->registeruint32[FR_index] | UART1_IN_ASSERTED_BIT;
	vm->memoryuint32[UART1_IN / sizeof(unsigned int)] = c;
	return 0; /* Item was input */
}

unsigned int vm_getc(struct virtual_machine * vm, unsigned int * rtn){
	if(!(vm->registeruint32[FR_index] & UART1_OUT_READY_BIT)){
		/*  Set the flag bit back to ready. */
		vm->registeruint32[FR_index] = vm->registeruint32[FR_index] | UART1_OUT_READY_BIT;
		/*  Assert the interrupt */
		vm->registeruint32[FR_index] = vm->registeruint32[FR_index] | UART1_OUT_ASSERTED_BIT;
		*rtn = vm->memoryuint32[UART1_OUT / sizeof(unsigned int)];
		return 1;
	}else{
		return 0;
	}
}

void step(struct virtual_machine * vm){
	if(vm->registeruint32[FR_index] & HALTED_BIT){
		/*Processor has been halted */
		return; 
	}

	fetch_decode_execute(vm);
	vm->cycles_executed = vm->cycles_executed + 1;

	if(vm->registeruint32[FR_index] & HALTED_BIT){
		/*Processor has been halted */
		return; 
	}

	if(vm->registeruint32[FR_index] & RTE_BIT){
		vm->registeruint32[FR_index] = vm->registeruint32[FR_index] & ~RTE_BIT; /* Unset RET bit. */
		vm->registeruint32[FR_index] = vm->registeruint32[FR_index] | GLOBAL_INTERRUPT_ENABLE_BIT; /* Set global interrupt enable. */
		vm->registeruint32[PC_index] = vm->memoryuint32[vm->registeruint32[SP_index] / sizeof(unsigned int)]; /* Set PC to [SP] */
		vm->registeruint32[SP_index] = vm->registeruint32[SP_index] + vm->registeruint32[WR_index];/* Set SP to SP + WR */
		return; 
	}

	/*Check for timer interrupt condition */
	if(vm->memoryuint32[TIMER_PERIOD / sizeof(unsigned int)] && vm->cycles_executed % vm->memoryuint32[TIMER_PERIOD / sizeof(unsigned int)] == 0){
		/*Assert our timer interrupt */
		vm->registeruint32[FR_index] = vm->registeruint32[FR_index] | TIMER1_ASSERTED_BIT;
		vm->cycles_executed = 0; /* Avoid eventual overflow */
		return; 
	}

	if(vm->registeruint32[FR_index] & GLOBAL_INTERRUPT_ENABLE_BIT){
		if(vm->registeruint32[FR_index] & TIMER1_ENABLE_BIT && (vm->registeruint32[FR_index] & TIMER1_ASSERTED_BIT)){
			do_interrupt(vm);
			return; 
		}else if(vm->registeruint32[FR_index] & UART1_OUT_ENABLE_BIT && (vm->registeruint32[FR_index] & UART1_OUT_ASSERTED_BIT)){
			do_interrupt(vm);
			return; 
		}else if(vm->registeruint32[FR_index] & UART1_IN_ENABLE_BIT && (vm->registeruint32[FR_index] & UART1_IN_ASSERTED_BIT)){
			do_interrupt(vm);
			return; 
		}
	}
}

static void setup_virtual_machine(struct virtual_machine * vm){
	/*  TODO: currently assuming that sizeof(unsigned int) == 4 */
	unsigned int min_memory = ((0x300030 + 4) / 4);
	unsigned int i;
	unsigned int * current_template;
	/*  Allocate enough memory to hold all the instructions */
	vm->num_memory_words = vm->l1_f->num_instructions > min_memory ? vm->l1_f->num_instructions : min_memory;
	vm->num_registers = 64;
	vm->memoryuint32 = malloc(vm->num_memory_words * sizeof(unsigned int));
	vm->registeruint32 = malloc(vm->num_registers * sizeof(unsigned int));

	vm->registeruint32[PC_index] = 0x0; /* PC */
	vm->registeruint32[SP_index] = 0x0; /* SP */
	vm->registeruint32[FP_index] = 0x0; /* FP */
	vm->registeruint32[ZR_index] = 0x0; /* ZR */
	vm->registeruint32[FR_index] = 0x200; /* FR */
	vm->registeruint32[WR_index] = 0x4; /* WR*/
	vm->registeruint32[6] = 0x0;
	vm->registeruint32[7] = 0x0;
	vm->registeruint32[8] = 0x0;
	vm->registeruint32[9] = 0x0;
	vm->registeruint32[10] = 0x0;
	vm->registeruint32[11] = 0x0;
	vm->registeruint32[12] = 0x0;
	vm->registeruint32[13] = 0x0;
	vm->registeruint32[14] = 0x0;
	vm->registeruint32[15] = 0x0;
	vm->registeruint32[16] = 0x0;

	vm->memoryuint32[UART1_OUT / sizeof(unsigned int)] = 0;
	vm->memoryuint32[UART1_IN / sizeof(unsigned int)] = 0;
	vm->memoryuint32[IRQ_HANDLER / sizeof(unsigned int)] = 0;
	vm->memoryuint32[TIMER_PERIOD / sizeof(unsigned int)] = 0;

	/*  Assemble all the instructions */
	for(i = 0; i < vm->l1_f->num_instructions; i++){
		unsigned memory_index = ((vm->l1_f->offset) / 4) + i;
		unsigned int ins = 0;
		if(vm->l1_f->instructions[i]->type == DW_INSTRUCTION){
			ins += vm->l1_f->instructions[i]->constant;
		}else{
			unsigned int k;
			ins += get_instruction_op_code(vm->l1_f->instructions[i]->type);
			current_template = get_instruction_template(vm->l1_f->instructions[i]->type);
			for(k = 1; k < 5; k++){
				if(current_template[k] == REGISTER){
					ins += (0x400000 / do_exponent(0x40, k -1)) * vm->l1_f->instructions[i]->r[k-1]->register_number;
				}else if(current_template[k] == DECIMAL_CONSTANT || current_template[k] == HEXIDECIMAL_CONSTANT){
					ins += vm->l1_f->instructions[i]->constant;
					if(vm->l1_f->instructions[i]->constant_is_negative){
						ins += (0x1 << 15);
					}
				}else if(current_template[k] == NOTHING){
					/* Do nothing */
				}else{
					assert(0 && "unknown template");
				}
			}
		}
		vm->memoryuint32[memory_index] = ins;
	}


}

struct virtual_machine * vm_create(char * in_file){
	struct virtual_machine * vm = malloc(sizeof(struct virtual_machine));
	vm->state = malloc(sizeof(struct parser_state));
	vm->state->in_bytes = malloc(1);
	vm->state->input_size = 0;
	vm->state->size_buffer = 1;
	vm->state->buffer_position = 0;
	load_file(vm->state, in_file);
	vm->state->in_bytes[vm->state->input_size] = 0;
	if((vm->l1_f = l1_file(vm->state))){
		setup_virtual_machine(vm);
	}else{
		assert(0 && "Unable to parser instructions.");
	}
	return vm;
}

void vm_destroy(struct virtual_machine * vm){
	unsigned int i;
	free(vm->memoryuint32);
	free(vm->registeruint32);
	free(vm->state->in_bytes);
	free(vm->state);
	for(i = 0; i < vm->l1_f->num_instructions; i++){
		free(vm->l1_f->instructions[i]->r[0]);
		free(vm->l1_f->instructions[i]->r[1]);
		free(vm->l1_f->instructions[i]->r[2]);
		free(vm->l1_f->instructions[i]->r[3]);
		free(vm->l1_f->instructions[i]->r);
		free(vm->l1_f->instructions[i]);
	}
	free(vm->l1_f->instructions);
	free(vm->l1_f);
	free(vm);
}
