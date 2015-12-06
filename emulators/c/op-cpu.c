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

enum instruction_type {
	ADD_INSTRUCTION = 0,
	SUB_INSTRUCTION = 1,
	MUL_INSTRUCTION = 2,
	DIV_INSTRUCTION = 3,
	AND_INSTRUCTION = 4,
	OR_INSTRUCTION = 5,
	NOT_INSTRUCTION = 6,
	LOA_INSTRUCTION = 7,
	STO_INSTRUCTION = 8,
	SHR_INSTRUCTION = 9,
	SHL_INSTRUCTION = 10,
	BEQ_INSTRUCTION = 11,
	BLT_INSTRUCTION = 12,
	LL_INSTRUCTION = 13
};

struct virtual_machine {
	unsigned int cycles_executed;
	unsigned int num_memory_words;
	unsigned int num_registers;
	unsigned int pad;
	unsigned int * memoryuint32;
	unsigned int * registeruint32;
};

static void do_interrupt(struct virtual_machine *);
static void fetch_decode_execute(struct virtual_machine *);
void setup_virtual_machine(struct virtual_machine *);

static enum instruction_type lookup_instruction_op_code(unsigned int op_code){
	unsigned int type = op_code >> OP_CODE_OFFSET;
	assert(type < NUM_INSTRUCTION_TYPES && "Invalid opcode.");
	return type;
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
	int branch_dist = current_inst & BRANCH_DISTANCE_SIGN_BIT ? -((BRANCH_DISTANCE_MASK & ~(current_inst & BRANCH_DISTANCE_MASK)) + 1) : (current_inst & BRANCH_DISTANCE_MASK);
	unsigned int literal = LITERAL_MASK & current_inst;
	unsigned int ra = (ra_MASK & current_inst) >> ra_OFFSET;
	unsigned int rb = (rb_MASK & current_inst) >> rb_OFFSET;
	unsigned int rc = (rc_MASK & current_inst) >> rc_OFFSET;

	enum instruction_type op_type = lookup_instruction_op_code(current_inst & OP_CODE_MASK);
	assert(vm->registeruint32[PC_index] / sizeof(unsigned int) < vm->num_memory_words);
	vm->registeruint32[PC_index] += sizeof(unsigned int);


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
			if(vm->registeruint32[rc]){
				vm->registeruint32[ra] = vm->registeruint32[rb] / vm->registeruint32[rc];
			}else{
				/*  Division by zero detected */
				vm->registeruint32[FR_index] = vm->registeruint32[FR_index] | DIV_ZERO_ASSERTED_BIT;
			}
			break;
		}case BEQ_INSTRUCTION:{
			if(vm->registeruint32[ra] == vm->registeruint32[rb]){
				vm->registeruint32[PC_index] = vm->registeruint32[PC_index] + (unsigned int)((int)sizeof(unsigned int) * branch_dist);
			}
			break;
		}case BLT_INSTRUCTION:{
			if(vm->registeruint32[ra] < vm->registeruint32[rb]){
				vm->registeruint32[PC_index] = vm->registeruint32[PC_index] + (unsigned int)((int)sizeof(unsigned int) * branch_dist);
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
			vm->registeruint32[ra] = literal;
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
		if(vm->registeruint32[FR_index] & DIV_ZERO_ASSERTED_BIT){
			do_interrupt(vm);
			return; 
		}else if(vm->registeruint32[FR_index] & TIMER1_ENABLE_BIT && (vm->registeruint32[FR_index] & TIMER1_ASSERTED_BIT)){
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
	unsigned int start_addr = ((unsigned int)start[0] << 24u) + ((unsigned int)start[1] << 16u) + ((unsigned int)start[2] << 8u) + (unsigned int)start[3];
	unsigned int end_addr = ((unsigned int)end[0] << 24u) + ((unsigned int)end[1] << 16u) + ((unsigned int)end[2] << 8u) + (unsigned int)end[3];
	unsigned int current_addr = start_addr;
	unsigned int i = 0;
	unsigned int memory_index = 0;
	/*  Allocate enough memory to hold all the instructions */

	vm->cycles_executed = 0;
	vm->num_memory_words = (end_addr - start_addr) / sizeof(unsigned int);
	vm->num_registers = 1u << BITS_PER_REGISTER;
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
		unsigned int value = ((unsigned int)data[i][1u] << 24u) + ((unsigned int)data[i][2u] << 16u) + ((unsigned int)data[i][3u] << 8u) + (unsigned int)data[i][4u];
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
