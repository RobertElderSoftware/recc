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
#include "op-cpu.h"

/*

C89 implemention of op-cpu emulator.
This implementation assumes that sizeof(unsigned int) == 4

*/

static unsigned int LITERAL22BITMASK = 0x003FFFFF;
static unsigned int UART1_OUT        = 0x00300000;
static unsigned int UART1_IN         = 0x00300010;
static unsigned int IRQ_HANDLER      = 0x00300020;
static unsigned int TIMER_PERIOD     = 0x00300030;

static unsigned int PC_index = 0;
static unsigned int SP_index = 1;
static unsigned int FR_index = 4;
static unsigned int WR_index = 5;

static unsigned int HALTED_BIT                  = 1 << 0;
static unsigned int GLOBAL_INTERRUPT_ENABLE_BIT = 1 << 1;
static unsigned int RTE_BIT                     = 1 << 2;
static unsigned int TIMER1_ENABLE_BIT           = 1 << 3;
static unsigned int TIMER1_ASSERTED_BIT         = 1 << 4;
static unsigned int UART1_OUT_ENABLE_BIT        = 1 << 5;
static unsigned int UART1_OUT_ASSERTED_BIT      = 1 << 6;
static unsigned int UART1_IN_ENABLE_BIT         = 1 << 7;
static unsigned int UART1_IN_ASSERTED_BIT       = 1 << 8;
static unsigned int UART1_OUT_READY_BIT         = 1 << 9;
static unsigned int UART1_IN_READY_BIT          = 1 << 10;

static unsigned int num_instruction_types = 14;

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
	SHL_INSTRUCTION
};

struct virtual_machine {
	unsigned int cycles_executed;
	unsigned int num_memory_words;
	unsigned int num_registers;
	unsigned int * memoryuint32;
	unsigned int * registeruint32;
};

static unsigned int instruction_op_codes [14][2] = {
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

static enum instruction_type lookup_instruction_op_code(unsigned int);
static void do_interrupt(struct virtual_machine *);
static void fetch_decode_execute(struct virtual_machine *);
void setup_virtual_machine(struct virtual_machine *);

static enum instruction_type lookup_instruction_op_code(unsigned int op_code){
	unsigned int i;
	for(i = 0; i < num_instruction_types; i++){
		if(instruction_op_codes[i][1] == op_code){
			return instruction_op_codes[i][0];
		}
	}
	assert(0 && "Not possible.");
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
	int branch_dist = current_inst & 0x8000 ? -((0xFFFF & ~(current_inst & 0xFFFF)) + 1) : (current_inst & 0xFFFF);
	unsigned int literal22bit = LITERAL22BITMASK & current_inst;
	unsigned int ra = (0x0FC00000 & current_inst) / 0x400000;
	unsigned int rb = (0x003F0000 & current_inst) / 0x10000;
	unsigned int rc = (0x0000FC00 & current_inst) / 0x400;
	unsigned int rd = (0x000003F0 & current_inst) / 0x10;
	enum instruction_type op_type = lookup_instruction_op_code(current_inst & 0xF0000000);
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
		/* Processor has been halted */
		return; 
	}

	fetch_decode_execute(vm);
	vm->cycles_executed = vm->cycles_executed + 1;

	if(vm->registeruint32[FR_index] & HALTED_BIT){
		/*  Processor has been halted */
		return; 
	}

	if(vm->registeruint32[FR_index] & RTE_BIT){
		vm->registeruint32[FR_index] = vm->registeruint32[FR_index] & ~RTE_BIT; /* Unset RET bit. */
		vm->registeruint32[FR_index] = vm->registeruint32[FR_index] | GLOBAL_INTERRUPT_ENABLE_BIT; /* Set global interrupt enable. */
		vm->registeruint32[PC_index] = vm->memoryuint32[vm->registeruint32[SP_index] / sizeof(unsigned int)]; /* Set PC to [SP] */
		vm->registeruint32[SP_index] = vm->registeruint32[SP_index] + vm->registeruint32[WR_index];/* Set SP to SP + WR */
		return; 
	}

	/*  Check for timer interrupt condition */
	if(vm->memoryuint32[TIMER_PERIOD / sizeof(unsigned int)] && vm->cycles_executed % vm->memoryuint32[TIMER_PERIOD / sizeof(unsigned int)] == 0){
		/*  Assert our timer interrupt */
		vm->registeruint32[FR_index] = vm->registeruint32[FR_index] | TIMER1_ASSERTED_BIT;
		vm->cycles_executed = 0; /*  Avoid eventual overflow */
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

struct virtual_machine * vm_create(unsigned char start[4], unsigned char end[4], unsigned char data[1][5]){
	struct virtual_machine * vm = malloc(sizeof(struct virtual_machine));
	unsigned int start_addr = (start[0] << 24) + (start[1] << 16) + (start[2] << 8) + start[3];
	unsigned int end_addr = (end[0] << 24) + (end[1] << 16) + (end[2] << 8) + end[3];
	unsigned int current_addr = start_addr;
	unsigned int i = 0;
	unsigned int memory_index = 0;
	/*  Allocate enough memory to hold all the instructions */

	vm->cycles_executed = 0;
	vm->num_memory_words = (end_addr - start_addr) / sizeof(unsigned int);
	vm->num_registers = 64;
	vm->memoryuint32 = malloc(vm->num_memory_words * sizeof(unsigned int));
	vm->registeruint32 = malloc(vm->num_registers * sizeof(unsigned int));

	/*  Initialize registers */
	for(i = 0; i < 17; i++){
		vm->registeruint32[i] = 0x0;
	}
	vm->registeruint32[FR_index] = 0x200;  /* FR */
	vm->registeruint32[WR_index] = sizeof(unsigned int);    /* WR */

	vm->memoryuint32[UART1_OUT / sizeof(unsigned int)]     = 0;
	vm->memoryuint32[UART1_IN / sizeof(unsigned int)]      = 0;
	vm->memoryuint32[IRQ_HANDLER / sizeof(unsigned int)]   = 0;
	vm->memoryuint32[TIMER_PERIOD / sizeof(unsigned int)]  = 0;

	/*  Go through all the loader directives and load the machine code */
	i = 0;
	while(current_addr != end_addr){
		unsigned int is_sw_directive = data[i][0];
		unsigned int value = (data[i][1] << 24) + (data[i][2] << 16) + (data[i][3] << 8) + data[i][4];
		if(is_sw_directive){ /* Skip Word directive */
			current_addr += sizeof(unsigned int) * value;
			memory_index += value;
		}else{ /*  32 bit machine instruction */
			current_addr += sizeof(unsigned int);
			vm->memoryuint32[memory_index] = value;
			memory_index++;
		}
		i++;
	}

	return vm;
}

void vm_destroy(struct virtual_machine * vm){
	free(vm->memoryuint32);
	free(vm->registeruint32);
	free(vm);
}
